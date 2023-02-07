// include guard
#ifndef ExportCSV_H
#define ExportCSV_H

// include Arduino library
#include "Arduino.h"

class ExportCSV_
{
  public:
    ExportCSV_();
    ExportCSV_(int sensorCount);
    void enableSerialPlotter();
    void setupExportCSV(unsigned long startingTime);
    void setupExportCSV(unsigned long startingTime, char *cols);
    void exportCSVColHeaders();
    void storeCurrentTime();
    void storeSensorData(int sensorData);
    void exportDataRow();

  private:
    // Number of EMG Sensors Used
    int numSensors;

    // Variables to store ExportData
    unsigned long startTime;
    char buffer[100];
    char columnHeaders[100];
    char sensorDataArr[100];

    // Flag to Toggle between Serial Plotting and Putty CSV Export
    int serialPlotterFlag;

    // Private Functions
    void setDefaultColHeaders();
};

#endif
