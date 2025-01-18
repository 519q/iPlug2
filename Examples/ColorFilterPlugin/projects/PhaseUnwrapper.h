#pragma once
#include "FilterParameters.h"

class PhaseUnwrapper
{
public:
  double process(double phase, FilterParameters& params);

private:
  double lastPhase{};
  double totalPhase{};
  int sampleCounter{};
  double phaseVelocity{};
  double accumulatedRotations{};
  bool shouldReset(FilterParameters& params) const;

  void reset();
};
