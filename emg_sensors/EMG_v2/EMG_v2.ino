#include "EMG_Sensor.h"
#include "EMGFilters.h"
#include "ExportCSV_.h"

#define SENSOR1_PIN A1
#define SENSOR2_PIN A2

#define ERROR_LED 13

// Modify value according to number of sensors used
#define SENSOR_COUNT 2
// Set 1 for Serial Plotting and 0 for Putty CSV Export
int enableSerialPlot = 1;

// Set 0 if Timing o/p need not be printed
#define TIMING_DEBUG 0

unsigned long runTime;
unsigned long timeBudget;
unsigned long startTime;

// discrete filters must works with fixed sample frequence
// our emg filter only support "SAMPLE_FREQ_500HZ" or "SAMPLE_FREQ_1000HZ"
// other sampleRate inputs will bypass all the EMG_FILTER
int sampleRate = SAMPLE_FREQ_500HZ;

EMG_Sensor emg[SENSOR_COUNT] = { EMG_Sensor(SENSOR1_PIN, sampleRate), EMG_Sensor(SENSOR2_PIN, sampleRate)};

ExportCSV myCSV(SENSOR_COUNT);

void setup() {
  // open serial
  Serial.begin(115200);

  pinMode(ERROR_LED, OUTPUT);

    // setup for time cost measure
    // using micros()
    timeBudget = 1e6 / sampleRate;
  // micros will overflow and auto return to zero every 70 minutes

  initialiseSensors();

  if (enableSerialPlot){
    myCSV.enableSerialPlotter();
  }

  startTime = millis();
  myCSV.setupExportCSV(startTime);
  // myCSV.setupExportCSV(startTime, "Bicep, Tricep");   // Use this function call instead to set specific column headers for sensors
  myCSV.exportCSVColHeaders();
}

void loop() {
  /* add main program code here */
  /*------------start here-------------------*/
  runTime = micros();

  myCSV.storeCurrentTime();

  streamSensorData();

  runTime = micros() - runTime;

  timingDebug();

  /*------------end here---------------------*/

  // In order to make sure the operating frequency of the code
  // matches the sampling rate
  maintainOperatingFrequency();
}
