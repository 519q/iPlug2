#pragma once
#include "FilterParameters.h"
#include "IPlugConstants.h"
#include "projects/SmoothTools.h"
static double m_Vintage_scale{1 << VINTAGE_BIT_RATE};

static double filnalTanh(double input, FilterParameters params) { return tanh(input /** mapRange((1. - params.m_resonance), 0.5, 1)*/); }

class DCStop
{
private:
  double m_alpha{};
  double m_state{};

public:
  void process(double& input, FilterParameters& params, double cutoff = 0.05)
  {
    const double cutoffFreq = 2.5 * std::pow(8000.0, cutoff /*params.m_cutoff*/);
    m_alpha = std::exp(-2.0 * iplug::PI * cutoffFreq / params.m_sampleRate);
    m_state = (1 - m_alpha) * input + m_alpha * m_state;
    input -= m_state;
  }
};
// enum class ShaperParams
//{
//   SHAPE,
//   DRIVE,
//   ASYM,
//   MAX_SHAPER_PARAMS
// };
class Shapers
{
protected:
  virtual ~Shapers() {}

  virtual void Process(double& input, FilterParameters& params, double driveDivider) = 0; // Pure virtual function
};

class Tripler : public Shapers
{
  // https://www.desmos.com/calculator/sisddmcflj
private:
  static constexpr double a{8};
  static constexpr double b{1.28};
  static constexpr double c{0.6};
  static constexpr double d{2.5};
  double m_drive{};

public:
  Tripler()
    : Shapers{}
  {
  }

  void Process(double& input, FilterParameters& params, double driveDivider = 1) override
  {
    m_drive = params.m_drive;
    const double shaped = (-d * input * (std::tanh(a * std::pow(input, 2) - b) - c)) * 0.3;
    input = interpolateLin(input, shaped, m_drive);
  }
};

class AsymShape : public Shapers
{
private:
public:
  AsymShape()
    : Shapers{}
  {
  }

  void Process(double& input, FilterParameters& params, double driveDivider = 1) override
  {
    if (input > 0)
    {
      input *= 1 - (0.5 * params.m_bias);
    }
  }
};

class Sigmoidal : public Shapers
{
private:
  AsymShape asym{};
  DCStop dcstop{};

public:
  Sigmoidal()
    : Shapers{}
  {
  }

  void Process(double& input, FilterParameters& params, double driveDivider = 1) override
  {
    if (params.m_drive > 0)
    {
      params.m_drive /= driveDivider;
      const double t = 0.4 - ((0.4 - 0.01) * params.m_shape);
      const double z = 0.7 - ((0.7 - 0.24) * params.m_shape);
      double shaped = (input / (t + std::abs(input))) * z;
      if (params.m_bias > 0)
      {
        asym.Process(shaped, params);
      }
      dcstop.process(shaped, params);
      input = interpolateLin(input, shaped, params.m_drive);
    }
  }
};
