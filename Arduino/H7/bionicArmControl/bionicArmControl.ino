/*
    Bionic Arm Control

    * This is developed for the Portenta H7 board

    This file contains the main loop for the bionic arm control system.
    - emg part
    - motor part
*/

#include "Arduino.h"
#include "RPC.h"
#include <Servo.h>
#include "EMG_Sensor.h"
#include "EMGFilters.h"
#include "pins.h"
#include "Globals.h"
#include "RealTimeGestures.h"

/* Instantiate EMG_Sensors
    EMG_Sensor(int sensorPin, SAMPLE_FREQUENCY sampleRate, int threshold);
*/
EMG_Sensor emg[SENSOR_COUNT] = {EMG_Sensor(SENSOR1_PIN, sampleRate, 10), EMG_Sensor(SENSOR2_PIN, sampleRate, 10)};

// Instantiate Servo objects
Servo srvArrHand[5]; // array of servo objects for hand
Servo srvArrElbow[2]; 
Servo thumb;
Servo thumbBase;
Servo indexMiddle;
Servo ringPinky;

// Instantiate RealTimeGestures object
RealTimeGestures *gesturesPtr;

void setup()
{
  Serial.begin(115200);

  emgSetup();
  motorSetup();
}

/*
    NOTE:
    - Both emgLoop & motorLoop has implementations for maintaining the operating frequency
        * This should be combined together as a single implementation in the main loop
    - This code is not tested on the Arm yet!!!

*/

void loop()
{
  emgLoop();
  motorLoop();
}
