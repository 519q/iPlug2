#pragma once
#include <cmath>
constexpr int LUT_BITS{12};
constexpr int LUT_SIZE{1 << LUT_BITS};
constexpr int VINTAGE_BIT_RATE{16};

class FilterParameters
{
public:
  double m_cutoff{};
  double m_resonance{};
  double m_bandwidth{};
  double m_drive{};
  double m_shape{};
  double m_bias{};
  double m_sampleRate{};
  int m_oversample{};

  void setFilterParameters(double cutoff, double resonance, double bandwidth, double drive, double shape, double bias, int overSampling, double sampleRate)
  {
    m_cutoff = cutoff;
    m_resonance = resonance;
    m_bandwidth = bandwidth;
    m_drive = drive;
    m_shape = shape;
    m_bias = bias;
    m_oversample = overSampling;
    m_sampleRate = sampleRate * std::pow(2, m_oversample);
  }
  // static void setSampleRate(FilterParameters& params, double sampleRate) { params.m_sampleRate = sampleRate; }
  // static void setOverSampleRate(FilterParameters& params) { params.m_sampleRate = params.m_sampleRate * std::pow(2, params.m_oversample);}
};

