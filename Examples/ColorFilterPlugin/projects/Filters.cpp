#include "Filters.h"
#include "projects/Shapers.h"
/*
1. Direct Form I: y[n] = b0*x[n] + b1*x[n-1] - a1*y[n-1]
                     b0 = (1-a1)
IIR
exponential average moving filter
- y[n] is the current output sample
- x[n] is the current input
* sample
- x[n-1] is the previous input sample
- y[n-1] is the previous output sample
- b0 and b1 are feedforward coefficients
- a1 is the feedback coefficient.
*/

void DF1_1P_LP::Process(double& input, FilterParameters& params)
{
  const double cutoffFreq = 2.5 * std::pow(8000.0, params.m_cutoff);
  m_alpha = std::exp(-2.0 * iplug::PI * cutoffFreq / params.m_sampleRate);
  m_state = (1 - m_alpha) * input + m_alpha * m_state;
  input = m_state;
}

void DF1_1P_LP_Vintage::MapLUTCutoff(FilterParameters& params)
{
  // Ensure normalization to the range [0.0, 1.0]
  params.m_cutoff = std::clamp(params.m_cutoff, 0.0, 1.0);

  // Safely map knobValue to LUT index
  m_cutoffIndex = static_cast<int>(params.m_cutoff * (LUT_SIZE - 1) + 0.5); // Add 0.5 for rounding
}

void DF1_1P_LP_Vintage::SetSampleRate(FilterParameters& params)
{
  if (m_sampleRate != params.m_sampleRate)
  {
    m_sampleRate = params.m_sampleRate;
    InitLUT();
  }
}

void DF1_1P_LP_Vintage::InitLUT()
{
  double nyquist = m_sampleRate / 2.0;
  for (int i = 0; i < LUT_SIZE; i++)
  {
    double cutoffFreq = 2.5 * std::pow(8000.0, (double)i / (double)LUT_SIZE);
    cutoffFreq = std::min(cutoffFreq, nyquist); // Clamp cutoffFreq to Nyquist

    m_LUT[i] = static_cast<int>((std::exp(-2.0 * iplug::PI * cutoffFreq / m_sampleRate)) * m_scale);
  }
}

void DF1_1P_LP_Vintage::Process(double& input, FilterParameters& params)
{
  SetSampleRate(params);
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
  input = (double)m_state / m_scale;
}

void DF1_2P_LP::Process(double& input, FilterParameters& params)
{
  const double cutoffOffset{0.25};
  double m_cutoff_scaled = cutoffOffset + (((1.0 - cutoffOffset) - 0.07) * params.m_cutoff);
  const double cutoffFreq = 2.5 * std::pow(8000.0, m_cutoff_scaled);
  m_alpha = std::exp(-2.0 * iplug::PI * cutoffFreq / params.m_sampleRate);
  if (params.m_resonance > 0)
  {
    double resoScaling{2.};
    double bandpass = (m_state[0] - m_state[1]);
    double feedback = bandpass * params.m_resonance * resoScaling;
    // input += feedback;
    input += feedback;
  }
  for (double& pole : m_state)
  {
    pole = (1 - m_alpha) * input + m_alpha * pole;
    double shaped = pole;
    params.m_drive /= 2;
    // input = std::clamp(shaped, -1.0, 1.0);
    if (params.m_drive > 0)
    {
      sigmoidalShaper.Process(shaped, params);
    }
    input = shaped;
  }
  input = tanh(input * mapRange((1. - params.m_resonance), 0.5, 1));
}

void DF1_3P_LP::Process(double& input, FilterParameters& params)
{
  double resoScaling{1.2};
  const double cutoffOffset{0.25};
  double m_cutoff_scaled = cutoffOffset + (((1.0 - cutoffOffset) - 0.07) * params.m_cutoff);
  const double cutoffFreq = 2.5 * std::pow(8000.0, m_cutoff_scaled);
  m_alpha = std::exp(-2.0 * iplug::PI * cutoffFreq / params.m_sampleRate);
  if (params.m_resonance > 0)
  {
    double bandpass = (m_state[0] - m_state[2]);
    double feedback = bandpass * params.m_resonance * resoScaling;
    input += std::clamp(feedback, -clampValue, clampValue);
    // input += feedback;
  }
  for (double& pole : m_state)
  {
    pole = (1 - m_alpha) * input + m_alpha * pole;
    double shaped = pole;
    params.m_drive /= 1.3;
    if (params.m_drive > 0)
    {
      params.m_drive *= 0.65;
      sigmoidalShaper.Process(shaped, params);
    }
    input = shaped;
  }
  input = tanh(input * mapRange((1. - params.m_resonance), 0.5, 1));
}

