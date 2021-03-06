#include <Wire.h>
#include "Adafruit_MPR121.h"
#include <UnoJoy.h>

// You can have up to 4 on one i2c bus but one is enough for testing!
Adafruit_MPR121 cap = Adafruit_MPR121();

uint16_t currtouched = 0;
dataForController_t controllerData;

void setup() {
  setupUnoJoy();
  // Default address is 0x5A, if tied to 3.3V its 0x5B
  // If tied to SDA its 0x5C and if SCL then 0x5D
  if (!cap.begin(0x5A)) {
    while (1);
  }

  cap.writeRegister(MPR121_ECR, B10000000); // Stop MPR121
  cap.setThresholds(30, 20);

  cap.writeRegister(MPR121_AUTOCONFIG0, B00101011);
  cap.writeRegister(MPR121_AUTOCONFIG1, 0);
  cap.writeRegister(MPR121_UPLIMIT, 196);
  cap.writeRegister(MPR121_TARGETLIMIT, 176);
  cap.writeRegister(MPR121_LOWLIMIT, 127);
  cap.writeRegister(MPR121_ECR, B10000110); // Start MPR121 with 5 channels
}

void loop() {
  controllerData = getBlankDataForController();
  // Get the currently touched pads
  currtouched = cap.touched();

  controllerData.triangleOn = !!(currtouched & _BV(0));
  controllerData.squareOn   = !!(currtouched & _BV(1));
  controllerData.crossOn    = !!(currtouched & _BV(2));
  controllerData.circleOn   = !!(currtouched & _BV(3));
  controllerData.startOn    = !!(currtouched & _BV(4));
  controllerData.selectOn   = !!(currtouched & _BV(5));

  setControllerData(controllerData);
}
