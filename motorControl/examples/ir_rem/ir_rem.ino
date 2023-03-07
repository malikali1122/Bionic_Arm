#include <IRremote.hpp>
#include "Gestures.h"

const int RECV_PIN = 2;
long int decodedData;
decode_results results;

// pwm pins used for 
#define THUMB_PIN 3
#define THUMB_ROT_PIN 5
#define INDEX_MIDDLE_PIN 6
#define RING_PINKY_PIN 9

int handState[4]; // angular position array for each motor
Servo srvArr[4]; // array of servo objects
Servo Thumb;
Servo ThumbRot;
Servo IndexMiddle;
Servo RingPinky;

void setup(){
  Serial.begin(9600);
  IrReceiver.begin(RECV_PIN, ENABLE_LED_FEEDBACK); // Start the receiver
  srvArr[0].attach(THUMB_PIN);
  srvArr[1].attach(THUMB_ROT_PIN);
  srvArr[2].attach(INDEX_MIDDLE_PIN);
  srvArr[3].attach(RING_PINKY_PIN);
  srvArr[0].write(180);
  srvArr[1].write(0);
  srvArr[2].write(180);
  srvArr[3].write(180);
  int handState[4] = {180,0, 180, 180}; // initialize positions for servo
}

void loop() {
  if (IrReceiver.decode()) {
      decodedData = IrReceiver.decodedIRData.decodedRawData;
      if (decodedData == -217252096) {    // btn 1 pressed
        Serial.println("Button 1 Pressed");
        gestClench(srvArr, handState);
      }
      else if (decodedData == -417792256) {    // btn 2 pressed
        Serial.println("Button 2 Pressed");
        gestPinch(srvArr, handState);
      }
      else if (decodedData == -1587609856) {    // btn 3 pressed
        Serial.println("Button 3 Pressed");
        gestOpen(srvArr, handState);
      }
      IrReceiver.resume(); // Enable receiving of the next value
  }
}

