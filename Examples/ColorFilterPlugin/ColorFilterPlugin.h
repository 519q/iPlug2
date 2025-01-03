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

enum class FilterType
{
  LP,
  BP,
  BS,
  HP
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
    static const std::initializer_list<const char*> SVF1 = {"SVF1_2P", "SVF1_4P", "SVF1_6P"};

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
  int m_ovrsmpFactor{};
  OverSampler<sample> mOverSampler{kNone, true, 2, 2};
  int m_filterAlgo{};
  int m_df1retainer{};
  int m_df2retainer{(int)FilterTypes::DF2_2P};
  int m_svf1retainer{(int)FilterTypes::SVF1_2P};
  int columns = 5;
  int rows = 1;
  int padding = 40;
  IRECT m_ButtonsPanel{};
  double retained_kFilterSelector{};

public:
  ColorFilterPlugin(const InstanceInfo& info);
  // Override SerializeParams to save plugin state
  bool SerializeState(IByteChunk& chunk) const override
  {
    // Call the base class implementation to serialize parameters
    if (!SerializeParams(chunk))
      return false;

    // Add any additional state you want to save here
    chunk.Put(&m_df1retainer);
    chunk.Put(&m_df2retainer);
    chunk.Put(&m_svf1retainer);
    chunk.Put(&retained_kFilterSelector);

    return true;
  }
  // Override UnserializeParams to restore plugin state
  int UnserializeState(const IByteChunk& chunk, int startPos) override
  {
    // Call the base class implementation to unserialize parameters
    startPos = UnserializeParams(chunk, startPos);

    // Add any additional state you want to restore here
    startPos = chunk.Get(&m_df1retainer, startPos);
    startPos = chunk.Get(&m_df2retainer, startPos);
    startPos = chunk.Get(&m_svf1retainer, startPos);
    startPos = chunk.Get(&retained_kFilterSelector, startPos);
    if (GetUI())
      SendParameterValueFromDelegate(kFilterSelector, retained_kFilterSelector, 0);


    return UnserializeParams(chunk, startPos);
  }

#if IPLUG_DSP // http://bit.ly/2S64BDd
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
  void DefineSelector(int& selector) const;
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnReset() override;
  void OnParamChange(int paramIdx, EParamSource, int sampleOffset) override;
#endif
};
