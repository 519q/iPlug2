#include "ColorFilterPlugin.h"
#include "IControls.h"
#include "IPlugConstants.h"
#include "IPlug_include_in_plug_src.h"

ColorFilterPlugin::ColorFilterPlugin(const InstanceInfo& info)
  : iplug::Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kGain)->InitDouble("PostGain", 0., 0., 200.0, 0.01, "%");
  GetParam(kFilterCutoff)->InitDouble("FilterCutoff", 0., 0., 100.0, 0.01, "%");
  GetParam(kFilterResonance)->InitDouble("FilterResonance", 0., 0., 100.0, 0.01, "%");
  GetParam(kFilterBypass)->InitBool("FilterBypass", false, "", IParam::kFlagStepped, "", "Off", "On");
  GetParam(kShaperDrive)->InitDouble("ShaperDrive", 0., 0., 100.0, 0.01, "%");
  GetParam(kShaperShape)->InitDouble("ShaperShape", 0., 0., 100.0, 0.01, "%");
  GetParam(kShaperBias)->InitDouble("ShaperBias", 0., 0., 100.0, 0.01, "%");
  GetParam(kShaperBypass)->InitBool("ShaperBypass", false, "", IParam::kFlagStepped, "", "Off", "On");
  GetParam(kFilterBypass)->InitBool("FilterBypass", false, "", IParam::kFlagStepped, "", "Off", "On");
  GetParam(kFilterSelector)->InitInt("FilterSelector", 0, 0, 8, "FilterSelector", IParam::kFlagStepped, "");
  // GetParam(kHighpass)->InitBool("Highpass", 0, "Highpass", IParam::kFlagStepped, "", "", "");
  GetParam(kOverSampling)->InitInt("FilterOversampler", 0, 0, 4, "OverSampler", IParam::kFlagStepped, "");
  // https: // coolors.co/palette/000814-001d3d-003566-ffc300-ffd60a

#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() { return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT)); };

  mLayoutFunc = [&](IGraphics* pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(IColor(255, 0x74, 0x8c, 0xab));
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
    const IRECT b = pGraphics->GetBounds();
    const IRECT FilterPanel = b.GetFromLeft(400.f).GetFromTop(100.f);
    const IRECT ShaperPanel = b.GetFromLeft(400.f).GetFromTop(300.f);
    const IRECT ButtonsPanel = b.GetFromLeft(400.f).GetFromTop(400.f);
    // const IRECT controls = b.GetGridCell(1, 2, 2);
    //  gain
    int columns = 5;
    int rows = 1;
    int padding = 50;
    // gain
    pGraphics->AttachControl(new IVKnobControl(ShaperPanel.GetGridCell(1, 5, rows, columns).GetFromTop(75).GetMidHPadded(padding), kGain));
    // filter
    pGraphics->AttachControl(new IVKnobControl(FilterPanel.GetGridCell(1, 2, rows, columns).GetFromTop(150).GetFromLeft(40).GetMidHPadded(padding), kFilterCutoff));
    pGraphics->AttachControl(new IVKnobControl(FilterPanel.GetGridCell(1, 3, rows, columns).GetFromTop(75).GetMidHPadded(padding), kFilterResonance));
    pGraphics->AttachControl(
      new IVToggleControl(FilterPanel.GetGridCell(1, 5, rows, columns).GetFromTop(75).GetMidHPadded(padding), kFilterBypass, "FilterBypass", DEFAULT_STYLE.WithColor(kFG, COLOR_WHITE), "Off", "On"));
    // shaper
    pGraphics->AttachControl(new IVKnobControl(ShaperPanel.GetGridCell(1, 1, rows, columns).GetFromTop(75).GetMidHPadded(padding), kShaperDrive));
    pGraphics->AttachControl(new IVKnobControl(ShaperPanel.GetGridCell(1, 2, rows, columns).GetFromTop(75).GetMidHPadded(padding), kShaperShape));
    pGraphics->AttachControl(new IVKnobControl(ShaperPanel.GetGridCell(1, 3, rows, columns).GetFromTop(75).GetMidHPadded(padding), kShaperBias));

    pGraphics->AttachControl(new IVRadioButtonControl(ButtonsPanel.GetGridCell(1, 1, rows, columns).GetFromTop(100).GetMidHPadded(padding), kFilterSelector,
                                                      {"DF1_1P", "DF1_1P_sVintage", "DF1_2P", "DF1_3P", "DF1_4P", "DF1_6P", "DF2_2P", "DF2_4P", "SVF1_4P"}, "FilterSelector", DEFAULT_STYLE,
                                                      EVShape::Ellipse, EDirection::Vertical, 5.f));
    // pGraphics->AttachControl(new IVSwitchControl(ButtonsPanel.GetGridCell(1, 4, rows, columns).GetFromTop(75).GetMidHPadded(padding), kHighpass, "Highpass"));
    pGraphics->AttachControl(new IVRadioButtonControl(ButtonsPanel.GetGridCell(1, 3, rows, columns).GetFromTop(75).GetMidHPadded(padding), kOverSampling, {"None", "2x", "4x", "8x", "16x"},
                                                      "FilterOversampling", DEFAULT_STYLE, EVShape::Ellipse, EDirection::Vertical, 5.f));
    // pGraphics->AttachControl(
    //   new IVToggleControl(ShaperPanel.GetGridCell(1, 5, rows, columns).GetFromTop(75).GetMidHPadded(padding), kShaperBypass, "ShaperBypass", DEFAULT_STYLE.WithColor(kFG, COLOR_WHITE), "Off",
    //   "On"));
  };
