#pragma once
#include "IIR_HilbertTransform.h"
#include "Shapers.h"
#include "FilterSelector.h"

// filter for phase and clipper for maginitude, without clipper there is click on phase wrap(is seems)
class SpectralFilter
{
private:
  IIR_HilbertTransform IIR_hilbert{};
  CeilLimiter magnitudeLimiter{};
  FilterSelector fitlerSelector{};

public:
  double Process(double input, FilterParameters& params);
};
