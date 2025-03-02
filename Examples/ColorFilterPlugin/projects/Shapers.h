#pragma once
#include "FilterParameters.h"
#include "IPlugConstants.h"
#include "projects/DebugPrint.h"
#include "projects/MiscTools.h"

static double m_Vintage_scale{1 << VINTAGE_BIT_RATE};
constexpr double processingFloor = 0.0001;

static double filnalTanh(double input) { return tanh(input /** mapRange((1. - params.m_resonance), 0.5, 1)*/); }

enum class MainShaperTypes
{
  Sigmoidal,
  MAX_SHAPER_TYPES
};

enum class SpectralShaperTypes
{
  TanhShaper,
  PolynomialShaper,
  CubicShaper,
  Clipper,
  MAX_SHAPER_TYPES
};

enum class AsymShaperTypes
{
  Lin,
  Expo,
  Quad,
  Tanh,
  Sigm,
  MAX_SHAPER_TYPES
};

class Shapers
{
public:
  virtual double Process(double input, double controlParam, double bias) = 0;
};

class CubicShaper : public Shapers
{
public:
  double Process(double input, double controlParam, double bias) override
  {
    double t = controlParam;
    double a = 7; // Controls the curve
    double scaled_t = std::log1p(a * t) / std::log1p(a);

    double strength = 4.5;
    double scaledInput = input * (1 + strength * controlParam);
    // Cubic soft clipping
    double shaped = scaledInput - (1.0 / 3.0) * std::pow(scaledInput, 3);
    shaped = filnalTanh(shaped);
    return shaped / (1 + strength * 0.3 * scaled_t);
  }
};

class PolynomialShaper : public Shapers
{
public:
  double Process(double input, double controlParam, double bias) override
  {
    // Polynomial shaping (adjust coefficients for different curves)
    {
      double shaped = (input - (6 * controlParam) * std::pow(input, 3) + (8 * controlParam) * std::pow(input, 5)) * (1 + 1 * controlParam);
      return filnalTanh(shaped);
    }
  }
};

class SoftClipper : public Shapers
{
public:
  double softclip(double x) { return x / (1.0 + fabs(x)); }
};
class Clipper : public Shapers
{
public:
  double Process(double input, double controlParam, double bias) override
  {
    double offset = 0.01;
    double ceil = 1 - (controlParam - offset);
    if (input > ceil)
      input = ceil;
    return input;
  }
};

// variable transfer curve :
//
//   class CeilLimiter
//{
// public:
//   double Process(double input, double drive, double shape)
//   {
//     // shape: 0 = hard clip, 1 = soft clip
//     double ceil = drive;
//
//     if (std::abs(input) > ceil)
//     {
//       double x = input / ceil;
//       double curve = 1.0 + shape * 10.0; // Adjust multiplier to taste
//       return ceil * (x / (1.0 + std::pow(std::abs(x), curve)));
//     }
//
//     return input;
//   }
// };

class TanhShaper : public Shapers
{
public:
  double Process(double input, double controlParam, double bias) override
  {
    double t = controlParam;
    double a = 9.0; // Controls the curve
    double scaled_t = std::log1p(a * t) / std::log1p(a);
    double strength = 25 * 0.4;
    double inputProcess = input * (1 + strength * controlParam);
    inputProcess = tanh(inputProcess);
    // if (inputProcess > 1 - drive)
    //   inputProcess = 1 - drive;
    return inputProcess / (1 + strength * 0.2 * scaled_t);
  }
};

class FoldbackShaper : public Shapers
{
public:
  double Process(double input, double controlParam, double bias) override
  {
    double threshold = controlParam;
    // Foldback distortion
    if (input > threshold || input < -threshold)
    {
      input = std::fmod(input + threshold, 4.0 * threshold) - 2.0 * threshold;
    }
    return input;
  }
};

class ReflectShaper : public Shapers
{
public:
  double Process(double input, double controlParam, double bias) override
  {
    double threshold = controlParam;

    // If the knob is below the processing floor, return the input unchanged
    if (threshold < processingFloor)
      return input;

    // Compute the absolute value of the input for reflection
    double absInput = std::abs(input);

    // If the input is within the threshold, return it unchanged
    if (absInput <= threshold)
      return input;

    // Apply foldback reflection
    double folded = std::fmod(absInput, 2.0 * threshold);
    if (folded > threshold)
      folded = 2.0 * threshold - folded;

    // Restore the original sign of the input
    folded = (input >= 0.0) ? folded : -folded;

    return folded;
  }
};

