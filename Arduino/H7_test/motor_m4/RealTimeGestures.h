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
    Elbow Left  100-180
    Elbow Right 80-0
*/

class RealTimeGestures {
  public:
    RealTimeGestures(Servo *handServoArr, Servo *elbowServoArr, int msMainLoopTime) {
      this->handServoArr = handServoArr;
      this->elbowServoArr = elbowServoArr;
      this->delayNumLoops = (26 / msMainLoopTime); // servos need to wait 30 ms, thus divide by main loop sample time

      // Define final angles for each gesture
      openFinalAngles = new int[5]{170, 0, 180, 180, 180}; // WITH MOVEABLE THUMB
      // openFinalAngles = new int[5]{0, 0, 180, 180, 180}; // WITHOUT MOVEABLE THUMB
      clenchedFinalAngles = new int[5]{15, 60, 0, 30, 0}; // WITH MOVEABLE THUMB
      // clenchedFinalAngles = new int[5]{60, 60, 0, 30, 0}; // WITHOUT MOVEABLE THUMB
      pinchFinalAngles = new int[5]{40, 60, 180, 0, 0};

      elbowFlexedAngles = new int[2]{100, 80};
      elbowRelaxedAngles = new int[2]{180, 0};

      handState = new int[5]{180, 0, 180, 180, 180};
      elbowState = new int[2]{180, 0};

      deltaHandAngles = new float[5]{0, 0, 0, 0, 0};
      deltaElbowAngles = new float[2]{0, 0};
      // force initial relaxed position
      fistClenched = false;
      elbowFlexed = false;

      isDelayingElbow = false;
      isDelayingHand = false;
      
      handMovesRemaining = 0;
      elbowMovesRemaining = 0;

      loopsWaitedElbow = 0;
      loopsWaitedHand = 0;
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
      if (loopsWaitedElbow >= delayNumLoops) {
        loopsWaitedElbow = 0;
        isDelayingElbow = false;
      }
      if (loopsWaitedHand >= delayNumLoops) {
        loopsWaitedHand = 0;
        isDelayingHand = false;
      }

      if (!isDelayingElbow)
      {
        if (elbowMovesRemaining > 0) {
          for (int i = 0; i <= 1; i++)
          {
            elbowState[i] = elbowState[i] + round(0.2 * deltaElbowAngles[i]);
            elbowServoArr[i].write(elbowState[i]);
          }
          elbowMovesRemaining--;
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
            handState[i] = handState[i] + round(0.2 * deltaHandAngles[i]);
            if (handState[i] > 180) {
              handState[i] = 180;
            }
            else if (handState[i] < 0) {
              handState[i] = 0;
            }
            handServoArr[i].write(handState[i]);
          }
          handMovesRemaining--;
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

    float *deltaHandAngles;
    float *deltaElbowAngles;

    int loopsWaitedHand = 0;
    int loopsWaitedElbow = 0;

    int delayNumLoops = 15;
    int handMovesRemaining = 0;
    int elbowMovesRemaining = 0;
    bool isDelayingHand;
    bool isDelayingElbow;

    // state boolean flags
    bool fistClenched = 0;
    bool elbowFlexed = 0;

    void moveServos(int *finalAngles, int isHand) {
      if (isHand)
      {
        for (int i = 0; i < 5; i++) {
          deltaHandAngles[i] = finalAngles[i] - handState[i];
          isDelayingHand = false;
          handMovesRemaining = 5;
          loopsWaitedHand = 0;
        }
      }
      else {
        for (int j = 0; j <= 1; j++) {
          deltaElbowAngles[j] = finalAngles[j] - elbowState[j];
          isDelayingElbow = false;
          elbowMovesRemaining = 5;
          loopsWaitedElbow = 0;
        }
      }
    }
};

#endif
