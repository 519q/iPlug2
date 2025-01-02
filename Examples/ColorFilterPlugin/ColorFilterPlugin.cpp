#include "ColorFilterPlugin.h"
#include "IPlugConstants.h"
#include "IPlug_include_in_plug_src.h"

ColorFilterPlugin::ColorFilterPlugin(const InstanceInfo& info)
  : iplug::Plugin(info, MakeConfig(kNumParams, kNumPresets))
{

  GetParam(kFilterAlgo)->InitInt("FilterAlgo", 0, 0, 2, "FilterAlgo", IParam::kFlagStepped, "");


  GetParam(kGain)->InitDouble("PostGain", 0., 0., 200.0, 0.01, "%");
  GetParam(kFilterCutoff)->InitDouble("FilterCutoff", 0., 0., 100.0, 0.01, "%");
  GetParam(kFilterResonance)->InitDouble("FilterResonance", 0., 0., 100.0, 0.01, "%");
  GetParam(kFilterBandwidth)->InitDouble("FilterBandwidth", 0., 0., 100.0, 0.01, "%");
  GetParam(kFilterBypass)->InitBool("FilterBypass", false, "", IParam::kFlagStepped, "", "Off", "On");
  GetParam(kShaperDrive)->InitDouble("ShaperDrive", 0., 0., 100.0, 0.01, "%");
  GetParam(kShaperShape)->InitDouble("ShaperShape", 0., 0., 100.0, 0.01, "%");
  GetParam(kShaperBias)->InitDouble("ShaperBias", 0., 0., 100.0, 0.01, "%");
  GetParam(kShaperBypass)->InitBool("ShaperBypass", false, "", IParam::kFlagStepped, "", "Off", "On");
  GetParam(kFilterBypass)->InitBool("FilterBypass", false, "", IParam::kFlagStepped, "", "Off", "On");
  GetParam(kFilterVintage)->InitBool("FilterVintage", false, "", IParam::kFlagStepped, "", "Off", "On");

  GetParam(kFilterSelector)->InitInt("FilterSelector", 0, 0, 8, "", IParam::kFlagStepped, "");


  GetParam(kFilterType)->InitInt("FilterType", 0, 0, 3, "", IParam::kFlagStepped, "");
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
    ButtonsPanel = b.GetFromLeft(400.f).GetFromTop(400.f);
    // iRectList.Add(FilterPanel);
    // iRectList.Add(ShaperPanel);
    // iRectList.Add(ButtonsPanel);
    //  const IRECT controls = b.GetGridCell(1, 2, 2);
    //   gain

    pGraphics->AttachControl(new IVRadioButtonControl(ButtonsPanel.GetGridCell(1, 2, rows, columns).GetFromTop(100).GetMidHPadded(padding), kFilterSelector, getInitList(filterAlgo), "FilterSelector",
                                                      DEFAULT_STYLE, EVShape::Ellipse, EDirection::Vertical, 5.f));

    // gain
    pGraphics->AttachControl(new IVKnobControl(ShaperPanel.GetGridCell(1, 5, rows, columns).GetFromTop(75).GetMidHPadded(padding), kGain));
    // filter
    pGraphics->AttachControl(new IVKnobControl(FilterPanel.GetGridCell(1, 2, rows, columns).GetFromTop(150).GetFromLeft(40).GetMidHPadded(padding), kFilterCutoff));
    pGraphics->AttachControl(new IVKnobControl(FilterPanel.GetGridCell(1, 3, rows, columns).GetFromTop(75).GetMidHPadded(padding), kFilterResonance));
    pGraphics->AttachControl(new IVKnobControl(FilterPanel.GetGridCell(1, 4, rows, columns).GetFromTop(75).GetMidHPadded(padding), kFilterBandwidth));
    pGraphics->AttachControl(
      new IVToggleControl(FilterPanel.GetGridCell(1, 5, rows, columns).GetFromTop(75).GetMidHPadded(padding), kFilterBypass, "FilterBypass", DEFAULT_STYLE.WithColor(kFG, COLOR_WHITE), "Off", "On"));
    // shaper
    pGraphics->AttachControl(new IVKnobControl(ShaperPanel.GetGridCell(1, 1, rows, columns).GetFromTop(75).GetMidHPadded(padding), kShaperDrive));
    pGraphics->AttachControl(new IVKnobControl(ShaperPanel.GetGridCell(1, 2, rows, columns).GetFromTop(75).GetMidHPadded(padding), kShaperShape));
    pGraphics->AttachControl(new IVKnobControl(ShaperPanel.GetGridCell(1, 3, rows, columns).GetFromTop(75).GetMidHPadded(padding), kShaperBias));

    pGraphics->AttachControl(
      new IVToggleControl(ShaperPanel.GetGridCell(1, 4, rows, columns).GetFromTop(75).GetMidHPadded(padding), kFilterVintage, "FilterVintage", DEFAULT_STYLE.WithColor(kFG, COLOR_WHITE), "Off", "On"));

    pGraphics->AttachControl(new IVRadioButtonControl(ButtonsPanel.GetGridCell(1, 1, rows, columns).GetFromTop(100).GetMidHPadded(padding), kFilterAlgo, {"DF1", "DF2", "SVF1"}, "FilterAlgo",
                                                      DEFAULT_STYLE, EVShape::Ellipse, EDirection::Vertical, 5.f));


    // pGraphics->AttachControl(new IVRadioButtonControl(ButtonsPanel.GetGridCell(1, 2, rows, columns).GetFromTop(100).GetMidHPadded(padding), // Use the stored bounds
    //                                                   kFilterSelector, GetFilterSelectorOptions(filterAlgo), "FilterSelector", DEFAULT_STYLE, EVShape::Ellipse, EDirection::Vertical, 5.f));


    pGraphics->AttachControl(new IVRadioButtonControl(
      ButtonsPanel.GetGridCell(1, 5, rows, columns).GetFromTop(75).GetMidHPadded(padding), kFilterType, {"LP", "BP", "BS", "HP"}, "FilterType", DEFAULT_STYLE, EVShape::Ellipse, EDirection::Vertical));
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
      FilterParameters fParams{};


      filterAlgo = GetParam(kFilterAlgo)->Value();
      // gain
      const double gain = GetParam(kGain)->Value() / 100.;
      // Filter
      const double filterCutoff = GetParam(kFilterCutoff)->Value() / 100.0;
      const double filterReso = GetParam(kFilterResonance)->Value() / 100.0;
      const double filterBadwidth = GetParam(kFilterBandwidth)->Value() / 100.0;
      const double filterBypass = GetParam(kFilterBypass)->Value();
      const int filterSelector = GetParam(kFilterSelector)->Value();
      const int filterType = GetParam(kFilterType)->Value();
      const int filterVintage = GetParam(kFilterVintage)->Value();
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
      const double smoothedFilterBandwidth = mFilterBandwidthSmooth.Process(filterBadwidth);
      // const double smoothedFilterSelector = mFilterSelectorSmooth.Process(filterSelector);
      //  shaper
      const double smoothedShaperDrive = mShaperDriveSmooth.Process(shaperDrive);
      const double smoothedShaperShape = mShaperShapeSmooth.Process(shaperShape);
      const double smoothedShaperBias = mShaperBiasSmooth.Process(shaperAsym);
      const double smoothedShaperBypass = mShaperBypassSmooth.Process(shaperBypassMix);
      fParams.setFilterParameters(smoothedFilterCutoff, smoothedFilterResonance, smoothedFilterBandwidth, smoothedShaperDrive, smoothedShaperShape, smoothedShaperBias, oversamplingFactor, sampleRate);

      const double L = inputs[0][s];
      const double R = inputs[1][s];
      double inputL = L;
      double inputR = R;
      if (!filterVintage)
      {
        if (filterType == 0)
        {
          SmoothBypass::processSmoothBypass(
            std::function<void(double&)>([filterSelector, this, &fParams](double& input) -> void { return filterSwitcherLP_L.Process(input, filterSelector, fParams); }), inputL, smoothedFilterBypass);
          SmoothBypass::processSmoothBypass(
            std::function<void(double&)>([filterSelector, this, &fParams](double& input) -> void { return filterSwitcherLP_R.Process(input, filterSelector, fParams); }), inputR, smoothedFilterBypass);
        }

        if (filterType == 1)
        {
          SmoothBypass::processSmoothBypass(
            std::function<void(double&)>([filterSelector, this, &fParams](double& input) -> void { return filterSwitcherBP_L.Process(input, filterSelector, fParams); }), inputL, smoothedFilterBypass);
          SmoothBypass::processSmoothBypass(
            std::function<void(double&)>([filterSelector, this, &fParams](double& input) -> void { return filterSwitcherBP_R.Process(input, filterSelector, fParams); }), inputR, smoothedFilterBypass);
        }

        if (filterType == 2)
        {
          SmoothBypass::processSmoothBypass(
            std::function<void(double&)>([filterSelector, this, &fParams](double& input) -> void { return filterSwitcherBS_L.Process(input, filterSelector, fParams); }), inputL, smoothedFilterBypass);
          SmoothBypass::processSmoothBypass(
            std::function<void(double&)>([filterSelector, this, &fParams](double& input) -> void { return filterSwitcherBS_R.Process(input, filterSelector, fParams); }), inputR, smoothedFilterBypass);
        }

        if (filterType == 3)
        {
          SmoothBypass::processSmoothBypass(
            std::function<void(double&)>([filterSelector, this, &fParams](double& input) -> void { return filterSwitcherHP_L.Process(input, filterSelector, fParams); }), inputL, smoothedFilterBypass);
          SmoothBypass::processSmoothBypass(
            std::function<void(double&)>([filterSelector, this, &fParams](double& input) -> void { return filterSwitcherHP_R.Process(input, filterSelector, fParams); }), inputR, smoothedFilterBypass);
        }
      }

      else
      {
        if (filterType == 0)
        {
          SmoothBypass::processSmoothBypass(
            std::function<void(double&)>([filterSelector, this, &fParams](double& input) -> void { return filterSwitcherLP_Vintage_L.Process(input, filterSelector, fParams); }), inputL,
            smoothedFilterBypass);
          SmoothBypass::processSmoothBypass(
            std::function<void(double&)>([filterSelector, this, &fParams](double& input) -> void { return filterSwitcherLP_Vintage_R.Process(input, filterSelector, fParams); }), inputR,
            smoothedFilterBypass);
        }

        if (filterType == 1)
        {
          SmoothBypass::processSmoothBypass(
            std::function<void(double&)>([filterSelector, this, &fParams](double& input) -> void { return filterSwitcherBP_Vintage_L.Process(input, filterSelector, fParams); }), inputL,
            smoothedFilterBypass);
          SmoothBypass::processSmoothBypass(
            std::function<void(double&)>([filterSelector, this, &fParams](double& input) -> void { return filterSwitcherBP_Vintage_R.Process(input, filterSelector, fParams); }), inputR,
            smoothedFilterBypass);
        }

        if (filterType == 2)
        {
          SmoothBypass::processSmoothBypass(
            std::function<void(double&)>([filterSelector, this, &fParams](double& input) -> void { return filterSwitcherBS_Vintage_L.Process(input, filterSelector, fParams); }), inputL,
            smoothedFilterBypass);
          SmoothBypass::processSmoothBypass(
            std::function<void(double&)>([filterSelector, this, &fParams](double& input) -> void { return filterSwitcherBS_Vintage_R.Process(input, filterSelector, fParams); }), inputR,
            smoothedFilterBypass);
        }

        if (filterType == 3)
        {
          SmoothBypass::processSmoothBypass(
            std::function<void(double&)>([filterSelector, this, &fParams](double& input) -> void { return filterSwitcherHP_Vintage_L.Process(input, filterSelector, fParams); }), inputL,
            smoothedFilterBypass);
          SmoothBypass::processSmoothBypass(
            std::function<void(double&)>([filterSelector, this, &fParams](double& input) -> void { return filterSwitcherHP_Vintage_R.Process(input, filterSelector, fParams); }), inputR,
            smoothedFilterBypass);
        }
      }

      outputs[0][s] = inputL * smoothedGain;
      outputs[1][s] = inputR * smoothedGain;
    }
  });
  if (mFactorChanged)
  {
    mOverSampler.SetOverSampling((iplug::EFactor)ovrsmpFactor);
    mOverSampler.Reset(GetBlockSize());
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
  case kFilterAlgo:
    filterAlgo = GetParam(kFilterAlgo)->Int();

    if (filterAlgo == (int)FilterAlgo::DF1)
    {
      GetParam(kFilterSelector)->InitInt("FilterSelector", 0, 0, 4, "", IParam::kFlagStepped, "");
    }
    else if (filterAlgo == (int)FilterAlgo::DF2)
    {
      GetParam(kFilterSelector)->InitInt("FilterSelector", 5, 5, 6, "", IParam::kFlagStepped, "");
    }
    else
    {
      GetParam(kFilterSelector)->InitInt("FilterSelector", 7, 7, 8, "", IParam::kFlagStepped, "");
    }

    IGraphics* pGraphics = GetUI();


    // remove control
    if (pGraphics)
    {
      int idx = 0;
      while (idx < pGraphics->NControls())
      {
        IControl* pControl = pGraphics->GetControl(idx);
        if (pControl && pControl->GetParamIdx() == kFilterSelector)
        {
          pGraphics->RemoveControl(idx);
          // Don't increment idx since RemoveControl shifts everything down
        }
        else
        {
          idx++;
        }
      }
      pGraphics->SetAllControlsDirty();
      pGraphics->AttachControl(new IVRadioButtonControl(ButtonsPanel.GetGridCell(1, 2, rows, columns).GetFromTop(100).GetMidHPadded(padding), kFilterSelector, getInitList(filterAlgo),
                                                        "FilterSelector", DEFAULT_STYLE, EVShape::Ellipse, EDirection::Vertical, 5.f));

      // Attach new control
    }
    break;
  }
}

#endif
