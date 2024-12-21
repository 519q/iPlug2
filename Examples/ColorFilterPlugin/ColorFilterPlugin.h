#pragma once
#include "IPlug_include_in_plug_hdr.h"
#include "OverSampler.h"
#include "Smoothers.h"
#include "projects/FilterSwitcher.h"
#include "projects/Filters.h"
#include "projects/Shapers.h"
#include "projects/SmoothTools.h"
const int kNumPresets = 1;


enum EParams
{
  kGain,
  kFilterCutoff,
  kFilterResonance,
  kFilterBypass,
  kShaperDrive,
  kShaperShape,
  kShaperBias,
  kShaperBypass,
  kFilterSelector,
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

public:
  ColorFilterPlugin(const InstanceInfo& info);
  // FilterParameters fParams{};
  FilterSwitcher filterSwitcher;
  iplug::LogParamSmooth<double> mGainSmooth{10};
  int ovrsmpFactor{};

  iplug::LogParamSmooth<double> mShaperDriveSmooth{10};
  iplug::LogParamSmooth<double> mShaperShapeSmooth{10};
  iplug::LogParamSmooth<double> mShaperBiasSmooth{10};
  iplug::LogParamSmooth<double> mShaperBypassSmooth{30};

  iplug::LogParamSmooth<double> mFilterSelectorSmooth{30};

  iplug::LogParamSmooth<double> mFilterCutoffSmooth{10};
  iplug::LogParamSmooth<double> mFilterResonanceSmooth{10};
  iplug::LogParamSmooth<double> mFilterBypassSmooth{30};

#if IPLUG_DSP // http://bit.ly/2S64BDd
  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnReset() override;
  void OnParamChange(int paramIdx, EParamSource, int sampleOffset) override;
#endif
};
