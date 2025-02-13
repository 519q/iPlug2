#include "BandStopFilters.h"

void BandstopFilters::Process(double& input, FilterParameters& params)
{
  m_stateL = input;
  params.m_cutoff -= bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  lp->Process(m_stateL, params);
  m_stateH = input;
  params.m_cutoff += bandwidthLowestLimit + (m_bandwidth / 2) * params.m_bandwidth;
  hp->Process(m_stateH, params);
  input = m_stateH + m_stateL;
}
