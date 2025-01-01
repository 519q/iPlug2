#include "VintageHighpassFilters.h"

void DF1_1P_HP_Vintage::Process(double& input, FilterParameters& params)
{
  SetSampleRate_DF1(params);
  if (params.m_cutoff < 0)
  {
    params.m_cutoff = 0;
  }
  else if (params.m_cutoff > 1)
  {
    params.m_cutoff = 1;
  }
  MapLUTCutoff(params);
  int m_cutoffIndex = static_cast<int>(params.m_cutoff * (LUT_SIZE - 1) + 0.5);
  double m_alpha = static_cast<double>(m_LUT[m_cutoffIndex]) / m_scale;
  double fixedPointInput = (double)input * m_scale;
  m_state = (1 - m_alpha) * fixedPointInput + m_alpha * m_state;
  input -= (double)m_state / m_scale;
}

void DF1_2P_HP_Vintage::Process(double& input, FilterParameters& params)
{
  SetSampleRate_DF1(params);
  if (params.m_cutoff < 0)
  {
    params.m_cutoff = 0;
  }
  else if (params.m_cutoff > 1)
  {
    params.m_cutoff = 1;
  }
  MapLUTCutoff(params);
  const double cutoffOffset{0.1};
  double m_cutoff_scaled = cutoffOffset + ((1.0 - cutoffOffset) * params.m_cutoff);
  int resonance{};
  int m_cutoffIndex = static_cast<int>(m_cutoff_scaled * (LUT_SIZE - 1) + 0.5);
  double m_alpha = static_cast<double>(m_LUT[m_cutoffIndex]) / m_scale;
  double fixedPointInput = (double)input * m_scale;
  if (params.m_resonance > 0)
  {
    double resoScaling{2.};
    bandpass = (m_state[1] - m_state[0]);
    resonance = (bandpass * params.m_resonance * resoScaling);
  }
  for (int& pole : m_state)
  {
    fixedPointInput += resonance;

    pole = (1 - m_alpha) * fixedPointInput + m_alpha * pole;
    int shaped = -pole;
    // input = std::clamp(shaped, -1.0, 1.0);
    fixedPointInput += shaped;
  }
  input = fixedPointInput / m_scale;
  if (params.m_drive > 0)
  {
    params.m_drive *= 3.6;
    sigmoidalShaper.Process(input, params);
  }
  input = tanh(input * mapRange((1. - params.m_resonance), 0.5, 1));
}

void DF1_3P_HP_Vintage::Process(double& input, FilterParameters& params)
{
  SetSampleRate_DF1(params);
  if (params.m_cutoff < 0)
  {
    params.m_cutoff = 0;
  }
  else if (params.m_cutoff > 1)
  {
    params.m_cutoff = 1;
  }
  MapLUTCutoff(params);
  const double cutoffOffset{0.1};
  double m_cutoff_scaled = cutoffOffset + ((1.0 - cutoffOffset) * params.m_cutoff);
  int resonance{};
  int m_cutoffIndex = static_cast<int>(m_cutoff_scaled * (LUT_SIZE - 1) + 0.5);
  double m_alpha = static_cast<double>(m_LUT[m_cutoffIndex]) / m_scale;
  double fixedPointInput = (double)input * m_scale;
  if (params.m_resonance > 0)
  {
    double resoScaling{1.2};
    bandpass = (-m_state[0] + m_state[2]);
    resonance = (bandpass * params.m_resonance * resoScaling);
  }
  for (int& pole : m_state)
  {
    fixedPointInput += resonance;

    pole = (1 - m_alpha) * fixedPointInput + m_alpha * pole;
    int shaped = -pole;
    fixedPointInput += shaped;
  }
  input = fixedPointInput / m_scale;
  if (params.m_drive > 0)
  {
    params.m_drive *= 4;
    sigmoidalShaper.Process(input, params);
  }
  input = tanh(input * mapRange((1. - params.m_resonance), 0.5, 1));
}

