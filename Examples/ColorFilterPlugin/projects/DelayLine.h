#pragma once
#include <algorithm>
#include <cmath>
#include <vector>
#include "projects/FilterParameters.h"
#include "projects/DCBlock.h"

class DelayLine
{
private:
  std::vector<double> mBuffer{};
  size_t mMaxDelaySamples{};
  size_t mWriteIndex{};
  double mDelayTime{};
  double mDelayTimeSeconds{};
  double mFeedback{};
  double mLastOutput{};
  bool mIIR{};
  DampFilter dampFilter{};

public:
  void SetUpDelayLine(size_t maxDelaySamples, double sampleRate)
  {
    mMaxDelaySamples = maxDelaySamples;
    mBuffer.resize(maxDelaySamples, 0.0);
    //mDelayTime = std::clamp(mDelayTimeSeconds * mSampleRate, 2.0, (double)mMaxDelaySamples - 1);
  }

  void SetDelayTime(double delayValue, FilterParameters& params)
  {
    //mDelayTimeSeconds = 1 / delayHz;
    mDelayTimeSeconds = delayValue; // Store time in seconds
    mDelayTime = std::clamp(mDelayTimeSeconds * params.m_sampleRate, 2.0, (double)mMaxDelaySamples - 1);
  }

  double Process(double input, FilterParameters& params)
  {
    if (params.m_delayMix)
    {
      if (mBuffer.empty())
        return input;
      double fb = params.m_delayFeedback;
      //fb = std::clamp(fb, -1.2, 1.2);
      SetDelayTime(params.m_delayTime, params);
      if (!mIIR)
      {
        // Write to delay buffer
        mBuffer[mWriteIndex] = input + fb * mLastOutput;
      }

      // Calculate read index with sub-sample precision
      double readIndex = mWriteIndex - mDelayTime;
      if (readIndex < 0)
        readIndex += mMaxDelaySamples;

      size_t indexInt = (size_t)readIndex;
      double frac = readIndex - indexInt;

      // All-pass interpolation for high-quality fractional delay
      double delayedSample = mBuffer[indexInt];
      double nextSample = mBuffer[std::min((indexInt + 1), mMaxDelaySamples) % mMaxDelaySamples];

      double interpolatedSample = delayedSample + frac * (nextSample - delayedSample);
      dampFilter.Process(interpolatedSample, params);
      double returnValue{};
      if (!mIIR)
      {
        // Update buffer index
        mWriteIndex = (mWriteIndex + 1) % mMaxDelaySamples;

        mLastOutput = interpolatedSample;
        returnValue = interpolatedSample;
      }
      else
      {
        // Compute new output with feedback
        double output = input + fb * interpolatedSample;

        // Store new output in buffer (mixing into itself)
        mBuffer[mWriteIndex] = output;

        // Update buffer index
        mWriteIndex = (mWriteIndex + 1) % mMaxDelaySamples;

        mLastOutput = output; // Save for next cycle
        returnValue = output;
      }
      return interpolateLin(input, returnValue, params.m_delayMix);
    }
    else
      return input;
  }

  void SetIIR(bool IIR) { mIIR = IIR; }
};