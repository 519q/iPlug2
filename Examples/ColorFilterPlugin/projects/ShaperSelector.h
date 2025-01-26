#pragma once
#include "Shapers.h"
#include <array>
#include <memory>

#define ShaperArray std::array<std::unique_ptr<Shapers>, static_cast<int>(ShaperTypes::MAX_SHAPER_TYPES)>

class ShaperSelector
{
private:
  ShaperArray Shaper_Array{
    // clang-format off
    std::make_unique<TanhShaper>(),
    std::make_unique<PolynomialShaper>(),
    std::make_unique<CubicShaper>()
    // clang-format on
  };

public:
  double Process(double input, FilterParameters& params)
  {
    input = Shaper_Array[params.m_spectralShaperSelector]->Process(input, params);
    return input;
  }
};
