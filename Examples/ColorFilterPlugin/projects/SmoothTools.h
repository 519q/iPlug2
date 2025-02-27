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
