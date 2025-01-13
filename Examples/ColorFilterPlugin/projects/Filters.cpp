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

double Filters::getOmega(FilterParameters& params) { return 2.0 * iplug::PI * getCutoffFreq(params) / params.m_sampleRate; }

double Filters::getCutoffFreq(FilterParameters& params) { return 20.0 * std::pow(1000.0, params.m_cutoff); }

void DF1_1P_LP::getCoefficients(FilterParameters& params, double cutoffOffset)
{
  if (isDirty(params))
  {
    m_alpha = std::exp(-getOmega(params));
  }
}

void DF1_1P_LP::processPoleLP(double& input, double& state, FilterParameters& params) const
{
  state = (1 - m_alpha) * input + m_alpha * state;
  double shaped = state;
  input = shaped;
}

void DF1_1P_LP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params);
  processPoleLP(input, m_state, params);
}

void DF1_2P_LP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params);
  input += resonate(input, params, m_poles[0], m_poles[1], DF1_2P_ResoScaling);
  for (double& pole : m_poles)
  {
    processPoleLP(input, pole, params);
  }
}

void DF1_3P_LP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params);
  input += resonate(input, params, m_poles[0], m_poles[2], DF1_3P_ResoScaling);
  for (double& pole : m_poles)
  {
    processPoleLP(input, pole, params);
  }
}

void DF1_4P_LP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params);
  input += resonate(input, params, m_poles[1], m_poles[3], DF1_4P_ResoScaling);
  for (double& pole : m_poles)
  {
    processPoleLP(input, pole, params);
  }
}

void DF1_6P_LP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params);
  input += resonate(input, params, m_poles[1], m_poles[5], DF1_6P_ResoScaling);
  for (double& pole : m_poles)
  {
    processPoleLP(input, pole, params);
  }
}

void DF2_2P_LP::getCoefficients(FilterParameters& params, double resoScaling, double cutoffOffset, double offset, bool isHP)
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

    feedback_amount = 0.4 * (1.0 - (getCutoffFreq(params) / 20000.0));
  }
}

void DF2_2P_LP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params, DF2_2P_ResoScaling);
  process2pole(input, params, m_state[0], m_state[1]);
  input = filnalTanh(input, params);
}

void DF2_4P_LP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params, DF2_4P_ResoScaling);
  process2pole(input, params, m_state[0], m_state[1]);
  process2pole(input, params, m_state[2], m_state[3]);
}

void SVF1_2P_LP::getCoefficients(FilterParameters& params, double cutoffOffset, double offset)
{
  double w0 = 2.0 * iplug::PI * getCutoffFreq(params) / params.m_sampleRate;
  f = 2.0 * sin(getOmega(params) / 2.0); // bilinear transform coefficient
  f = std::clamp(f, 0.0, 1.0);                                 // for stability
}

void SVF1_2P_LP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params);
  input += resonate(input, params, m_poles[0], m_poles[1], SVF1_2P_ResoScaling);
  for (double& pole : m_poles)
  {
    processPoleLP(input, pole, params);
  }
};

void SVF1_4P_LP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params);
  input += resonate(input, params, m_poles[1], m_poles[3], SVF1_4P_ResoScaling);
  for (double& pole : m_poles)
  {
    processPoleLP(input, pole, params);
  }
};

void SVF1_6P_LP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params);
  input += resonate(input, params, m_poles[1], m_poles[5], SVF1_6P_ResoScaling);
  for (double& pole : m_poles)
  {
    processPoleLP(input, pole, params);
  }
};
