#include "BandStopFilters.h"

void DF1_1P_BS::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  lp.Process(m_stateL, params);
  m_stateH = input;
  params.m_cutoff += 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  hp.Process(m_stateH, params);
  input = m_stateH + m_stateL;
}

void DF1_2P_BS::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  lp.Process(m_stateL, params);
  m_stateH = input;
  params.m_cutoff += 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  hp.Process(m_stateH, params);
  input = m_stateH + m_stateL;
}

void DF1_3P_BS::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  lp.Process(m_stateL, params);
  m_stateH = input;
  params.m_cutoff += 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  hp.Process(m_stateH, params);
  input = m_stateH + m_stateL;
}

void DF1_4P_BS::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  lp.Process(m_stateL, params);
  m_stateH = input;
  params.m_cutoff += 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  hp.Process(m_stateH, params);
  input = m_stateH + m_stateL;
}

void DF1_6P_BS::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  lp.Process(m_stateL, params);
  m_stateH = input;
  params.m_cutoff += 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  hp.Process(m_stateH, params);
  input = m_stateH + m_stateL;
}

void DF2_2P_BS::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  lp.Process(m_stateL, params);
  m_stateH = input;
  params.m_cutoff += 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  hp.Process(m_stateH, params);
  input = m_stateH + m_stateL;
}

void DF2_4P_BS::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  lp.Process(m_stateL, params);
  m_stateH = input;
  params.m_cutoff += 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  hp.Process(m_stateH, params);
  input = m_stateH + m_stateL;
}

void SVF1_2P_BS::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  lp.Process(m_stateL, params);
  m_stateH = input;
  params.m_cutoff += 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  hp.Process(m_stateH, params);
  input = m_stateH + m_stateL;
}

void SVF1_4P_BS::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  lp.Process(m_stateL, params);
  m_stateH = input;
  params.m_cutoff += 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  hp.Process(m_stateH, params);
  input = m_stateH + m_stateL;
}

void SVF1_6P_BS::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  lp.Process(m_stateL, params);
  m_stateH = input;
  params.m_cutoff += 0.05 + (m_bandwidth / 2) * params.m_bandwidth;
  hp.Process(m_stateH, params);
  input = m_stateH + m_stateL;
}
