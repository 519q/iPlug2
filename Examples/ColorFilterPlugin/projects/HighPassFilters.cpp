#include "HighPassFilters.h"

void DF1_1P_HP::processPoleHP(double& input, double& state, FilterParameters& params) const
{
  state = (1 - m_alpha) * input + m_alpha * state;
  double shaped = -state;
  input += shaped;
}

void DF1_1P_HP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params, 0.1);
  processPoleHP(input, m_state, params);
}

void DF1_2P_HP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params, 0.1);
  double reso = resonate(input, params, m_poles[1], m_poles[0], DF1_2P_ResoScaling);
  for (double& pole : m_poles)
  {
    input += reso;
    processPoleHP(input, pole, params);
  }
}

void DF1_3P_HP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params, 0.1);
  double reso = resonate(input, params, m_poles[2], m_poles[0], DF1_3P_ResoScaling);
  for (double& pole : m_poles)
  {
    input += reso;
    processPoleHP(input, pole, params);
  }
}

void DF1_4P_HP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params, 0.1);
  double reso = resonate(input, params, m_poles[3], m_poles[1], DF1_4P_ResoScaling);
  for (double& pole : m_poles)
  {
    input += reso;
    processPoleHP(input, pole, params);
  }
}

void DF1_6P_HP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params, 0.1);
  double reso = resonate(input, params, m_poles[5], m_poles[1], DF1_6P_ResoScaling);
  for (double& pole : m_poles)
  {
    input += reso;
    processPoleHP(input, pole, params);
  }
}

void DF2_2P_HP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params, DF2_2P_ResoScaling, 0.25, -0.01, true);
  process2pole(input, params, m_poles[0], m_poles[1]);
}

void DF2_4P_HP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params, DF2_4P_ResoScaling, 0.25, -0.01, true);
  process2pole(input, params, m_poles[0], m_poles[1]);
  process2pole(input, params, m_poles[2], m_poles[3]);
}

void SVF1_2P_HP::processPoleHP(double& input, double& state, FilterParameters& params)
{
  state = state + f * (input - state);
  input -= state;
}

void SVF1_2P_HP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params, 0.25, -0.1);
  double reso = resonate(input, params, m_poles[1], m_poles[0], SVF1_2P_ResoScaling);
  for (double& pole : m_poles)
  {
    input += reso;
    processPoleHP(input, pole, params);
  }
};

void SVF1_4P_HP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params, 0.25, -0.1);
  double reso = resonate(input, params, m_poles[3], m_poles[1], SVF1_4P_ResoScaling);
  for (double& pole : m_poles)
  {
    input += reso;
    processPoleHP(input, pole, params);
  }
};

void SVF1_6P_HP::Process(double& input, FilterParameters& params)
{
  getCoefficients(params, 0.25, -0.1);
  double reso = resonate(input, params, m_poles[5], m_poles[1], SVF1_6P_ResoScaling);
  for (double& pole : m_poles)
  {
    input += reso;
    processPoleHP(input, pole, params);
  }
};
