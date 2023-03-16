#include "globals.h"
#include "EMGFilters.h"

int prevControlSignal = 0;

// Set 1 for Serial Plotting and 0 for Putty CSV Export
int enableSerialPlot = 1;

// Set 0 if Timing o/p need not be printed
const int TIMING_DEBUG = 0;

unsigned long runTime = 0;
unsigned long timeBudget = 0;

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
int valueArray1 = 0;
int temp1 = 0;
int temp2 = 0;
long averageVal1 = 0;
long envelopeVal = 0;
int signalReadings1[envelopeReach] = {0};
int eventFlag1 = 0;       // Toggle between 0 and 1 if the envelope rises above 0 or falls back to 0 (start and end flag)
long eventArea1 = 0;      // Variable to track the intensity (cumulative) as an event occurs
long eventStartTime1 = 0; // Variable to record the start time for any one event
long eventStopTime1 = 0;  // Variable to record the stop time for any one event
long previousEnvelope1 = 0;
int gestureID = 0; // Initial state for the gesture identifier

long averageVal2 = 0;
int envelopeIndex2 = 0;
int signalReadings2[envelopeReach] = {0};
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