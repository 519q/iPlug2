#include "SpectralFilter.h"

double SpectralFilter::Process(double input, FilterParameters& params) // phase filter + magn clipper
{
  double magnitude{};
  double phase{};
  if (params.m_spectralFilter_IR == (int)Spectral_IR::FIR)
  {
    magnitude = FIR_hilbert.getMagnitude(input, params.m_spectralFilterFIR_Order);
    phase = FIR_hilbert.getPhase(input, params.m_spectralFilterFIR_Order);
  }
  else
  {
    auto output = IIR_hilbert.getMagintude_Phase(input);
    magnitude = output.magnitude;
    phase = output.phase;
  }
  double a = 9.0; // Controls the curve
  double scaled_t = std::log1p(a * params.m_cutoff) / std::log1p(a);
  double ceil = scaled_t;
  if (magnitude > ceil)
    magnitude = ceil;
  phase = fitlerSelector.ProcessSpectral(phase, params);
  return magnitude * std::cos(phase);
}