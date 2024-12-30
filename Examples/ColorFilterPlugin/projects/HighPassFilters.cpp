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
  const double cutoffOffset{0.1};
  double m_cutoff_scaled = cutoffOffset + ((1.0 - cutoffOffset) * params.m_cutoff);
  const double cutoffFreq = 2.2 * std::pow(8000.0, m_cutoff_scaled);
  m_alpha = std::exp(-2.0 * iplug::PI * cutoffFreq / params.m_sampleRate);
  double resonance{};
  if (params.m_resonance > 0)
  {
    double resoScaling{2.};
    bandpass = (m_state[1] - m_state[0]);
    resonance = (bandpass * params.m_resonance * resoScaling);
  }
  for (double& pole : m_state)
  {
    input += resonance;

    pole = (1 - m_alpha) * input + m_alpha * pole;
    double shaped = -pole;
    // input = std::clamp(shaped, -1.0, 1.0);
    input += shaped;
    dcstop.process(input, params);
  }
  if (params.m_drive > 0)
  {
    params.m_drive *= 3.6;
    sigmoidalShaper.Process(input, params);
  }
  input = tanh(input * mapRange((1. - params.m_resonance), 0.5, 1));
}

void DF1_3P_HP::Process(double& input, FilterParameters& params)
{
  const double cutoffOffset{0.25};
  double m_cutoff_scaled = cutoffOffset + (((1.0 - cutoffOffset) - 0.07) * params.m_cutoff);
  const double cutoffFreq = 2.5 * std::pow(8000.0, m_cutoff_scaled);
  m_alpha = std::exp(-2.0 * iplug::PI * cutoffFreq / params.m_sampleRate);
  double resonance{};
  if (params.m_resonance > 0)
  {
    double resoScaling{1.2};
    double bandpass = (-m_state[0] + m_state[2]);
    resonance = bandpass * params.m_resonance * resoScaling /** std::max(1., (1.2 * params.m_drive))*/;
    // input += std::clamp(feedback, -clampValue, clampValue);
  }
  for (double& pole : m_state)
  {
    input += resonance;
    pole = (1 - m_alpha) * input + m_alpha * pole;
    double shaped = -pole;
    input += shaped;
    dcstop.process(input, params);
  }
  if (params.m_drive > 0)
  {
    params.m_drive *= 4;
    sigmoidalShaper.Process(input, params);
  }
  input = tanh(input * mapRange((1. - params.m_resonance), 0.5, 1));
}

void DF1_4P_HP::Process(double& input, FilterParameters& params)
{
  const double cutoffOffset{0.25};
  double m_cutoff_scaled = cutoffOffset + (((1.0 - cutoffOffset) - 0.07) * params.m_cutoff);
  const double cutoffFreq = 2.5 * std::pow(8000.0, m_cutoff_scaled);
  m_alpha = std::exp(-2.0 * iplug::PI * cutoffFreq / params.m_sampleRate);
  double resonance{};
  if (params.m_resonance > 0)
  {
    double resoScaling{1.55};
    double bandpass = (-m_state[1] + m_state[3]);
    resonance = bandpass * params.m_resonance * resoScaling;
  }
  for (double& pole : m_state)
  {
    input += std::clamp(resonance, -clampValue, clampValue);
    pole = (1 - m_alpha) * input + m_alpha * pole;
    double shaped = -pole;
    input += shaped;
    dcstop.process(input, params);
  }
  if (params.m_drive > 0)
  {
    params.m_drive *= 4;
    sigmoidalShaper.Process(input, params);
  }
  input = tanh(input * mapRange((1. - params.m_resonance), 0.5, 1));
}

void DF1_6P_HP::Process(double& input, FilterParameters& params)
{
  const double cutoffOffset{0.25};
  double m_cutoff_scaled = cutoffOffset + (((1.0 - cutoffOffset) - 0.07) * params.m_cutoff);
  const double cutoffFreq = 2.5 * std::pow(8000.0, m_cutoff_scaled);
  m_alpha = std::exp(-2.0 * iplug::PI * cutoffFreq / params.m_sampleRate);
  double resonance{};
  if (params.m_resonance > 0)
  {
    double resoScaling{0.93};
    double bandpass = (-m_state[1] + m_state[5]);
    resonance = bandpass * params.m_resonance * resoScaling;
  }
  for (double& pole : m_state)
  {
    input += resonance;
    pole = (1 - m_alpha) * input + m_alpha * pole;
    double shaped = -pole;
    input += shaped;
    dcstop.process(input, params);
  }
  if (params.m_drive > 0)
  {
    params.m_drive *= 4;
    sigmoidalShaper.Process(input, params);
  }
  input = tanh(input * mapRange((1. - params.m_resonance), 0.5, 1));
}

