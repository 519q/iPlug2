#pragma once
#include "IPlug_include_in_plug_hdr.h"
#include "OverSampler.h"
#include "Smoothers.h"
#include "projects/FilterSwitcher.h"
#include "projects/Filters.h"
// #include "projects/HighPassFilters.h"
#include "projects/Shapers.h"
#include "projects/SmoothTools.h"
const int kNumPresets = 1;


enum EParams
{
  kGain,
  kFilterCutoff,
  kFilterResonance,
  kFilterBandwidth,
  kFilterBypass,
  kShaperDrive,
  kShaperShape,
  kShaperBias,
  kShaperBypass,
  kFilterSelector,
  kFilterType,
  kFilterVintage,
  kOverSampling,
  kNumParams
};


using namespace iplug;
using namespace igraphics;


class ColorFilterPlugin final : public Plugin
{
private:
  bool mFactorChanged = true;
  OverSampler<sample> mOverSampler{kNone, true, 2, 2};
  int ovrsmpFactor{};

public:
  ColorFilterPlugin(const InstanceInfo& info);
  // Direct Processing
  FilterSwitcher filterSwitcherLP_L{FilterPresets::getLPFilters()};
  FilterSwitcher filterSwitcherLP_R{FilterPresets::getLPFilters()};

  FilterSwitcher filterSwitcherBP_L{FilterPresets::getBPFilters()};
  FilterSwitcher filterSwitcherBP_R{FilterPresets::getBPFilters()};

  FilterSwitcher filterSwitcherBS_L{FilterPresets::getBSFilters()};
  FilterSwitcher filterSwitcherBS_R{FilterPresets::getBSFilters()};

  FilterSwitcher filterSwitcherHP_L{FilterPresets::getHPFilters()};
  FilterSwitcher filterSwitcherHP_R{FilterPresets::getHPFilters()};

  // Vintage Processing
  FilterSwitcher filterSwitcherLP_Vintage_L{FilterPresets::getVintage_LPFilters()};
  FilterSwitcher filterSwitcherLP_Vintage_R{FilterPresets::getVintage_LPFilters()};

  FilterSwitcher filterSwitcherBP_Vintage_L{FilterPresets::getVintage_BPFilters()};
  FilterSwitcher filterSwitcherBP_Vintage_R{FilterPresets::getVintage_BPFilters()};

  FilterSwitcher filterSwitcherBS_Vintage_L{FilterPresets::getVintage_BSFilters()};
  FilterSwitcher filterSwitcherBS_Vintage_R{FilterPresets::getVintage_BSFilters()};

  FilterSwitcher filterSwitcherHP_Vintage_L{FilterPresets::getVintage_HPFilters()};
  FilterSwitcher filterSwitcherHP_Vintage_R{FilterPresets::getVintage_HPFilters()};

  iplug::LogParamSmooth<double> mGainSmooth{10};
  iplug::LogParamSmooth<double> mShaperDriveSmooth{10};
  iplug::LogParamSmooth<double> mShaperShapeSmooth{10};
  iplug::LogParamSmooth<double> mShaperBiasSmooth{10};
  iplug::LogParamSmooth<double> mShaperBypassSmooth{30};

  // iplug::LogParamSmooth<double> mFilterSelectorSmooth{30};

  iplug::LogParamSmooth<double> mFilterCutoffSmooth{10};
  iplug::LogParamSmooth<double> mFilterResonanceSmooth{10};
  iplug::LogParamSmooth<double> mFilterBandwidthSmooth{10};
  iplug::LogParamSmooth<double> mFilterBypassSmooth{30};

#if IPLUG_DSP // http://bit.ly/2S64BDd
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnReset() override;
  void OnParamChange(int paramIdx, EParamSource, int sampleOffset) override;
#endif
};
