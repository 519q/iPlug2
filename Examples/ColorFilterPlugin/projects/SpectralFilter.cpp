#include "SpectralFilter.h"

double SpectralFilter::Process(double input, FilterParameters& params) // phase filter + magn clipper
{
  double magnitude{};
  double phase{};
  if (params.m_spectralFilter_IR == (int)Spectral_IR::FIR)
  {
    auto output = transformer.ProcessFIR(input, params.m_spectralFilterOrder, params.m_spectralFilterOrderODD);
    magnitude = output.magnitude;
    phase = output.phase;
  }
  else if (params.m_spectralFilter_IR == (int)Spectral_IR::LATTICE)
  {
    auto output = transformer.ProcessLATTICE(input, params.m_spectralFilterOrder);
    magnitude = output.magnitude;
    phase = output.phase;
  }
  else if (params.m_spectralFilter_IR == (int)Spectral_IR::IIR)
  {
    auto output = transformer.ProcessIIR(input, params.m_spectralFilterOrder);
    magnitude = output.magnitude;
    phase = output.phase;
  }
  double a = 9.0; // Controls the curve
  double scaled_t = std::log1p(a * params.m_cutoff) / std::log1p(a);
  double ceil = scaled_t;
  double output{};
  magnitude += params.m_spectralFilterDrive;
  if (magnitude > ceil)
    magnitude = ceil;
  if (params.m_spectralFilter_Harder)
  {
    phase = fitlerSelector.Process(phase, params);
    output = magnitude * std::cos(phase);
    // output = magnitude * (std::cos(phase) + params.m_spectralFilterDrive);
  }
  else
  {
    phase = fitlerSelector.Process(std::cos(phase), params);
    output = magnitude * phase;
    // output = magnitude * (phase + params.m_spectralFilterDrive);
  }
  if (params.m_spectralFilterDrive > 0)
  {
    double a = 16.0; // Controls the curve
    double scaled_t = std::log1p(a * params.m_spectralFilterDrive) / std::log1p(a);
    output *= (1 - (scaled_t * 0.6));
  }
  if ((params.m_spectralFilterDrive > 0) || (params.m_filterSelector >= (int)FilterTypes::DF2_2P - 1 && params.m_filterSelector <= (int)FilterTypes::DF2_6P - 1))
    dcblock.Process(output, params);

  return output;
}