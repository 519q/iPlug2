#include "VintageFilters.h"

void VintageProcessor::MapLUTCutoff(FilterParameters& params)
{
  // Ensure normalization to the range [0.0, 1.0]
  params.m_cutoff = std::clamp(params.m_cutoff, 0.0, 1.0);
  // Safely map knobValue to LUT index
  m_cutoffIndex = static_cast<int>(params.m_cutoff * (LUT_SIZE - 1) + 0.5); // Add 0.5 for rounding
};

void VintageProcessor::InitLUT_DF1()
{
  m_LUT.resize(LUT_SIZE);
  double nyquist = m_sampleRate / 2.0;
  for (int i = 0; i < LUT_SIZE; i++)
  {

    double cutoffFreq = 20 * std::pow(1000.0, (double)i / (double)LUT_SIZE);
    cutoffFreq = std::min(cutoffFreq, nyquist); // Clamp cutoffFreq to Nyquist
    m_LUT[i] = static_cast<int>((std::exp(-2.0 * iplug::PI * cutoffFreq / m_sampleRate)) * m_scale);
  }
};

void VintageProcessor::InitLUT_SVF1()
{
  m_LUT.resize(LUT_SIZE);
  double nyquist = m_sampleRate / 2.0;
  for (int i = 0; i < LUT_SIZE; i++)
  {
    double cutoffFreq = 20 * std::pow(1000.0, (double)i / (double)LUT_SIZE);
    cutoffFreq = std::min(cutoffFreq, nyquist); // Clamp cutoffFreq to Nyquist
    double w0 = 2.0 * iplug::PI * cutoffFreq / m_sampleRate;
    m_LUT[i] = static_cast<int>((2.0 * sin(w0 / 2.0)) * m_scale);
  }
};

// void VintageProcessor::InitLUT_DF2()
//{
//   m_LUT_cutoff.resize(LUT_SIZE);
//   m_LUT_resonance.resize(LUT_SIZE);
//   double nyquist = m_sampleRate / 2.0;
//
//   for (int i = 0; i < LUT_SIZE; i++)
//   {
//     double cutoffFreq = 2.5 * std::pow(8000.0, (double)i / (double)LUT_SIZE);
//     cutoffFreq = std::min(cutoffFreq, nyquist); // Clamp cutoffFreq to Nyquist
//
//     double omega = 2.0 * iplug::PI * cutoffFreq / m_sampleRate;
//     double alpha = std::sin(omega) / (2.0 * 0.1); // Assuming Q=0.1 for LUT
//
//     // Calculate coefficients for a low-pass filter
//     double cosw = std::cos(omega);
//     double a0 = 1.0 + alpha;
//     double b0 = ((1.0 - cosw) / 2.0) / a0;
//     double b1 = (1.0 - cosw) / a0;
//     double b2 = ((1.0 - cosw) / 2.0) / a0;
//     double a1 = (-2.0 * cosw) / a0;
//     double a2 = (1.0 - alpha) / a0;
//
//     // Store coefficients in LUTs
//     m_LUT_cutoff[i] = static_cast<int>(b0 * m_scale);       // Example: store b0
//     m_LUT_resonance[i] = static_cast<int>(alpha * m_scale); // Example: store alpha
//   }
// }

void VintageProcessor::SetSampleRate_DF1(FilterParameters& params)
{
  if (m_sampleRate != params.m_sampleRate)
  {
    m_sampleRate = params.m_sampleRate;
    InitLUT_DF1();
  }
};

void VintageProcessor::SetSampleRate_SVF1(FilterParameters& params)
{
  if (m_sampleRate != params.m_sampleRate)
  {
    m_sampleRate = params.m_sampleRate;
    InitLUT_SVF1();
  }
};

// void VintageProcessor::SetSampleRate_DF2(FilterParameters& params)
//{
//   if (m_sampleRate != params.m_sampleRate)
//   {
//     m_sampleRate = params.m_sampleRate;
//     InitLUT_DF2();
//   }
// };

void DF1_1P_LP_Vintage::getAlpha(FilterParameters& params)
{
  if (isDirty(params))
  {
    SetSampleRate_DF1(params);
    MapLUTCutoff(params);
    int m_cutoffIndex = static_cast<int>(params.m_cutoff * (LUT_SIZE - 1) + 0.5);
    m_alpha = static_cast<double>(m_LUT[m_cutoffIndex]) / m_scale;
  }
}

void DF1_1P_LP_Vintage::processPoleLP(int& input, int& state, FilterParameters& params) const
{
  state = (1 - m_alpha) * input + m_alpha * state;
  input = state;
}

void DF1_1P_LP_Vintage::Process(double& input, FilterParameters& params)
{
  getAlpha(params);
  int fixedPointInput = (double)input * m_scale;
  processPoleLP(fixedPointInput, m_poles, params);
  input = (double)fixedPointInput / m_scale;
}

