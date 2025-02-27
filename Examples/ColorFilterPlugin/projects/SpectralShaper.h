#pragma once
#include "ShaperSelector.h"
#include "projects/FilterParameters.h"
#include "projects/Filters.h"
#include "projects/HilbertTransformers.h"
#include "projects/Shapers.h"
#include "Phaser.h"

class WaveShaper
{
public:
  double process(double phase, FilterParameters& params)
  {
    double result = cos(phase) /* + params.m_SH_shape*/; // bias
    if (result > 0)
    {
      if (result > params.m_spectralShaperDrive)
        result = params.m_spectralShaperDrive;
    }
    if (result < 0)
    {
      if (result < -params.m_spectralShaperDrive)
        result = -params.m_spectralShaperDrive;
    }
    return result;
  }

private:
  double lerp(double a, double b, double t) { return a + t * (b - a); }
};

class SpectralShaper
{
private:
  Hilbert_Transformer transformer{};
  ShaperSelector shaperSelector{};
  WaveShaper waveShaper{};

public:
  SpectralShaper();

  void Process(double& input, FilterParameters& params);
  double triangleWave(double phase);
  double cosWave(double phase);
  double morphSine(double phase, FilterParameters& params);
};
