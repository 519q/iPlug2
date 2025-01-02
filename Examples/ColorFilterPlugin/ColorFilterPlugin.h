#pragma once
#include "IControls.h"
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
  kFilterAlgo,
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

enum class FilterAlgo
{
  DF1,
  DF2,
  SVF1,
  MAX_ALGO
};

using namespace iplug;
using namespace igraphics;


class ColorFilterPlugin final : public Plugin
{
private:
  std::initializer_list<const char*> getInitList(int indx)
  {
    static const std::initializer_list<const char*> DF1 = {"DF1_1P", "DF1_2P", "DF1_3P", "DF1_4P", "DF1_6P"};
    static const std::initializer_list<const char*> DF2 = {"DF2_2P", "DF2_4P"};
    static const std::initializer_list<const char*> SVF1 = {"SVF1_2P", "SVF1_4P"};

    switch (indx)
    {
    case (int)FilterAlgo::DF1:
      return DF1;
    case (int)FilterAlgo::DF2:
      return DF2;
    case (int)FilterAlgo::SVF1:
      return SVF1;
    default:
      return {}; // Empty initializer list for safety
    }
  }
  bool mFactorChanged = true;
  // bool mFilterAlgoChanged = false;
  int ovrsmpFactor{};
  // IRECTList iRectList{};
  OverSampler<sample> mOverSampler{kNone, true, 2, 2};
  // IVRadioButtonControl* mFilterSelectorControl = nullptr;


public:
  ColorFilterPlugin(const InstanceInfo& info);
  IRECT ButtonsPanel{};
  int columns = 5;
  int rows = 1;
  int padding = 40;
  int filterAlgo{};
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
