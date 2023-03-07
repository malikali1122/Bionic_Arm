#include <Arduino.h>
#include <Servo.h>
#include "ServoControl.h"

ServoControl sir_vogh(15); // pun; on pin 15

int srvMax, srvMin;
void setup() {
  Serial.begin(9600);
  sir_vogh.attach(3);

  sir_vogh.moveTo(0);  
  delay(500); 
  sir_vogh.synchronize(); // start-up sync to find potentiometer readings at servo limits
}

void loop() {
  sir_vogh.move(1);
}