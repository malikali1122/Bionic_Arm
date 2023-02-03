#include "EMG_Sensor.h"
#include "EMGFilters.h"

#define sensorPin1 A0
#define sensorPin2 A1
#define sensorPin3 A3

#define TIMING_DEBUG 1

unsigned long runTime;
unsigned long timeBudget;

// discrete filters must works with fixed sample frequence
// our emg filter only support "SAMPLE_FREQ_500HZ" or "SAMPLE_FREQ_1000HZ"
// other sampleRate inputs will bypass all the EMG_FILTER
int sampleRate = SAMPLE_FREQ_1000HZ;

void setup() {
  // put your setup code here, to run once:

  // setup for time cost measure
  // using micros()
  timeBudget = 1e6 / sampleRate;
  // micros will overflow and auto return to zero every 70 minutes

}

void loop() {
  /* add main program code here */
  // In order to make sure the ADC sample frequence on arduino,
  // the time cost should be measured each loop
  /*------------start here-------------------*/
  runTime = micros();

  runTime = micros() - runTime;
  timingDebug();

  /*------------end here---------------------*/
  maintainOperatingFrequency();
}
