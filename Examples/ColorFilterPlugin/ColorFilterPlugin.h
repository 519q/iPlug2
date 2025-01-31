#pragma once
#include "IControls.h"
#include "IPlug_include_in_plug_hdr.h"
#include "OverSampler.h"
#include "Smoothers.h"
#include "projects/FFT_F_I.h"
#include "projects/SpectralFilter.h"
#include "projects/SpectralShaper.h"
// #include "projects/FilterSwitcher.h"
#include "projects/FilterSelector.h"
#include "projects/Filters.h"
#include "projects/Shapers.h"
#include "projects/SmoothTools.h"
// #include "projects/aligned_memory.cpp"
#include "projects/CustomGUI.h"
#include "projects/DebugLogger.h"
//#define PLUG_HAS_CLAP 1

const int kNumPresets = 1;


enum EParams
{
  kGain,
  kFilterCutoff,
  kFilterResonance,
  kFilterBandwidth,
  kFilterAlgo,
  kFilterType,
  kFilterSelector,
  kFilterSelector_BS,
  kSpectralFilterOn,
  kSpectralFilterAlgo,
  kSpectralFilterSelector,
  kSpectralFilter_IR,
  kFilterBypass,

  kShaperDrive,
  kShaperShape,
  kShaperBias,

  kSpectralShaperShape,
  kSpectralShaper_IR,
  kSpectralShaperSelector,

  kFilterFIR_O,
  kFilterFIR_O_Plus1,
  kShaperFIR_O,
  kShaperFIR_O_Plus1,

  kOverSampling,
  kBypass,
  kNumParams
};

using namespace iplug;
using namespace igraphics;


class ColorFilterPlugin final : public Plugin
{
private:
  //   ██████  ██    ██ ██      ██████  ██████  ███    ██ ████████ ██████   ██████  ██      ███████
  //  ██       ██    ██ ██     ██      ██    ██ ████   ██    ██    ██   ██ ██    ██ ██      ██
  //  ██   ███ ██    ██ ██     ██      ██    ██ ██ ██  ██    ██    ██████  ██    ██ ██      ███████
  //  ██    ██ ██    ██ ██     ██      ██    ██ ██  ██ ██    ██    ██   ██ ██    ██ ██           ██
  //   ██████   ██████  ██      ██████  ██████  ██   ████    ██    ██   ██  ██████  ███████ ███████
  const int columns = 5;
  const int columns_BP = 4;
  const int rows = 1;
  const int padding = 25;
  const int buttonsPadding = 35;
  const double getFromTopFilter = 65;
  const double getFromTopShaper = 60;
  IControl* mCutoff_Knob{};
  IControl* mCutoff_Knob_Spectral{};
  IControl* mReso_Knob{};
  IControl* mReso_Knob_Spectral{};
  IControl* mF_BW_Knob{};
  IControl* mFilter_Type_RB{};
  IControl* mFilterBypassSwitch{};
  IControl* mFilterSelectorSwitch_BS{};
  IControl* mFilterBypassSwitch_Spectral{};
  IControl* mFilterAlgoSwitch{};
  IControl* mFilterSelectorSwitch{};
  IControl* mSpectral_FilterAlgoSwitch{};
  IControl* mSpectral_FilterSelectorSwitch{};
  IControl* mSpectralFilter_IR{};
  IControl* mSpectralFilterOnToggle{};
  IVKnobControl* mFilterFirQ_Odd{};
  IVKnobControl* mFilterFirQ_Even{};
  IControl* mFilterFirQ_Plus1{};
  IVKnobControl* mShaperFirQ{};
  IControl* mShaperFirQ_Plus1{};

  bool mFactorChanged = true;
  int m_ovrsmpFactor{};
  OverSampler<sample> mOverSampler{kNone, true, 2, 2};
  int m_filterAlgo{};
  int m_df1retainer{};
  int m_df2retainer{(int)FilterTypes::DF2_2P};
  int m_svf1retainer{(int)FilterTypes::SVF1_2P};

  int m_df1retainer_BS{};
  int m_df2retainer_BS{};
  int m_svf1retainer_BS{};