void DF1_4P_HP_Vintage::Process(double& input, FilterParameters& params)
{
  SetSampleRate_DF1(params);
  if (params.m_cutoff < 0)
  {
    params.m_cutoff = 0;
  }
  else if (params.m_cutoff > 1)
  {
    params.m_cutoff = 1;
  }
  MapLUTCutoff(params);
  const double cutoffOffset{0.1};
  double m_cutoff_scaled = cutoffOffset + ((1.0 - cutoffOffset) * params.m_cutoff);
  int resonance{};
  int m_cutoffIndex = static_cast<int>(m_cutoff_scaled * (LUT_SIZE - 1) + 0.5);
  double m_alpha = static_cast<double>(m_LUT[m_cutoffIndex]) / m_scale;
  double fixedPointInput = (double)input * m_scale;
  if (params.m_resonance > 0)
  {
    double resoScaling{1.5};
    bandpass = (-m_state[1] + m_state[3]);
    resonance = (bandpass * params.m_resonance * resoScaling);
  }
  for (int& pole : m_state)
  {
    fixedPointInput += resonance;

    pole = (1 - m_alpha) * fixedPointInput + m_alpha * pole;
    int shaped = -pole;
    fixedPointInput += shaped;
  }
  input = fixedPointInput / m_scale;
  if (params.m_drive > 0)
  {
    params.m_drive *= 4;
    sigmoidalShaper.Process(input, params);
  }
  input = tanh(input * mapRange((1. - params.m_resonance), 0.5, 1));
}

void DF1_6P_HP_Vintage::Process(double& input, FilterParameters& params)
{
  SetSampleRate_DF1(params);
  if (params.m_cutoff < 0)
  {
    params.m_cutoff = 0;
  }
  else if (params.m_cutoff > 1)
  {
    params.m_cutoff = 1;
  }
  MapLUTCutoff(params);
  const double cutoffOffset{0.1};
  double m_cutoff_scaled = cutoffOffset + ((1.0 - cutoffOffset) * params.m_cutoff);
  int resonance{};
  int m_cutoffIndex = static_cast<int>(m_cutoff_scaled * (LUT_SIZE - 1) + 0.5);
  double m_alpha = static_cast<double>(m_LUT[m_cutoffIndex]) / m_scale;
  double fixedPointInput = (double)input * m_scale;
  if (params.m_resonance > 0)
  {
    double resoScaling{0.9};
    bandpass = (-m_state[1] + m_state[5]);
    resonance = (bandpass * params.m_resonance * resoScaling);
  }
  for (int& pole : m_state)
  {
    fixedPointInput += resonance;

    pole = (1 - m_alpha) * fixedPointInput + m_alpha * pole;
    int shaped = -pole;
    fixedPointInput += shaped;
  }
  input = fixedPointInput / m_scale;
  if (params.m_drive > 0)
  {
    params.m_drive *= 4;
    sigmoidalShaper.Process(input, params);
  }
  input = tanh(input * mapRange((1. - params.m_resonance), 0.5, 1));
}

void DF2_2P_HP_Vintage::Process(double& input, FilterParameters& params)
{
  double resoScaling{12.3};
  const double cutoffOffset{0.25};
  double m_cutoff_scaled = cutoffOffset + (((1.0 - cutoffOffset) /* - 0.07*/) * params.m_cutoff);
  // Get cutoff freq in hz from parameter of double range 0...1
  const double cutoffFreq = 2.5 * std::pow(8000.0, m_cutoff_scaled);
  // Normalize frequency
  double omega = 2.0 * iplug::PI * cutoffFreq / params.m_sampleRate;
  // Drift
  static double drift = 1.0;
  drift += (rand() / (double)RAND_MAX - 0.5) * 0.00005;
  drift = std::clamp(drift, 0.99, 1.01);
  omega *= drift;
  // Constrain Q
  const double q = std::max(0.1, params.m_resonance * resoScaling) * std::max(1., (2. * params.m_drive));
  const double alpha = std::sin(omega) / (2.0 * q);

  const double cosw = std::cos(omega);
  const double a0 = 1.0 + alpha;

  // Calculate coefficients
  double b0 = ((1.0 + cosw) / 2.0) / a0;
  double b1 = -(1.0 + cosw) / a0;
  double b2 = ((1.0 + cosw) / 2.0) / a0;
  double a1 = (-2.0 * cosw) / a0;
  double a2 = (1.0 - alpha) / a0;
  // Add component tolerances
  double tolerance = 0.0001 * ((params.m_cutoff * params.m_cutoff) / 2);
  b0 *= (1.0 + tolerance * (static_cast<double>(rand()) / RAND_MAX - 0.5));
  b1 *= (1.0 + tolerance * (static_cast<double>(rand()) / RAND_MAX - 0.5));
  b2 *= (1.0 + tolerance * (static_cast<double>(rand()) / RAND_MAX - 0.5));
  a1 *= (1.0 + tolerance * (static_cast<double>(rand()) / RAND_MAX - 0.5));
  a2 *= (1.0 + tolerance * (static_cast<double>(rand()) / RAND_MAX - 0.5));
  // Direct Form II processing
  if (params.m_drive > 0)
  {
    sigmoidalShaper.Process(input, params);
  }
  int inputScaled = input * m_scale;
  int w = inputScaled - a1 * m_state[0] - a2 * m_state[1] + antiDenormal;
  // Apply gain compensation and soft clipping
  inputScaled = std::clamp(static_cast<int>((b0 * w + b1 * m_state[0] + b2 * m_state[1]) * mapRange((1. - params.m_resonance), 0.5, 1)), 0, m_scale - 1);
  inputScaled = (b0 * w + b1 * m_state[0] + b2 * m_state[1]) * mapRange((1. - params.m_resonance), 0.5, 1);
  // input = tanh(b0 * w + b1 * m_state[0] + b2 * m_state[1]);
  input = (double)inputScaled / m_scale;
  if (params.m_drive > 0)
  {
    sigmoidalShaper.Process(input, params);
  }
  input = tanh(input);
  // Update states
  m_state[1] = m_state[0];
  m_state[0] = w;
  // CapLeak
  const double leakage = 0.999;
  m_state[0] *= leakage;
  m_state[1] *= leakage;
}

