#include "HighPassFilters.h"

void DF1_1P_HP::Process(double& input, FilterParameters& params)
{
  int length = m_poles.size();
  getCoefficients(params);
  double reso{};
  if (length > 1)
  {
    int bpPole1 = length - 1;
    int minusbpPole2 = 0;
    if (length > 3)
      minusbpPole2 = 1;
    reso = resonate(params, m_poles[bpPole1], m_poles[minusbpPole2], ResoScaling);
  }
  for (double& pole : m_poles)
  {
    input += reso;
    processPole(input, pole, params, true);
  }
}

void DF2_2P_HP::Process(double& input, FilterParameters& params)
{
  int length = m_poles.size();
  getCoefficients(params, ResoScaling, true);
  process2pole(input, params, m_poles[0], m_poles[1]);
  if (length > 2)
    process2pole(input, params, m_poles[2], m_poles[3]);
  if (length > 4)
    process2pole(input, params, m_poles[4], m_poles[5]);
}

void SVF1_1P_HP::Process(double& input, FilterParameters& params)
{
  int length = m_poles.size();
  getCoefficients(params);
  double reso{};
  if (length > 1)
  {
    int bpPole1 = length - 1;
    int minusbpPole2 = 0;
    if (length > 3)
      minusbpPole2 = 1;
    reso = resonate(params, m_poles[bpPole1], m_poles[minusbpPole2], ResoScaling);
  }
  for (double& pole : m_poles)
  {
    input += reso;
    processPole(input, pole, params, true);
  }
};

void ZDF_1P_HP::Process(double& input, FilterParameters& params)
{
  int length = m_poles.size();
  setCutoff(params);
  double reso{};
  if (length > 1)
  {
    int bpPole1 = length - 1;
    int minusbpPole2 = 0;
    if (length > 3)
      minusbpPole2 = 1;
    reso = resonate(params, m_poles[bpPole1], m_poles[minusbpPole2], ResoScaling);
  }
  for (double& pole : m_poles)
  {
    input += reso;
    processOnePole(input, pole, true);
  }
}
