#include "Arduino.h"
#include "ExportCSV.h"

// Constructor
ExportCSV::ExportCSV() : numSensors(1), buffer(""), columnHeaders(""), sensorDataArr("") {}
ExportCSV::ExportCSV(int sensorCount) : numSensors(sensorCount), buffer(""), columnHeaders(""), sensorDataArr("") {}

void ExportCSV::enableSerialPlotter()
{
    serialPlotterFlag = 1;
}

void ExportCSV::setupExportCSV(unsigned long startingTime)
{
    if (!serialPlotterFlag)
    {
        strcat(columnHeaders, "Time, ");
    }
    setDefaultColHeaders();
    startTime = startingTime;
}

void ExportCSV::setupExportCSV(unsigned long startingTime, char *cols)
{
    if (!serialPlotterFlag)
    {
        strcat(columnHeaders, "Time, ");
    }
    strcat(columnHeaders, cols);
    startTime = startingTime;
}

void ExportCSV::setDefaultColHeaders()
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

void ExportCSV::exportCSVColHeaders()
{
    Serial.println("");
    Serial.println(columnHeaders);
}

void ExportCSV::storeCurrentTime()
{
    if (!serialPlotterFlag)
    {
        ltoa(millis() - startTime, buffer, 10);
        strcat(sensorDataArr, buffer);
        strcat(sensorDataArr, ", ");
    }
}

void ExportCSV::storeSensorData(int sensorVal)
{
    sprintf(buffer, "%d, ", sensorVal);
    strcat(sensorDataArr, buffer);
}

void ExportCSV::exportDataRow()
{
    Serial.println(sensorDataArr);
    sensorDataArr[0] = 0;
}