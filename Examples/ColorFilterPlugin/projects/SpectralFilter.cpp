#include "SpectralFilter.h"

double SpectralFilter::Process(double input, FilterParameters& params) // phase filter + magn clipper
{
  auto output = IIR_hilbert.getMagintude_Phase(input);
  double magnitude = output.magnitude;
  double phase = output.phase;
  double clippedMagnitude = magnitudeLimiter.Process(magnitude, params);
  phase = fitlerSelector.ProcessSpectral(phase, params);
  return clippedMagnitude * std::cos(phase);
}