void DF1_2P_LP_Vintage::Process(double& input, FilterParameters& params)
{
  getAlpha(params);
  int fixedPointInput = (double)input * m_scale;
  fixedPointInput += resonate(fixedPointInput, params, m_poles[0], m_poles[1], 2);
  for (int& pole : m_poles)
  {
    processPoleLP(fixedPointInput, pole, params);
  }
  input = (double)fixedPointInput / m_scale;
}

void DF1_3P_LP_Vintage::Process(double& input, FilterParameters& params)
{
  getAlpha(params);
  int fixedPointInput = (double)input * m_scale;
  fixedPointInput += resonate(fixedPointInput, params, m_poles[0], m_poles[2], 1.2);
  for (int& pole : m_poles)
  {
    processPoleLP(fixedPointInput, pole, params);
  }
  input = (double)fixedPointInput / m_scale;
}

void DF1_4P_LP_Vintage::Process(double& input, FilterParameters& params)
{
  getAlpha(params);
  int fixedPointInput = (double)input * m_scale;
  fixedPointInput += resonate(fixedPointInput, params, m_poles[1], m_poles[3], 1.44);
  for (int& pole : m_poles)
  {
    processPoleLP(fixedPointInput, pole, params);
  }
  input = (double)fixedPointInput / m_scale;
}

void DF1_6P_LP_Vintage::Process(double& input, FilterParameters& params)
{
  getAlpha(params);
  int fixedPointInput = (double)input * m_scale;
  fixedPointInput += resonate(fixedPointInput, params, m_poles[1], m_poles[5], 0.85);
  for (int& pole : m_poles)
  {
    processPoleLP(fixedPointInput, pole, params);
  }
  input = (double)fixedPointInput / m_scale;
}

void DF2_2P_LP_Vintage::addTolerances(FilterParameters& params)
{
  double tolerance = 0.000005 * ((params.m_cutoff * params.m_cutoff) / 2);
  b0 *= (1.0 + tolerance * (static_cast<double>(rand()) / RAND_MAX - 0.5));
  b1 *= (1.0 + tolerance * (static_cast<double>(rand()) / RAND_MAX - 0.5));
  b2 *= (1.0 + tolerance * (static_cast<double>(rand()) / RAND_MAX - 0.5));
  a1 *= (1.0 + tolerance * (static_cast<double>(rand()) / RAND_MAX - 0.5));
  a2 *= (1.0 + tolerance * (static_cast<double>(rand()) / RAND_MAX - 0.5));
}

void DF2_2P_LP_Vintage::addLeakage()
{
  const double leakage = 0.999;
  for (int pole : m_poles)
  {
    pole *= leakage;
  }
}

void DF2_2P_LP_Vintage::Process(double& input, FilterParameters& params)
{
  getCoefficients(params, 12.3);
  addTolerances(params);
  int inputScaled = input * m_scale;
  process2pole(inputScaled, params, m_poles[0], m_poles[1]);
  input = (double)inputScaled / m_scale;
  addLeakage();
}

void DF2_4P_LP_Vintage::Process(double& input, FilterParameters& params)
{
  getCoefficients(params, 6);
  addTolerances(params);
  int inputScaled = input * m_scale;
  process2pole(inputScaled, params, m_poles[0], m_poles[1]);
  input = (double)inputScaled / m_scale;
  inputScaled = input * m_scale;
  process2pole(inputScaled, params, m_poles[2], m_poles[3]);
  input = (double)inputScaled / m_scale;
  addLeakage();
}

void SVF1_2P_LP_Vintage::getCoefficients(FilterParameters& params, double cutoffOffset, double offset)
{
  SetSampleRate_SVF1(params);
  double m_cutoff_scaled = cutoffOffset + (((1.0 - cutoffOffset + offset) * params.m_cutoff) - 0.1078 + offset) * (1 + 0.005 * params.m_oversample);
  int m_cutoffIndex = static_cast<int>(m_cutoff_scaled * (LUT_SIZE - 1) + 0.5);
  f = static_cast<double>(m_LUT[m_cutoffIndex]) / m_scale;
}

void SVF1_2P_LP_Vintage::Process(double& input, FilterParameters& params)
{
  getCoefficients(params);
  int inputScaled = input * m_scale;
  inputScaled += resonate(inputScaled, params, m_poles[0], m_poles[1], 2);
  for (int& pole : m_poles)
  {
    processPole(inputScaled, pole, params);
  }
  input = (double)inputScaled / m_scale;
};

void SVF1_4P_LP_Vintage::Process(double& input, FilterParameters& params)
{
  getCoefficients(params);
  int inputScaled = input * m_scale;
  inputScaled += resonate(inputScaled, params, m_poles[1], m_poles[3], 1.55);
  for (int& pole : m_poles)
  {
    processPole(inputScaled, pole, params);
  }
  input = (double)inputScaled / m_scale;
};

void SVF1_6P_LP_Vintage::Process(double& input, FilterParameters& params)
{
  getCoefficients(params);
  int inputScaled = input * m_scale;
  inputScaled += resonate(inputScaled, params, m_poles[1], m_poles[5], 0.95);
  for (int& pole : m_poles)
  {
    processPole(inputScaled, pole, params);
  }
  input = (double)inputScaled / m_scale;
};
