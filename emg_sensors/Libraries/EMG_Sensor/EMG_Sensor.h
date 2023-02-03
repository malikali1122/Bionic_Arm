// include guard
#ifndef IR_H
#define IR_H

// include Arduino library
#include "Arduino.h"

#include "EMGFilters.h"

class EMG_Sensor
{
  public:
    EMG_Sensor(int sensorPin, int sampleRate);
    void init();
    void setThreshold(int threshold);
    int readSensorData();

  private:
    int sensorPin;
    int threshold;
    int sampleRate;
    int humFreq;
    // char muscle[];

    EMGFilters myFilter;
};

#endif
