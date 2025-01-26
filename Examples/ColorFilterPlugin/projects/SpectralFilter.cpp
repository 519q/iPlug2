#include "SpectralFilter.h"

double SpectralFilter::Process(double input, FilterParameters& params) // phase filter + magn clipper
{
  double magnitude{};
  double phase{};
  if (params.m_spectralFilter_IR == (int)Spectral_IR::FIR)
  {
    magnitude = FIR_hilbert.getMagnitude(input);
    phase = FIR_hilbert.getPhase(input);
  }
  else
  {
    auto output = IIR_hilbert.getMagintude_Phase(input);
    magnitude = output.magnitude;
    phase = output.phase;
  } 
  double ceil = params.m_cutoff;
  if (magnitude > ceil)
    magnitude = ceil;
  phase = fitlerSelector.ProcessSpectral(phase, params);
  return magnitude * std::cos(phase);
}