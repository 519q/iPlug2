#pragma once
#include <cmath>
#include <vector>
#include "IPlugConstants.h"

class DCBlocker_FIR
{
private:
  std::vector<double> coeffs;
  std::vector<double> buffer;
  int filterOrder;
  double sampleRate;

public:
  DCBlocker_FIR(int order, double sr)
    : filterOrder(order)
    , sampleRate(sr)
  {
    buffer.resize(filterOrder, 0.0);
    computeCoefficients();
  }

  void computeCoefficients()
  {
    coeffs.resize(filterOrder);
    double fc = 5.0 / (sampleRate / 2.0); // Normalize cutoff frequency (5 Hz)
    int mid = filterOrder / 2;

    for (int i = 0; i < filterOrder; ++i)
    {
      if (i == mid)
        coeffs[i] = 1.0 - 2.0 * fc;
      else
      {
        double x = i - mid;
        coeffs[i] = -2.0 * fc * std::sin(2.0 * iplug::PI * fc * x) / (iplug::PI * x);
      }

      // Apply Hamming window
      coeffs[i] *= 0.54 - 0.46 * std::cos(2.0 * iplug::PI * i / (filterOrder - 1));
    }
  }

  void updateSampleRate(double newSampleRate)
  {
    if (newSampleRate != sampleRate)
    {
      sampleRate = newSampleRate;
      computeCoefficients(); // Recalculate coefficients
    }
  }

  double process(double input, double newSampleRate)
  {
    updateSampleRate(newSampleRate);
    // Shift buffer
    for (size_t i = buffer.size() - 1; i > 0; --i)
      buffer[i] = buffer[i - 1];

    buffer[0] = input;

    // Apply FIR filter
    double output = 0.0;
    for (size_t i = 0; i < buffer.size(); ++i)
      output += buffer[i] * coeffs[i];

    return output;
  }
};