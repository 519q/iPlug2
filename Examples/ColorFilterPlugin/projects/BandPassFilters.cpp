#include "BandPassFilters.h"

void DF1_1P_BP::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= bandwidthLowestLimit + (m_bandwidth / 2);
  lpl.Process(m_stateL, params);

  m_stateH = input;
  params.m_cutoff += bandwidthLowestLimit + (m_bandwidth / 2);
  lph.Process(m_stateH, params);
  input = m_stateL - m_stateH;
}

void DF1_2P_BP::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lpl.Process(m_stateL, params);

  m_stateH = input;
  params.m_cutoff += bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lph.Process(m_stateH, params);
  input = m_stateL - m_stateH;
}

void DF1_3P_BP::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lpl.Process(m_stateL, params);

  m_stateH = input;
  params.m_cutoff += bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lph.Process(m_stateH, params);
  input = m_stateL - m_stateH;
}

void DF1_4P_BP::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lpl.Process(m_stateL, params);

  m_stateH = input;
  params.m_cutoff += bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lph.Process(m_stateH, params);
  input = m_stateL - m_stateH;
}

void DF1_6P_BP::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lpl.Process(m_stateL, params);

  m_stateH = input;
  params.m_cutoff += bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lph.Process(m_stateH, params);
  input = m_stateL - m_stateH;
}

void DF2_2P_BP::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lpl.Process(m_stateL, params);

  m_stateH = input;
  params.m_cutoff += bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lph.Process(m_stateH, params);
  input = m_stateL - m_stateH;
}

void DF2_4P_BP::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lpl.Process(m_stateL, params);

  m_stateH = input;
  params.m_cutoff += bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lph.Process(m_stateH, params);
  input = m_stateL - m_stateH;
}

void SVF1_2P_BP::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lpl.Process(m_stateL, params);

  m_stateH = input;
  params.m_cutoff += bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lph.Process(m_stateH, params);
  input = m_stateL - m_stateH;
}

void SVF1_4P_BP::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lpl.Process(m_stateL, params);

  m_stateH = input;
  params.m_cutoff += bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lph.Process(m_stateH, params);
  input = m_stateL - m_stateH;
}

void SVF1_6P_BP::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lpl.Process(m_stateL, params);

  m_stateH = input;
  params.m_cutoff += bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lph.Process(m_stateH, params);
  input = m_stateL - m_stateH;
}
