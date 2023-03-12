#include "Arduino.h"
#include "RPC.h"

bool PRINT_RPC_STREAM = true;
int prevControlSignal = 0;

void setup()
{
  // Initialize RPC library; this also boots the M4 core
  RPC.begin();
  Serial.begin(115200);
  while (!Serial)
  {
  } // Wait until the Serial connection is ready

  // DEV: Add emg setup code here
}

void loop()
{
  // DEV: Add emg loop code here
  // int controlSignal = [get the new control signal]

  updateControlSignal(controlSignal);

  if (PRINT_RPC_STREAM)
    printRPCStream();
}

// On change of control signal, send it to the M4
// and also update in M7
void updateControlSignal(int controlSignal)
{
  if (prevControlSignal != controlSignal)
  {
    // DEV: Check if this function works
    RPC.callNonblocking("updateControl", controlSignal);
    // RPC.callAsync("updateControl", controlSignal);
    
    // DEV: Fallback
    // RPC.call("updateControl", controlSignal);
    prevControlSignal = controlSignal;
  }
}

/**
 * Prints the RPC stream to the Serial port
 **/
void printRPCStream()
{
  // On M7, let's print everything that is received over the RPC1 stream interface
  // Buffer it, otherwise all characters will be interleaved by other prints
  String buffer = "";
  while (RPC.available())
  {
    buffer += (char)RPC.read(); // Fill the buffer with characters
  }

  if (buffer.length() > 0)
  {
    Serial.print(buffer);
  }
}
