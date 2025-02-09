#pragma once
#include "Filters.h"
class DCBlock
{
  double m_state{};
  double m_alpha{};
  double m_sampleRate{};
  double m_cutoff{0.0001};

public:
  void processPoleHP(double& input)
  {
    m_state = (1 - m_alpha) * input + m_alpha * m_state;
    double shaped = -m_state;
    input += shaped;
  }
  void getCoefficients(double sampleRate)
  {
    if (m_sampleRate != sampleRate)
    {
      m_sampleRate = sampleRate;
      m_alpha = std::exp(-getOmega(m_cutoff));
    }
  }
  double getOmega(double cutoff) { return 2.0 * iplug::PI * getCutoffFreq(cutoff) / m_sampleRate; }
  double getCutoffFreq(double cutoff) { return 20.0 * std::pow(1000.0, cutoff); }

  void Process(double& input, FilterParameters& params)
  {
    getCoefficients(params.m_sampleRate);
    processPoleHP(input);
  }
};
