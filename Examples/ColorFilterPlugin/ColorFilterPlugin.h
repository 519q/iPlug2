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
  kDryWet,
  kGain,
  kFilterCutoff,
  kFilterResonance,
  kFilterBandwidth,
  kFilterAlgo,
  kFilterType,
  kFilterSelector,
  kFilterSelector_BS, // bandstop wont work in 1p setup
  kSpectralFilterOn,
  // kSpectralFilterAlgo,
  // kSpectralFilterSelector,
  kSpectralFilter_IR,
  kSpectralFilter_Drive,
  kSpectralFilter_Harder,
  kFilterBypass,

  kShaperDrive,
  kShaperShape,
  kShaperBias,

  kSpectralShaperDrive,
  kSpectralShaper_IR,
  kSpectralShaperSelector,

  kFilterFIR_Q,
  kFilterFIR_Q_Plus1,
  kShaperFIR_Q,
  kShaperFIR_Q_Plus1,

  kFilterIIR_Q,
  kShaperIIR_Q,

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
  const int columns_Shaper = 5;
  const int columns_BP = 5;
  const int rows = 1;
  const int padding = 25;
  const int buttonsPadding = 35;
  const double getFromTopFilter = 60;
  const double getFromTopShaper = 60;
  double m_Plus1_Scale = 0.3;


  IControl* mPostGain{};
  IVKnobControl* mCutoff_Knob{};
  IControl* mReso_Knob{};
  IControl* mF_BW_Knob{};
  IControl* mFilter_Type_RadioButton{};
  IControl* mFilterBypassSwitch{};
  IControl* mFilterSelectorSwitch_BS{};
  IControl* mFilterAlgoSwitch{};
  IControl* mFilterSelectorSwitch{};
  IVKnobControl* mSpectralFilterDrive{};
  IControl* mSpectralFilter_IR{};
  IControl* mSpectralFilter_Harder{};
  IControl* mSpectralFilterOnToggle{};
  IControl* mShaperShape{};
  IControl* mShaperBias{};
  //IVKnobControl* mFilterFirQ_Odd{};
  IVKnobControl* mFilterFirQ{};
  IControl* mFilterFirQ_Plus1{};
  IVKnobControl* mFilterIirQ{};
  //IVKnobControl* mShaperFirQ_Odd{};
  IVKnobControl* mShaperFirQ{};
  IControl* mShaperFirQ_Plus1{};
  IVKnobControl* mShaperIirQ{};
  IControl* mSpectralShaper_IR{};
  IVKnobControl* mSpectralShaperSelector{};

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

    return UnserializeParams(chunk, startPos);
  }

  void OnUIOpen() override
  {
    IEditorDelegate::OnUIOpen();
    if (GetUI())
      GetUI()->SetDrawScale(mDrawScaleRetainer);
  }

  void OnUIClose() override { mDrawScaleRetainer = GetUI()->GetDrawScale(); }
  void SetModData(int knobTag)
  {
    mModData.ctrlTag = knobTag; // The control tag that should receive this data
    mModData.nChans = 1;        // We only have 1 value in .vals
    mModData.chanOffset = 0;
    float value = GetModulatedParamOffset(knobTag); // Usually 0 unless dealing with multi-channel offsets
    mModData.vals[0] = value;
    // Enqueue this data for the knob
    mModValueSender.PushData(mModData);
  }
  template <typename... Tags>
  void BatchSetModData(Tags... knobTag)
  {
    (SetModData(knobTag), ...);
  }

  void OnIdle() override
  {
    if (GetUI()) // Ensure UI exists before sending data
    {
      mModValueSender.TransmitData(*this); // Send modulation data to UI controls
    }
  }
  double getFinalParamValue(int paramId)
  {
    double baseValue = GetParam(paramId)->Value() / 100.0;
    double modValue = GetModulatedParamOffset(paramId);
    double result = std::clamp(mSmoothers[paramId].Process(baseValue + modValue), 0., 1.);
    return result;
  }
  int Map01To2_200_Stepped(double normalized, bool odd)
  {
    // Map [0..1] to [0..99] for the step index
    double dblIndex = normalized * 99.0;    // 0..99
    int stepIndex = (int)(dblIndex + 0.5); // round to nearest integer
    int base = 2;
    if (odd)
      base -= 1;
    return base + stepIndex * 2.0; // 2, 4, 6, ...
  }
  int Map01To1_40_Stepped(double normalized)
  {
    // Map [0..1] to [0..99] for the step index
    double dblIndex = normalized * 39.0;    // 0..99
    int stepIndex = (int)(dblIndex + 0.5); // round to nearest integer
    int base = 1;
    return base + stepIndex; // 2, 4, 6, ...
  }
  double mapGain(double gain)
  {
    if (gain <= 0.5)
    {
      gain *= 2; // log scaling below 0db
      const double a = 3; // Controls the curve
      gain = std::log1p(a * gain) / std::log1p(a);
    }
    else
    {
      gain *= 2;
      double aboveOne = gain - 1.; // in [0..1]
      // Now map that 0..1 to +0..+12 dB
      // i.e. at x=1 => 0 dB => multiplier=1
      // at x=2 => +12 dB => multiplier=10^(12/20)=~3.98
      double dB = 12. * aboveOne;       // 0..12
      gain = std::pow(10.f, dB / 20.f); // convert dB to linear
    }
    return gain;
  }
#if IPLUG_DSP // http://bit.ly/2S64BDd
  ISender<1, 64, float> mModValueSender{};
  ISenderData<1, float> mModData{};

  FilterParameters fParams{};
  FilterSelector filterSelectorL{};
  FilterSelector filterSelectorR{};

  SpectralFilter spectralFilterL{};
  SpectralFilter spectralFilterR{};

  Sigmoidal sigmoidalShaperL{};
  Sigmoidal sigmoidalShaperR{};

  // RingBuffer mRingBufferL{};
  // RingBuffer mRingBufferR{};

  // FFT_F_I fftL{};
  // FFT_F_I fftR{};

  SpectralShaper mSpectralShaperL{};
  SpectralShaper mSpectralShaperR{};
  int m_OS_LatencySamples{};
  // int m_RB_LatencySamples{};

  double paramSmoothing = 10;
  double buttonSmoothing = 30;
  std::array<iplug::LogParamSmooth<double>, kNumParams> mSmoothers;

  void DefineSelector(int selector) const;
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnReset() override;
  void OnParamChange(int paramIdx, EParamSource, int sampleOffset) override;
  void RemoveControl(int paramIdx);
  template <typename... Args>
  void DecideControlHideStatus(bool hideCondition, Args... controls);
  template <typename... Args>
  void DecideControlDisableStatus(bool disableCondition, Args... controls);
  void CalculateLatency();
  void DecideOnReset();
#endif
};
