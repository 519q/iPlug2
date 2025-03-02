#pragma once

#include "IControls.h"
#include "IPlugPaths.h"
#include "IPlug_include_in_plug_hdr.h"
#include "OverSampler.h"
#include "Smoothers.h"
#include "projects/DelayLine.h"
#include "projects/FFT_F_I.h"
#include "projects/FilterSelector.h"
#include "projects/Filters.h"
#include "projects/Phaser.h"
#include "projects/Shapers.h"
#include "projects/MiscTools.h"
#include "projects/SpectralFilter.h"
#include "projects/SpectralShaper.h"
// #include "projects/aligned_memory.cpp"
#include "projects/CustomGUI.h"
#include "projects/DebugPrint.h"
//#include <vld.h>
//class LeakReporter
//{
//public:
//  ~LeakReporter() { VLDReportLeaks(); }
//};
const int kNumPresets = 1;

enum EParams
{
  kGainIn,
  kGainOut,
  kInOutBypass,
  kFilterCutoff,
  kFilterResonance,
  kFilterBandwidth,
  kFilterAlgo,
  kFilterType,
  kFilterSelector,
  kSpectralFilterOn,
  kSpectralFilter_IR,
  kSpectralFilter_Drive,
  kSpectralFilter_Harder,
  kSpectralFilterOrder,
  kSpectralFilterOdd,
  kFilterBypass,

  kShaperDrive,
  kShaperShape,
  kShaperBias,
  kShaperBypass,

  kSpectralShaperDrive,
  kSpectralShaper_IR,
  kSpectralShaperSelector,
  kSpectralShaperOrder,
  kSpectralShaperOdd,
  kSpectralShaperBypass,

  kPhaserFreq,
  kPhaserDepth,
  kPhaserSelector,
  kPhaserMix,
  kPhaserBypass,

  kDelayMix,
  kDelayTime,
  kDelayFeedback,
  kDelayIR,
  kDelayDampFilterCutoff,
  kDelayBypass,

  kClipMix,
  kClipSelector,
  kClipBypass,

  kOverSampling,
  kDryWet,
  kBypass,
  kNumParams
};

enum NonControlParams
{
  kShaperVisualizer = 100,
  kSpectralShaperVisualizer,
  kClipVisualizer,
  kShowValues
};

using namespace iplug;
using namespace igraphics;

class ColorFilterPlugin final : public Plugin
{
private:

  double bigKnobSize = 70.08;
  double medKnobSize = 39.79;
  double medSmallerKnobSize = 33.5;
  IRECT b{};
  BitmapKnobWithValueBar* mPreGain{};
  BitmapKnobWithValueBar* mPostGain{};
  IControl* mInOutBypass{};

  BitmapKnobWithValueBar* mCutoff{};
  BitmapKnobWithValueBar* mReso{};
  BitmapKnobWithValueBar* mF_BW{};
  IControl* mFilterType{};
  IControl* mFilterAlgo{};
  IVRadioButtonControl_Bitmap* mFilterSelector{};
  IControl* mFilterBypass{};

  BitmapKnobWithValueBar* mSpectralFilterOrder{};
  IControl* mSpectralFilterOdd{};
  BitmapKnobWithValueBar* mSpectralFilterDrive{};
  IControl* mSpectralFilterIR{};
  IControl* mSpectralFilterHarder{};
  IControl* mSpectralFilterBypass{};


  BitmapKnobWithValueBar* mShaperDrive{};
  BitmapKnobWithValueBar* mShaperShape{};
  BitmapKnobWithValueBar* mShaperBias{};
  IControl* mShaperBypass{};

  BitmapKnobWithValueBar* mSpectralShaperDrive{};
  BitmapKnobWithValueBar* mSpectralShaperOrder{};
  BitmapKnobWithValueBar* mSpectralShaperSelector{};
  IControl* mSpectralShaperOdd{};
  IControl* mSpectralShaperIR{};
  IControl* mSpectralShaperBypass{};

  BitmapKnobWithValueBar* mDelayMix{};
  BitmapKnobWithValueBar* mDelayTime{};
  BitmapKnobWithValueBar* mDelayFeedback{};
  BitmapKnobWithValueBar* mDelayFilter{};
  IControl* mDelayIR{};
  IControl* mDelayBypass{};

