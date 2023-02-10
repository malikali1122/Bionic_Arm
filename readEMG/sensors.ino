// Initialise the EMG sensors
void initialiseSensors() {
  for (int i = 0; i < SENSOR_COUNT; i++) {
    emg[i].init();
  }
}

// Stream the 
void streamSensorData() {
  for (int i = 0; i < SENSOR_COUNT; i++) {
    int value = emg[i].readSensorData();
    Serial.print(value);
  }
  Serial.println();
}