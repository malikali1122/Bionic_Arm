#ifndef _REALTIMEGESTURES_H
#define _REALTIMEGESTURES_H

#include <Servo.h>
#include <Arduino.h>

/* NOTES on ARM
    Actuator Open-Closed
    Thumb       0-135
    ThumbBase   180-135
    Index       0-135
    Middle      30-150
    RingPinky   0-150
    Linaccs     37-160 (40-145)
*/

class RealTimeGestures {
  public:
    RealTimeGestures(Servo *handServoArr, Servo *elbowServoArr, int *handState, int msMainLoopTime) {
      this->handServoArr = servoArr;
      this->delayNumLoops = (30 / msMainLoopTime); // servos need to wait 30 ms, thus divide by main loop sample time

      // Define final angles for each gesture
      clenchedFinalAngles = new int[5]{135, 135, 135, 150, 150};
      openFinalAngles = new int[5]{0, 180, 0, 30, 0};
      pinchFinalAngles = new int[5]{40, 60, 180, 0, 0};

      elbowFlexedAngles = new int[2]{180, 180};
      elbowRelaxedAngles = new int[2]{0 0};

      handState = new int[5]{0, 0, 0, 0, 0};
      elbowState = new int[2]{0, 0};

      // force initial relaxed position
      fistClenched = true;
      elbowFlexed = true;

      toggleFist();
      toggleElbow();
    }

    void toggleFist() {
      if (fistClenched) {
        moveServos(openFinalAngles, 1);
        fistClenched = false;
      }
      else {
        moveServos(clenchedFinalAngles, 1);
        fistClenched = true;
      }
    }

    void toggleElbow() {
      if (elbowFlexed) {
        moveServos(elbowRelaxedAngles, 0);
        elbowFlexed = false;
      }
      else {
        moveServos(elbowFlexedAngles, 0);
        elbowFlexed = true;
      }
    }

    void pinch() {
      moveServos(pinchFinalAngles, 1);
    }

    void periodicUpdate() {
      if (loopsWaitedElbow >= numLoopsToWait) {
        loopsWaitedElbow = 0;
        isDelayingElbow = false;
      }
      if (loopsWaitedHand >= numLoopsToWait) {
        loopsWaitedHand = 0;
        isDelayingHand = false;
      }

      if (!isDelayingElbow)
      {
        if (elbowMovesRemaining > 0) {
          for (int i = 0; i < 5; i++)
          {
            elbowState[i] = handSelbowStatetate[i] + 0.2 * deltaElbowAngles[i];
            elbowServoArr[i].write(elbowState[i]);
          }
          movesRemaining--;
          isDelayingElbow = true;
        }
      }
      else {
        loopsWaitedElbow++;
      }

      if (!isDelayingHand) {
        if (handMovesRemaining > 0) {
          for (int i = 0; i < 5; i++)
          {
            handState[i] = handState[i] + 0.2 * deltaHandAngles[i];
            handServoArr[i].write(handState[i]);
          }
          movesRemaining--;
          isDelayingHand = true;
        }
      }
      else {
        loopsWaitedHand++; // delay until x ms delay is reached.
      }
    }

  private:
    Servo *handServoArr, *elbowServoArr;
    int numServos;
    int *handState;
    int *elbowState;
    
    int *clenchedFinalAngles;
    int *openFinalAngles;
    int *pinchFinalAngles;
    int *elbowFlexedAngles;
    int *elbowRelaxedAngles;

    int deltaHandAngles[5];
    int deltaElbowAngles[2];

    int loopsWaitedHand = 0;
    int loopsWaitedElbow = 0;

    int delayNumLoops = 15;
    int handMovesRemaining = 5;
    int elbowMovesRemaining = 5;
    bool isDelayingHand = true;
    bool isDelayingElbow = true;

    // state boolean flags
    bool fistClenched = 0;
    bool elbowFlexed = 0;

    void moveServos(int *finalAngles, int isHand) {
      // moveServos: 
      if (isHand)
      {
        for (int i = 0; i < 5; i++) {
          deltaHandAngles[i] = finalAngles[i] - handState[i];
          isDelayingHand = false;
          handMovesRemaining = 0;
        }
      }
      else {
        for (int i = 0; i < 2; i++) {
          deltaElbowAngles[i] = finalAngles[i] - elbowState[i];
          isDelayingElbow = false;
          elbowMovesRemaining = 0;
        }
      }
    }
};

#endif
