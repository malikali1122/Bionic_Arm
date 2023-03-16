#include "Arduino.h"
#include "RPC.h"
#include "EMG_Sensor.h"
#include "EMGFilters.h"
#include "RealTimeGestures.h"
#include "pins.h"

int prevControlSignal = 0;


// Modify value according to number of sensors used
#define SENSOR_COUNT 2
// Set 1 for Serial Plotting and 0 for Putty CSV Export
int enableSerialPlot = 1;

// Set 0 if Timing o/p need not be printed
#define TIMING_DEBUG 0

unsigned long runTime;
unsigned long timeBudget;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// START OF TUNING PARAMETERS

const long intensityThreshold1 = 2000; // Threshold for hard-coded determination of low or high intensity
const long intensityThreshold2 = 2000;

const long durationThreshold1 = 2000; // Threshold (ms) for the distinction between short and long signal
const long durationThreshold2 = 2000;

const int averageLength = 1000; // Constant (need tuning?)
const int envelopeReach = 100;  // May need tuning!!

// END OF TUNING PARAMETERS
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

int envelopeIndex1 = 0;
int valueArray1;
int temp1 = 0;
int temp2 = 0;
long averageVal1 = 0;
long envelopeVal = 0;
int signalReadings1[envelopeReach];
int eventFlag1 = 0;       // Toggle between 0 and 1 if the envelope rises above 0 or falls back to 0 (start and end flag)
long eventArea1 = 0;      // Variable to track the intensity (cumulative) as an event occurs
long eventStartTime1 = 0; // Variable to record the start time for any one event
long eventStopTime1 = 0;  // Variable to record the stop time for any one event
long previousEnvelope1 = 0;
int gestureID = 0; // Initial state for the gesture identifier

long averageVal2 = 0;
int envelopeIndex2 = 0;
int signalReadings2[envelopeReach];
int eventFlag2 = 0;       // Toggle between 0 and 1 if the envelope rises above 0 or falls back to 0 (start and end flag)
long eventArea2 = 0;      // Variable to track the intensity (cumulative) as an event occurs
long eventStartTime2 = 0; // Variable to record the start time for any one event
long eventStopTime2 = 0;  // Variable to record the stop time for any one event
long previousEnvelope2 = 0;

int channelID = 1; // Default value for channel selection

// discrete filters must works with fixed sample frequence
// our emg filter only support "SAMPLE_FREQ_500HZ" or "SAMPLE_FREQ_1000HZ"
// other sampleRate inputs will bypass all the EMG_FILTER
SAMPLE_FREQUENCY sampleRate = SAMPLE_FREQ_500HZ;

EMG_Sensor emg[SENSOR_COUNT] = {EMG_Sensor(SENSOR1_PIN, sampleRate, 10), EMG_Sensor(SENSOR2_PIN, sampleRate, 10)};

void emgSetup();
int getControlSignal();

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