void DF1_4P_LP::Process(double& input, FilterParameters& params)
{
  double resoScaling{1.44};
  const double cutoffOffset{0.25};
  double m_cutoff_scaled = cutoffOffset + (((1.0 - cutoffOffset) - 0.07) * params.m_cutoff);
  const double cutoffFreq = 2.5 * std::pow(8000.0, m_cutoff_scaled);
  m_alpha = std::exp(-2.0 * iplug::PI * cutoffFreq / params.m_sampleRate);
  if (params.m_resonance > 0)
  {
    double bandpass = (m_state[1] - m_state[3]);
    double resonance = bandpass * params.m_resonance * resoScaling/* * std::max(1., (2. * params.m_drive))*/;
    input += std::clamp(resonance, -clampValue, clampValue);
  }
  for (double& pole : m_state)
  {
    pole = (1 - m_alpha) * input + m_alpha * pole;
    double shaped = pole;
    if (params.m_drive > 0)
    {
      params.m_drive *= 0.65;
      sigmoidalShaper.Process(shaped, params);
    }
    input = shaped;
  }
  input = tanh(input * mapRange((1. - params.m_resonance), 0.5, 1));
}

void DF1_6P_LP::Process(double& input, FilterParameters& params)
{
  double resoScaling{0.95};
  const double cutoffOffset{0.25};
  double m_cutoff_scaled = cutoffOffset + (((1.0 - cutoffOffset) - 0.07) * params.m_cutoff);
  const double cutoffFreq = 2.5 * std::pow(8000.0, m_cutoff_scaled);
  m_alpha = std::exp(-2.0 * iplug::PI * cutoffFreq / params.m_sampleRate);
  if (params.m_resonance > 0)
  {
    double bandpass = (m_state[1] - m_state[5]);
    double resonance = bandpass * params.m_resonance * resoScaling/* * std::max(1., (2. * params.m_drive))*/;
    input += std::clamp(resonance, -clampValue, clampValue);
  }
  for (double& pole : m_state)
  {
    pole = (1 - m_alpha) * input + m_alpha * pole;
    double shaped = pole;
    if (params.m_drive > 0)
    {
      params.m_drive *= 0.65;
      sigmoidalShaper.Process(shaped, params);
    }
    input = shaped;
  }
  input = tanh(input * mapRange((1. - params.m_resonance), 0.5, 1));
}

void DF2_2P_LP::Process(double& input, FilterParameters& params)
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
  double b0 = ((1.0 - cosw) / 2.0) / a0;
  double b1 = (1.0 - cosw) / a0;
  double b2 = ((1.0 - cosw) / 2.0) / a0;
  double a1 = (-2.0 * cosw) / a0;
  double a2 = (1.0 - alpha) / a0;
  // Prevent denormals

  // Direct Form II processing
  if (params.m_drive > 0)
  {
    sigmoidalShaper.Process(input, params);
  }
  double w = input - a1 * m_state[0] - a2 * m_state[1] + antiDenormal;
  // Apply gain compensation and soft clipping
  input = (b0 * w + b1 * m_state[0] + b2 * m_state[1]) * mapRange((1. - params.m_resonance), 0.5, 1);
  // input = tanh(b0 * w + b1 * m_state[0] + b2 * m_state[1]);
  if (params.m_drive > 0)
  {
    sigmoidalShaper.Process(input, params);
  }
  input = tanh(input);
  // Update states
  m_state[1] = m_state[0];
  m_state[0] = w;
}

