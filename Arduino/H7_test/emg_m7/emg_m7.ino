#include "Arduino.h"
#include "RPC.h"
#include "EMG_Sensor.h"
#include "EMGFilters.h"

bool PRINT_RPC_STREAM = true;
int prevControlSignal = 0;

#define SENSOR1_PIN A0
#define SENSOR2_PIN A1

#define ERROR_LED 13

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
const int envelopeReach = 100; // May need tuning!!

// END OF TUNING PARAMETERS
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


int envelopeIndex1 = 0;
int valueArray1;
int temp1 = 0; 
int temp2 = 0; 
long averageVal1 = 0;
long envelopeVal = 0;
int signalReadings1[envelopeReach];
int eventFlag1 = 0; // Toggle between 0 and 1 if the envelope rises above 0 or falls back to 0 (start and end flag)
long eventArea1 = 0; // Variable to track the intensity (cumulative) as an event occurs
long eventStartTime1 = 0; // Variable to record the start time for any one event
long eventStopTime1 = 0; // Variable to record the stop time for any one event
long previousEnvelope1 = 0;
int gestureID = 0; // Initial state for the gesture identifier

long averageVal2 = 0;
int envelopeIndex2 = 0;
int signalReadings2[envelopeReach];
int eventFlag2 = 0; // Toggle between 0 and 1 if the envelope rises above 0 or falls back to 0 (start and end flag)
long eventArea2 = 0; // Variable to track the intensity (cumulative) as an event occurs
long eventStartTime2 = 0; // Variable to record the start time for any one event
long eventStopTime2 = 0; // Variable to record the stop time for any one event
long previousEnvelope2 = 0;

int channelID = 1; // Default value for channel selection

int testSignal = 0;


// discrete filters must works with fixed sample frequence
// our emg filter only support "SAMPLE_FREQ_500HZ" or "SAMPLE_FREQ_1000HZ"
// other sampleRate inputs will bypass all the EMG_FILTER
SAMPLE_FREQUENCY sampleRate = SAMPLE_FREQ_500HZ;

EMG_Sensor emg[SENSOR_COUNT] = {EMG_Sensor(SENSOR1_PIN, sampleRate, 10), EMG_Sensor(SENSOR2_PIN, sampleRate, 10)};

void emgSetup();
int getControlSignal();

void setup()
{
  // Initialize RPC library; this also boots the M4 core
  RPC.begin();
  Serial.begin(115200);
  while (!Serial)
  {
  } // Wait until the Serial connection is ready

  // DEV: Add emg setup code here
  // emgSetup();
}

int getTestSignal() {
  testSignal = (testSignal + 1)%7;
  return testSignal;
}

void loop()
{
  // DEV: Add emg loop code here
  // int controlSignal = getControlSignal();
  int controlSignal = getTestSignal();

  updateControlSignal(controlSignal);

  if (PRINT_RPC_STREAM)
    printRPCStream();

  delay(2);
}

// On change of control signal, send it to the M4
// and also update in M7
void updateControlSignal(int controlSignal)
{
  
    RPC.send("updateControl", controlSignal);
    Serial.println("M7: Control Signal: " + String(controlSignal));

  // if (prevControlSignal != controlSignal)
  // {
  //   RPC.send("updateControl", controlSignal);
  //   // DEV: Fallback to RPC.call if RPC.send doesn't work
  //   // RPC.call("updateControl", controlSignal);
  //   Serial.println("M7: executing updateControl with " + String(controlSignal));
  //   prevControlSignal = controlSignal;
  // }
}

/**
 * Prints the RPC stream to the Serial port
 **/
void printRPCStream()
{
  // On M7, let's print everything that is received over the RPC1 stream interface
  // Buffer it, otherwise all characters will be interleaved by other prints
  String buffer = "";
  while (RPC.available())
  {
    buffer += (char)RPC.read(); // Fill the buffer with characters
  }

  if (buffer.length() > 0)
  {
    Serial.print(buffer);
  }
}
