#include "Arduino.h"
#include "ExportCSV.h"

// Constructor
ExportCSV::ExportCSV() : numSensors(1), buffer(""), columnHeaders("Time"), sensorDataArr("") {}
ExportCSV::ExportCSV(int sensorCount) : numSensors(sensorCount), buffer(""), columnHeaders("Time"), sensorDataArr("") {}

void ExportCSV::init()
{
    // numSensors = 1;

    // buffer[0] = 0;
    // columnHeaders[0] = 0;
    // sensorDataArr[0] = 0;
}

void ExportCSV::startTimer()
{
    startTime = millis();
}

void ExportCSV::setNumberofSensors(int num)
{
    numSensors = num;
    setDefaultColHeaders();
}

void ExportCSV::setDefaultColHeaders()
{
    // strcat(columnHeaders, "Time, ");
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

void ExportCSV::setColHeaders(char *cols)
{
    columnHeaders[0] = 0;
    strcat(columnHeaders, "Time, ");
    strcat(columnHeaders, cols);
}

void ExportCSV::exportCSVColHeaders()
{
    Serial.println("");
    Serial.println(columnHeaders);
}

void ExportCSV::storeCurrentTime()
{
    ltoa(millis() - startTime, buffer, 10);
    strcat(sensorDataArr, buffer);
    strcat(sensorDataArr, ", ");
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