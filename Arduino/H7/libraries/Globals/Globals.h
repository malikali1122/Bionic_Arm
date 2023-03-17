#ifndef GLOBALS_H
#define GLOBALS_H

#include "EMGFilters.h"

extern int prevControlSignal;

// Modify value according to number of sensors used
constexpr int SENSOR_COUNT = 2;
// Set 1 for Serial Plotting and 0 for Putty CSV Export
extern int enableSerialPlot;

// Set 0 if Timing o/p need not be printed
extern const int TIMING_DEBUG;

extern unsigned long runTime;
extern unsigned long timeBudget;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// START OF TUNING PARAMETERS

extern const long intensityThreshold1;
extern const long intensityThreshold2;

extern const long durationThreshold1;
extern const long durationThreshold2;

extern const int averageLength;
extern const int envelopeReach;

// END OF TUNING PARAMETERS
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

extern int envelopeIndex1;
extern int valueArray1;
extern int temp1;
extern int temp2;
extern long averageVal1;
extern long envelopeVal;
extern int signalReadings1[];
extern int eventFlag1;
extern long eventArea1;
extern long eventStartTime1;
extern long eventStopTime1;
extern long previousEnvelope1;
extern int gestureID;

extern long averageVal2;
extern int envelopeIndex2;
extern int signalReadings2[];
extern int eventFlag2;
extern long eventArea2;
extern long eventStartTime2;
extern long eventStopTime2;
extern long previousEnvelope2;

extern int channelID;

extern SAMPLE_FREQUENCY sampleRate;

extern unsigned long loopStartTime;
extern unsigned long nsTimeLapsed;
extern unsigned long nsTimeBudget;

extern int toggleFist;
extern int toggleElbow;

#endif // GLOBALS_H
