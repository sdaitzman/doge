// *-------------*
// |  DogeClean  |
// *-------------*
// This work by Sam Daitzman et al. is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
// To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.

// such medicine
// very adherence
// much secure
// wow

// to install required libraries:
// $ cd ~/Documents/Arduino/Libraries
// $ git clone https://github.com/billroy/bitlash.git
// $ git clone https://github.com/adafruit/Adafruit_Motor_Shield_V2_Library.git
// restart Arduino

// I recommend using serial-web-terminal to access bitlash
// https://github.com/billroy/serial-web-terminal
// all standard options should work

// libraries:
#include "bitlash.h"                          // bitlash for functions, serial, process handling-ish
#include <Wire.h>                             // communication
#include <Adafruit_MotorShield.h>             // motor shield version 2.0
#include "utility/Adafruit_PWMServoDriver.h"  // servo driver library

#define stepsPerRotation    200   // number of steps per 360˚
#define rotateStepperPort   1     // port for stepper to rotate assembly
#define dispenseStepperPort 2     // port for stepper to dispense pills
#define motorSpeed          250   // RPM for both motors
#define baudRate            57600 // baudrate - can be changed for other libraries
                                  // but if you do and use serial-web-terminal for access:
                                  // $ node index.js --baud <baudRate>

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); // create the motor shield object

Adafruit_StepperMotor *rotateStepper = AFMS.getStepper(stepsPerRotation, rotateStepperPort);     // motor port #2 (M3 and M4) - rotates assembly into place
Adafruit_StepperMotor *dispenseStepper = AFMS.getStepper(stepsPerRotation, dispenseStepperPort); // motor port #2 (M3 and M4) - dispenses pills from bottle


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
  
  int bigStepSize  = stepsPerRotation / bottlesTotal; // 1 rotation / number of bottles
  int bigSteps     = bottleID - 1;                    // e.g. 0-3 not 1-4; less energy
  
  int steps        = bigSteps * bigStepSize;          // small step size, from number of bottles * steps per bottle
  
  rotateStepper->step(steps, FORWARD, DOUBLE);                           // move into position
  dispenseStepper->step(stepsPerRotation * pillsTotal, FORWARD, DOUBLE); // dispense pills
  rotateStepper->step(steps, BACKWARD, DOUBLE);                          // returns to previous position
}



void setup() {
    
  AFMS.begin(); // create motor object with the default frequency 1.6KHz
  rotateStepper->setSpeed(motorSpeed);
  dispenseStepper->setSpeed(motorSpeed);
  
  addBitlashFunction("rotate", (bitlash_function) rotateStepperHandler);      // adds rotate(n)   - rotates n 1.8˚ steps
  addBitlashFunction("dispenser", (bitlash_function) dispenseStepperHandler); // adds stepper(n)  - dispenser motor / moves n 1.8˚ steps
  addBitlashFunction("dispense", (bitlash_function) dispenseHandler);         // adds dispense(n) - rotates to position n, dispenses, returns
  
  
  initBitlash(baudRate); // begin bitlash at 57600 baud

}

void loop() {
  runBitlash(); // runs bitlash
}

