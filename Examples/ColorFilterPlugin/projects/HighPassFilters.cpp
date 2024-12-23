#include "HighPassFilters.h"

void DF1_1P_HP::Process(double& input, FilterParameters& params)
{
  const double cutoffFreq = 2.5 * std::pow(8000.0, params.m_cutoff);
  m_alpha = std::exp(-2.0 * iplug::PI * cutoffFreq / params.m_sampleRate);
  // const double output = (input - m_state) * (1 - m_alpha);
  // m_state = (input - m_state) * (1 - m_alpha);
  m_state = (1 - m_alpha) * input + m_alpha * m_state;
  input -= m_state;
}

void DF1_2P_HP::Process(double& input, FilterParameters& params)
{
  double resoScaling{2.};
  const double cutoffOffset{0.1};
  double m_cutoff_scaled = cutoffOffset + ((1.0 - cutoffOffset) * params.m_cutoff);
  const double cutoffFreq = 2.2 * std::pow(8000.0, m_cutoff_scaled);
  m_alpha = std::exp(-2.0 * iplug::PI * cutoffFreq / params.m_sampleRate);
  bandpass = (m_state[1] - m_state[0]);
  double feedback = (bandpass * params.m_resonance * resoScaling);

  // input += std::clamp(feedback, -clampValue, clampValue);
    //input += feedback;
  for (double& pole : m_state)
  {
    input += feedback;

    pole = (1 - m_alpha) * input + m_alpha * pole;
    double shaped = -pole;
    // input = std::clamp(shaped, -1.0, 1.0);
    input += shaped;
    dcstop.process(input, params);
  }
    params.m_drive *= 4;
    if (params.m_drive > 0)
    {
      sigmoidalShaper.Process(input, params);
    }
  input = tanh(input * mapRange((1. - params.m_resonance), 0.5, 1));
}
