#include "ColorFilterPlugin.h"
#include "IPlugConstants.h"
#include "IPlug_include_in_plug_src.h"
class RoundedPanelControl : public IControl
{
public:
  RoundedPanelControl(const IRECT& bounds, const IColor& color, float cornerRadius = 5)
    : IControl(bounds)
    , mColor(color)
    , mCornerRadius(cornerRadius)
  {
  }

  void Draw(IGraphics& g) override
  {
    // Draw a rounded rectangle with the specified color and corner radius
    g.FillRoundRect(mColor, mRECT, mCornerRadius);
    // g.DrawRoundRect(IColor(255, 0xa5, 0x9b, 0xbf), mRECT, mCornerRadius, mCornerRadius, mCornerRadius, mCornerRadius); // border
  }

private:
  IColor mColor;
  float mCornerRadius;
};
ColorFilterPlugin::ColorFilterPlugin(const InstanceInfo& info)
  : iplug::Plugin(info, MakeConfig(kNumParams, kNumPresets))
{

  GetParam(kFilterAlgo)->InitInt("FilterAlgo", 0, 0, 3, "FilterAlgo", IParam::kFlagStepped, "");


  GetParam(kGain)->InitDouble("PostGain", 0., 0., 200.0, 0.01, "%");
  GetParam(kFilterCutoff)->InitDouble("Cutoff", 0., 0., 100.0, 0.01, "%");
  GetParam(kFilterResonance)->InitDouble("Resonance", 0., 0., 100.0, 0.01, "%");
  GetParam(kFilterBandwidth)->InitDouble("Bandwidth", 0., 0., 100.0, 0.01, "%");
  GetParam(kFilterBypass)->InitBool("FilterBypass", false, "", IParam::kFlagStepped, "", "Off", "On");
  GetParam(kShaperDrive)->InitDouble("ShaperDrive", 0., 0., 100.0, 0.01, "%");
  GetParam(kShaperShape)->InitDouble("ShaperShape", 0., 0., 100.0, 0.01, "%");
  GetParam(kShaperBias)->InitDouble("ShaperBias", 0., 0., 100.0, 0.01, "%");
  GetParam(kShaperBypass)->InitBool("ShaperBypass", false, "", IParam::kFlagStepped, "", "Off", "On");
  // GetParam(kFilterVintage)->InitBool("FilterVintage", false, "", IParam::kFlagStepped, "", "Off", "On");

  GetParam(kFilterSelector)->InitInt("FilterSelector", 0, 0, 12, "", IParam::kFlagStepped, "");


  GetParam(kFilterType)->InitInt("FilterType", 0, 0, 3, "", IParam::kFlagStepped, "");
  GetParam(kOverSampling)->InitInt("Oversampler", 0, 0, 4, "OverSampler", IParam::kFlagStepped, "");
  // https: // coolors.co/palette/000814-001d3d-003566-ffc300-ffd60a


#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() { return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT)); };

  mLayoutFunc = [&](IGraphics* pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(IColor(255, 0x8b, 0x86, 0xc1));
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
    const IRECT b = pGraphics->GetBounds();
    const IRECT FilterPanel = b.GetFromLeft(400.f).GetFromTop(90.f);
    const IRECT ShaperPanel = b.GetFromLeft(400.f).GetFromTop(200.f).GetTranslated(20, 0);
    m_ButtonsPanel = b.GetFromLeft(400.f).GetFromTop(300.f);
    // pGraphics->FillRect(IColor(255, 255, 0, 0), FilterPanel);
    // pGraphics->FillRect(IColor(255, 0x40, 0x6d, 0x80), m_ButtonsPanel);
    double topSpaceSize = 70;
    double filterPanelSize = 120;
    double shaperPanelSize = 100;
    double buttonsPanelSize = 140;
    pGraphics->AttachControl(new RoundedPanelControl(FilterPanel.GetFromTop(filterPanelSize).GetFromLeft(450.f).GetTranslated(75, topSpaceSize), IColor(255, 0x9A, 0x8F, 0xB7), 5));
    pGraphics->AttachControl(new RoundedPanelControl(FilterPanel.GetFromTop(shaperPanelSize).GetFromLeft(450.f).GetTranslated(75, topSpaceSize + filterPanelSize), IColor(255, 0x8f, 0x83, 0xaf), 5));
    pGraphics->AttachControl(
      new RoundedPanelControl(FilterPanel.GetFromTop(buttonsPanelSize).GetFromLeft(450.f).GetTranslated(75, topSpaceSize + filterPanelSize + shaperPanelSize), IColor(255, 0x83, 0x76, 0xa7), 5));
    // pGraphics->AttachControl(new IPanelControl(ShaperPanel, IColor(255, 0x80, 0x40, 0x6d)));
    //  iRectList.Add(FilterPanel);
    //  iRectList.Add(ShaperPanel);
    //  iRectList.Add(ButtonsPanel);
    //   const IRECT controls = b.GetGridCell(1, 2, 2);
    //    gain

    pGraphics->AttachControl(new IVRadioButtonControl(m_ButtonsPanel.GetGridCell(1, 2, rows, columns).GetFromTop(100).GetMidHPadded(padding), kFilterSelector, getInitList(m_filterAlgo),
                                                      "FilterSelector", DEFAULT_STYLE, EVShape::Ellipse, EDirection::Vertical, 5.f));

    // gain
    pGraphics->AttachControl(new IVKnobControl(ShaperPanel.GetGridCell(1, 5, rows, columns).GetFromTop(75).GetMidHPadded(padding), kGain));
    // filter
    pGraphics->AttachControl(new IVKnobControl(FilterPanel.GetGridCell(1, 2, rows, columns).GetFromTop(100).GetTranslated(-50, -10)./*GetFromLeft(180).*/ GetMidHPadded(padding), kFilterCutoff));
    pGraphics->AttachControl(new IVKnobControl(FilterPanel.GetGridCell(1, 3, rows, columns).GetFromTop(75).GetMidHPadded(padding), kFilterResonance));
    pGraphics->AttachControl(new IVKnobControl(FilterPanel.GetGridCell(1, 4, rows, columns).GetFromTop(75).GetMidHPadded(padding), kFilterBandwidth));
    pGraphics->AttachControl(
      new IVToggleControl(FilterPanel.GetGridCell(1, 5, rows, columns).GetFromTop(75).GetMidHPadded(padding), kFilterBypass, "FilterBypass", DEFAULT_STYLE.WithColor(kFG, COLOR_WHITE), "Off", "On"));
    // shaper
    pGraphics->AttachControl(new IVKnobControl(ShaperPanel.GetGridCell(1, 1, rows, columns).GetFromTop(75).GetMidHPadded(padding), kShaperDrive));
    pGraphics->AttachControl(new IVKnobControl(ShaperPanel.GetGridCell(1, 2, rows, columns).GetFromTop(75).GetMidHPadded(padding), kShaperShape));
    pGraphics->AttachControl(new IVKnobControl(ShaperPanel.GetGridCell(1, 3, rows, columns).GetFromTop(75).GetMidHPadded(padding), kShaperBias));


    // pGraphics->AttachControl(
    // new IVToggleControl(ShaperPanel.GetGridCell(1, 4, rows, columns).GetFromTop(75).GetMidHPadded(padding), kFilterVintage, "FilterVintage", DEFAULT_STYLE.WithColor(kFG, COLOR_WHITE), "Off",
    // "On"));

    pGraphics->AttachControl(new IVRadioButtonControl(m_ButtonsPanel.GetGridCell(1, 1, rows, columns).GetFromTop(100).GetMidHPadded(padding), kFilterAlgo, {"DF1", "DF2", "SVF1", "ZDF1"}, "FilterAlgo",
                                                      DEFAULT_STYLE, EVShape::Ellipse, EDirection::Vertical, 5.f));


    // pGraphics->AttachControl(new IVRadioButtonControl(ButtonsPanel.GetGridCell(1, 2, rows, columns).GetFromTop(100).GetMidHPadded(padding), // Use the stored bounds
    //                                                   kFilterSelector, GetFilterSelectorOptions(filterAlgo), "FilterSelector", DEFAULT_STYLE, EVShape::Ellipse, EDirection::Vertical, 5.f));


    pGraphics->AttachControl(new IVRadioButtonControl(m_ButtonsPanel.GetGridCell(1, 5, rows, columns).GetFromTop(75).GetMidHPadded(padding), kFilterType, {"LP", "BP", "BS", "HP"}, "FilterType",
                                                      DEFAULT_STYLE, EVShape::Ellipse, EDirection::Vertical));
    pGraphics->AttachControl(new IVRadioButtonControl(m_ButtonsPanel.GetGridCell(1, 3, rows, columns).GetFromTop(75).GetMidHPadded(padding), kOverSampling, {"None", "2x", "4x", "8x", "16x"},
                                                      "FilterOversampling", DEFAULT_STYLE, EVShape::Ellipse, EDirection::Vertical, 5.f));

    m_filterAlgo = GetParam(kFilterAlgo)->Int();
    int selector = GetParam(kFilterSelector)->Int();
  };
#endif
}

