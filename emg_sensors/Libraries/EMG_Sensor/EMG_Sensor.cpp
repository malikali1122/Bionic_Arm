
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

#include "Arduino.h"
#include "EMG_Sensor.h"

#include "EMGFilters.h"

EMGFilters myFilter;

// Constructor
EMG_Sensor::EMG_Sensor(int sensorPin, int sampleRate) : // private variables
                                                        sensorPin(sensorPin),
                                                        sampleRate(sampleRate),

                                                        // Calibration:
                                                        // put on the sensors, and release your muscles;
                                                        // wait a few seconds, and select the max value as the threshold;
                                                        // any value under threshold will be set to zero
                                                        threshold(0),

                                                        // For countries where power transmission is at 50 Hz
                                                        // For countries where power transmission is at 60 Hz, need to change to
                                                        // "NOTCH_FREQ_60HZ"
                                                        // our emg filter only support 50Hz and 60Hz input
                                                        // other inputs will bypass all the EMG_FILTER
                                                        humFreq(NOTCH_FREQ_50HZ),

                                                        myFilter()
{
}

// Initialize the sensor
void EMG_Sensor::init()
{
    // Initialise the filter
    myFilter.init(sampleRate, humFreq, true, true, true);
}

// Set the threshold
void EMG_Sensor::setThreshold(int threshold)
{
    this->threshold = threshold;
}

// Read the sensor data
int EMG_Sensor::readSensorData()
{
    int value = analogRead(sensorPin);

    // filter processing
    int dataAfterFilter = myFilter.update(value);

    int envelope = sq(dataAfterFilter);
    // any value under threshold will be set to zero
    envelope = (envelope > threshold) ? envelope : 0;

    return envelope;
}

// void setup()
// {
//     // /* add setup code here */
//     // myFilter.init(sampleRate, humFreq, true, true, true);

//     // // open serial
//     // Serial.begin(115200);
// }

// void loop()
// {
//     /* add main program code here */
//     // In order to make sure the ADC sample frequence on arduino,
//     // the time cost should be measured each loop
//     /*------------start here-------------------*/
//     timeStamp = micros();

//     int value = analogRead(SensorInputPin);

//     // filter processing
//     int dataAfterFilter = myFilter.update(value);

//     int envelope = sq(dataAfterFilter);
//     // any value under threshold will be set to zero
//     envelope = (envelope > threshold) ? envelope : 0;

//     timeStamp = micros() - timeStamp;
//     if (TIMING_DEBUG)
//     {
//         // Serial.print("Read Data: "); Serial.println(value);
//         // Serial.print("Filtered Data: ");Serial.println(dataAfterFilter);
//         Serial.print("Squared Data: ");
//         Serial.println(envelope);
//         // Serial.print("Filters cost time: "); Serial.println(timeStamp);
//         // the filter cost average around 520 us
//     }

//     /*------------end here---------------------*/
//     // if less than timeBudget, then you still have (timeBudget - timeStamp) to
//     // do your work
//     delayMicroseconds(500);
//     // if more than timeBudget, the sample rate need to reduce to
//     // SAMPLE_FREQ_500HZ
// }