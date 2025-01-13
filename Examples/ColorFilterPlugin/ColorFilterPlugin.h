#pragma once
#include "IControls.h"
#include "IPlug_include_in_plug_hdr.h"
#include "OverSampler.h"
#include "Smoothers.h"
#include "projects/SpectralShaper.h"
#include "projects/FilterSwitcher.h"
#include "projects/Filters.h"
#include "projects/Shapers.h"
#include "projects/SmoothTools.h"
#include "projects/aligned_memory.cpp"

const int kNumPresets = 1;


enum EParams
{
  kGain,

  kFilterCutoff,
  kFilterResonance,
  kFilterBandwidth,
  kFilterBypass,
  kFilterAlgo,
  kFilterType,
  kFilterSelector,
  // kFilterVintage,

  kShaperDrive,
  kShaperShape,
  kShaperBias,
  //kShaperBypass,

  kSpectralShaperShape,
  //kSpectralShaperDC,
  //kSpectralShaperPhase,

  kOverSampling,
  kNumParams
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
    //static const std::initializer_list<const char*> ZDF1 = {"ZDF1_2P", "ZDF2_1P", "ZDF2_2P"};

    switch (indx)
    {
    case (int)FilterAlgo::DF1:
      return DF1;
    case (int)FilterAlgo::DF2:
      return DF2;
    case (int)FilterAlgo::SVF1:
      return SVF1;
    //case (int)FilterAlgo::ZDF1:
    //  return ZDF1;
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
  //int m_zdf1retainer{(int)FilterTypes::ZDF1_2P};
  int columns = 5;
  int rows = 1;
  int padding = 40.;
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
    //chunk.Put(&m_zdf1retainer);
    chunk.Put(&m_filterAlgo);
    chunk.Put(&retained_kFilterSelector);

    return SerializeParams(chunk);
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
    //startPos = chunk.Get(&m_zdf1retainer, startPos);
    startPos = chunk.Get(&m_filterAlgo, startPos);
    startPos = chunk.Get(&retained_kFilterSelector, startPos);
    if (GetUI())
    {
      SendParameterValueFromDelegate(kFilterSelector, retained_kFilterSelector, 0);
      SendParameterValueFromDelegate(kFilterSelector, m_filterAlgo, 0);
    }

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

  Sigmoidal sigmoidalShaperL{};
  Sigmoidal sigmoidalShaperR{};

  RingBuffer mRingBufferL{};
  RingBuffer mRingBufferR{};

  SpectralShaper mDomeShaperL{};
  SpectralShaper mDomeShaperR{};
  // std::vector<double> processedL, processedR;
  // std::map<double, double> sineLUT = mDomeShaperL.generateSineLUT();


  // Vintage Processing
  // FilterSwitcher filterSwitcherLP_Vintage_L{FilterPresets::getVintage_LPFilters()};
  // FilterSwitcher filterSwitcherLP_Vintage_R{FilterPresets::getVintage_LPFilters()};
  // FilterSwitcher filterSwitcherBP_Vintage_L{FilterPresets::getVintage_BPFilters()};
  // FilterSwitcher filterSwitcherBP_Vintage_R{FilterPresets::getVintage_BPFilters()};
  // FilterSwitcher filterSwitcherBS_Vintage_L{FilterPresets::getVintage_BSFilters()};
  // FilterSwitcher filterSwitcherBS_Vintage_R{FilterPresets::getVintage_BSFilters()};
  // FilterSwitcher filterSwitcherHP_Vintage_L{FilterPresets::getVintage_HPFilters()};
  // FilterSwitcher filterSwitcherHP_Vintage_R{FilterPresets::getVintage_HPFilters()};
  double knobSmoothing = 10;
  double buttonSmoothing = 30;
  iplug::LogParamSmooth<double> mGainSmooth{knobSmoothing};
  iplug::LogParamSmooth<double> mShaperDriveSmooth{knobSmoothing};
  iplug::LogParamSmooth<double> mShaperShapeSmooth{knobSmoothing};
  iplug::LogParamSmooth<double> mShaperBiasSmooth{knobSmoothing};
  //iplug::LogParamSmooth<double> mShaperBypassSmooth{buttonSmoothing};

  iplug::LogParamSmooth<double> mSpectralShaperShapeSmooth{knobSmoothing};
  // iplug::LogParamSmooth<double> mFilterSelectorSmooth{30};

  iplug::LogParamSmooth<double> mFilterCutoffSmooth{knobSmoothing};
  iplug::LogParamSmooth<double> mFilterResonanceSmooth{knobSmoothing};
  iplug::LogParamSmooth<double> mFilterBandwidthSmooth{knobSmoothing};
  iplug::LogParamSmooth<double> mFilterBypassSmooth{buttonSmoothing};
  void DefineSelector(int& selector) const;
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnReset() override;
  void OnParamChange(int paramIdx, EParamSource, int sampleOffset) override;
#endif
};
