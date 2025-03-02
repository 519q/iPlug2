#include "BandPassFilters.h"

void BandpassFilters::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lpl->Process(m_stateL, params);

  m_stateH = input;
  params.m_cutoff += bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lph->Process(m_stateH, params);
  input = m_stateL - m_stateH;
}
