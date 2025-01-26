#pragma once
#include "BandPassFilters.h"
#include "BandStopFilters.h"
#include "Filters.h"
#include "HighPassFilters.h"

#define FilterArray std::array<std::unique_ptr<Filters>, static_cast<int>(FilterTypes::MAX_FILTER_TYPES)>
#define SpectralFilterArray std::array<std::unique_ptr<Filters>, static_cast<int>(SpectralFilterTypes::MAX_SPECTRAL_FILTER_TYPES)>
static const std::initializer_list<const char*> SpectralFilterTypes = {"DF1_1P", "DF1_2P", "DF1_3P", "DF1_4P", "DF1_6P", "SVF1_2P", "SVF1_4P", "SVF1_6P"};

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

  SpectralFilterArray LP_Array_Spectral{
    std::make_unique<DF1_1P_LP>(),
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
    LP_Array_Spectral[params.m_spectralFilterSelector]->Process(input, params);
    return input;
  }

  static std::initializer_list<const char*> getInitList(int indx)
  {
    static const std::initializer_list<const char*> DF1 = {"DF1_1P", "DF1_2P", "DF1_3P", "DF1_4P", "DF1_6P"};
    static const std::initializer_list<const char*> DF2 = {"DF2_2P", "DF2_4P"};
    static const std::initializer_list<const char*> SVF1 = {"SVF1_2P", "SVF1_4P", "SVF1_6P"};

    switch (indx)
    {
    case (int)FilterAlgo::DF1:
      return DF1;
    case (int)FilterAlgo::DF2:
      return DF2;
    case (int)FilterAlgo::SVF1:
      return SVF1;
    // case (int)FilterAlgo::ZDF1:
    //   return ZDF1;
    default:
      return {}; // Empty initializer list for safety
    }
  }
  static std::initializer_list<const char*> getInitListSpectral(int indx)
  {
    static const std::initializer_list<const char*> DF1 = {"DF1_1P", "DF1_2P", "DF1_3P", "DF1_4P", "DF1_6P"};
    static const std::initializer_list<const char*> SVF1 = {"SVF1_2P", "SVF1_4P", "SVF1_6P"};

    switch (indx)
    {
    case (int)SpectralFilterAlgo::DF1:
      return DF1;
    case (int)SpectralFilterAlgo::SVF1:
      return SVF1;
    // case (int)FilterAlgo::ZDF1:
    //   return ZDF1;
    default:
      return {}; // Empty initializer list for safety
    }
  }
};
