#include "EMG_Sensor.h"

#define sensorPin1 A0
#define sensorPin2 A1
#define sensorPin3 A3

unsigned long runTime;
unsigned long timeBudget;

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
  if (TIMING_DEBUG)
  {
      // Serial.print("Read Data: "); Serial.println(value);
      // Serial.print("Filtered Data: ");Serial.println(dataAfterFilter);
      Serial.print("Squared Data: ");
      Serial.println(envlope);
      // Serial.print("Filters cost time: "); Serial.println(runTime);
      // the filter cost average around 520 us
  }

  /*------------end here---------------------*/
  // if runTime less than timeBudget, then you still have (timeBudget - runTime)
  // to do your work
  if(timeBudget > runTime) 
    delayMicroseconds(timeBudget - runTime);
  else {
    Serial.print("ERROR: runTime exceeds maximum possible value.\nrunTime: ");
    Serial.print(runTime);
    Serial.print(", timeBudget: ");
    Serial.println(timeBudget);
  }
  // if more than timeBudget, the sample rate need to reduce to
  // SAMPLE_FREQ_500HZ

}
