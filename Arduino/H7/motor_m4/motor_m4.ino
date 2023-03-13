#include "Arduino.h"
#include "RPC.h"

int controlSignal = 0;

/* Update the control signal */
void updateControl(int newControlSignal)
{
  RPC.println("M4: executing updateControl with " + String(newControlSignal));
  controlSignal = newControlSignal;
}

void setup()
{
  // Initialize RPC library; this also boots the M4 core
  RPC.begin();
  Serial.begin(115200);
  while (!Serial) {} // Wait until the Serial connection is ready

  // M4 CPU becomes the server, so it makes the functions available under the defined names
  RPC.bind("updateControl", updateControl);

  //DEV : Add motor setup code here

}

void loop()
{
  // DEV: Add motor loop code here
  // use controlSignal for switch case
}
