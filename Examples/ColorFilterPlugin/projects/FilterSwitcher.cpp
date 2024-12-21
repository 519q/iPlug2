#include "FilterSwitcher.h"

FilterSwitcher::FilterSwitcher()

{
  m_filters[static_cast<size_t>(FilterTypes::DF1_1P)] = std::make_unique<DF1_1P>();
  m_filters[static_cast<size_t>(FilterTypes::DF1_1P_Vintage)] = std::make_unique<DF1_1P_Vintage>();
  m_filters[static_cast<size_t>(FilterTypes::DF1_2P)] = std::make_unique<DF1_2P>();
  m_filters[static_cast<size_t>(FilterTypes::DF1_3P)] = std::make_unique<DF1_3P>();
  m_filters[static_cast<size_t>(FilterTypes::DF1_4P)] = std::make_unique<DF1_4P>();
  m_filters[static_cast<size_t>(FilterTypes::DF1_6P)] = std::make_unique<DF1_6P>();
  m_filters[static_cast<size_t>(FilterTypes::DF2_2P)] = std::make_unique<DF2_2P>();
  m_filters[static_cast<size_t>(FilterTypes::DF2_4P)] = std::make_unique<DF2_4P>();
  m_filters[static_cast<size_t>(FilterTypes::SVF1_4P)] = std::make_unique<SVF1_4P>();
  // Add more filters
}

void FilterSwitcher::Process(double& input, int filterSelector, FilterParameters fParams)
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

