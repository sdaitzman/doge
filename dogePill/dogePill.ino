// This work by Sam Daitzman et al. is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
// To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.

// such medicine
// very adherence
// much secure
// wow

// libraries:
#include "bitlash.h"                          // bitlash for functions, serial, process handling
#include <Wire.h>                             // communication
#include <Adafruit_MotorShield.h>             // motor shield version 2.0
#include "utility/Adafruit_PWMServoDriver.h"  // servo driver library


Adafruit_MotorShield AFMS = Adafruit_MotorShield(); // create the motor shield object

Adafruit_StepperMotor *dispenseStepper = AFMS.getStepper(200, 1); // motor port #2 (M3 and M4)
Adafruit_StepperMotor *rotateStepper = AFMS.getStepper(200, 2); // motor port #2 (M3 and M4)


numvar rotateStepperHandler(void) {
  rotateStepper->step(getarg(1), FORWARD, DOUBLE); // the actual moving part
}

numvar dispenseStepperHandler(void) {
  dispenseStepper->step(getarg(1), FORWARD, DOUBLE); // moving part
}

// actually dispenses pills
numvar dispenseHandler(void) {
  int bottleID     = getarg(1);          // bottle to dispense from
  int pillsTotal   = getarg(2);          // pills to dispense
  int bottlesTotal = getarg(3);          // number of bottles
  
  int bigStepSize  = 200 / bottlesTotal; // 1 rotation / number of bottles
  int bigSteps     = bottleID - 1;       // e.g. 0-3 not 1-4; less energy
  
  int steps        = bigSteps * bigStepSize;
  
  rotateStepper->step(steps, FORWARD, DOUBLE);    // move into position
  dispenseStepper->step(200 * pillsTotal, FORWARD, DOUBLE); // dispense pills
  rotateStepper->step(steps, BACKWARD, DOUBLE);   // returns to previous position
}



void setup() {
    
  AFMS.begin();  // create with the default frequency 1.6KHz
  rotateStepper->setSpeed(250);
  dispenseStepper->setSpeed(250);
  
  addBitlashFunction("rotate", (bitlash_function) rotateStepperHandler);      // adds rotate(n)   - rotates n 1.8˚ steps
  addBitlashFunction("dispenser", (bitlash_function) dispenseStepperHandler); // adds stepper(n)  - dispenser motor / moves n 1.8˚ steps
  addBitlashFunction("dispense", (bitlash_function) dispenseHandler);         // adds dispense(n) - rotates to position n, dispenses, returns
  
  
  initBitlash(57600);

}

void loop() {
  runBitlash();
}

