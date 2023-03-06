#ifndef _Gestures_H
#define _Gestures_H


#include <Servo.h>
#include <Arduino.h>
#define NUMSERVOS 5

// gesture function prototypes
void gestClench(Servo (&servoArr)[NUMSERVOS], int (&handState)[NUMSERVOS]); // pass by reference to allow changes
void gestOpen(Servo (&servoArr)[NUMSERVOS], int (&handState)[NUMSERVOS]); // hand state is changed after every servo write operation
void gestPinch(Servo (&servoArr)[NUMSERVOS], int (&handState)[NUMSERVOS]);

/* NOTES on ARM
    Actuator Open-Closed
    Thumb       0-135
    ThumbBase   180-135
    Index       0-135
    Middle      30-150
    RingPinky   0-150
    Linaccs     37-160 (40-145)
*/

// GESTURES
void gestClench(Servo (&servoArr)[NUMSERVOS], int (&handState)[NUMSERVOS]) {
  int finalAngles[NUMSERVOS] = {135, 135, 135, 150 150};
  int deltaAngles[NUMSERVOS];
  for (int i = 0; i < 4; i++)
  {
    deltaAngles[i] = finalAngles[i] - handState[i]; // calculate required change in position from current motor positions to desired
  }

  for (int incr = 0; incr <= 4; incr++) // move up in 20% increments from current position
  {
    for (int i =0; i < 4; i++)
    {
      handState[i] = handState[i] + 0.2 * deltaAngles[i];
      servoArr[i].write(handState[i]);
    }
    delay(30); // remove delay and add number of loops waited
  }
}

void gestOpen(Servo (&servoArr)[4], int (&handState)[4]) {
  int finalAngles[4] = {180, 0, 180, 180};
  int deltaAngles[4];
  for (int i = 0; i < 4; i++)
  {
    deltaAngles[i] = finalAngles[i] - handState[i];
  }

  for (int incr = 0; incr <= 4; incr++) // move up in 20% increments from current position
  {
    for (int i =0; i < 4; i++)
    {
      handState[i] = handState[i] + 0.2 * deltaAngles[i]; // increase motor position in 20% increments. could be parametrized in future
      servoArr[i].write(handState[i]); // move servo to new position
    }
    delay(30); // allow servo to move into position before looping again
  }
}

void gestPinch(Servo (&servoArr)[4], int (&handState)[4]) {
  int finalAngles[4] = {40, 60, 180, 0};
  int deltaAngles[4];
  for (int i = 0; i < 4; i++)
  {
    deltaAngles[i] = finalAngles[i] - handState[i];
  }

  for (int incr = 0; incr <= 4; incr++)
  {
    for (int i =0; i < 4; i++)
    {
      handState[i] = handState[i] + 0.2 * deltaAngles[i];
      servoArr[i].write(handState[i]);
    }
    delay(30);
  }
}

#endif