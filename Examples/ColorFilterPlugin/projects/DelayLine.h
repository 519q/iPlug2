#pragma once
#include <vector>

class DelayLine
{
public:
  // Constructor: Initialize the delay line with a specific delay (in samples)
  DelayLine(size_t delay = 0);

  // Set the delay (in samples)
  void setDelay(size_t delay);

  // Process a single sample: Add it to the delay line and return the delayed sample
  double process(double input);

private:
  std::vector<double> buffer; // Circular buffer to store delayed samples
  size_t position = 0;       // Current position in the buffer
};