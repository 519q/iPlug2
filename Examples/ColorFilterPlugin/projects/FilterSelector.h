#pragma once
#include "BandPassFilters.h"
#include "BandStopFilters.h"
#include "Filters.h"
#include "HighPassFilters.h"
#include <array>

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
    std::make_unique<DF2_6P_LP>(),

    std::make_unique<SVF1_1P_LP>(),
    std::make_unique<SVF1_2P_LP>(),
    std::make_unique<SVF1_3P_LP>(),
    std::make_unique<SVF1_4P_LP>(),
    std::make_unique<SVF1_6P_LP>(),

    std::make_unique<ZDF_1P_LP>(),
    std::make_unique<ZDF_2P_LP>(),
    std::make_unique<ZDF_3P_LP>(),
    std::make_unique<ZDF_4P_LP>(),
    std::make_unique<ZDF_6P_LP>()};



  FilterArray HP_Array{
    std::make_unique<DF1_1P_HP>(),
    std::make_unique<DF1_2P_HP>(),
    std::make_unique<DF1_3P_HP>(),
    std::make_unique<DF1_4P_HP>(),
    std::make_unique<DF1_6P_HP>(),
    // 5
    std::make_unique<DF2_2P_HP>(),
    std::make_unique<DF2_4P_HP>(),
    std::make_unique<DF2_6P_HP>(),
    // 3
    std::make_unique<SVF1_1P_HP>(),
    std::make_unique<SVF1_2P_HP>(),
    std::make_unique<SVF1_3P_HP>(),
    std::make_unique<SVF1_4P_HP>(),
    std::make_unique<SVF1_6P_HP>(),
    // 5
    std::make_unique<ZDF_1P_HP>(),
    std::make_unique<ZDF_2P_HP>(),
    std::make_unique<ZDF_3P_HP>(),
    std::make_unique<ZDF_4P_HP>(),
    std::make_unique<ZDF_6P_HP>()};
    // 5
  FilterArray BP_Array{
    std::make_unique<DF1_1P_BP>(),
    std::make_unique<DF1_2P_BP>(),
    std::make_unique<DF1_3P_BP>(),
    std::make_unique<DF1_4P_BP>(),
    std::make_unique<DF1_6P_BP>(),

    std::make_unique<DF2_2P_BP>(),
    std::make_unique<DF2_4P_BP>(),
    std::make_unique<DF2_6P_BP>(),

    std::make_unique<SVF1_1P_BP>(),
    std::make_unique<SVF1_2P_BP>(),
    std::make_unique<SVF1_3P_BP>(),
    std::make_unique<SVF1_4P_BP>(),
    std::make_unique<SVF1_6P_BP>(),

    std::make_unique<ZDF_1P_BP>(),
    std::make_unique<ZDF_2P_BP>(),
    std::make_unique<ZDF_3P_BP>(),
    std::make_unique<ZDF_4P_BP>(),
    std::make_unique<ZDF_6P_BP>()};

  FilterArray BS_Array{
    std::make_unique<DF1_1P_BS>(),
    std::make_unique<DF1_2P_BS>(),
    std::make_unique<DF1_3P_BS>(),
    std::make_unique<DF1_4P_BS>(),
    std::make_unique<DF1_6P_BS>(),

    std::make_unique<DF2_2P_BS>(),
    std::make_unique<DF2_4P_BS>(),
    std::make_unique<DF2_6P_BS>(),

    std::make_unique<SVF1_1P_BS>(),
    std::make_unique<SVF1_2P_BS>(),
    std::make_unique<SVF1_3P_BS>(),
    std::make_unique<SVF1_4P_BS>(),
    std::make_unique<SVF1_6P_BS>(),

    std::make_unique<ZDF_1P_BS>(),
    std::make_unique<ZDF_2P_BS>(),
    std::make_unique<ZDF_3P_BS>(),
    std::make_unique<ZDF_4P_BS>(),
    std::make_unique<ZDF_6P_BS>()};

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

  static std::initializer_list<const char*> getInitList(int indx)
  {
    static const std::initializer_list<const char*> DF1 = {"1P", "2P", "3P", "4P", "6P"};
    static const std::initializer_list<const char*> DF2 = {"2P", "4P", "6P"};
    static const std::initializer_list<const char*> SVF1 = {"1P", "2P", "3P", "4P", "6P"};
    static const std::initializer_list<const char*> ZDF1 = {"1P", "2P", "3P", "4P", "6P"};

    switch (indx)
    {
    case (int)FilterAlgo::DF1:
      return DF1;
    case (int)FilterAlgo::DF2:
      return DF2;
    case (int)FilterAlgo::SVF1:
      return SVF1;
    case (int)FilterAlgo::ZDF1:
      return ZDF1;
    default:
      return {}; // Empty initializer list for safety
    }
  }

};
