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
double Filters::resonate(FilterParameters& params, double bpPole1, double minusbpPole2, double resoScl)
{
  if (params.m_resonance > 0)
  {
    double resoScaling{resoScl};
    double bandpass = (bpPole1 - minusbpPole2);
    double feedback = bandpass * params.m_resonance * resoScaling;
    return feedback;
  }
  return 0;
}
bool Filters::isDirty(FilterParameters& params)
{
  if ((m_cutoff != params.m_cutoff) || (m_reso != params.m_resonance) || (m_sampleRate != params.m_sampleRate))
  {
    m_cutoff = params.m_cutoff;
    m_reso = params.m_resonance;
    m_sampleRate = params.m_sampleRate;
    return true;
  }
  return false;
}

double Filters::getOmega(FilterParameters& params) { return 2.0 * iplug::PI * getCutoffFreq(params, params.m_cutoff) / params.m_sampleRate; }

double Filters::getCutoffFreq(FilterParameters& params, double cutoff)
{
  double cutoffFreq = 20.0 * std::pow(1000.0, cutoff);
  double maxCutoff = 0.499 * params.m_sampleRate;
  if (cutoffFreq > maxCutoff)
    cutoffFreq = maxCutoff;
  return cutoffFreq;
}

void DF1_1P_LP::getCoefficients(FilterParameters& params)
{
  if (isDirty(params))
  {
    m_alpha = std::exp(-getOmega(params));
  }
}

void DF1_1P_LP::processPole(double& input, double& state, FilterParameters& params, bool HP) const
{
  state = (1 - m_alpha) * input + m_alpha * state;
  if (!HP)
  {
    input = state;
  }
  else
  {
    input += -state;
  }
}

void DF1_1P_LP::Process(double& input, FilterParameters& params)
{
  int length = m_poles.size();
  getCoefficients(params);
  if (length > 1)
  {
    int bpPole1 = 0;
    int minusbpPole2 = length - 1;
    if (length > 3)
      bpPole1 = 1;
    input += resonate(params, m_poles[bpPole1], m_poles[minusbpPole2], ResoScaling);
  }
  for (double& pole : m_poles)
  {
    processPole(input, pole, params);
  }
}

void DF2_2P_LP::getCoefficients(FilterParameters& params, double resoScaling, bool isHP)
{
  if (isDirty(params))
  {
    const double q = std::max(0.1, params.m_resonance * resoScaling) * std::max(1., (2. * params.m_drive));
    const double alpha = std::sin(getOmega(params)) / (2.0 * q);
    const double cosw = std::cos(getOmega(params));
    const double a0 = 1.0 + alpha;

    if (!isHP)
    {
      b0 = ((1.0 - cosw) / 2.0) / a0;
      b1 = (1.0 - cosw) / a0;
      b2 = ((1.0 - cosw) / 2.0) / a0;
      a1 = (-2.0 * cosw) / a0;
      a2 = (1.0 - alpha) / a0;
    }
    else
    {
      b0 = ((1.0 + cosw) / 2.0) / a0;
      b1 = -(1.0 + cosw) / a0;
      b2 = ((1.0 + cosw) / 2.0) / a0;
      a1 = (-2.0 * cosw) / a0;
      a2 = (1.0 - alpha) / a0;
      a2 = (1.0 - alpha) / a0;
    }

    feedback_amount = 0.4 * (1.0 - (getCutoffFreq(params, params.m_cutoff) / 20000.0));
  }
}

void DF2_2P_LP::process2pole(double& input, FilterParameters& params, double& state1, double& state2) const
{
  double w = input - a1 * state1 - a2 * state2 + antiDenormal;
  input = (b0 * w + b1 * state1 + b2 * state2) * mapRange((1. - params.m_resonance), 0.5, 1);
  state2 = state1;
  state1 = w;
}

void DF2_2P_LP::Process(double& input, FilterParameters& params)
{
  int length = m_poles.size();
  getCoefficients(params, ResoScaling);
  process2pole(input, params, m_poles[0], m_poles[1]);
  if (length > 2)
    process2pole(input, params, m_poles[2], m_poles[3]);
  if (length > 4)
    process2pole(input, params, m_poles[4], m_poles[5]);
}

void SVF1_1P_LP::getCoefficients(FilterParameters& params)
{
  if (isDirty(params))
  {
    double w0 = 2.0 * iplug::PI * getCutoffFreq(params, params.m_cutoff) / params.m_sampleRate;
    m_alpha = 2.0 * sin(getOmega(params) / 2.0); // bilinear transform coefficient
    m_alpha = std::clamp(m_alpha, 0.0, 1.0);     // for stability
  }
}

void SVF1_1P_LP::processPole(double& input, double& state, FilterParameters& params, bool isHP) const
{
  state = state + m_alpha * (input - state);
  if (!isHP)
  {
    input = state;
  }
  else
  {
    input -= state;
  }
}

void SVF1_1P_LP::Process(double& input, FilterParameters& params)
{
  int length = m_poles.size();
  getCoefficients(params);
  if (length > 1)
  {
    int bpPole1 = 0;
    if (length > 3)
      bpPole1 = 1;
    int minusbpPole2 = length - 1;
    input += resonate(params, m_poles[bpPole1], m_poles[minusbpPole2], ResoScaling);
  }
  for (double& pole : m_poles)
  {
    processPole(input, pole, params);
  }
};

void ZDF_1P_LP::setCutoff(FilterParameters& params)
{
  if (isDirty(params))
  {
    double cutoffFreq = getCutoffFreq(params, params.m_cutoff);
    m_alpha = std::tan(iplug::PI * (cutoffFreq / params.m_sampleRate));
  }
}

void ZDF_1P_LP::processOnePole(double& input, double& state, bool HP) const
{
  double v = (input - state) / (1.0 + m_alpha);
  state = m_alpha * v + state;
  if (!HP)
    input = state;
  else
    input -= state;
}

void ZDF_1P_LP::Process(double& input, FilterParameters& params)
{
  int length = m_poles.size();
  setCutoff(params);
  if (length > 1)
  {
    int bpPole1 = 0;
    if (length > 3)
      bpPole1 = 1;
    int minusbpPole2 = length - 1;
    input += resonate(params, m_poles[bpPole1], m_poles[minusbpPole2], ResoScaling);
  }

  for (double& pole : m_poles)
    processOnePole(input, pole);
}
