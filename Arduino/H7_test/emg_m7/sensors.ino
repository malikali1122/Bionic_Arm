// Initialise the EMG sensors
void initialiseSensors() {
  for (int i = 0; i < SENSOR_COUNT; i++) {
    emg[i].init();
  }
}

/*   Stream the sensor data
  The data streamed is received by pyserial module. 
  It reads the data by using ser.readline() which stops reading only
  on finding a end of line character.
  => Using println function so that each sensor value is received 
  seperately
*/

// Temporarily commented!!!!!!!

// void streamSensorData() {
//   for (int i = 0; i < SENSOR_COUNT; i++) {
//     int value = emg[i].readSensorData();
//     Serial.println(value);
//   }
//   Serial.println();
// }

// DEV: change as required

int streamSensorData(int SENSOR_ID) {
  
  int value = emg[SENSOR_ID].readSensorData();
  //myCSV.storeSensorData(value); temporarily commented!!!

 // myCSV.exportDataRow(); temporarily commented!!
 
  return value;
}
