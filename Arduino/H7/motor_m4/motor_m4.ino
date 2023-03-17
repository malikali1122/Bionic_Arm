#include "Arduino.h"
#include "RPC.h"
#include "RealTimeGestures.h"


/*  
    This example says that D3 & D6 have some issue with PWM
    https://github.com/khoih-prog/Portenta_H7_TimerInterrupt/blob/main/examples/PWM/PWM_Multi/PWM_Multi.ino

    This forum post says that D1, D3 and D6 have issues with PWM
    https://forum.arduino.cc/t/portenta-pwm-issues/866822/3
*/

// pwm pins used for 
#define THUMB_PIN 6
#define THUMB_BASE_PIN 5
#define INDEX_PIN 4
#define MIDDLE_PIN 3
#define RING_PINKY_PIN 2
#define ELBOW_L_PIN 1
#define ELBOW_R_PIN 0

Servo srvArrHand[5]; // array of servo objects for hand
Servo srvArrElbow[2];
Servo thumb;
Servo thumbBase;
Servo indexMiddle;
Servo ringPinky;
RealTimeGestures* gesturesPtr;

unsigned long loopStartTime, nsTimeLapsed;
unsigned long nsTimeBudget;

int toggleFist = 0;
int toggleElbow = 0;

void motorSetup();
void motorLoop(int signalInput);

/* Update the control signal */
void updateControl(int controlSignal)
{ 
  switch(controlSignal) {
    case 3:
      if (!toggleFist)
        toggleFist = 1;
      break;
    case 6:
      if (!toggleElbow)
        toggleElbow = 1;
      break;
    case 0:
      RPC.println("M4: Received Control Signal 0");
    default:
      RPC.println("Invalid input");
      break;
  }
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
  RPC.println("M4: controlSignal = " + String(controlSignal));
  motorLoop(controlSignal);
  motorLoop();
}