class AsymShapeLin : public Shapers
{
private:
public:
  double Process(double input, double controlParam, double bias) override
  {
    if (controlParam > 0)
    {
      if (input > 0)
      {
        input *= 1 - (0.5 * controlParam);
      }
    }
    else if (controlParam < 0)
      if (input < 0)
      {
        input *= 1 - (0.5 * controlParam);
      }
    return input;
  }
};
class AsymShapeExpo : public Shapers
{
public:
  double Process(double input, double controlParam, double bias) override
  {
    // double bias = controlParam;

    if (bias > 0)
    {
      if (input > 0)
        input *= exp(-bias * 0.5);
    }
    else if (bias < 0)
    {
      if (input < 0)
        input *= exp(bias * 0.5);
    }

    return input;
  }
};
class AsymShapeQuadratic : public Shapers
{
public:
  double Process(double input, double controlParam, double bias) override
  {
    // double bias = controlParam;
    if (bias > 0 && input > 0)
      input = input - bias * input * input; // More saturation for positive values
    else if (bias < 0 && input < 0)
      input = input + bias * input * input; // More saturation for negative values
    return input;
  }
};
class AsymShapeTanh : public Shapers
{
public:
  double Process(double input, double controlParam, double bias) override
  {
    // double bias = controlParam;
    if (bias > 0 && input > 0)
      input = tanh(input * (1 - bias));
    else if (bias < 0 && input < 0)
      input = tanh(input * (1 + bias));

    return input;
  }
};
class AsymShapeSigmoid : public Shapers
{
public:
  double Process(double input, double controlParam, double bias) override
  {
    if (bias > 0 && input > 0)
      input = input / (1.0 + bias * input);
    else if (bias < 0 && input < 0)
      input = input / (1.0 - bias * input);
    return input;
  }
};
class SigmoidalShaper : public Shapers
{
public:
  double sigmoidal(double input, double t, double c) { return (input / (t + std::abs(input))) * c; }
  double Process(double input, double controlParam, double bias) override
  {
    double shaped{};
    double scaledControlParam = scaleLog(controlParam, 6);
    const double t = 0.4 - ((0.4 - 0.01) * scaledControlParam);
    const double c = 0.7 - ((0.7 - 0.24) * scaledControlParam);
    if (!bias)
    {
      shaped = sigmoidal(input, t, c);
    }
    else if (bias > 0)
    {
      const double t_pos = 0.4 - (((0.4 - 0.01) * scaledControlParam * (1 - scaleExponential(bias, 5))));
      const double c_pos = 0.7 - (((0.7 - 0.24) * scaledControlParam * (1 - scaleExponential(bias, 5))));

      if (input > 0)
        shaped = sigmoidal(input, t_pos, c_pos);
      else
        shaped = sigmoidal(input, t, c);
    }
    else if (bias < 0)
    {
      const double t_neg = 0.4 - (((0.4 - 0.01) * scaledControlParam * (1 - scaleExponential(-bias, 5))));
      const double c_neg = 0.7 - (((0.7 - 0.24) * scaledControlParam * (1 - scaleExponential(-bias, 5))));
      if (input > 0)
        shaped = sigmoidal(input, t, c);
      else
        shaped = sigmoidal(input, t_neg, c_neg);
    }
    return shaped;
  }
};
inline double softclip(double x)
{
  return x / (1.0 + fabs(x)); // Simple soft clip (approximates diode behavior)
}

inline double diodeNonlinearity(double x)
{
  // For positive input, use a more aggressive exponential compression.
  if (x >= 0)
  {
    return 1.0 - exp(-x);
  }
  else // For negative input, reduce the conduction (e.g., by scaling x)
  {
    return -(1.0 - exp(x * 0.5)); // 0.5 scaling factor introduces asymmetry.
  }
}
class ClipperMixer
{
public:
  double Process(double input, double selector)
  {
    double processed = input;
    double a_{};
    double b_{};
    if (selector < 2)
    {
      a_ = filnalTanh(processed);
      b_ = softclip(processed);
      processed = interpolateLin(a_, b_, selector - 1);
    }
    else if (selector < 3)
    {
      a_ = softclip(processed);
      b_ = diodeNonlinearity(processed);
      processed = interpolateLin(a_, b_, selector - 2);
    }
    return processed;
  }
};