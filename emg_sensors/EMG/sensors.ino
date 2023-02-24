// Initialise the EMG sensors
void initialiseSensors() {
  for (int i = 0; i < SENSOR_COUNT; i++) {
    emg[i].init();
  }
}

// Stream the 
int streamSensorData(int SENSOR_ID) {
  int temp[SENSOR_COUNT];
  for (int i = 0; i < SENSOR_COUNT; i++) {
    int value = emg[i].readSensorData();
    //myCSV.storeSensorData(value); temporarily commented!!!
    temp[i] = value;
  }
  
 // myCSV.exportDataRow(); temporarily commented!!
 
  return temp[SENSOR_ID-1];
}
