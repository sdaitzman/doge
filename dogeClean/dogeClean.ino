// This work by Sam Daitzman is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
// To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.

// such clean
// very wow
// much clean

// define pins for indicator LEDs
#define errorPin 13
#define warningPin 12
#define goodPin 11
#define idlePin 10

// libraries:
#include "bitlash.h"                          // bitlash for functions, serial, process handling
#include <Wire.h>                             // communication
#include <Adafruit_MotorShield.h>             // motor shield version 2.0
#include "utility/Adafruit_PWMServoDriver.h"  // servo driver library


Adafruit_MotorShield AFMS = Adafruit_MotorShield(); // create the motor shield object

Adafruit_StepperMotor *cleanMotors = AFMS.getStepper(200, 2); // motor port #2 (M3 and M4)

// turns all indicator LEDs off
void resetter() {
  digitalWrite(errorPin, LOW);
  digitalWrite(warningPin, LOW);
  digitalWrite(goodPin, LOW);
  digitalWrite(idlePin, LOW);
}

// switches to error
numvar error(void) {
  resetter();
  pinMode(errorPin, OUTPUT);
  digitalWrite(errorPin, HIGH);
}

// switches to warning
numvar warning(void) {
  resetter();
  pinMode(warningPin, OUTPUT);
  digitalWrite(warningPin, HIGH);
}

// switches to good
numvar good(void) {
  resetter();
  pinMode(goodPin, OUTPUT);
  digitalWrite(goodPin, HIGH);
}

// switches to idle
numvar idle(void) {
  resetter();
  pinMode(idlePin, OUTPUT);
  digitalWrite(idlePin, HIGH);
}

// resets all LEDs and calls back
numvar reset(void) {
  resetter();
  Serial.println("reset");
}

numvar stepper(void) {
  cleanMotors->step(getarg(1), FORWARD, DOUBLE);                                      // the actual moving part
  idle();
  Serial.println(" Done.");
}

void setup() {
  
  AFMS.begin();  // create with the default frequency 1.6KHz
  cleanMotors->setSpeed(250);  // 250 rpm
  
  initBitlash(9600);
  
  addBitlashFunction("stepper", (bitlash_function) stepper); // adds stepper(n) - steps n 1.8˚ steps
  
}

void loop() {
  runBitlash();
}

