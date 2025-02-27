#include "SpectralShaper.h"
// #include "projects/DebugPrint.h"

SpectralShaper::SpectralShaper() {}

void SpectralShaper::Process(double& input, FilterParameters& params)
{
  double magnitude{};
  double phase{};

  if (params.m_spectralShaper_IR == (int)Spectral_IR::IIR)
  {
    auto output = transformer.ProcessIIR(input, params.m_spectralShaperOrder);
    magnitude = output.magnitude;
    phase = output.phase;
  }
  else if (params.m_spectralShaper_IR == (int)Spectral_IR::LATTICE)
  {
    auto output = transformer.ProcessLATTICE(input, params.m_spectralShaperOrder);
    magnitude = output.magnitude;
    phase = output.phase;
  }
  else if (params.m_spectralShaper_IR == (int)Spectral_IR::FIR)
  {
    auto output = transformer.ProcessFIR(input, params.m_spectralShaperOrder, params.m_spectralShaperOrderODD);
    magnitude = output.magnitude;
    phase = output.phase;
  }

  // double real = hilbert.getReal(input);
  // double imag = hilbert.getImag(input);
  magnitude = shaperSelector.ProcessSpectralShaperArray(magnitude, params);
  // double shapedMagnitude = magnitude;
  // input = magnitude * cosWave(phase);
  // input = magnitude * triangleWave(phase);
  // input = magnitude * waveShaper.process(phase, params);
  // input = magnitude * morphSine(phase, params);
  input = magnitude * cosWave(phase);
  // input = magnitude;
}

double SpectralShaper::cosWave(double phase) { return std::cos(phase); }

double SpectralShaper::triangleWave(double phase)
{
  // Normalize phase to [0, 2π)
  phase += iplug::PI;
  phase = std::fmod(phase, 2.0 * iplug::PI);
  if (phase < 0)
    phase += 2.0 * iplug::PI;

  // Triangle wave formula
  if (phase < iplug::PI)
  {
    return -1.0 + (2.0 * phase / iplug::PI); // Rising edge
  }
  else
  {
    return 3.0 - (2.0 * phase / iplug::PI); // Falling edge
  }
}

double SpectralShaper::morphSine(double phase, FilterParameters& params)
{
  // shapeParam controls the shape:
  // - shapeParam = 0: Thin blips (narrow pulse)
  // - shapeParam = 0.5: Sine wave
  // - shapeParam = 1: Almost square wave (wide pulse)

  // Apply a nonlinear transformation to the phase
  double transformedPhase = (std::sin(phase) / std::sqrt(1.0 + params.m_spectralShaperDrive * std::pow(std::sin(phase), 2))) + iplug::PI / 2;

  // Scale and bias to ensure the output is in [-1, 1]
  double output = transformedPhase / std::sqrt(1.0 + params.m_spectralShaperDrive);

  return output;
}

// double SpectralShaper::morphSine(double phase, FilterParameters& params)
//{
//   // shapeParam controls the shape:
//   // - shapeParam = 0: Sine wave
//   // - shapeParam = 1: Square-like wave
//
//   // Apply a polynomial waveshaper
//   double sine = std::sin(phase);
//   double shaped = sine - params.m_SH_shape * std::pow(sine, 3); // Cubic shaping
//
//   // Normalize to ensure the output is in [-1, 1]
//   double maxAmplitude = 1.0 - params.m_SH_shape * std::pow(1.0, 3); // Max amplitude for sine = 1
//   shaped /= maxAmplitude;
//
//   return shaped;
// }