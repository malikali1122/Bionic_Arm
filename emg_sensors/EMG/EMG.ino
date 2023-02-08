#include "EMG_Sensor.h"
#include "EMGFilters.h"
#include "ExportCSV.h"

#define SENSOR1_PIN A1
#define SENSOR2_PIN A2

#define ERROR_LED 13

// Modify value according to number of sensors used
#define SENSOR_COUNT 2

// Set 0 if Timing o/p need not be printed
#define TIMING_DEBUG 0

unsigned long runTime;
unsigned long timeBudget;

// discrete filters must works with fixed sample frequence
// our emg filter only support "SAMPLE_FREQ_500HZ" or "SAMPLE_FREQ_1000HZ"
// other sampleRate inputs will bypass all the EMG_FILTER
int sampleRate = SAMPLE_FREQ_500HZ;

EMG_Sensor emg[SENSOR_COUNT] = { EMG_Sensor(SENSOR1_PIN, sampleRate), EMG_Sensor(SENSOR2_PIN, sampleRate)};

ExportCSV myCSV;

void setup() {
  // open serial
  Serial.begin(115200);

  pinMode(ERROR_LED, OUTPUT);

    // setup for time cost measure
    // using micros()
    timeBudget = 1e6 / sampleRate;
  // micros will overflow and auto return to zero every 70 minutes

  initialiseSensors();

  myCSV.init();
  myCSV.setNumberofSensors(SENSOR_COUNT);
  myCSV.setColHeaders("Trap_L, Trap_R");
  myCSV.exportCSVColHeaders();
  myCSV.startTimer();

  	  // Set threshold
  emg[0].setThreshold(0);
  emg[1].setThreshold(0);
}

void loop() {
  /* add main program code here */
  /*------------start here-------------------*/
  runTime = micros();

  // myCSV.storeCurrentTime();

  streamSensorData();

  runTime = micros() - runTime;

  timingDebug();

  /*------------end here---------------------*/

  // In order to make sure the operating frequency of the code
  // matches the sampling rate
  maintainOperatingFrequency();
}
