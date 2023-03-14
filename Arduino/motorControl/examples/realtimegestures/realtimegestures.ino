#include "RealTimeGestures.h"

// pwm pins used for 
#define THUMB_PIN 2
#define THUMB_BASE_PIN 3
#define INDEX_PIN 5
#define MIDDLE_PIN 7
#define RING_PINKY_PIN 9
#define ELBOW_L_PIN 11
#define ELBOW_R_PIN 13

Servo srvArrHand[5]; // array of servo objects for hand
Servo srvArrElbow[2];
Servo thumb;
Servo thumbBase;
Servo indexMiddle;
Servo ringPinky;
RealTimeGestures* gesturesPtr;

unsigned long loopStartTime, nsTimeLapsed;
unsigned long nsTimeBudget;

void setup(){
  Serial.begin(9600);
  srvArrHand[0].attach(THUMB_PIN);
  srvArrHand[1].attach(THUMB_BASE_PIN);
  srvArrHand[2].attach(INDEX_PIN);
  srvArrHand[3].attach(MIDDLE_PIN);
  srvArrHand[4].attach(RING_PINKY_PIN);

  srvArrElbow[0].attach(ELBOW_L_PIN);
  srvArrElbow[1].attach(ELBOW_R_PIN);

  nsTimeBudget = 2000;
  gesturesPtr = new RealTimeGestures(srvArrHand, srvArrElbow, 2);

  // safe start position
  srvArrHand[0].write(180);
  //srvArrHand[0].write(0); // WITHOUT MOVEABLE THUMB
  srvArrHand[1].write(0);
  srvArrHand[2].write(180);
  srvArrHand[3].write(180);
  srvArrHand[4].write(180);

  srvArrElbow[0].write(100); // LEFT ELBOW MOTOR
  srvArrElbow[1].write(80); // RIGHT ELBOW MOTOR

  delay(1000);
  srvArrElbow[0].write(180); // LEFT ELBOW MOTOR
  srvArrElbow[1].write(0); // RIGHT ELBOW MOTOR
  delay(1000);
}

void loop() {
  loopStartTime = micros();
  (*gesturesPtr).periodicUpdate();
  if (Serial.available() > 0) {
    char input = Serial.read();
    switch(input) {
      case '3':
        (*gesturesPtr).toggleFist();
        break;
      case '6':
        (*gesturesPtr).toggleElbow();
        break;
      default:
        Serial.println("Invalid input");
        break;
    }
  }
  nsTimeLapsed = micros() - loopStartTime;
  if (nsTimeLapsed <  nsTimeBudget) {
    delayMicroseconds(nsTimeBudget - nsTimeLapsed);
  }
}

