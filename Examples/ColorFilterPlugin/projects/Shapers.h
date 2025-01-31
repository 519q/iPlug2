#pragma once
#include "FilterParameters.h"
#include "IPlugConstants.h"
#include "projects/SmoothTools.h"

static double m_Vintage_scale{1 << VINTAGE_BIT_RATE};
constexpr double processingFloor = 0.0001;

static double filnalTanh(double input, FilterParameters params) { return tanh(input /** mapRange((1. - params.m_resonance), 0.5, 1)*/); }

enum class ShaperTypes
{
  TanhShaper,
  PolynomialShaper,
  CubicShaper,
  Clipper,
  MAX_SHAPER_TYPES
};

class Shapers
{
public:
  virtual double Process(double input, FilterParameters& params) = 0;
};

class CubicShaper : public Shapers
{
public:
  double Process(double input, FilterParameters& params) override
  {
    if (params.m_spectralShaperShape < processingFloor)
      return input;
    double t = params.m_spectralShaperShape;
    double a = 7; // Controls the curve
    double scaled_t = std::log1p(a * t) / std::log1p(a);

    double strength = 4.5;
    double scaledInput = input * (1 + strength * params.m_spectralShaperShape);
    // Cubic soft clipping
    double shaped = scaledInput - (1.0 / 3.0) * std::pow(scaledInput, 3);
    shaped = filnalTanh(shaped, params);
    return shaped / (1 + strength * 0.3 * scaled_t);
  }
};

class PolynomialShaper : public Shapers
{
public:
  double Process(double input, FilterParameters& params) override
  {
    if (params.m_spectralShaperShape < processingFloor)
      return input;
    // Polynomial shaping (adjust coefficients for different curves)
    else
    {
      double shaped = (input - (6 * params.m_spectralShaperShape) * std::pow(input, 3) + (8 * params.m_spectralShaperShape) * std::pow(input, 5)) * (1 + 1 * params.m_spectralShaperShape);
      return filnalTanh(shaped, params);
    }
  }
};

class Clipper : public Shapers
{
public:
  double Process(double input, FilterParameters& params) override
  {
    double offset = 0.01;
    double ceil = 1 - (params.m_spectralShaperShape - offset);
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
  double Process(double input, FilterParameters& params) override
  {
    if (params.m_spectralShaperShape < processingFloor)
    {
      return input;
    }
    double t = params.m_spectralShaperShape;
    double a = 9.0; // Controls the curve
    double scaled_t = std::log1p(a * t) / std::log1p(a);
    double strength = 25 * 0.4;
    double inputProcess = input * (1 + strength * params.m_spectralShaperShape);
    inputProcess = tanh(inputProcess);
    // if (inputProcess > 1 - drive)
    //   inputProcess = 1 - drive;
    return inputProcess / (1 + strength * 0.2 * scaled_t);
  }
};

class FoldbackShaper : public Shapers
{
public:
  double Process(double input, FilterParameters& params) override
  {
    if (params.m_spectralShaperShape < processingFloor)
      return input;
    double threshold = params.m_spectralShaperShape;
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
  double Process(double input, FilterParameters& params) override
  {
    double threshold = params.m_spectralShaperShape;

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

class AsymShape
{
private:
public:
  double Process(double input, FilterParameters& params)
  {
    if (input > 0)
    {
      input *= 1 - (0.5 * params.m_bias);
    }
    return input;
  }
};

class Sigmoidal
{
private:
  AsymShape asym{};

public:
  double Process(double input, FilterParameters& params)
  {
    if (params.m_drive > 0)
    {
      const double t = 0.4 - ((0.4 - 0.01) * params.m_shape);
      const double z = 0.7 - ((0.7 - 0.24) * params.m_shape);
      double shaped = (input / (t + std::abs(input))) * z;
      if (params.m_bias > 0)
      {
        shaped = asym.Process(shaped, params);
      }
      input = interpolateLin(input, shaped, params.m_drive);
    }
    return input;
  }
};
