#include "PhaseUnwrapper.h"
#include "IPlugConstants.h"
#include <algorithm>

double PhaseUnwrapper::process(double phase, FilterParameters& params)
{
  // Normalize incoming phase
  while (phase < 0)
    phase += 2.0 * iplug::PI;
  while (phase >= 2.0 * iplug::PI)
    phase -= 2.0 * iplug::PI;

  // Calculate phase difference
  double diff = phase - lastPhase;
  if (diff < -iplug::PI)
    diff += 2.0 * iplug::PI;
  if (diff > iplug::PI)
    diff -= 2.0 * iplug::PI;

  // Track phase velocity for reset detection
  phaseVelocity = 0.99 * phaseVelocity + 0.01 * diff;

  // Reset logic
  sampleCounter++;
  if (shouldReset(params))
  {
    reset();
  }

  totalPhase += diff;
  lastPhase = phase;

  return totalPhase;
}

bool PhaseUnwrapper::shouldReset(FilterParameters& params) const
{
  // Reset if:
  // 1. Phase velocity near zero (static signal)
  // 2. Accumulated too long
  // 3. Phase velocity changed dramatically
  return (std::abs(phaseVelocity) < 0.0001) || (sampleCounter > params.m_sampleRate) || // adjust based on sample rate
         (std::abs(phaseVelocity) > iplug::PI);
}

void PhaseUnwrapper::reset()
{
  totalPhase = lastPhase;
  sampleCounter = 0;
  phaseVelocity = 0.0;
}