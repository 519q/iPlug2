#pragma once
#include "FilterParameters.h"
#include "projects/FIR_HilbertTransform.h"
#include "projects/Filters.h"
#include "projects/IIR_HilbertTransform.h"
#include "projects/Shapers.h"

class WaveShaper
{
public:
  double process(double phase, FilterParameters& params)
  {
    double result = cos(phase)/* + params.m_SH_shape*/; // bias
    if (result > 0)
    {
      if (result > params.m_SH_shape)
        result = params.m_SH_shape;
    }
    if (result < 0)
    {
      if (result < -params.m_SH_shape)
        result = -params.m_SH_shape;
    }
    return result;
  }

private:
  double lerp(double a, double b, double t) { return a + t * (b - a); }
};

class SpectralShaper
{
private:
   IIR_HilbertTransform IIR_hilbert{};
  FIR_HilbertTransform FIR_hilbert{};
  // TanhShaper shaper{};
  PolynomialShaper shaper{};
  // CeilLimiter shaper{};
  // FoldbackShaper shaper{};
  // CubicShaper shaper{};
  WaveShaper waveShaper;

public:
  SpectralShaper();

  void Process(double& input, FilterParameters& params);

  double triangleWave(double phase);

  double cosWave(double phase);

  double morphSine(double phase, FilterParameters& params);
};
