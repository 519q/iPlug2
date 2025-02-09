#pragma once
#include "IIR_HilbertTransform.h"
#include "FIR_HilbertTransform.h"
#include "Shapers.h"
#include "FilterSelector.h"
#include "DCBlock.h"

// filter for phase and clipper for maginitude, without clipper there is click on phase wrap(is seems)
class SpectralFilter
{
private:
  IIR_HilbertTransform IIR_hilbert{};
  FIR_HilbertTransform FIR_hilbert{};
  FilterSelector fitlerSelector{};
  DCBlock dcblock{};

public:
  double Process(double input, FilterParameters& params);
};