//*****************************

//*****************************

#if IPLUG_DSP
void ColorFilterPlugin::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  const int nChans = NOutChansConnected();
  const double sampleRate = GetSampleRate();
  mOverSampler.ProcessBlock(inputs, outputs, nFrames, nChans, nChans, [&](sample** inputs, sample** outputs, int nFrames) {
    for (int s = 0; s < nFrames; s++)
    {
      FilterParameters fParams{};


      m_filterAlgo = GetParam(kFilterAlgo)->Value();
      // gain
      const double gain = GetParam(kGain)->Value() / 100.;
      // Filter
      const double filterCutoff = GetParam(kFilterCutoff)->Value() / 100.0;
      const double filterReso = GetParam(kFilterResonance)->Value() / 100.0;
      const double filterBadwidth = GetParam(kFilterBandwidth)->Value() / 100.0;
      const double filterBypass = GetParam(kFilterBypass)->Value();
      const double filterSelector = GetParam(kFilterSelector)->Value();
      const int filterType = GetParam(kFilterType)->Value();
      // const int filterVintage = GetParam(kFilterVintage)->Value();
      //  shaper
      const double shaperDrive = GetParam(kShaperDrive)->Value() / 100.;
      const double shaperShape = GetParam(kShaperShape)->Value() / 100.;
      const double shaperAsym = GetParam(kShaperBias)->Value() / 100.;
      const double shaperBypassMix = GetParam(kShaperBypass)->Value();
      const int oversamplingFactor = GetParam(kOverSampling)->Value();

      m_ovrsmpFactor = oversamplingFactor;
      retained_kFilterSelector = filterSelector;

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
      int selector{};
      // if (!filterVintage)
      //{
      if (filterType == (int)FilterType::LP)
      {
        DefineSelector(selector);
        SmoothBypass::processSmoothBypass(
          std::function<void(double&)>([selector, this, &fParams](double& input) -> void { return filterSwitcherLP_L.Process(input, selector, fParams); }), inputL, smoothedFilterBypass);
        SmoothBypass::processSmoothBypass(
          std::function<void(double&)>([selector, this, &fParams](double& input) -> void { return filterSwitcherLP_R.Process(input, selector, fParams); }), inputR, smoothedFilterBypass);
      }

      if (filterType == (int)FilterType::BP)
      {
        DefineSelector(selector);

        SmoothBypass::processSmoothBypass(
          std::function<void(double&)>([selector, this, &fParams](double& input) -> void { return filterSwitcherBP_L.Process(input, selector, fParams); }), inputL, smoothedFilterBypass);
        SmoothBypass::processSmoothBypass(
          std::function<void(double&)>([selector, this, &fParams](double& input) -> void { return filterSwitcherBP_R.Process(input, selector, fParams); }), inputR, smoothedFilterBypass);
      }

      if (filterType == (int)FilterType::BS)
      {
        DefineSelector(selector);

        SmoothBypass::processSmoothBypass(
          std::function<void(double&)>([selector, this, &fParams](double& input) -> void { return filterSwitcherBS_L.Process(input, selector, fParams); }), inputL, smoothedFilterBypass);
        SmoothBypass::processSmoothBypass(
          std::function<void(double&)>([selector, this, &fParams](double& input) -> void { return filterSwitcherBS_R.Process(input, selector, fParams); }), inputR, smoothedFilterBypass);
      }

      if (filterType == (int)FilterType::HP)
      {
        DefineSelector(selector);

        SmoothBypass::processSmoothBypass(
          std::function<void(double&)>([selector, this, &fParams](double& input) -> void { return filterSwitcherHP_L.Process(input, selector, fParams); }), inputL, smoothedFilterBypass);
        SmoothBypass::processSmoothBypass(
          std::function<void(double&)>([selector, this, &fParams](double& input) -> void { return filterSwitcherHP_R.Process(input, selector, fParams); }), inputR, smoothedFilterBypass);
      }
      //}

      /* else
       {
         if (filterType == (int)FilterType::LP)
         {
           DefineSelector(selector);

           SmoothBypass::processSmoothBypass(
             std::function<void(double&)>([selector, this, &fParams](double& input) -> void { return filterSwitcherLP_Vintage_L.Process(input, selector, fParams); }), inputL, smoothedFilterBypass);
           SmoothBypass::processSmoothBypass(
             std::function<void(double&)>([selector, this, &fParams](double& input) -> void { return filterSwitcherLP_Vintage_R.Process(input, selector, fParams); }), inputR, smoothedFilterBypass);
         }

         if (filterType == (int)FilterType::BP)
         {
           DefineSelector(selector);

           SmoothBypass::processSmoothBypass(
             std::function<void(double&)>([selector, this, &fParams](double& input) -> void { return filterSwitcherBP_Vintage_L.Process(input, selector, fParams); }), inputL, smoothedFilterBypass);
           SmoothBypass::processSmoothBypass(
             std::function<void(double&)>([selector, this, &fParams](double& input) -> void { return filterSwitcherBP_Vintage_R.Process(input, selector, fParams); }), inputR, smoothedFilterBypass);
         }

         if (filterType == (int)FilterType::BS)
         {
           DefineSelector(selector);

           SmoothBypass::processSmoothBypass(
             std::function<void(double&)>([selector, this, &fParams](double& input) -> void { return filterSwitcherBS_Vintage_L.Process(input, selector, fParams); }), inputL, smoothedFilterBypass);
           SmoothBypass::processSmoothBypass(
             std::function<void(double&)>([selector, this, &fParams](double& input) -> void { return filterSwitcherBS_Vintage_R.Process(input, selector, fParams); }), inputR, smoothedFilterBypass);
         }

         if (filterType == (int)FilterType::HP)
         {
           DefineSelector(selector);

           SmoothBypass::processSmoothBypass(
             std::function<void(double&)>([selector, this, &fParams](double& input) -> void { return filterSwitcherHP_Vintage_L.Process(input, selector, fParams); }), inputL, smoothedFilterBypass);
           SmoothBypass::processSmoothBypass(
             std::function<void(double&)>([selector, this, &fParams](double& input) -> void { return filterSwitcherHP_Vintage_R.Process(input, selector, fParams); }), inputR, smoothedFilterBypass);
         }
       }*/
      sigmoidalShaperL.Process(inputL, fParams);
      sigmoidalShaperR.Process(inputR, fParams);
      filnalTanh(inputL, fParams);
      filnalTanh(inputR, fParams);
      mRingBufferL.write(inputL);
      mRingBufferR.write(inputR);
      mRingBufferL.readChunk();
      mRingBufferR.readChunk();
      //mDomeShaperL.ProcessBuffer(mRingBufferL.getChunkBuffer());
      //mDomeShaperL.ProcessBuffer(mRingBufferL.getOverlapBuffer());
      mRingBufferL.writeChunk();
      mRingBufferR.writeChunk();
      outputs[0][s] = mRingBufferL.read();
      outputs[1][s] = mRingBufferR.read();
    }
  });

  if (mFactorChanged)
  {
    mOverSampler.SetOverSampling((iplug::EFactor)m_ovrsmpFactor);
    mOverSampler.Reset(GetBlockSize());
    mFactorChanged = false;
  }
}

