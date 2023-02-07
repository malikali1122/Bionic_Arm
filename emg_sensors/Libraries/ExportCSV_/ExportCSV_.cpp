#include "Arduino.h"
#include "ExportCSV_.h"

// Constructor
ExportCSV_::ExportCSV_() : numSensors(1), buffer(""), columnHeaders(""), sensorDataArr(""), serialPlotterFlag(0) {}
ExportCSV_::ExportCSV_(int sensorCount) : numSensors(sensorCount), buffer(""), columnHeaders(""), sensorDataArr(""), serialPlotterFlag(0) {}

void ExportCSV_::enableSerialPlotter()
{
    serialPlotterFlag = 1;
}

void ExportCSV_::setupExportCSV(unsigned long startingTime)
{
    if (!serialPlotterFlag)
    {
        strcat(columnHeaders, "Time, ");
    }
    setDefaultColHeaders();
    startTime = startingTime;
}

void ExportCSV_::setupExportCSV(unsigned long startingTime, char *cols)
{
    if (!serialPlotterFlag)
    {
        strcat(columnHeaders, "Time, ");
    }
    strcat(columnHeaders, cols);
    startTime = startingTime;
}

void ExportCSV_::setDefaultColHeaders()
{
    for (int i = 0; i < numSensors; i++)
    {
        if (i == (numSensors - 1))
        {
            sprintf(buffer, "Sensor%d", i + 1);
            strcat(columnHeaders, buffer);
        }
        else
        {
            sprintf(buffer, "Sensor%d, ", i + 1);
            strcat(columnHeaders, buffer);
        }
    }
}

void ExportCSV_::exportCSVColHeaders()
{
    Serial.println("");
    Serial.println(columnHeaders);
}

void ExportCSV_::storeCurrentTime()
{
    if (!serialPlotterFlag)
    {
        ltoa(millis() - startTime, buffer, 10);
        strcat(sensorDataArr, buffer);
        strcat(sensorDataArr, ", ");
    }
}

void ExportCSV_::storeSensorData(int sensorVal)
{
    sprintf(buffer, "%d, ", sensorVal);
    strcat(sensorDataArr, buffer);
}

void ExportCSV_::exportDataRow()
{
    Serial.println(sensorDataArr);
    sensorDataArr[0] = 0;
}