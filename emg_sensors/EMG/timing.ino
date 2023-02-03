
// In order to make sure that the arduino runs at
// ADC sample frequence (sampleRate), delay is adjusted.
void maintainOperatingFrequency() {
  // if runTime less than timeBudget, then you still have (timeBudget - runTime)
  // to do your work
  if (timeBudget > runTime)
    delayMicroseconds(timeBudget - runTime);
  else {
    // if more than timeBudget, the sample rate need to reduce to
    // SAMPLE_FREQ_500HZ
    Serial.print("ERROR: runTime exceeds maximum possible value, thus deviating from the sampleRate.\nrunTime: ");
    Serial.print(runTime);
    Serial.print(", timeBudget: ");
    Serial.println(timeBudget);

    if (sampleRate == SAMPLE_FREQ_1000HZ)
      Serial.println("Current sampleRate is 1000Hz. Either reduce the runTime or reduce the sampleRate to 500Hz.") else Serial.println("Current sampleRate is 500Hz. Further reduction in sample rete is not possible. Try to reduce the runTime further.")
  }
}

// When TIMING_DEBUG is high, console the timing values.
void timingDebug() {
  if (TIMING_DEBUG) {
    Serial.println("")
    Serial.print("Filters cost time: ");
    // the filter cost average around 520 us
    Serial.println(runTime);
    Serial.print("Time Budget: ");
    Serial.println(timeBudget);
    Serial.print("Difference:")
    Serial.println(timeBudget - runTime)
    Serial.println("")
  }
}