#endif
}

//*****************************

//*****************************
// double
#if IPLUG_DSP
void ColorFilterPlugin::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  const int nChans = NOutChansConnected();
  const double sampleRate = GetSampleRate();
  mOverSampler.ProcessBlock(inputs, outputs, nFrames, nChans, nChans, [&](sample** inputs, sample** outputs, int nFrames) {
    for (int s = 0; s < nFrames; s++)
    {


      // gain
      const double gain = GetParam(kGain)->Value() / 100.;
      // Filter
      const double filterCutoff = GetParam(kFilterCutoff)->Value() / 100.0;
      const double filterReso = GetParam(kFilterResonance)->Value() / 100.0;
      const double filterBypass = GetParam(kFilterBypass)->Value();
      const int filterSelector = GetParam(kFilterSelector)->Value();
      // shaper
      const double shaperDrive = GetParam(kShaperDrive)->Value() / 100.;
      const double shaperShape = GetParam(kShaperShape)->Value() / 100.;
      const double shaperAsym = GetParam(kShaperBias)->Value() / 100.;
      const double shaperBypassMix = GetParam(kShaperBypass)->Value();
      const int oversamplingFactor = GetParam(kOverSampling)->Value();
      ovrsmpFactor = oversamplingFactor;
      // gain
      const double smoothedGain = mGainSmooth.Process(gain);
      // filter
      const double smoothedFilterCutoff = mFilterCutoffSmooth.Process(filterCutoff);
      const double smoothedFilterResonance = mFilterResonanceSmooth.Process(filterReso);
      const double smoothedFilterBypass = mFilterBypassSmooth.Process(filterBypass);
      // const double smoothedFilterSelector = mFilterSelectorSmooth.Process(filterSelector);
      //  shaper
      const double smoothedShaperDrive = mShaperDriveSmooth.Process(shaperDrive);
      const double smoothedShaperShape = mShaperShapeSmooth.Process(shaperShape);
      const double smoothedShaperBias = mShaperBiasSmooth.Process(shaperAsym);
      const double smoothedShaperBypass = mShaperBypassSmooth.Process(shaperBypassMix);
      FilterParameters fParams{};

      fParams.setFilterParameters(smoothedFilterCutoff, smoothedFilterResonance, smoothedShaperDrive, smoothedShaperShape, smoothedShaperBias, oversamplingFactor, sampleRate);

      //for (int c = 0; c < nChans; c++)
      //{
      //  const double x = inputs[c][s];
      //  double input = x;
      //  std::function<void(double&)> processorLambda = [filterSelector, this, &fParams](double& input) -> void { return filterSwitcher.Process(input, filterSelector, fParams); };
      //  filterSwitcher.Process(input, filterSelector, fParams); // Unified interface for switching filters
      //  outputs[c][s] = input * smoothedGain;
      //}

      for (int c = 0; c < nChans; c++)
      {
        const double L = inputs[0][s];
        const double R = inputs[1][s];
          double inputL = L;
          double inputR = R;
        std::function<void(double&)> processorLambda = [filterSelector, this, &fParams](double& input) -> void { return filterSwitcher.Process(input, filterSelector, fParams); };
         filterSwitcherR.Process(inputL, filterSelector, fParams); // Unified interface for switching filters
         filterSwitcherL.Process(inputR, filterSelector, fParams); // Unified interface for switching filters

         outputs[0][s] = inputL * smoothedGain;
         outputs[1][s] = inputR * smoothedGain;
      }
    }
  });
  if (mFactorChanged)
  {
    if (mOverSampler.RateToFactor(mOverSampler.GetRate()) != ovrsmpFactor)
    {
      mOverSampler.SetOverSampling((iplug::EFactor)ovrsmpFactor);
      mOverSampler.Reset(GetBlockSize());
    }
    mFactorChanged = false;
  }
}

void ColorFilterPlugin::OnReset() { mOverSampler.Reset(GetBlockSize()); }
void ColorFilterPlugin::OnParamChange(int paramIdx, EParamSource, int sampleOffset)
{
  switch (paramIdx)
  {
  case kOverSampling:
    mFactorChanged = true;
    break;
  }
};
#endif
