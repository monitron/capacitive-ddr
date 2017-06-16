#include <Wire.h>
#include "Adafruit_MPR121.h"
#include <UnoJoy.h>

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

// Keeps track of the last pins touched
// so we know when buttons are 'released'
uint16_t lasttouched = 0;
uint16_t currtouched = 0;
dataForController_t controllerData;

void setup() {
  setupUnoJoy();
  Serial.begin(9600);

  while (!Serial) { // needed to keep leonardo/micro from starting too fast!
    delay(10);
  }
  
  Serial.println("Adafruit MPR121 Capacitive Touch sensor test"); 
  
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");
}

void loop() {
  // Get the currently touched pads
  currtouched = cap.touched();
  
  for (uint8_t i=0; i<5; i++) {
    // it if *is* touched and *wasnt* touched before, alert!
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" touched");
      if (i == 0) {
        controllerData.triangleOn = true;
      }
      if (i == 1) {
        controllerData.circleOn = true;
      }
      if (i == 2) {
        controllerData.squareOn = true;
      }
      if (i == 3) {
        controllerData.crossOn = true;
      }
      if (i == 4) {
        controllerData.startOn = true;
      }
      if (i == 5) {
        controllerData.selectOn = true;
      }
    }
    // if it *was* touched and now *isnt*, alert!
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      if (i == 0) {
        controllerData.triangleOn = false;
      }
      if (i == 1) {
        controllerData.circleOn = false;
      }
      if (i == 2) {
        controllerData.squareOn = false;
      }
      if (i == 3) {
        controllerData.crossOn = false;
      }
      if (i == 4) {
        controllerData.startOn = false;
      }
      if (i == 5) {
        controllerData.selectOn = false;
      }
    }
  }

  // reset our state
  lasttouched = currtouched;

  // comment out this line for detailed data from the sensor!
  return;
  
  // debugging info, what
  Serial.print("\t\t\t\t\t\t\t\t\t\t\t\t\t 0x"); Serial.println(cap.touched(), HEX);
  Serial.print("Filt: ");
  for (uint8_t i=0; i<12; i++) {
    Serial.print(cap.filteredData(i)); Serial.print("\t");
  }
  Serial.println();
  Serial.print("Base: ");
  for (uint8_t i=0; i<12; i++) {
    Serial.print(cap.baselineData(i)); Serial.print("\t");
  }
  Serial.println();
  
  // put a delay so it isn't overwhelming
  delay(100);
}
