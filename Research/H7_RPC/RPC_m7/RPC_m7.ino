#include "Arduino.h"
#include "RPC.h"

/**
 * Adds two numbers and returns the sum
 **/
int add(int a, int b)
{
  Serial.println("M7: executing add with " + String(a) + " and " + String(b));
  delay(700); // Simulate work
  return a + b;
}

void setup()
{

  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize RPC library; this also boots the M4 core
  RPC.begin();
  Serial.begin(115200);
  while (!Serial) {} // Wait until the Serial connection is ready

  // M7 CPU becomes the server, so it makes two functions available under the defined names
  RPC.bind("remoteAdd", add);
}

void loop()
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