void DF2_4P_HP_Vintage::Process(double& input, FilterParameters& params)
{
  double resoScaling{6.6};
  const double cutoffOffset{0.25};
  double m_cutoff_scaled = cutoffOffset + (((1.0 - cutoffOffset) /* - 0.07*/) * params.m_cutoff);
  // Get cutoff freq in hz from parameter of double range 0...1
  const double cutoffFreq = 2.5 * std::pow(8000.0, m_cutoff_scaled);
  // Normalize frequency
  double omega = 2.0 * iplug::PI * cutoffFreq / params.m_sampleRate;
  // Drift
  static double drift = 1.0;
  drift += (rand() / (double)RAND_MAX - 0.5) * 0.00005;
  drift = std::clamp(drift, 0.99, 1.01);
  omega *= drift;
  // Constrain Q
  const double q = std::max(0.1, params.m_resonance * resoScaling) * std::max(1., (2. * params.m_drive));
  const double alpha = std::sin(omega) / (2.0 * q);

  const double cosw = std::cos(omega);
  const double a0 = 1.0 + alpha;

  // Calculate coefficients
  double b0 = ((1.0 + cosw) / 2.0) / a0;
  double b1 = -(1.0 + cosw) / a0;
  double b2 = ((1.0 + cosw) / 2.0) / a0;
  double a1 = (-2.0 * cosw) / a0;
  double a2 = (1.0 - alpha) / a0;
  // Add component tolerances
  double tolerance = 0.0001 * ((params.m_cutoff * params.m_cutoff) / 2);
  b0 *= (1.0 + tolerance * (static_cast<double>(rand()) / RAND_MAX - 0.5));
  b1 *= (1.0 + tolerance * (static_cast<double>(rand()) / RAND_MAX - 0.5));
  b2 *= (1.0 + tolerance * (static_cast<double>(rand()) / RAND_MAX - 0.5));
  a1 *= (1.0 + tolerance * (static_cast<double>(rand()) / RAND_MAX - 0.5));
  a2 *= (1.0 + tolerance * (static_cast<double>(rand()) / RAND_MAX - 0.5));
  // Direct Form II processing
  if (params.m_drive > 0)
  {
    sigmoidalShaper.Process(input, params);
  }
  int inputScaled = input * m_scale;
  int w = inputScaled - a1 * m_state[0] - a2 * m_state[1] + antiDenormal;
  // Apply gain compensation and soft clipping
  inputScaled = std::clamp(static_cast<int>((b0 * w + b1 * m_state[0] + b2 * m_state[1]) * mapRange((1. - params.m_resonance), 0.5, 1)), 0, m_scale - 1);
  inputScaled = (b0 * w + b1 * m_state[0] + b2 * m_state[1]) * mapRange((1. - params.m_resonance), 0.5, 1);
  // input = tanh(b0 * w + b1 * m_state[0] + b2 * m_state[1]);
  input = (double)inputScaled / m_scale;
  if (params.m_drive > 0)
  {
    sigmoidalShaper.Process(input, params);
  }
  double stage1_output = tanh(input);
  // Update states
  m_state[1] = m_state[0];
  m_state[0] = w;
  // CapLeak
  const double leakage = 0.999;
  m_state[0] *= leakage;
  m_state[1] *= leakage;
  inputScaled = stage1_output * m_scale;
  // Process second 2-pole stage
  int w2 = inputScaled - a1 * m_state[2] - a2 * m_state[3] + antiDenormal;
  inputScaled = (b0 * w2 + b1 * m_state[2] + b2 * m_state[3]) * mapRange((1. - params.m_resonance), 0.5, 1);
  input = (double)inputScaled / m_scale;
  if (params.m_drive > 0)
  {
    sigmoidalShaper.Process(input, params);
  }
  input = tanh(input);

  // Update states for second stage
  m_state[3] = m_state[2];
  m_state[2] = w2;
  m_state[3] *= leakage;
  m_state[2] *= leakage;
}

