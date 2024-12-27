#pragma once
#include "Filters.h"
#include "HighPassFilters.h"
#include "BandPassFilters.h"
//#include "SmoothTools.h"


class Filters;

class FilterSwitcher
{
private:
                                                                                                   // Interpolation amount (0 = current filter, 1 = target filter)

public:
  std::array<std::unique_ptr<Filters>, static_cast<int>(FilterTypes::MAX_FILTER_TYPES)> m_filters; // array with filter objects
  FilterTypes m_currentFilter{};                                                                   // Current filter index
  FilterTypes m_targetFilter{};                                                                    // Target filter index for interpolation
  //FilterSwitcherLP();
  template <typename... Filters>
  FilterSwitcher(Filters&&... filters)
    : m_filters{std::make_unique<Filters>(std::forward<Filters>(filters))...}
  {
  }
  void Process(double& input, int filterSelector, FilterParameters fParams);
};

namespace FilterPresets
{
inline auto getHPFilters() { return FilterSwitcher{DF1_1P_HP{}, DF1_2P_HP{}, DF1_3P_HP{}, DF1_4P_HP{}, DF1_6P_HP{}, DF2_2P_HP{}, DF2_4P_HP{}, SVF1_2P_HP{}, SVF1_4P_HP{}}; }
inline auto getBPFilters() { return FilterSwitcher{DF1_1P_BP{}, DF1_2P_BP{}, DF1_3P_BP{}, DF1_4P_BP{}, DF1_6P_BP{}, DF2_2P_BP{}, DF2_4P_BP{}, SVF1_2P_BP{}, SVF1_4P_BP{}}; }
inline auto getLPFilters() { return FilterSwitcher{DF1_1P_LP{}, DF1_2P_LP{}, DF1_3P_LP{}, DF1_4P_LP{}, DF1_6P_LP{}, DF2_2P_LP{}, DF2_4P_LP{}, SVF1_2P_LP{}, SVF1_4P_LP{}}; }
} // namespace FilterPresets
