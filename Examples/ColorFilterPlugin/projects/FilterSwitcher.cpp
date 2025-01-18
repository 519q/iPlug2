#include "FilterSwitcher.h"


void FilterSwitcher::Process(double& input, int filterSelector, FilterParameters& fParams)
{
  if (filterSelector != (int)m_currentFilter && filterSelector < (int)FilterTypes::MAX_FILTER_TYPES)
  {
    m_currentFilter = (FilterTypes)filterSelector;
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



