#pragma once
#include "Shapers.h"
#include <array>
#include <memory>

#define SpectralShaperArray std::array<std::unique_ptr<Shapers>, static_cast<int>(SpectralShaperTypes::MAX_SHAPER_TYPES)>
#define AsymShaperArray std::array<std::unique_ptr<Shapers>, static_cast<int>(AsymShaperTypes::MAX_SHAPER_TYPES)>
#define MainShaperArray std::array<std::unique_ptr<Shapers>, static_cast<int>(MainShaperTypes::MAX_SHAPER_TYPES)>

class ShaperSelector
{
private:
  SpectralShaperArray Spectral_Shaper_Array{
    // clang-format off
    std::make_unique<TanhShaper>(),
    std::make_unique<PolynomialShaper>(),
    std::make_unique<CubicShaper>(),
    std::make_unique<Clipper>()
    // clang-format on
  };
  SpectralShaperArray Main_Shaper_Array{
    // clang-format off
    std::make_unique<SigmoidalShaper>()

    // clang-format on
  };
  AsymShaperArray Asym_Shaper_Array{
    // clang-format off
    std::make_unique<AsymShapeLin>(),
    std::make_unique<AsymShapeExpo>(),
    std::make_unique<AsymShapeQuadratic>(),
    std::make_unique<AsymShapeTanh>(),
    std::make_unique<AsymShapeSigmoid>(),
    // clang-format on
  };

public:
  double ProcessSpectralShaperArray(double input, FilterParameters& params)
  {
    double a{};
    double b{};
    double output{};
    double control = params.m_spectralShaperSelector;
    if (control < 2)
    {
      a = Spectral_Shaper_Array[0]->Process(input, params.m_spectralShaperDrive, 0);
      b = Spectral_Shaper_Array[1]->Process(input, params.m_spectralShaperDrive, 0);
      output = interpolateLin(a, b, control - 1);
    }
    else if (control < 3)
    {
      a = Spectral_Shaper_Array[1]->Process(input, params.m_spectralShaperDrive, 0);
      b = Spectral_Shaper_Array[2]->Process(input, params.m_spectralShaperDrive, 0);
      output = interpolateLin(a, b, control - 2);
    }
    else if (control < 4)
    {
      a = Spectral_Shaper_Array[2]->Process(input, params.m_spectralShaperDrive, 0);
      b = Spectral_Shaper_Array[3]->Process(input, params.m_spectralShaperDrive, 0);
      output = interpolateLin(a, b, control - 3);
    }

    return output;
  }
};
