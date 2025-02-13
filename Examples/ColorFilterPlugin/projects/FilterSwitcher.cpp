#include "FilterSwitcher.h"


void FilterSwitcher::Process(double& input, int m_filterSelector, FilterParameters& fParams)
{
  if (m_filterSelector != (int)m_currentFilter && m_filterSelector < (int)FilterTypes::MAX_FILTER_TYPES)
  {
    m_currentFilter = (FilterTypes)m_filterSelector;
  }
  if (m_filters[static_cast<size_t>(m_currentFilter)])
  {
    m_filters[static_cast<size_t>(m_currentFilter)]->Process(input, fParams);
  }
  if (m_currentFilter != m_targetFilter)
  {
    m_currentFilter = m_targetFilter;
  }
}



