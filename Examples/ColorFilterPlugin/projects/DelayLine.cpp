#include "DelayLine.h"

//// Constructor: Initialize the delay line with a specific delay (in samples)
//DelayLine::DelayLine(size_t delay) { setDelay(delay); }
//
//// Set the delay (in samples)
//void DelayLine::setDelay(size_t delay)
//{
//  buffer.resize(delay, 0.0f); // Resize the buffer and fill with zeros
//  position = 0;
//}
//
//// Process a single sample: Add it to the delay line and return the delayed sample
//double DelayLine::process(double input)
//{
//  if (buffer.empty())
//  {
//    return input; // If delay is 0, return the input directly
//  }
//
//  double output = buffer[position];           // Get the delayed sample
//  buffer[position] = input;                  // Store the new input sample
//  position = (position + 1) % buffer.size(); // Move the position forward
//  return output;
//}