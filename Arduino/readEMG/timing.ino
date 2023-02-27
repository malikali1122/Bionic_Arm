
// In order to make sure that the arduino runs at
// ADC sample frequence (sampleRate), delay is adjusted.
void maintainOperatingFrequency()
{
  // if runTime less than timeBudget, then you still have (timeBudget - runTime)
  // to do your work
  if (timeBudget > runTime)
  {
    delayMicroseconds(timeBudget - runTime);
    digitalWrite(ERROR_LED, LOW);
  }
  else
  {
    digitalWrite(ERROR_LED, HIGH);
  }
}

// When TIMING_DEBUG is high, console the timing values.
void timingDebug()
{
  Serial.println("");
  Serial.print("runTime: ");
  // the filter cost average around 520 us
  Serial.println(runTime);
  Serial.print("Time Budget: ");
  Serial.println(timeBudget);
  Serial.print("Difference:");
  Serial.println(timeBudget - runTime);
  Serial.println("");
}