// include guard
#ifndef EMG_SENSOR_H
#define EMG_SENSOR_H

// include Arduino library
#include "Arduino.h"

#include "EMGFilters.h"

class EMG_Sensor
{
  public:
    EMG_Sensor(int sensorPin, SAMPLE_FREQUENCY sampleRate, int threshold);
    void init();
    int readSensorData();

  private:
    int sensorPin;
    int threshold;
    SAMPLE_FREQUENCY sampleRate;
    NOTCH_FREQUENCY humFreq;
    // char muscle[];

    EMGFilters myFilter;
};

#endif