void SVF1_2P_HP_Vintage::Process(double& input, FilterParameters& params)
{
  SetSampleRate_SVF1(params);
  double resoScaling{2.01};
  const double cutoffOffset{0.25};
  double m_cutoff_scaled = cutoffOffset + (((1.0 - cutoffOffset) * params.m_cutoff) - 0.23) * (1 + 0.005 * params.m_oversample);
  int m_cutoffIndex = static_cast<int>(m_cutoff_scaled * (LUT_SIZE - 1) + 0.5);
  double f = static_cast<double>(m_LUT[m_cutoffIndex]) / m_scale;
  int fixedPointInput = (double)input * m_scale;
  // Resonance
  /*
    y[n] = y[n - 1] + f * (x[n] - y[n - 1])
   */

  for (int& pole : m_state)
  {
    if (params.m_resonance > 0)
    {
      int bandpass = - m_state[0] + m_state[1];
      double scaledDrive = params.m_drive;
      double resoCompensation = 1.0 / (1.0 + 0.015 * (params.m_oversample));
      double resonance = bandpass * params.m_resonance * resoScaling * resoCompensation;
      fixedPointInput += resonance;
    }
    pole = pole + f * (fixedPointInput - pole);
    fixedPointInput -= pole;
  }
  input = (double)fixedPointInput / m_scale;
  if (params.m_drive > 0)
  {
    double driveCompensation = 1.0 / (1.0 + 2.8 * (params.m_oversample));
    params.m_drive *= driveCompensation * 0.5;
    sigmoidalShaper.Process(input, params);
  }
  input = tanh(input);
};

void SVF1_4P_HP_Vintage::Process(double& input, FilterParameters& params)
{
  SetSampleRate_SVF1(params);
  double resoScaling{1.55};
  const double cutoffOffset{0.25};
  double m_cutoff_scaled = cutoffOffset + (((1.0 - cutoffOffset) * params.m_cutoff) - 0.2) * (1 + 0.005 * params.m_oversample);
  int m_cutoffIndex = static_cast<int>(m_cutoff_scaled * (LUT_SIZE - 1) + 0.5);
  double f = static_cast<double>(m_LUT[m_cutoffIndex]) / m_scale;
  int fixedPointInput = (double)input * m_scale;
  // Resonance
  /*
    y[n] = y[n - 1] + f * (x[n] - y[n - 1])
   */

  for (int& pole : m_state)
  {
    if (params.m_resonance > 0)
    {
      int bandpass = - m_state[1] + m_state[3];
      double scaledDrive = params.m_drive;
      double resoCompensation = 1.0 / (1.0 + 0.015 * (params.m_oversample));
      double resonance = bandpass * params.m_resonance * resoScaling * resoCompensation;
      fixedPointInput += resonance;
    }
    pole = pole + f * (fixedPointInput - pole);
    fixedPointInput -= pole;
  }
  input = (double)fixedPointInput / m_scale;
  if (params.m_drive > 0)
  {
    double driveCompensation = 1.0 / (1.0 + 2.8 * (params.m_oversample));
    params.m_drive *= driveCompensation * 0.5;
    sigmoidalShaper.Process(input, params);
  }
  input = tanh(input);
};
