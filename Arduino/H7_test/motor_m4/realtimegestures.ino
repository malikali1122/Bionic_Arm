

void motorSetup_(){
  srvArrHand[0].attach(THUMB_PIN);
  srvArrHand[1].attach(THUMB_BASE_PIN);
  srvArrHand[2].attach(INDEX_PIN);
  srvArrHand[3].attach(MIDDLE_PIN);
  srvArrHand[4].attach(RING_PINKY_PIN);

  srvArrElbow[0].attach(ELBOW_L_PIN);
  srvArrElbow[1].attach(ELBOW_R_PIN);

  nsTimeBudget = 2000;
  // gesturesPtr = new RealTimeGestures(srvArrHand, srvArrElbow, 2);

  // // safe start position
  srvArrHand[0].write(180);
  // //srvArrHand[0].write(0); // WITHOUT MOVEABLE THUMB
  // srvArrHand[1].write(0);
  // srvArrHand[2].write(180);
  // srvArrHand[3].write(180);
  // srvArrHand[4].write(180);

  // srvArrElbow[0].write(100); // LEFT ELBOW MOTOR
  // srvArrElbow[1].write(80); // RIGHT ELBOW MOTOR

  delay(1000);
  // srvArrElbow[0].write(180); // LEFT ELBOW MOTOR
  // srvArrElbow[1].write(0); // RIGHT ELBOW MOTOR
  // delay(1000);
}



void motorSetup(){
  srvArrHand[0].attach(THUMB_PIN);
  srvArrHand[1].attach(THUMB_BASE_PIN);
  srvArrHand[2].attach(INDEX_PIN);
  srvArrHand[3].attach(MIDDLE_PIN);
  srvArrHand[4].attach(RING_PINKY_PIN);

  srvArrElbow[0].attach(ELBOW_L_PIN);
  srvArrElbow[1].attach(ELBOW_R_PIN);

  nsTimeBudget = 2000;
  // gesturesPtr = new RealTimeGestures(srvArrHand, srvArrElbow, 2);

  // // safe start position
  // srvArrHand[0].write(180);
  // //srvArrHand[0].write(0); // WITHOUT MOVEABLE THUMB
  // srvArrHand[1].write(0);
  // srvArrHand[2].write(180);
  // srvArrHand[3].write(180);
  // srvArrHand[4].write(180);

  // srvArrElbow[0].write(100); // LEFT ELBOW MOTOR
  // srvArrElbow[1].write(80); // RIGHT ELBOW MOTOR

  delay(1000);
  srvArrHand[0].write(90);
  delay(1000);
  // srvArrElbow[0].write(180); // LEFT ELBOW MOTOR
  // srvArrElbow[1].write(0); // RIGHT ELBOW MOTOR
  // delay(1000);
}

void motorLoop() {
  loopStartTime = micros();
  // (*gesturesPtr).periodicUpdate();

  for(int i=0; i<5; i++) {
    srvArrHand[i].write(180);
    delay(1000);
    srvArrHand[i].write(0);
    delay(1000);
  }

  
  srvArrElbow[0].write(100);
  srvArrElbow[1].write(80);
  delay(1000);

  
  
  srvArrElbow[0].write(180);
  srvArrElbow[1].write(0);
  delay(1000);

  RPC.println("Inside motorLoop");

  switch(controlSignal) {
      RPC.println("Inside switch");
      case 3:
        RPC.println("Doing 3");
        // (*gesturesPtr).toggleFist();
        break;
      case 6:
        RPC.println("Doing 6");
        // (*gesturesPtr).toggleElbow();
        break;
      default:
        RPC.println("Invalid input");
        break;
  }
    
  nsTimeLapsed = micros() - loopStartTime;
  if (nsTimeLapsed <  nsTimeBudget) {
    delayMicroseconds(nsTimeBudget - nsTimeLapsed);
  }
}