void ColorFilterPlugin::OnReset()
{
  mOverSampler.Reset(GetBlockSize());
  SendParameterValueFromDelegate(kFilterSelector, retained_kFilterSelector, 0);
  int latencySamples = 2 * mRingBufferL.getChunkSize() - mRingBufferL.getHopSize();
  SetLatency(latencySamples);
}
void ColorFilterPlugin::OnParamChange(int paramIdx, EParamSource, int sampleOffset)
{
  switch (paramIdx)
  {
  case kOverSampling:
    mFactorChanged = true;
    break;
  case kFilterSelector:
    m_filterAlgo = GetParam(kFilterAlgo)->Int();
    if (m_filterAlgo == (int)FilterAlgo::DF1)
    {
      m_df1retainer = GetParam(kFilterSelector)->Int();
    }
    else if (m_filterAlgo == (int)FilterAlgo::DF2)
    {
      m_df2retainer = GetParam(kFilterSelector)->Int();
    }
    else if (m_filterAlgo == (int)FilterAlgo::SVF1)
    {
      m_svf1retainer = GetParam(kFilterSelector)->Int();
    }
    else if (m_filterAlgo == (int)FilterAlgo::ZDF1)
    {
      m_zdf1retainer = GetParam(kFilterSelector)->Int();
    }
    break;
  case kFilterAlgo:
    m_filterAlgo = GetParam(kFilterAlgo)->Int();
    int min_value{};
    int max_value{};
    double default_value{};
    if (m_filterAlgo == (int)FilterAlgo::DF1)
    {
      default_value = m_df1retainer;
      min_value = 0;
      max_value = 4;
    }
    else if (m_filterAlgo == (int)FilterAlgo::DF2)
    {
      default_value = m_df2retainer;
      min_value = 5;
      max_value = 6;
    }
    else if (m_filterAlgo == (int)FilterAlgo::SVF1)
    {
      default_value = m_svf1retainer;
      min_value = 7;
      max_value = 9;
    }
    else if (m_filterAlgo == (int)FilterAlgo::ZDF1)
    {
      default_value = m_zdf1retainer;
      min_value = 10;
      max_value = 12;
    }


    GetParam(kFilterSelector)->InitInt("FilterSelector", default_value, min_value, max_value, "", IParam::kFlagStepped, "");
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
      // pGraphics->SetAllControlsDirty();

      pGraphics->AttachControl(new IVRadioButtonControl(m_ButtonsPanel.GetGridCell(1, 2, rows, columns).GetFromTop(100).GetMidHPadded(padding), kFilterSelector, getInitList(m_filterAlgo),
                                                        "FilterSelector", DEFAULT_STYLE, EVShape::Ellipse, EDirection::Vertical, 5.f));
      SendParameterValueFromDelegate(kFilterSelector, default_value, 0);
    }
    break;
  }
}

void ColorFilterPlugin::DefineSelector(int& selector) const
{
  if (m_filterAlgo == (int)FilterAlgo::DF1)
  {
    selector = m_df1retainer;
  }
  else if (m_filterAlgo == (int)FilterAlgo::DF2)
  {
    selector = m_df2retainer;
  }
  else if (m_filterAlgo == (int)FilterAlgo::SVF1)
  {
    selector = m_svf1retainer;
  }
  else if (m_filterAlgo == (int)FilterAlgo::ZDF1)
  {
    selector = m_zdf1retainer;
  }
}
#endif
