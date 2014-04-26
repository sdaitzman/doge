// *----------------------------------*
// |      DogeClean - NO BITLASH      |
// |  forked on Wed. April 23, 2014   |
// *----------------------------------*
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
#include <Wire.h>                             // communication
#include <Adafruit_MotorShield.h>             // motor shield version 2.0
#include "utility/Adafruit_PWMServoDriver.h"  // servo driver library

// ********* CONFIGURATION: ********* 
#define stepsPerRotation    200   // number of steps per 360˚
#define rotateStepperPort   1     // port for stepper to rotate assembly
#define dispenseStepperPort 2     // port for stepper to dispense pills
#define buttonInPin         0    // analog pin for dispense button
#define buttonOutPin        11    // digigtal pin for dispense button out
#define alertPin            2     // pin for LED / buzzer alert
#define motorSpeed          5     // RPM for both motors
#define baudRate            57600 // baudrate - can be changed for other libraries
                                  // but if you do and use serial-web-terminal for access:
                                  // $ node index.js --baud <baudRate>

#define dispenseFrequency   60000 // ms frequency to dispense pills at
#define buttonThreshold     20

Adafruit_MotorShield AFMS = Adafruit_MotorShield(); // create the motor shield object

// motor port #2 (M3 and M4) - rotates assembly into place:
Adafruit_StepperMotor *rotateStepper = AFMS.getStepper(stepsPerRotation, rotateStepperPort);

// motor port #2 (M3 and M4) - dispenses pills from bottle
Adafruit_StepperMotor *dispenseStepper = AFMS.getStepper(stepsPerRotation, dispenseStepperPort);


int buttonPressed       = 0;                 // 1 when button pressed
int millisSinceDispense = dispenseFrequency; // first pill dispensed dispenseFrequency ms post-boot
int dispensedPill       = 0;


void rotateStepperHandler(int rotateSteps) {
  rotateStepper->step(rotateSteps, FORWARD, DOUBLE); // the actual moving part
}

void dispenseStepperHandler(int dispenseSteps) {
  dispenseStepper->step(dispenseSteps, FORWARD, DOUBLE); // moving part
}

// actually dispenses pills
void dispenseHandler(int bottleID, int pillsTotal, int bottlesTotal) {
  
  int bigStepSize  = stepsPerRotation / bottlesTotal; // 1 rotation / number of bottles
  int bigSteps     = bottleID - 1;                    // e.g. 0-3 not 1-4; less energy
  
  int steps        = bigSteps * bigStepSize;          // small step size, from number of bottles * steps per bottle
  
  rotateStepper->step(steps, FORWARD, DOUBLE);                           // move into position
  dispenseStepper->step(stepsPerRotation * pillsTotal, FORWARD, DOUBLE); // dispense pills
  rotateStepper->step(steps, BACKWARD, DOUBLE);                          // returns to previous position
}

void speedHandler(int stepperSpeed) {
  rotateStepper->setSpeed(stepperSpeed);   // set rotate speed to arg
  dispenseStepper->setSpeed(stepperSpeed); // set dispense speed to arg
}



void setup() {
    
  AFMS.begin(); // create motor object with the default frequency 1.6KHz
  rotateStepper->setSpeed(motorSpeed);
  dispenseStepper->setSpeed(motorSpeed);
  
  pinMode(buttonInPin, INPUT);
  pinMode(buttonOutPin, OUTPUT);
  digitalWrite(buttonOutPin, HIGH);
  
  pinMode(alertPin, OUTPUT);
  
  Serial.begin(baudRate); // begin bitlash at 57600 baud

}

void loop() {
  dispensedPill = 0;
  digitalWrite(alertPin, HIGH);
  while(dispensedPill == 0) {
    if(analogRead(buttonInPin) <= buttonThreshold && buttonPressed == 0) {
      buttonPressed = 1;
    } else if(analogRead(buttonInPin) > buttonThreshold && buttonPressed == 1) {
      buttonPressed = 0;
      digitalWrite(alertPin, LOW);
      dispensedPill = 1;
      dispenseHandler(3, 2, 6);
    } else if(analogRead(buttonInPin) > buttonThreshold && buttonPressed == 0) {
      
    }
    
    Serial.print(millis());Serial.print(" - - - - - - ");
  Serial.println(analogRead(buttonInPin));
  delay(50);
    
  }

  
  
  delay(dispenseFrequency);
  
}

