#pragma once
#include "projects/HilbertTransformers.h"
#include "Shapers.h"
#include "FilterSelector.h"
#include "DCBlock.h"
#include "DelayLine.h"

// filter for phase and clipper for maginitude, without clipper there is click on phase wrap(is seems)
class SpectralFilter
{
private:
  IIR_HilbertTransform IIR_hilbert{};
  FIR_HilbertTransform FIR_hilbert{};
  LatticeHilbertTransform Lattice_hilbert{};
  FilterSelector fitlerSelector{};
  DCBlock dcblock{};

public:
  double Process(double input, FilterParameters& params);
};