void DF2_2P_HP::Process(double& input, FilterParameters& params)
{
  double resoScaling{12};
  const double cutoffOffset{0.25};
  double m_cutoff_scaled = cutoffOffset + (((1.0 - cutoffOffset) - 0.07) * params.m_cutoff);
  // Get cutoff freq in hz from parameter of double range 0...1
  const double cutoffFreq = 2.5 * std::pow(8000.0, m_cutoff_scaled);
  // Normalize frequency
  const double omega = 2.0 * iplug::PI * cutoffFreq / params.m_sampleRate;

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

  // Direct Form II processing
  if (params.m_drive > 0)
  {
    dcstop.process(input, params);
    sigmoidalShaper.Process(input, params);
  }
  double w = input - a1 * m_state[0] - a2 * m_state[1] + antiDenormal;
  // Apply gain compensation and soft clipping
  input = (b0 * w + b1 * m_state[0] + b2 * m_state[1]) * mapRange((1. - params.m_resonance), 0.5, 1);
  // input = tanh(b0 * w + b1 * m_state[0] + b2 * m_state[1]);
  if (params.m_drive > 0)
  {
    dcstop.process(input, params);
    sigmoidalShaper.Process(input, params);
  }
  input = tanh(input);
  // Update states
  m_state[1] = m_state[0];
  m_state[0] = w;
}

void DF2_4P_HP::Process(double& input, FilterParameters& params)
{
  double resoScaling{6};
  const double cutoffOffset{0.25};
  double m_cutoff_scaled = cutoffOffset + (((1.0 - cutoffOffset) - 0.07) * params.m_cutoff);
  // Get cutoff freq in hz from parameter of double range 0...1
  const double cutoffFreq = 2.5 * std::pow(8000.0, m_cutoff_scaled);
  // Normalize frequency
  const double omega = 2.0 * iplug::PI * cutoffFreq / params.m_sampleRate;

  // Constrain Q
  const double q = std::max(0.1, params.m_resonance * resoScaling) * std::max(1., (2. * params.m_drive));
  const double alpha = std::sin(omega) / (2.0 * q);

  const double cosw = std::cos(omega);
  const double a0 = 1.0 + alpha;

  // Calculate coefficients for 2-pole section
  double b0 = ((1.0 + cosw) / 2.0) / a0;
  double b1 = -(1.0 + cosw) / a0;
  double b2 = ((1.0 + cosw) / 2.0) / a0;
  double a1 = (-2.0 * cosw) / a0;
  double a2 = (1.0 - alpha) / a0;

  // Prevent denormals
  if (params.m_drive > 0)
  {
    dcstop.process(input, params);
    sigmoidalShaper.Process(input, params);
  }
  // Process first 2-pole stage
  double w1 = input - a1 * m_state[0] - a2 * m_state[1] + antiDenormal;
  // double stage1_output = b0 * w1 + b1 * m_state[0] + b2 * m_state[1] * std::max(0.1, (1. - params.m_resonance));
  double stage1_output = (b0 * w1 + b1 * m_state[0] + b2 * m_state[1]) * mapRange((1. - params.m_resonance), 0.5, 1);
  const double feedback_amount = 0.4 * (1.0 - (cutoffFreq / 20000.0));
  stage1_output += input * feedback_amount * params.m_resonance;

  if (params.m_drive > 0)
  {
    dcstop.process(input, params);
    sigmoidalShaper.Process(stage1_output, params);
  }
  // Update states for first stage
  m_state[1] = m_state[0];
  m_state[0] = w1;

  // Process second 2-pole stage
  double w2 = stage1_output - a1 * m_state[2] - a2 * m_state[3] + antiDenormal;
  input = (b0 * w2 + b1 * m_state[2] + b2 * m_state[3]) * mapRange((1. - params.m_resonance), 0.5, 1);
  if (params.m_drive > 0)
  {
    dcstop.process(input, params);
    sigmoidalShaper.Process(input, params);
  }
  input = tanh(input);

  // Update states for second stage
  m_state[3] = m_state[2];
  m_state[2] = w2;
}

