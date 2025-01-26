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

  kOverSampling,
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
  int columns = 5;
  int rows = 1;
  int padding = 25;
  int buttonsPadding = 35;
  double getFromTopFilter = 65;
  double getFromTopShaper = 60;
  IControl* mCutoff_Knob{};
  IControl* mCutoff_Knob_Spectral{};
  IControl* mReso_Knob{};
  IControl* mReso_Knob_Spectral{};
  IControl* mF_BW_Knob{};
  IControl* mFilter_Type_RB{};
  IControl* mFilterBypassSwitch{};
  IControl* mFilterBypassSwitch_Spectral{};
  IControl* mFilterAlgoSwitch{};
  IControl* mFilterSelectorSwitch{};
  IControl* mSpectral_FilterAlgoSwitch{};
  IControl* mSpectral_FilterSelectorSwitch{};
  IControl* mSpectralFilter_IR{};
  IControl* mSpectralFilterOnToggle{};

  bool mFactorChanged = true;
  int m_ovrsmpFactor{};
  OverSampler<sample> mOverSampler{kNone, true, 2, 2};
  int m_filterAlgo{};
  int m_df1retainer{};
  int m_df2retainer{(int)FilterTypes::DF2_2P};
  int m_svf1retainer{(int)FilterTypes::SVF1_2P};

  int m_spectral_FilterAlgo{};
  int m_spectral_df1retainer{};
  int m_spectral_svf1retainer{(int)SpectralFilterTypes::SVF1_2P};

  IRECT m_ButtonsPanel{};
  IRECT m_FilterPanel{};
  IRECT m_ShaperPanel{};

public:
  ColorFilterPlugin(const InstanceInfo& info);
  // Override SerializeParams to save plugin state
  // mDrawScaleRetainer = GetUI()->GetDrawScale();
  // bool SerializeEditorState(IByteChunk& chunk) const override
  //{ return true;
  //}
  // int UnserializeEditorState(const IByteChunk& chunk, int startPos) override { return startPos; }
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
  // iplug::LogParamSmooth<double> mShaperBypassSmooth{buttonSmoothing};

  iplug::LogParamSmooth<double> mSpectralShaperShapeSmooth{knobSmoothing};
  // iplug::LogParamSmooth<double> mFilterSelectorSmooth{30};

  iplug::LogParamSmooth<double> mFilterCutoffSmooth{knobSmoothing};
  iplug::LogParamSmooth<double> mFilterResonanceSmooth{knobSmoothing};
  iplug::LogParamSmooth<double> mFilterBandwidthSmooth{knobSmoothing};
  // iplug::LogParamSmooth<double> mFilterBypassSmooth{buttonSmoothing};
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
