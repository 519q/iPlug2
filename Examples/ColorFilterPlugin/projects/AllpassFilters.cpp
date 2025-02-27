#include "AllpassFilters.h"

bool AllpassFilters::isDirty(FilterParameters& params)
{
  if ((m_freq != params.m_phaserFreq) || (m_depth != params.m_phaserDepth) || (m_sampleRate != params.m_sampleRate))
  {
    m_freq = params.m_phaserFreq;
    m_depth = params.m_phaserDepth;
    m_sampleRate = params.m_sampleRate;
    return true;
  }
  return false;
}
double AllpassFilters::calculateCoefficients(FilterParameters& params) { return -.95 + (std::clamp(params.m_phaserFreq, 0., .99) * 1.95); }
void FirstOrderAllpassOnePole::ProcessOnePole(double& input, FilterParameters& params, double A)
{
  double output = A * input + mDelay;
  mDelay = input - A * output;
  input = output;
}
void FirstOrderAllpassFilter::Process(double& input, FilterParameters& params)
{
  double output =
    mixer.Process(input, params, params.m_phaserDepth, 1, [this, &params](FirstOrderAllpassOnePole& filter, double& sample) { filter.ProcessOnePole(sample, params, calculateCoefficients(params)); });
  input = output;
}
void SecondOrderAllpassTwoPole::processTwoPole(double& input, double A)
{
  double output = A * input + mDelay2;
  mDelay2 = mDelay1;
  mDelay1 = input - A * output;
  input = output;
}
void SecondOrderAllpassFilter::Process(double& input, FilterParameters& params)
{
  double output =
    mixer.Process(input, params, params.m_phaserDepth, 1, [this, &params](SecondOrderAllpassTwoPole& filter, double& sample) { filter.processTwoPole(sample, calculateCoefficients(params)); });
  input = output;
}
void LatticeOnePole::processOnePole(double& input, double k)
{
  double v = input - k * z1;
  double outSample = k * v + z1;

  z1 = v; // Delay update
  input = outSample;
}
void LatticeAllpass::getCoeff(FilterParameters& params)
{
  if (isDirty(params))
  {
    // Convert cutoff to normalized frequency (0 to 0.5)
    double omega = 2.0 * iplug::PI * (getCutoffFreq(params, params.m_phaserFreq) / params.m_sampleRate);

    // Compute reflection coefficient (approximated for smoothness)
    m_k = (sin(omega) - 1.0) / (sin(omega) + 1.0);
  }
}
void LatticeAllpass::Process(double& input, FilterParameters& params)
{
  getCoeff(params);
  double output = mixer.Process(input, params, params.m_phaserDepth, 1, [this, &params](LatticeOnePole& filter, double& sample) { filter.processOnePole(sample, m_k); });
  input = output;
}