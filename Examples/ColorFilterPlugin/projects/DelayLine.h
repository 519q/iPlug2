#pragma once
#include <cmath>
#include <algorithm>
#include <vector>

class DelayLine
{
public:


  void setUpDelayLine(size_t maxDelay)
  {
    mBuffer.resize(maxDelay, 0.0);
    setDelay((double)maxDelay / 2); // Default delay at half max size}
  }
  // Set delay time (fractional values allowed)
  void setDelay(double delayTime) { mDelayTime = std::clamp(delayTime, 1.0, static_cast<double>(mMaxDelay - 1)); }

  // Set feedback amount
  void setFeedback(double feedback)
  {
    mFeedback = std::clamp(feedback, 0.0, 0.99); // Prevent runaway feedback
  }

  // Process a single sample with feedback and interpolation
  double process(double input)
  {
    // Compute floating-point read position
    double readPos = mWritePos - mDelayTime;
    if (readPos < 0)
      readPos += mMaxDelay;

    // Get integer and fractional part of read position
    int index1 = static_cast<int>(readPos);
    int index2 = (index1 + 1) % mMaxDelay;
    double frac = readPos - index1;

    // **Allpass interpolation**
    double output = mBuffer[index1] + frac * (mBuffer[index2] - mBuffer[index1]);

    // Write to delay buffer with feedback
    mBuffer[mWritePos] = input + (output * mFeedback);

    // Increment write position (circular buffer)
    mWritePos = (mWritePos + 1) % mMaxDelay;

    return output;
  }

private:
  std::vector<double> mBuffer; // Delay buffer
  size_t mMaxDelay;            // Maximum delay length (samples)
  double mDelayTime = 1.0;     // Current delay time (fractional allowed)
  double mFeedback = 0.0;      // Feedback amount
  size_t mWritePos = 0;        // Write position in buffer
};