void DF2_4P_LP::Process(double& input, FilterParameters& params)
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
  double b0 = ((1.0 - cosw) / 2.0) / a0;
  double b1 = (1.0 - cosw) / a0;
  double b2 = ((1.0 - cosw) / 2.0) / a0;
  double a1 = (-2.0 * cosw) / a0;
  double a2 = (1.0 - alpha) / a0;

  // Prevent denormals
  if (params.m_drive > 0)
  {
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
    sigmoidalShaper.Process(input, params);
  }
  input = tanh(input);

  // Update states for second stage
  m_state[3] = m_state[2];
  m_state[2] = w2;
}

void SVF1_2P_LP::Process(double& input, FilterParameters& params)
{
  double resoScaling{2};
  const double cutoffOffset{0.25};
  // Convert normalized cutoff (0-1) to Hz
  double m_cutoff_scaled = cutoffOffset + ((1.0 - cutoffOffset) * params.m_cutoff) * (1 + 0.005 * params.m_oversample);
  // Get cutoff freq in hz from parameter of double range 0...1
  const double cutoffFreq = 2.5 * std::pow(8000.0, m_cutoff_scaled);
  // Calculate coefficient using sample rate
  double w0 = 2.0 * iplug::PI * cutoffFreq / params.m_sampleRate;
  double f = 2.0 * sin(w0 / 2.0); // bilinear transform coefficient
  f = std::clamp(f, 0.0, 1.0);    // for stability
  // Resonance
  if (params.m_resonance > 0)
  {
    double bandpass = m_state[0] - m_state[1];
    double scaledDrive = params.m_drive;
    // double resoCompensation = 1.0 / sqrt(params.m_oversample + 1);
    double resoCompensation = 1.0 / (1.0 + 0.015 * (params.m_oversample));
    double resonance = bandpass * params.m_resonance * resoScaling * resoCompensation;
    //input += std::clamp(resonance, -clampValue, clampValue);
    //dcstop.process(resonance, params, 0.1);
    input += resonance;
  }
  /*
    y[n] = y[n - 1] + f * (x[n] - y[n - 1])
   */

  for (double& pole : m_state)
  {
    //dcstop.process(input, params);
    pole = pole + f * (input - pole);
    input = pole;
    if (params.m_drive > 0)
    {
      double driveCompensation = 1.0 / (1.0 + 2.8 * (params.m_oversample));
      params.m_drive *= driveCompensation * 0.5;
      sigmoidalShaper.Process(pole, params);
      //dcstop.process(input, params, 0.1);
      //dcstop.process(input, params, 0.1);
    }
  }
  input = tanh(input);
};

void SVF1_4P_LP::Process(double& input, FilterParameters& params)
{
  double resoScaling{1.55};
  const double cutoffOffset{0.25};
  // Convert normalized cutoff (0-1) to Hz
  double m_cutoff_scaled = cutoffOffset + ((1.0 - cutoffOffset) * params.m_cutoff) * (1 + 0.005 * params.m_oversample);
  // Get cutoff freq in hz from parameter of double range 0...1
  const double cutoffFreq = 2.5 * std::pow(8000.0, m_cutoff_scaled);
  // Calculate coefficient using sample rate
  double w0 = 2.0 * iplug::PI * cutoffFreq / params.m_sampleRate;
  double f = 2.0 * sin(w0 / 2.0); // bilinear transform coefficient
  f = std::clamp(f, 0.0, 1.0);    // for stability
  // Resonance
  if (params.m_resonance > 0)
  {
    double bandpass = m_state[1] - m_state[3];
    double scaledDrive = params.m_drive;
    // double resoCompensation = 1.0 / sqrt(params.m_oversample + 1);
    double resoCompensation = 1.0 / (1.0 + 0.015 * (params.m_oversample));
    double resonance = bandpass * params.m_resonance * resoScaling * resoCompensation;
    //input += std::clamp(resonance, -clampValue, clampValue);
    input += resonance;
  }
  /*
    y[n] = y[n - 1] + f * (x[n] - y[n - 1])
   */
  for (double& pole : m_state)
  {
    pole = std::clamp(pole + f * (input - pole), -clampValue, clampValue);
    input = pole;
    if (params.m_drive > 0)
    {
      //dcstop.process(input, params, 0.1);
      double driveCompensation = 1.0 / (1.0 + 2.8 * (params.m_oversample));
      params.m_drive *= driveCompensation * 0.5;
      sigmoidalShaper.Process(pole, params);
    }
  }
  input = tanh(input);
};