  int m_spectral_FilterAlgo{};
  int m_spectral_df1retainer{};
  int m_spectral_svf1retainer{(int)SpectralFilterTypes::SVF1_2P};

  IRECT m_ButtonsPanel{};
  IRECT m_FilterPanel{};
  IRECT m_ShaperPanel{};

public:
  ColorFilterPlugin(const InstanceInfo& info);

  bool SerializeState(IByteChunk& chunk) const override
  {
    // Call the base class implementation to serialize parameters
    if (!SerializeParams(chunk))
      return false;
    // Add any additional state you want to save here
    chunk.Put(&mDrawScaleRetainer);
    chunk.Put(&m_df1retainer);
    chunk.Put(&m_df2retainer);
    chunk.Put(&m_svf1retainer);
    chunk.Put(&m_filterAlgo);
    chunk.Put(&m_spectral_FilterAlgo);
    chunk.Put(&m_spectral_df1retainer);
    chunk.Put(&m_spectral_svf1retainer);
    return SerializeParams(chunk);
  }
  // Override UnserializeParams to restore plugin state
  int UnserializeState(const IByteChunk& chunk, int startPos) override
  {
    // Call the base class implementation to unserialize parameters
    startPos = UnserializeParams(chunk, startPos);

    // Add any additional state you want to restore here
    startPos = chunk.Get(&mDrawScaleRetainer, startPos);
    startPos = chunk.Get(&m_df1retainer, startPos);
    startPos = chunk.Get(&m_df2retainer, startPos);
    startPos = chunk.Get(&m_svf1retainer, startPos);
    startPos = chunk.Get(&m_filterAlgo, startPos);
    startPos = chunk.Get(&m_spectral_FilterAlgo, startPos);
    startPos = chunk.Get(&m_spectral_df1retainer, startPos);
    startPos = chunk.Get(&m_spectral_svf1retainer, startPos);

    return UnserializeParams(chunk, startPos);
  }

  void OnUIOpen() override
  {
    IEditorDelegate::OnUIOpen();
    if (GetUI())
      GetUI()->SetDrawScale(mDrawScaleRetainer);
  }

  void OnUIClose() override { mDrawScaleRetainer = GetUI()->GetDrawScale(); }
#if IPLUG_DSP // http://bit.ly/2S64BDd
  FilterParameters fParams{};

  FilterSelector filterSelectorL{};
  FilterSelector filterSelectorR{};

  SpectralFilter spectralFilterL{};
  SpectralFilter spectralFilterR{};

  Sigmoidal sigmoidalShaperL{};
  Sigmoidal sigmoidalShaperR{};

  RingBuffer mRingBufferL{};
  RingBuffer mRingBufferR{};

  SpectralShaper mSpectralShaperL{};
  SpectralShaper mSpectralShaperR{};

  double knobSmoothing = 10;
  double buttonSmoothing = 30;
  iplug::LogParamSmooth<double> mGainSmooth{knobSmoothing};
  iplug::LogParamSmooth<double> mShaperDriveSmooth{knobSmoothing};
  iplug::LogParamSmooth<double> mShaperShapeSmooth{knobSmoothing};
  iplug::LogParamSmooth<double> mShaperBiasSmooth{knobSmoothing};

  iplug::LogParamSmooth<double> mSpectralShaperShapeSmooth{knobSmoothing};

  iplug::LogParamSmooth<double> mFilterCutoffSmooth{knobSmoothing};
  iplug::LogParamSmooth<double> mFilterResonanceSmooth{knobSmoothing};
  iplug::LogParamSmooth<double> mFilterBandwidthSmooth{knobSmoothing};
  void DefineSelector(int selector) const;
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnReset() override;
  void OnParamChange(int paramIdx, EParamSource, int sampleOffset) override;
  void RemoveControl(int paramIdx);
  template <typename... Args>
  void DecideControlHideStatus(bool hideCondition, Args... controls);
  template <typename... Args>
  void DecideControlDisableStatus(bool disableCondition, Args... controls);
#endif
};
