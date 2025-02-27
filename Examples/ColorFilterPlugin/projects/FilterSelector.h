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

  int getFilterIndex(FilterAlgo algo, FilterPoles selector)
  {
    switch (algo)
    {
      case FilterAlgo::DF1:
      {
        switch (selector)
        {
          case FilterPoles::p1:
            return 0;
            case FilterPoles::p2:
            return 1;
            case FilterPoles::p3:
            return 2;
            case FilterPoles::p4:
            return 3;
            case FilterPoles::p6:
            return 4;
        }
      }
      case FilterAlgo::DF2:
      {
        switch (selector)
        {
            case FilterPoles::p2:
            return 5;
            case FilterPoles::p4:
            return 6;
            case FilterPoles::p6:
            return 7;
        }
      }
      case FilterAlgo::SVF1:
      {
        switch (selector)
        {
          case FilterPoles::p1:
            return 8;
            case FilterPoles::p2:
            return 9;
            case FilterPoles::p3:
            return 10;
            case FilterPoles::p4:
            return 11;
            case FilterPoles::p6:
            return 12;
        }
      }
      case FilterAlgo::ZDF1:
      {
        switch (selector)
        {
          case FilterPoles::p1:
            return 13;
            case FilterPoles::p2:
            return 14;
            case FilterPoles::p3:
            return 15;
            case FilterPoles::p4:
            return 16;
            case FilterPoles::p6:
            return 17;
        }
      }
      default: return 0;
    }
    //if (algo == FilterAlgo::DF1)
    //{
    //  if (selector == FilterPoles::p1)
    //  {
    //    return 0;
    //  }
    //  else if (selector == FilterPoles::p2)
    //  {
    //    return 1;
    //  }
    //  else if (selector == FilterPoles::p3)
    //  {
    //    return 2;
    //  }
    //  else if (selector == FilterPoles::p4)
    //  {
    //    return 3;
    //  }
    //  else if (selector == FilterPoles::p6)
    //  {
    //    return 4;
    //  }
    //}
    //else if (algo == FilterAlgo::DF2)
    //{
    //  if (selector == FilterPoles::p2)
    //  {
    //    return 5;
    //  }
    //  else if (selector == FilterPoles::p4)
    //  {
    //    return 6;
    //  }
    //  else if (selector == FilterPoles::p6)
    //  {
    //    return 7;
    //  }
    //  else return 0;
    //}
    //else if (algo == FilterAlgo::SVF1)
    //{
    //  if (selector == FilterPoles::p1)
    //  {
    //    return 8;
    //  }
    //  else if (selector == FilterPoles::p2)
    //  {
    //    return 9;
    //  }
    //  else if (selector == FilterPoles::p3)
    //  {
    //    return 10;
    //  }
    //  else if (selector == FilterPoles::p4)
    //  {
    //    return 11;
    //  }
    //  else if (selector == FilterPoles::p6)
    //  {
    //    return 12;
    //  }
    //}
    //else if (algo == FilterAlgo::ZDF1)
    //{
    //  if (selector == FilterPoles::p1)
    //  {
    //    return 13;
    //  }
    //  else if (selector == FilterPoles::p2)
    //  {
    //    return 14;
    //  }
    //  else if (selector == FilterPoles::p3)
    //  {
    //    return 15;
    //  }
    //  else if (selector == FilterPoles::p4)
    //  {
    //    return 16;
    //  }
    //  else if (selector == FilterPoles::p6)
    //  {
    //    return 17;
    //  }
    //}
    //else return 0;
  }
public:
  double Process(double input, FilterParameters& params)
  {
    if (params.m_filterType == (int)FilterType::LP)
    {
      LP_Array[getFilterIndex((FilterAlgo)params.m_filterAlgo, (FilterPoles)params.m_filterSelector)]->Process(input, params);
    }

    if (params.m_filterType == (int)FilterType::BP)
    {
      BP_Array[getFilterIndex((FilterAlgo)params.m_filterAlgo, (FilterPoles)params.m_filterSelector)]->Process(input, params);
    }

    if (params.m_filterType == (int)FilterType::BS)
    {
      BS_Array[getFilterIndex((FilterAlgo)params.m_filterAlgo, (FilterPoles)params.m_filterSelector)]->Process(input, params);
    }

    if (params.m_filterType == (int)FilterType::HP)
    {
      HP_Array[getFilterIndex((FilterAlgo)params.m_filterAlgo, (FilterPoles)params.m_filterSelector)]->Process(input, params);
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
