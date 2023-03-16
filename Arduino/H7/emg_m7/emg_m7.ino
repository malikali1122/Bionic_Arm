#include "Arduino.h"
#include "RPC.h"
#include "EMG_Sensor.h"
#include "EMGFilters.h"
#include "RealTimeGestures.h"
#include "pins.h"
#include "Globals.h"

/* Instantiate EMG_Sensors
    EMG_Sensor(int sensorPin, SAMPLE_FREQUENCY sampleRate, int threshold);
*/
EMG_Sensor emg[SENSOR_COUNT] = {EMG_Sensor(SENSOR1_PIN, sampleRate, 10), EMG_Sensor(SENSOR2_PIN, sampleRate, 10)};


/* ***************** MOTOR */


Servo srvArrHand[5]; // array of servo objects for hand
Servo srvArrElbow[2];
Servo thumb;
Servo thumbBase;
Servo indexMiddle;
Servo ringPinky;
RealTimeGestures *gesturesPtr;

unsigned long loopStartTime, nsTimeLapsed;
unsigned long nsTimeBudget;

int toggleFist = 0;
int toggleElbow = 0;

void motorSetup();
void motorLoop();

void setup()
{
  // Initialize RPC library; this also boots the M4 core
  // RPC.begin();
  Serial.begin(115200);
  while (!Serial)
  {
  } // Wait until the Serial connection is ready

  // DEV: Add emg setup code here
  emgSetup();
  motorSetup();
}

void loop()
{
  // DEV: Add emg loop code here
  int controlSignal = getControlSignal();

  updateControlSignal(controlSignal);

  motorLoop();
}

// On change of control signal, send it to the M4
// and also update in M7
void updateControlSignal(int controlSignal)
{
  // Serial.print("M7: CS - " + String(controlSignal));
  // RPC.send("updateControl", controlSignal);

  if (prevControlSignal != controlSignal)
  {
    switch (controlSignal)
    {
    case 3:
      if (!toggleFist)
        toggleFist = 1;
      break;
    case 6:
      if (!toggleElbow)
        toggleElbow = 1;
      break;
    case 0:
      Serial.println("M7: Received Control Signal 0");
    default:
      Serial.println("Invalid input");
      break;
    }
    prevControlSignal = controlSignal;
  }

  // if (prevControlSignal != controlSignal)
  // {
  //   RPC.send("updateControl", controlSignal);
  //   // DEV: Fallback to RPC.call if RPC.send doesn't work
  //   // RPC.call("updateControl", controlSignal);
  //   Serial.println("M7: executing updateControl with " + String(controlSignal));
  //   prevControlSignal = controlSignal;
  // }
}
