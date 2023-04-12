/*******************************************************
  File: force_sense_sketch.ino
  Author: davWilk
  Date: 12/04/2023
  Description: 
    Basic testing of analogue force sensors to allow feedback
    for motor control. Includes SG90 servo sweep that is halted
    upon sensing larger force than baseline
******************************************************/

/* WIRING BASICS
  5V ----- [L   FORCE SENSOR   R] ----- . ---- A0
                                        |
                                        R (10kOhm)
                                        |
                                       GND
*/

#include <Servo.h>

// MACROS/CONSTANTS
#define SENSOR_PIN A0 // Analogue Input 0
#define SERVO_PIN 3 // SG90 Servo Pin

// DECLARATIONS
int analogForceReading;
int initialForceReading;

// imitating RealTimeGestures structure
Servo *handServoArr;
int *openFinalAngles;
int *closedFinalAngles;
float *deltaHandAngles;
float *handState;
int loopsWaitedHand;
int handMovesRemaining;
int delayNumLoops;
short int isDelayingHand;
short int numActuators;

// new vars to implement in RealTimeGestures
short int *handHalted; 
short int numSteps;   // number of servo steps from openAngle to closedAngle

void setup() {
  Serial.begin(9600);
  handServoArr = new Servo[1];
  handServoArr[0].attach(SERVO_PIN);

  openFinalAngles = new int[1]{37};
  closedFinalAngles = new int[1]{160};
  handState = new float[1]{160};
  handHalted = new short int[1]{0};
  deltaHandAngles = new float[1]{0};
  loopsWaitedHand = 0;
  
  isDelayingHand = 0;
  handHalted = 0;
  numSteps = 10;
  numActuators = 1;
  delayNumLoops = 500;
  handMovesRemaining = numSteps;
  handServoArr[0].write(openFinalAngles[0]);
  handState[0]  = openFinalAngles[0];
  delay(1000);
}

unsigned long startTime;
void loop() {
  initialForceReading = analogRead(SENSOR_PIN);
  Serial.println(initialForceReading);

  // classic Robust(TM) code :P
  while (true) {
    startTime = micros();
    periodicUpdate();

    if (handMovesRemaining == 0) { handMovesRemaining = numSteps; } // allow infinite movement for testing
    // enforce sample time of 2ms; 500Hz
    while (micros() - startTime < 2000)
    {
      delayMicroseconds(1);
    }
  }
}


void periodicUpdate() {
  if (loopsWaitedHand >= delayNumLoops) {
    loopsWaitedHand = 0;
    isDelayingHand = false;
  }

  if (!isDelayingHand) {
    if (handMovesRemaining > 0) {
      for (int i = 0; i < numActuators; i++)
      {
        handHalted[i] = sensorReadingSignificant(analogRead(SENSOR_PIN));
        if (handHalted[i]) { Serial.println("HALTED"); }
        if (!handHalted[i]) {
          int limReached = limitReached(i);
          if (limReached == -1)
          {
            deltaHandAngles[i] = closedFinalAngles[i] - openFinalAngles[i];
            // Serial.print(deltaHandAngles[i] );
            isDelayingHand = false;
            handMovesRemaining = numSteps;
            loopsWaitedHand = 0;
          }
          else if (limReached == 1) {
            deltaHandAngles[i] = openFinalAngles[i] - closedFinalAngles[i];
            isDelayingHand = false;
            handMovesRemaining = numSteps;
            loopsWaitedHand = 0;
          }
          Serial.println(deltaHandAngles[0]);
          
          handState[i] = handState[i] + ((1.0/numSteps) * deltaHandAngles[i]);
          if (handState[i] > closedFinalAngles[i]) {
            handState[i] = closedFinalAngles[i];
          }
          else if (handState[i] < openFinalAngles[i]) {
            handState[i] = openFinalAngles[i];
          }
          Serial.println(handState[0]);
          handServoArr[i].write(round(handState[i]));
        }
        handMovesRemaining--;
        isDelayingHand = true;
      }
    }
  }
  else {
    loopsWaitedHand++; // delay until x ms delay is reached.
  }
}

bool sensorReadingSignificant(int sensor_val) {
  return (sensor_val > 1.1 * initialForceReading || sensor_val < 0.9 * initialForceReading);
}

int limitReached(int idx) {
  // open limit: return -1
  // closed limit: return 1
  // else: return 0
  if (handState[idx] == openFinalAngles[idx]) { return -1; }
  else if (handState[idx] == closedFinalAngles[idx]) { return 1; }
  else { return 0; }
}