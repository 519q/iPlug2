#pragma once
#include "Filters.h"
#include "SmoothTools.h"


class Filters;

class FilterSwitcher
{
private:
  std::array<std::unique_ptr<Filters>, static_cast<int>(FilterTypes::MAX_FILTER_TYPES)> m_filters; // array with filter objects
  FilterTypes m_currentFilter{};                                                                   // Current filter index
  FilterTypes m_targetFilter{};                                                                    // Target filter index for interpolation
                                                                                                   // Interpolation amount (0 = current filter, 1 = target filter)

public:
  FilterSwitcher();
  void Process(double& input, int filterSelector, FilterParameters fParams);
};
