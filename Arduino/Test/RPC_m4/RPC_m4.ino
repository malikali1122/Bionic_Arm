#include "Arduino.h"
#include "RPC.h"

/**
 * Returns the CPU that's currently running the sketch (M7 or M4)
 * Note that the sketch has to be uploaded to both cores. 
 **/
String currentCPU() {
  if (HAL_GetCurrentCPUID() == CM7_CPUID) {
    return "M7";
  } else {
    return "M4";
  }
}

/**
 * Adds two numbers and returns the sum
 **/
int addOnM7(int a, int b) {
  Serial.println(currentCPU() + ": executing add with " + String(a) + " and " + String(b));
  delay(700); // Simulate work
  return a + b;
}

void setup() {

  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize RPC library; this also boots the M4 core
  RPC.begin();
  Serial.begin(115200);
  //while (!Serial) {} // Uncomment this to wait until the Serial connection is ready

  // Both CPUs will execute this instruction, just at different times
  randomSeed(analogRead(A0)); // Initializes the pseudo-random number generator

  if (currentCPU() == "M7") {
    // M7 CPU becomes the server, so it makes two functions available under the defined names
    RPC.bind("remoteAdd", addOnM7);
  } 
}

void loop() {

  if (currentCPU() == "M4") {
    // On M4 let's blink an LED. While it's blinking, the callSubstractFromM4() thread is running, 
    // so it will execute roughly 3 times (2000 / 700 ms)
    digitalWrite(LED_BUILTIN, LOW);
    delay(1000);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(1000);

    int a = random(100);
    int b = random(100);
    // PRC.print works like a Serial port, but it needs a receiver (in this case the M7) 
    // to actually print the strings to the Serial port
    RPC.println(currentCPU() + ": calling add with " + String(a) + " and " + String(b));
    // Let's invoke addOnM7() and wait for a result.
    // This will be delayed by the forced delay() in addOnM7() function
    // Exercise: if you are not interested in the result of the operation, what operation would you invoke?
    auto result = RPC.call("remoteAdd", a, b).as<int>();    
    RPC.println(currentCPU() + ": Result is " + String(a) + " + " + String(b) + " = " + String(result));
  }
  
  if (currentCPU() == "M7") {
    // On M7, let's print everything that is received over the RPC1 stream interface
    // Buffer it, otherwise all characters will be interleaved by other prints
    String buffer = "";
    while (RPC.available()) {
      buffer += (char)RPC.read(); // Fill the buffer with characters
    }

    if (buffer.length() > 0) {
      Serial.print(buffer);
    }
  }

}
