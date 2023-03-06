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
    RealTimeGestures(Servo *handServoArr, Servo *elbowServoArr, int msMainLoopTime) {
      this->handServoArr = handServoArr;
      this->elbowServoArr = elbowServoArr;
      this->delayNumLoops = (26 / msMainLoopTime); // servos need to wait 30 ms, thus divide by main loop sample time

      // Define final angles for each gesture
      openFinalAngles = new int[5]{180, 0, 180, 180, 180};
      clenchedFinalAngles = new int[5]{10, 60, 0, 30, 0};
      pinchFinalAngles = new int[5]{40, 60, 180, 0, 0};

      elbowFlexedAngles = new int[2]{180, 180};
      elbowRelaxedAngles = new int[2]{0, 0};

      handState = new int[5]{180, 0, 180, 180, 180};
      elbowState = new int[2]{0, 0};

      deltaHandAngles = new float[5]{0, 0, 0, 0, 0};
      deltaElbowAngles = new int[2]{0, 0};
      // force initial relaxed position
      fistClenched = false;
      elbowFlexed = false;
      
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
          for (int i = 0; i < 5; i++)
          {
            elbowState[i] = elbowState[i] + 0.2 * deltaElbowAngles[i];
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
            handState[i] = handState[i] + 0.2 * deltaHandAngles[i];
            if (handState[i] > 180) {
              handState[i] = 180;
            }
            else if (handState[i] < 0) {
              handState[i] = 0;
            }
            handServoArr[i].write(handState[i]);
          }
          Serial.println(handState[0]);
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
    int *deltaElbowAngles;

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
          handMovesRemaining = 5;
        }
      }
      else {
        for (int i = 0; i < 2; i++) {
          deltaElbowAngles[i] = finalAngles[i] - elbowState[i];
          isDelayingElbow = false;
          elbowMovesRemaining = 5;
        }
      }
    }
};

#endif
