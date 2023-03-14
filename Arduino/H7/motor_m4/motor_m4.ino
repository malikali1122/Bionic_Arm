#include "Arduino.h"
#include "RPC.h"
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

int controlSignal = 0;

void motorSetup();
void motorLoop();

/* Update the control signal */
void updateControl(int newControlSignal)
{
  RPC.println("M4: executing updateControl with " + String(newControlSignal));
  controlSignal = newControlSignal;
}

void setup()
{
  // Initialize RPC library; this also boots the M4 core
  RPC.begin();
  Serial.begin(115200);
  while (!Serial) {} // Wait until the Serial connection is ready

  // M4 CPU becomes the server, so it makes the functions available under the defined names
  RPC.bind("updateControl", updateControl);

  //DEV : Add motor setup code herej
  motorSetup();

}

void loop()
{
  // DEV: Add motor loop code here
  // use controlSignal for switch case
  motorLoop();
}
