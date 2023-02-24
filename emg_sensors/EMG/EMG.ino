#include "EMG_Sensor.h"
#include "EMGFilters.h"
#include "ExportCSV.h"

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

int envelopeIndex1 = 0;
int valueArray1;
int temp1 = 0; 
int temp2 = 0; 
long averageVal1 = 0;
long envelopeVal1 = 0;
const int envelopeReach1 = 100; // Stating that N data points will be used in the envelope
int signalReadings1[envelopeReach1];
int eventFlag1 = 0; // Toggle between 0 and 1 if the envelope rises above 0 or falls back to 0 (start and end flag)
long eventArea1 = 0; // Variable to track the intensity (cumulative) as an event occurs
long eventStartTime1 = 0; // Variable to record the start time for any one event
long eventStopTime1 = 0; // Variable to record the stop time for any one event
long previousEnvelope1 = 0;
int gestureID1 = 0; // Initial state for the gesture identifier

long averageVal2 = 0;
long envelopeVal2 = 0;
const int envelopeReach2 = 100; // Stating that N data points will be used in the envelope
int signalReadings2[envelopeReach2];
int eventFlag2 = 0; // Toggle between 0 and 1 if the envelope rises above 0 or falls back to 0 (start and end flag)
long eventArea2 = 0; // Variable to track the intensity (cumulative) as an event occurs
long eventStartTime2 = 0; // Variable to record the start time for any one event
long eventStopTime2 = 0; // Variable to record the stop time for any one event
long previousEnvelope2 = 0;
int gestureID2 = 0; // Initial state for the gesture identifier


// discrete filters must works with fixed sample frequence
// our emg filter only support "SAMPLE_FREQ_500HZ" or "SAMPLE_FREQ_1000HZ"
// other sampleRate inputs will bypass all the EMG_FILTER
int sampleRate = SAMPLE_FREQ_500HZ;

EMG_Sensor emg[SENSOR_COUNT] = {EMG_Sensor(SENSOR1_PIN, sampleRate, 10), EMG_Sensor(SENSOR2_PIN, sampleRate, 10)};


ExportCSV myCSV(SENSOR_COUNT);

void setup()
{
  // open serial
  Serial.begin(115200);

  pinMode(ERROR_LED, OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(4,INPUT);
  pinMode(5,INPUT);

  // setup for time cost measure
  // using micros()
  timeBudget = 1e6 / sampleRate;
  // micros will overflow and auto return to zero every 70 minutes

  initialiseSensors();

  if (enableSerialPlot){
    myCSV.enableSerialPlotter();
  }

  startTime = millis();
  // myCSV.setupExportCSV(startTime);
  myCSV.setupExportCSV(startTime, "Gesture ID");   // Use this function call instead to set specific column headers for sensors
  myCSV.exportCSVColHeaders();
}

void loop()
{
  /* add main program code here */
  /*------------start here-------------------*/
  runTime = micros();

  myCSV.storeCurrentTime();

  temp1 = streamSensorData(1);
  temp2 = streamSensorData(2);
  
  averageVal1 = smoothing(temp1);
  averageVal2 = smoothing(temp2);
  envelopeVal1 = envelope(averageVal1);
  envelopeVal2 = envelope(averageVal2);
  
  
  //Serial.println(averageVal1);
  //Serial.println(envelopeVal1);
  
  
  if(digitalRead(2)==HIGH){
    gestureID = 1;
  }
  if(digitalRead(3)==HIGH){
    gestureID = 2;
  }
  if(digitalRead(4)==HIGH){
    gestureID = 3;
  }
  if(digitalRead(5)==HIGH){
    gestureID = 4;
  }

  if(envelopeVal1 == 1){
    Serial.println(gestureID); // Only print the gesture ID upon completion of gesture
  }

  if(envelopeVal2 == 1){
    Serial.println(gestureID); // Only print the gesture ID upon completion of gesture
  }
  
  runTime = micros() - runTime;

  timingDebug();

  /*------------end here---------------------*/

  // In order to make sure the operating frequency of the code
  // matches the sampling rate
  maintainOperatingFrequency();
}

long smoothing(int temp1){
  long movingAverage;
  const int averageLength = 1000;
  int readings1[averageLength]; // Length of the averaging filter to be applied.
  int readings2[averageLength];
  int readIndex1 = 0;
  int readIndex2 = 0;
  long total1 = 0;
  long total2 = 0;
  
  total1 = total1 - readings1[readIndex1];
  readings1[readIndex1] = temp1;
  total1 = total1 + readings1[readIndex1];
  readIndex1 = readIndex1 + 1;

  if(readIndex1 >= averageLength){
    readIndex1 = 0;
  }

  movingAverage = total1/averageLength;

  return movingAverage;
  
}

long envelope(long temp1){
  long envelopePeakValue=0; // Declaring a variable in which to store the maximum recorded value
  
  long tempStorage = 0;
  long eventDuration = 0;
  int printFlag = 0;
  int currentReading=0;
  int i; // For use as a counter / iterator

  signalReadings[envelopeIndex] = temp1; // Storing the most recent reading in the array
  envelopeIndex = envelopeIndex+1;

  if(envelopeIndex >= envelopeReach){
    envelopeIndex = 0;
  }

  for(i=0;i<envelopeReach;i++){ // Finding the largest element in the current envelope reach
    currentReading = signalReadings[i];
    if(currentReading > envelopePeakValue){ // Comparing to the current largest value in the array
      envelopePeakValue = currentReading; // If larger, assign as the new largest.
    }
  }

  // NOW CHECKING FOR EVENTS

  if((eventFlag == 1)&&(envelopePeakValue > 0)){ // If the envelope is above zero (and during an event)
      eventArea = eventArea + (envelopePeakValue*envelopePeakValue); // Adding on the squared envelope value
  }

  if((previousEnvelope == 0)&&(envelopePeakValue > 0)){ // If the envelope has just picked up above 0 (CAN ONLY BE TRUE ONCE)
    eventFlag = 1; // Signal the start of an event
    eventArea = eventArea + (envelopePeakValue*envelopePeakValue); // Adding on the squared envelope value
    eventStartTime = millis(); // Or some function to record the current time at the event start point
  }

  if((previousEnvelope > 0)&&(envelopePeakValue == 0)&&(eventFlag == 1)){ // If the envelope has just fallen back to 0 (CAN ONLY BE TRUE ONCE)
    eventStopTime = millis();
    eventDuration = eventStopTime - eventStartTime;
    eventFlag = 0;
    // Now need to save these features / export them
    //Serial.print("Intensity:");
    Serial.print(eventArea);
    Serial.print(",");
    //Serial.print("Duration:");
    Serial.print(eventDuration);
    Serial.print(",");
    eventDuration=0; // Resetting trackers
    eventStartTime=0;
    eventStopTime=0;
    eventArea=0;
    printFlag = 1;
  }

  tempStorage = envelopePeakValue; // Store the current largest envelope value
  previousEnvelope = envelopePeakValue;
  envelopePeakValue = 0; // Set the current best back to zero ready for the next function call
  
  //return tempStorage; // Return the current envelope peak value

  return printFlag;
  
}
  
