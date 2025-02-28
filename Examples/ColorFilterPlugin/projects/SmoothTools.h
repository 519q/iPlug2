#pragma once

#include <functional>

static const double smoothThreshold{0.001};

inline auto interpolateLin(double drySignal, double wetSignal, double control) { return (drySignal * (1.0 - control)) + (wetSignal * control); }
inline auto interpolateExp(double drySignal, double wetSignal, double control, double expFactor = 2)
{
  double expF{expFactor};
  double expControl = pow(control, expF);
  return (drySignal * (1.0 - expControl)) + (wetSignal * expControl);
}
inline auto interpolateLog(double drySignal, double wetSignal, double control, double logFactor = 10.)
{
  double logControl = log(control * logFactor + 1) / log(logFactor + 1);
  return (drySignal * (1.0 - logControl)) + (wetSignal * logControl);
}

inline double scaleLog(double controlParam, double scalingFactor)
{
  double a = scalingFactor; // Controls the curve
  double scaled_t = std::log1p(a * controlParam) / std::log1p(a);
  return scaled_t;
}
inline double scaleExponential(double controlParam, double scalingFactor)
{
  double k = scalingFactor; // Steepness factor
  double scaled_t = (std::exp(k * controlParam) - 1.0) / (std::exp(k) - 1.0);
  return scaled_t;
}
inline double mapRange(double value, double min, double max) { return min + (value * (max - min)); }

class RMS_PEAK_CALCULATOR
{
private:
  std::vector<double> mBuffer;      // Circular buffer to hold input samples
  int mBufferSize;      // Size of the buffer in frames
  int mBufferIndex = 0; // Current position in circular buffer
  float mRMSWeight = 0.5;
  float valueAmplifier = 3.5;
  float mCurrentRMS = 0.0f;   // Current RMS value
  float mCurrentPeak = 0.0f;  // Current peak value
  float mBlendedValue = 0.0f; // Current blended value
  bool mBufferFilled = false; // Flag to indicate if buffer is filled

public:
  RMS_PEAK_CALCULATOR(int bufferSize)
    : mBufferSize(bufferSize)
  {
    mBuffer.resize(mBufferSize);
  }

  ~RMS_PEAK_CALCULATOR() { }

  // Process a single sample (for one channel)
  void processSample(double sample)
  {
    // Add new sample to buffer
    mBuffer[mBufferIndex] = sample;

    // Update buffer index
    mBufferIndex = (mBufferIndex + 1) % mBufferSize;

    // Check if buffer is filled
    if (mBufferIndex == 0)
    {
      mBufferFilled = true;
    }

    // Calculate values if we have enough samples
    if (mBufferFilled)
    {
      mBufferFilled = false;
      calculateValues();
    }
  }

  // Calculate RMS, peak and blended values from current buffer
  void calculateValues()
  {
    float sumSquares = 0.0f;
    float peak = 0.0f;

    for (int i = 0; i < mBufferSize; i++)
    {
      float sample = std::abs(static_cast<float>(mBuffer[i]));
      sumSquares += sample * sample;
      peak = std::max(peak, sample);
    }

    // Calculate RMS
    mCurrentRMS = std::sqrt(sumSquares / (mBufferSize));
    mCurrentPeak = peak;
    // Blend RMS and peak
    mBlendedValue = mRMSWeight * mCurrentRMS + (1.0f - mRMSWeight) * mCurrentPeak;
  }

  // Getters for current values
  double getRoundedValueForUI() const { return mBlendedValue * valueAmplifier; }
  float getCurrentRMS() const { return mCurrentRMS; }
  float getCurrentPeak() const { return mCurrentPeak; }
};

class SmoothBypass
{
private:
public:
  template <typename T, typename U>
  inline static void processSmoothBypass(std::function<T(U&)> processor, U& signal, double smoothedBypass)
  {
    U dry = signal;
    if (std::abs(smoothedBypass - 1.0) < smoothThreshold)
    {
      (processor)(signal);
    }
    else if (std::abs(smoothedBypass) < smoothThreshold)
    {
      signal = dry;
    }
    else
    {
      (processor)(signal);
      signal = interpolateLin(dry, signal, smoothedBypass);
    }
  }
};
