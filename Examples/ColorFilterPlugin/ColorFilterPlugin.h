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
#include "projects/SmoothTools.h"
#include "projects/SpectralFilter.h"
#include "projects/SpectralShaper.h"
// #include "projects/aligned_memory.cpp"
#include "projects/CustomGUI.h"
#include "projects/DebugPrint.h"


const int kNumPresets = 1;

enum EParams
{
  kGainIn,
  kGainOut,
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
  kFilterHilbertOrder,
  kFilterOddOrder,
  kFilterBypass,

  kShaperDrive,
  kShaperShape,
  kShaperBias,

  kSpectralShaperDrive,
  kSpectralShaper_IR,
  kSpectralShaperSelector,

  kShaperHilbertOrder,
  kShaperOddOrder,

  kPhaserFreq,
  kPhaserDepth,
  kPhaserSelector,
  kPhaserMix,

  kDelayMix,
  kDelayTime,
  kDelayFeedback,
  kDelayIR,
  kDelayDampFilterCutoff,

  kClipMix,
  kClipSelector,

  kOverSampling,
  kDryWet,
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
  // const int columns = 5;
  // const int columns_Shaper = 5;
  // const int columns_BP = 5;
  // const int rows = 1;
  // const int padding = 25;
  // const int smallKnobPadding = 12;
  // const int buttonsPadding = 35;
  // const double SmallKnobScale = 0.6;
  // const double getFromTopFilter = 60;
  // const double getFromTopShaper = 60;
  // const double m_Plus1_Scale = 0.3;

  double bigKnobSize = 70.08;
  double medKnobSize = 39.79;
  double medSmallerKnobSize = 33.5;
  IRECT b{};
  IControl* mPreGain{};
  IControl* mPostGain{};

  IControl* mCutoff{};
  IControl* mReso{};
  IControl* mF_BW{};
  IControl* mFilterType{};
  IControl* mFilterAlgo{};
  IVRadioButtonControl_SVG* mFilterSelector{};
  IControl* mFilterBypass{};

  IControl* mSpectralFilterOrder{};
  IControl* mSpectralFilterOdd{};
  IControl* mSpectralFilterDrive{};
  IControl* mSpectralFilterIR{};
  IControl* mSpectralFilterHarder{};
  IControl* mSpectralFilterBypass{};


  IControl* mShaperDrive{};
  IControl* mShaperShape{};
  IControl* mShaperBias{};

  IControl* mSpectralShaperDrive{};
  IControl* mSpectralShaperOrder{};
  IControl* mSpectralShaperOdd{};
  IControl* mSpectralShaperSelector{};
  IControl* mSpectralShaperIR{};

  IControl* mDelayTime{};
  IControl* mDelayFeedback{};
  IControl* mDelayFilter{};
  IControl* mDelayIR{};
  IControl* mDelayMix{};

  IControl* mPhaserMix{};
  IControl* mPhaserOrder{};
  IControl* mPhaserFreq{};
  IControl* mPhaserSelector{};

  IControl* mUtilityMix{};
  IControl* mUtilityOversampling{};

  IControl* mClipperFlavour{};
  IControl* mClipperMix{};

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
  bool SerializeState(IByteChunk& chunk) const override;
  int UnserializeState(const IByteChunk& chunk, int startPos) override;
  void OnUIOpen() override;
  void OnUIClose() override;
  static void SaveGlobalSettings();
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

  // RingBuffer mRingBufferL{};
  // RingBuffer mRingBufferR{};
  // FFT_F_I fftL{};
  // FFT_F_I fftR{};

  SpectralShaper mSpectralShaperL{};
  SpectralShaper mSpectralShaperR{};
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
  void SetModData(int knobTag);
  template <typename... Tags>
  void BatchSetModData(Tags... knobTag);
  double getFinalParamValue(int paramId, double clampBottom = 0, double clampTop = 1, bool scaleDown = true);
  double dBtoControlParam(double param);

#endif
};