void SVF1_2P_HP::Process(double& input, FilterParameters& params)
{
  const double cutoffOffset{0.25};
  // Convert normalized cutoff (0-1) to Hz
  double m_cutoff_scaled = (cutoffOffset + ((1.0 - cutoffOffset - 0.11) * params.m_cutoff) - 0.07) * (1 + 0.005 * params.m_oversample);
  // Get cutoff freq in hz from parameter of double range 0...1
  const double cutoffFreq = 2.5 * std::pow(8000.0, m_cutoff_scaled);
  // Calculate coefficient using sample rate
  double w0 = 2.0 * iplug::PI * cutoffFreq / params.m_sampleRate;
  double f = 2.0 * sin(w0 / 2.0); // bilinear transform coefficient
  f = std::clamp(f, 0.0, 1.0);    // for stability
  // Resonance
  /*
    y[n] = y[n - 1] + f * (x[n] - y[n - 1])
   */
  for (double& pole : m_state)
  {
    if (params.m_resonance > 0)
    {
      double resoScaling{2.01};
      double bandpass = -m_state[0] + m_state[1];
      double scaledDrive = params.m_drive;
      // double resoCompensation = 1.0 / sqrt(params.m_oversample + 1);
      double resoCompensation = 1.0 / (1.0 + 0.05 * (params.m_oversample));
      double resonance = bandpass * params.m_resonance * resoScaling * resoCompensation;
      // dcstop.process(resonance, params);

      input += std::clamp(resonance, -clampValue, clampValue);
    }
    pole = pole + f * (input - pole);
    input -= pole;
  }
  if (params.m_drive > 0)
  {
    // dcstop.process(input, params);

    // double driveCompensation = 1.0 / (1.0 + 1. * (params.m_oversample));
    // params.m_drive *= driveCompensation;
    sigmoidalShaper.Process(input, params);
  }
  input = tanh(input);
};

void SVF1_4P_HP::Process(double& input, FilterParameters& params)
{
  const double cutoffOffset{0.25};
  // Convert normalized cutoff (0-1) to Hz
  // double m_cutoff_scaled = cutoffOffset + ((1.0 - cutoffOffset) * params.m_cutoff) * (1 + 0.005 * params.m_oversample);
  double m_cutoff_scaled = (cutoffOffset + ((1.0 - cutoffOffset - 0.1) * params.m_cutoff) - 0.09) * (1 + 0.005 * params.m_oversample);

  // Get cutoff freq in hz from parameter of double range 0...1
  const double cutoffFreq = 2.5 * std::pow(8000.0, m_cutoff_scaled);
  // Calculate coefficient using sample rate
  double w0 = 2.0 * iplug::PI * cutoffFreq / params.m_sampleRate;
  double f = 2.0 * sin(w0 / 2.0); // bilinear transform coefficient
  f = std::clamp(f, 0.0, 1.0);    // for stability
  // Resonance
  /*
    y[n] = y[n - 1] + f * (x[n] - y[n - 1])
   */
  for (double& pole : m_state)
  {
    if (params.m_resonance > 0)
    {
      double resoScaling{1.55};
      double bandpass = -m_state[1] + m_state[3];
      double scaledDrive = params.m_drive;
      // double resoCompensation = 1.0 / sqrt(params.m_oversample + 1);
      double resoCompensation = 1.0 / (1.0 + 0.015 * (params.m_oversample));
      double resonance = bandpass * params.m_resonance * resoScaling * resoCompensation;
      // input += std::clamp(resonance, -clampValue, clampValue);
      input += resonance;
    }
    pole = std::clamp(pole + f * (input - pole), -clampValue, clampValue);
    input -= pole;
  }
  if (params.m_drive > 0)
  {
    dcstop.process(input, params, 0.1);
    double driveCompensation = 1.0 / (1.0 + 2.8 * (params.m_oversample));
    params.m_drive *= driveCompensation;
    sigmoidalShaper.Process(input, params);
  }
  input = tanh(input);
};
