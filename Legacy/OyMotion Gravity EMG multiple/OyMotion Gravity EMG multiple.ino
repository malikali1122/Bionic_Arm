/*
* Copyright 2017, OYMotion Inc.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in
*    the documentation and/or other materials provided with the
*    distribution.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
* COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
* INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
* BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
* OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
* AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
* THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*/

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "EMGFilters.h"

void exportColumnHeaders(void);
void exportCurrentTimeToPutty(void);
void exportSignalDataToPutty(int value);

#define TIMING_DEBUG 1

// Modify value according to number of sensors used
#define SENSORS_COUNT 3

char colStr[100] = "";
char dataStr[100] = "";
char buffer[10];

EMGFilters myFilter;
// discrete filters must works with fixed sample frequence
// our emg filter only support "SAMPLE_FREQ_500HZ" or "SAMPLE_FREQ_1000HZ"
// other sampleRate inputs will bypass all the EMG_FILTER
int sampleRate = SAMPLE_FREQ_1000HZ;
// For countries where power transmission is at 50 Hz
// For countries where power transmission is at 60 Hz, need to change to
// "NOTCH_FREQ_60HZ"
// our emg filter only support 50Hz and 60Hz input
// other inputs will bypass all the EMG_FILTER
int humFreq = NOTCH_FREQ_50HZ;

// Calibration:
// put on the sensors, and release your muscles;
// wait a few seconds, and select the max value as the threshold;
// any value under threshold will be set to zero
static int Threshold = 16;

unsigned long timeStamp;
unsigned long timeBudget;
unsigned long runTime; //arduino reading time

int sensorPins[4] = {A1, A2, A3, A4};

void getSensorReading(int);

void setup() {
    /* add setup code here */
    myFilter.init(sampleRate, humFreq, true, true, true);

    // open serial
    Serial.begin(115200);

    // setup for time cost measure
    // using micros()
    timeBudget = 1e6 / sampleRate;
    // micros will overflow and auto return to zero every 70 minutes

    exportColumnHeaders();
}

void loop() {
    /* add main program code here */
    // In order to make sure the ADC sample frequence on arduino,
    // the time cost should be measured each loop
    /*------------start here-------------------*/

    exportCurrentTimeToPutty();
    
    for (int i = 0; i < SENSORS_COUNT; i++){
      getSensorReading(sensorPins[i]);
    }

    Serial.println(" ");

    /*------------end here---------------------*/
    // if less than timeBudget, then you still have (timeBudget - timeStamp) to
    // do your work
    delayMicroseconds(500);
    // if more than timeBudget, the sample rate need to reduce to
    // SAMPLE_FREQ_500HZ
}

void getSensorReading(int sensorNumber) {
    timeStamp = micros();

    int Value = analogRead(sensorNumber);

    // filter processing
    int DataAfterFilter = myFilter.update(Value);

    int envlope = sq(DataAfterFilter);
    // any value under threshold will be set to zero
    envlope = (envlope > Threshold) ? envlope : 0;

    timeStamp = micros() - timeStamp;

    exportSignalDataToPutty(envlope);
    // if (TIMING_DEBUG) {
    //     // Serial.print("Read Data: "); Serial.println(Value);
    //     // Serial.print("Filtered Data: ");Serial.println(DataAfterFilter);
    //     // Serial.print(sensorNumber);
    //     // Serial.print(" Squared Data:");
    //     Serial.print(envlope);
    //     Serial.print(",");
    //     // Serial.print("Filters cost time: "); Serial.println(timeStamp);
    //     // the filter cost average around 520 us
    // }
}

void exportColumnHeaders(void){
    colStr[0] = 0;  //clean out string

    strcat(colStr, "Time");
    strcat(colStr, ", "); //append the delimiter

    for (int i = 0; i < SENSORS_COUNT; i++){
      strcat(colStr, "Sensor");
      sprintf(buffer, "%d", i+1);
      strcat(colStr, buffer);
      strcat(colStr, ", "); //append the delimiter
    }

    Serial.println(colStr);

}

void exportCurrentTimeToPutty(void){
    dataStr[0] = 0; //clean out string
    ltoa(millis(),buffer,10); //convert long to charStr
    strcat(dataStr, buffer); //add it to the end
    strcat(dataStr, ", "); //append the delimiter
    Serial.print(dataStr);
}

void exportSignalDataToPutty(int value){
    dataStr[0] = 0; //clean out string
    sprintf(buffer, "%d", value);
    strcat(dataStr, buffer); //add it to the end
    strcat(dataStr, ", "); //append the delimiter
    Serial.print(dataStr);
}