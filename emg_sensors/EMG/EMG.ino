#include "EMG_Sensor.h"
#include "EMGFilters.h"

#define sensor1Pin A0
#define sensor2Pin A1
#define sensor3Pin A3

// Set 0 if Timing o/p need not be printed
#define TIMING_DEBUG 0

unsigned long runTime;
unsigned long timeBudget;

// discrete filters must works with fixed sample frequence
// our emg filter only support "SAMPLE_FREQ_500HZ" or "SAMPLE_FREQ_1000HZ"
// other sampleRate inputs will bypass all the EMG_FILTER
int sampleRate = SAMPLE_FREQ_1000HZ;

EMG_Sensor Sensor1(sensor1Pin, sampleRate);

void setup() {
  // open serial
  Serial.begin(115200);

  // setup for time cost measure
  // using micros()
  timeBudget = 1e6 / sampleRate;
  // micros will overflow and auto return to zero every 70 minutes

  Sensor1.init();
}

void loop() {
  /* add main program code here */
  /*------------start here-------------------*/
  runTime = micros();

  int value = Sensor1.readSensorData();







  runTime = micros() - runTime;

  timingDebug();

  /*------------end here---------------------*/

  // In order to make sure the operating frequency of the code
  // matches the sampling rate
  maintainOperatingFrequency();
}