  BitmapKnobWithValueBar* mPhaserMix{};
  BitmapKnobWithValueBar* mPhaserOrder{};
  BitmapKnobWithValueBar* mPhaserFreq{};
  IControl* mPhaserSelector{};
  IControl* mPhaserBypass{};

  BitmapKnobWithValueBar* mUtilityMix{};
  IControl* mUtilityOversampling{};

  BitmapKnobWithValueBar* mClipperFlavour{};
  BitmapKnobWithValueBar* mClipperMix{};
  IControl* mClipperBypass{};

  IControl* mClipVisualiser{};
  bool g_Bypass{};
  bool m_spectralFilterOn{};
  int spectralFilter_IR{};
  bool spectralFilter_Harder{};
  bool m_filterBypass{};
  bool spectralShaperOn{};
  int m_filterSelector{};
  int m_filterType{};
  int spectralShaper_IR{};
  double spectralShaperSelector{};
  double filterHilbertOrder{};
  double shaperHilbertOrder{};
  int m_phaserSelector{};
  int m_ovrsmpFactor{};
  bool mControlValuesViewState{};

  bool mAlgoChanged{};
  bool needsDecideOnReset{};
  bool mSelectorIsDirty{true};
  bool mOversamplingFactorChanged{};
  OverSampler<sample> mOverSampler{kNone, true, 2, 2};
  int m_filterAlgo{};
  int m_df1retainer{};
  int m_df2retainer{1};
  int m_svf1retainer{};
  int m_zdf1retainer{};

  IRECT m_ButtonsPanel{};
  IRECT m_FilterPanel{};
  IRECT m_ShaperPanel{};

public:
  ColorFilterPlugin(const InstanceInfo& info);
  ~ColorFilterPlugin()
  {
  }

  bool SerializeState(IByteChunk& chunk) const override;
  int UnserializeState(const IByteChunk& chunk, int startPos) override;
  void OnUIOpen() override;
  void OnUIClose() override;
  void SaveGlobalSettings();
  void LoadGlobalSettings();
  // void legacyInitializer(IRECT b);
#if IPLUG_DSP // http://bit.ly/2S64BDd
  ISender<1, 64, float> mModValueSender{};
  ISenderData<1, float> mModData{};

  FilterParameters fParams{};
  FilterSelector filterSelectorL{};
  FilterSelector filterSelectorR{};

  SpectralFilter spectralFilterL{};
  SpectralFilter spectralFilterR{};

  SigmoidalShaper sigmoidalShaperL{};
  SigmoidalShaper sigmoidalShaperR{};
  AsymShapeSigmoid sigmoidR{};
  AsymShapeSigmoid sigmoidL{};

  DelayLine delayLineL{};
  DelayLine delayLineR{};

  Phaser phaserL{};
  Phaser phaserR{};

  ClipperMixer clipL{};
  ClipperMixer clipR{};
  // RingBuffer mRingBufferL{};
  // RingBuffer mRingBufferR{};
  // FFT_F_I fftL{};
  // FFT_F_I fftR{};

  SpectralShaper mSpectralShaperL{};
  SpectralShaper mSpectralShaperR{};
  RMS_PEAK_CALCULATOR ClipPeakCalc{1500};
  RMS_PEAK_CALCULATOR ShaperPeakCalc{1500};
  RMS_PEAK_CALCULATOR SpectralShaperPeakCalc{1500};
  int m_OS_LatencySamples{};
  // int m_RB_LatencySamples{};

  double paramSmoothing = 5;
  double buttonSmoothing = 30;
  std::array<iplug::LogParamSmooth<double>, kNumParams> mSmoothers;

  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnReset() override;
  void OnIdle() override;
  void OnParamChange(int paramIdx, EParamSource, int sampleOffset) override;
  void DefineSelector();
  void RemoveControl(int paramIdx);
  template <typename... Args>
  void DecideControlHideStatus(bool hideCondition, Args... controls);
  template <typename... Args>
  void DecideControlDisableStatus(bool disableCondition, Args... controls);
  void CalculateLatency();
  void DecideOnReset();
  void SetVisualizationData(int knobTag, double value);
  void SetModData(int knobTag);
  template <typename... Tags>
  void BatchSetModData(Tags... knobTag);
  template <typename... Tags>
  void BatchSetShowValues(bool value, Tags... knobTag);
  double getFinalParamValue(int paramId, double clampBottom = 0, double clampTop = 1, bool scaleDown = true);
  double dBtoControlParam(double param);
  void setAllShowValues();
  
#endif
};
