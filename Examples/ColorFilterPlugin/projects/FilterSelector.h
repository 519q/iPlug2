#pragma once
#include "BandPassFilters.h"
#include "BandStopFilters.h"
#include "Filters.h"
#include "HighPassFilters.h"

#define FilterArray std::array<std::unique_ptr<Filters>, static_cast<int>(FilterTypes::MAX_FILTER_TYPES)>

class FilterSelector
{
private:
  // clang-format off
  FilterArray LP_Array{
    std::make_unique<DF1_1P_LP>(),
    std::make_unique<DF1_2P_LP>(),
    std::make_unique<DF1_3P_LP>(),
    std::make_unique<DF1_4P_LP>(),
    std::make_unique<DF1_6P_LP>(),
    std::make_unique<DF2_2P_LP>(),
    std::make_unique<DF2_4P_LP>(),
    std::make_unique<SVF1_2P_LP>(),
    std::make_unique<SVF1_4P_LP>(),
    std::make_unique<SVF1_6P_LP>()};

  FilterArray HP_Array{
    std::make_unique<DF1_1P_HP>(),
    std::make_unique<DF1_2P_HP>(),
    std::make_unique<DF1_3P_HP>(),
    std::make_unique<DF1_4P_HP>(),
    std::make_unique<DF1_6P_HP>(),
    std::make_unique<DF2_2P_HP>(),
    std::make_unique<DF2_4P_HP>(),
    std::make_unique<SVF1_2P_HP>(),
    std::make_unique<SVF1_4P_HP>(),
    std::make_unique<SVF1_6P_HP>()};

  FilterArray BP_Array{
    std::make_unique<DF1_1P_BP>(),
    std::make_unique<DF1_2P_BP>(),
    std::make_unique<DF1_3P_BP>(),
    std::make_unique<DF1_4P_BP>(),
    std::make_unique<DF1_6P_BP>(),
    std::make_unique<DF2_2P_BP>(),
    std::make_unique<DF2_4P_BP>(),
    std::make_unique<SVF1_2P_BP>(),
    std::make_unique<SVF1_4P_BP>(),
    std::make_unique<SVF1_6P_BP>()};

  FilterArray BS_Array{
    std::make_unique<DF1_1P_BS>(),
    std::make_unique<DF1_2P_BS>(),
    std::make_unique<DF1_3P_BS>(),
    std::make_unique<DF1_4P_BS>(),
    std::make_unique<DF1_6P_BS>(),
    std::make_unique<DF2_2P_BS>(),
    std::make_unique<DF2_4P_BS>(),
    std::make_unique<SVF1_2P_BS>(),
    std::make_unique<SVF1_4P_BS>(),
    std::make_unique<SVF1_6P_BS>()};

  FilterArray LP_ArraySpectral{
    std::make_unique<DF1_2P_LP>(),
    std::make_unique<DF1_3P_LP>(),
    std::make_unique<DF1_4P_LP>(),
    std::make_unique<DF1_6P_LP>(),
    std::make_unique<SVF1_2P_LP>(),
    std::make_unique<SVF1_4P_LP>(),
    std::make_unique<SVF1_6P_LP>()};
  // clang-format on

public:
  double Process(double input, FilterParameters& params)
  {
    if (params.m_filterType == (int)FilterType::LP)
    {
      LP_Array[params.m_filterSelector]->Process(input, params);
    }

    if (params.m_filterType == (int)FilterType::BP)
    {
      BP_Array[params.m_filterSelector]->Process(input, params);
    }

    if (params.m_filterType == (int)FilterType::BS)
    {
      BS_Array[params.m_filterSelector]->Process(input, params);
    }

    if (params.m_filterType == (int)FilterType::HP)
    {
      HP_Array[params.m_filterSelector]->Process(input, params);
    }
    return input;
  }

  double ProcessSpectral(double input, FilterParameters& params)
  {
    LP_ArraySpectral[2]->Process(input, params);
    return input;
  }
};
