#pragma once
#include "projects/AllpassFilters.h"
#include "projects/DebugPrint.h"
class Phaser
{
private:
  FirstOrderAllpassFilter firstOrderAllpassMixer{};
  SecondOrderAllpassFilter secondOrderAllpassMixer{};
  LatticeAllpass latticeAllpass{};

public:
  Phaser() {}
  double Process(double input, FilterParameters& params)
  {
    if (params.m_phaserMix)
    {
      double processed = input;
      if (params.m_phaserSelector == 0)
      {
        firstOrderAllpassMixer.Process(processed, params);
      }
      else if (params.m_phaserSelector == 1)
      {
        secondOrderAllpassMixer.Process(processed, params);
      }
      else if (params.m_phaserSelector == 2)
      {
        latticeAllpass.Process(processed, params);
      }
      return interpolateLin(input, processed, params.m_phaserMix);
    }
    return input;
  }
};
