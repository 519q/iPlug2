#pragma once
#include "DCBlock.h"
#include "DelayLine.h"
#include "FilterSelector.h"
#include "Shapers.h"
#include "projects/HilbertTransformers.h"

// filter for phase and clipper for maginitude, without clipper there is click on phase wrap(is seems)
class SpectralFilter
{
private:
  Hilbert_Transformer transformer{};
  FilterSelector fitlerSelector{};
  DCBlock dcblock{};

public:
  double Process(double input, FilterParameters& params);

};
