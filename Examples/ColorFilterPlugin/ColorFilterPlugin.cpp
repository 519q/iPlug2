#include "ColorFilterPlugin.h"
#include "IPlugConstants.h"
#include "IPlug_include_in_plug_src.h"
#include <filesystem>
#include <fstream>
#ifndef CLAP_API
static double GetModulatedParamOffset(int i) { return 0; }
  #define CLAP_PARAM_IS_MODULATABLE IParam::kFlagsNone
#endif // !CLAP


ColorFilterPlugin::ColorFilterPlugin(const InstanceInfo& info)
  : iplug::Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  LoadGlobalSettings();

  GetParam(kDryWet)->InitDouble("Mix", 100., 0., 100.0, 0.01, "", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kGainIn)->InitDouble("PreGain", 0., -24., 24.0, 0.01, "dB", CLAP_PARAM_IS_MODULATABLE);
  // mapParamToDB(kGainIn);
  GetParam(kGainOut)->InitDouble("PostGain", 0., -24., 24.0, 0.01, "dB", CLAP_PARAM_IS_MODULATABLE);
  // mapParamToDB(kGainOut);
  GetParam(kFilterCutoff)->InitDouble("Cutoff", 100., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kFilterResonance)->InitDouble("Resonance", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kFilterBandwidth)->InitDouble("Bandwidth", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kFilterSelector)->InitInt("FilterSelector", 0, 0, 4 /*(int)FilterTypes::MAX_FILTER_TYPES - 1*/, "", IParam::kFlagStepped, "");
  GetParam(kFilterType)->InitInt("FilterType", 0, 0, 3, "", IParam::kFlagStepped, "");
  GetParam(kFilterAlgo)->InitInt("FilterAlgo", 0, 0, 3, "", IParam::kFlagStepped, "");
  GetParam(kSpectralFilterOn)->InitBool("Spectral Toggle", false, "", IParam::kFlagStepped, "", "", "");
  GetParam(kSpectralFilter_Drive)->InitDouble("SpectralFitlerDrive", 0., 0., 20.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kSpectralFilter_IR)->InitInt("SpectralFilterIR", 0, 0, 2, "", IParam::kFlagStepped, "");
  GetParam(kSpectralFilter_Harder)->InitBool("SpectralFilterHarder", false, "", IParam::kFlagStepped, "", "Soft", "Hard");
  GetParam(kFilterBypass)->InitBool("FilterBypass", true, "", IParam::kFlagStepped, "", "Off", "On");
  GetParam(kShaperDrive)->InitDouble("ShaperDrive", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kShaperShape)->InitDouble("ShaperShape", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kShaperBias)->InitDouble("ShaperBias", 0., -100., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kSpectralShaperDrive)->InitDouble("SpectralDrive", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kSpectralShaper_IR)->InitInt("SpectralShaperIR", 0, 0, 2, "", IParam::kFlagStepped, "");
  GetParam(kSpectralShaperSelector)->InitDouble("ShaperSelector", 1, 1, (int)SpectralShaperTypes::MAX_SHAPER_TYPES, 0.0001, "", CLAP_PARAM_IS_MODULATABLE, "");

  GetParam(kFilterHilbertOrder)->InitDouble("FilterOrder", 0, 0, 100, 0.01, "", IParam::kFlagStepped | CLAP_PARAM_IS_MODULATABLE, "");
  GetParam(kFilterOddOrder)->InitBool("FilterOrderOdd", false, "", IParam::kFlagStepped | CLAP_PARAM_IS_MODULATABLE, "", "Even", "Odd");

  GetParam(kShaperHilbertOrder)->InitDouble("ShaperOrder", 0, 0, 100, 0.01, "", IParam::kFlagStepped | CLAP_PARAM_IS_MODULATABLE, "");
  GetParam(kShaperOddOrder)->InitBool("ShaperOrderOdd", false, "", IParam::kFlagStepped, "", "Even", "Odd");


  GetParam(kPhaserFreq)->InitDouble("PhaserFreq", 0., 0., 100.0, 0.0001, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kPhaserDepth)->InitDouble("PhaserDepth", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kPhaserSelector)->InitInt("PhaserSelector", 0, 0, 2, "", IParam::kFlagStepped, "");
  GetParam(kPhaserMix)->InitDouble("PhaserMix", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);


  GetParam(kDelayMix)->InitDouble("DelayMix", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kDelayTime)->InitDouble("DelayTime", 0., 0., 100.0, 0.01, "", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kDelayFeedback)->InitDouble("DelayFeedback", 0., -1., 1.0, 0.0001, "", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kDelayIR)->InitBool("DelayIR", false, "", IParam::kFlagStepped, "", "FIR", "IIR");
  GetParam(kDelayDampFilterCutoff)->InitDouble("DampFilter", 100., 0., 100.0, 0.01, "", CLAP_PARAM_IS_MODULATABLE);


  GetParam(kClipMix)->InitDouble("SoftClip", 100, 0, 100, 0.01, "%", CLAP_PARAM_IS_MODULATABLE, "");
  GetParam(kClipSelector)->InitDouble("ClipSelector", 2, 1, 3, 0.0001);

  GetParam(kOverSampling)->InitInt("Oversampler", 0, 0, 4, "OverSampler", IParam::kFlagStepped, "");
  GetParam(kBypass)->InitBool("Bypass", true, "", IParam::kFlagStepped, "", "Off", "On");

#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  #define ColorFilterStyle_SVG_KnobOver                                                                                                                                                                \
    ColorFilterStyle_SVG_KnobOver.WithDrawModValue().WithColor(kX1, IColor(255, 0x00, 0x8d, 0xcc)).WithColor(kFG, IColor(125, 0x00, 0x8d, 0xcc)).WithShowValue(false)
  mMakeGraphicsFunc = [&]() { return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT)); };
  mLayoutFunc = [&](IGraphics* pGraphics) {
    double buttonSize{};
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(Colors::FG_DIM);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
    b = pGraphics->GetBounds();
    // m_FilterPanel = b.GetFromLeft(PLUG_WIDTH).GetFromTop(PLUG_HEIGHT);
    pGraphics->AttachControl(new ISVGControl(b, pGraphics->LoadSVG("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/Frame 3.svg")));
    //  ██╗███╗   ██╗    ██╗ ██████╗ ██╗   ██╗████████╗
    //  ██║████╗  ██║   ██╔╝██╔═══██╗██║   ██║╚══██╔══╝
    //  ██║██╔██╗ ██║  ██╔╝ ██║   ██║██║   ██║   ██║
    //  ██║██║╚██╗██║ ██╔╝  ██║   ██║██║   ██║   ██║
    //  ██║██║ ╚████║██╔╝   ╚██████╔╝╚██████╔╝   ██║
    //  ╚═╝╚═╝  ╚═══╝╚═╝     ╚═════╝  ╚═════╝    ╚═╝
    // gain in
    mPreGain = pGraphics->AttachControl(new SVGKnobWithValueBarMED(b.GetFromLeft(medKnobSize).GetFromTop(medKnobSize).GetTranslated(40.14, 40.4), kGainIn, "", ColorFilterStyle_SVG_KnobOver), kGainIn);
    // gain out
    mPostGain =
      pGraphics->AttachControl(new SVGKnobWithValueBarMED(b.GetFromLeft(medKnobSize).GetFromTop(medKnobSize).GetTranslated(40.14, 85.6), kGainOut, "", ColorFilterStyle_SVG_KnobOver), kGainOut);
    //  ███████╗██╗██╗  ████████╗███████╗██████╗
    //  ██╔════╝██║██║  ╚══██╔══╝██╔════╝██╔══██╗
    //  █████╗  ██║██║     ██║   █████╗  ██████╔╝
    //  ██╔══╝  ██║██║     ██║   ██╔══╝  ██╔══██╗
    //  ██║     ██║███████╗██║   ███████╗██║  ██║
    //  ╚═╝     ╚═╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝
    // filter cutoff
    mCutoff = pGraphics->AttachControl(
      new SVGKnobWithValueBar(b.GetFromLeft(bigKnobSize).GetFromTop(bigKnobSize).GetTranslated(130.95, 47.8), kFilterCutoff, "", ColorFilterStyle_SVG_KnobOver), kFilterCutoff);
    // filter reso
    mReso = pGraphics->AttachControl(
      new SVGKnobWithValueBarMED(b.GetFromLeft(medKnobSize).GetFromTop(medKnobSize).GetTranslated(209.75, 40.8), kFilterResonance, "", ColorFilterStyle_SVG_KnobOver), kFilterResonance);
    // filter b/w
    mF_BW = pGraphics->AttachControl(
      new SVGKnobWithValueBarMED(b.GetFromLeft(medKnobSize).GetFromTop(medKnobSize).GetTranslated(209.75, 86.), kFilterBandwidth, "", ColorFilterStyle_SVG_KnobOver), kFilterBandwidth);
    // FilterSelector (Q)
    mFilterSelector = new IVRadioButtonControl_SVG(
      b.GetFromLeft(24).GetFromTop(96).GetTranslated(271.5, 47.2), kFilterSelector, {"6", "12", "18", "24", "36"}, "", ColorFilterStyle_SVG_KnobOver, EVShape::Ellipse, EDirection::Vertical);
    pGraphics->AttachControl(mFilterSelector, kFilterSelector);
    //  filter algo selector
    mFilterAlgo = pGraphics->AttachControl(
      new IVRadioButtonControl_SVG(b.GetFromLeft(120.7).GetFromTop(10).GetTranslated(113.85, 32.75), kFilterAlgo, {}, "", ColorFilterStyle_SVG_KnobOver, EVShape::Ellipse, EDirection::Horizontal),
      kFilterAlgo);
    // filter type
    mFilterType =
      new IVRadioButtonControl_SVG(b.GetFromLeft(120.7).GetFromTop(10).GetTranslated(113.85, 123.), kFilterType, {}, "", ColorFilterStyle_SVG_KnobOver, EVShape::Ellipse, EDirection::Horizontal);
    pGraphics->AttachControl(mFilterType, kFilterType);

    // kFilterBypass
    mFilterBypass =
      pGraphics->AttachControl(new IVToggleControl_SVG(b.GetFromTop(13.5).GetMidHPadded(13.5).GetTranslated(60.2, 30.84), kFilterBypass, "", ColorFilterStyle_SVG_KnobOver), kFilterBypass);
    //  ███████╗██████╗ ███████╗ ██████╗████████╗    ███████╗██╗██╗  ████████╗██████╗
    //  ██╔════╝██╔══██╗██╔════╝██╔════╝╚══██╔══╝    ██╔════╝██║██║  ╚══██╔══╝██╔══██╗
    //  ███████╗██████╔╝█████╗  ██║        ██║       █████╗  ██║██║     ██║   ██████╔╝
    //  ╚════██║██╔═══╝ ██╔══╝  ██║        ██║       ██╔══╝  ██║██║     ██║   ██╔══██╗
    //  ███████║██║     ███████╗╚██████╗   ██║       ██║     ██║███████╗██║   ██║  ██║
    //  ╚══════╝╚═╝     ╚══════╝ ╚═════╝   ╚═╝       ╚═╝     ╚═╝╚══════╝╚═╝   ╚═╝  ╚═╝
    // spectral filter order
    mSpectralFilterOrder = pGraphics->AttachControl(
      new SVGKnobWithValueBarMED(b.GetFromLeft(medKnobSize).GetFromTop(medKnobSize).GetTranslated(331.2, 40.4), kFilterHilbertOrder, "", ColorFilterStyle_SVG_KnobOver), kFilterHilbertOrder);
    // spectral filter drive
    mSpectralFilterDrive = pGraphics->AttachControl(
      new SVGKnobWithValueBarMED(b.GetFromLeft(medKnobSize).GetFromTop(medKnobSize).GetTranslated(331.2, 85.6), kSpectralFilter_Drive, "", ColorFilterStyle_SVG_KnobOver), kSpectralFilter_Drive);
    // spectral filter ir
    mSpectralFilterIR = pGraphics->AttachControl(
      new IVRadioButtonControl_SVG(b.GetFromLeft(24).GetFromTop(90.1).GetTranslated(431.2, 52.95), kSpectralFilter_IR, {}, "", ColorFilterStyle_SVG_KnobOver, EVShape::Ellipse, EDirection::Vertical),
      kSpectralFilter_IR);
    // kSpectralFilterHarder
    mSpectralFilterHarder = pGraphics->AttachControl(
      new IVToggleControl_SVG(b.GetFromTop(13.5).GetMidHPadded(13.5).GetTranslated(165.5, 60.8), kSpectralFilter_Harder, "", ColorFilterStyle_SVG_KnobOver), kSpectralFilter_Harder);
    // kSpectralFilter ODD
    mSpectralFilterOdd =
      pGraphics->AttachControl(new IVToggleControl_SVG(b.GetFromTop(13.5).GetMidHPadded(13.5).GetTranslated(165.5, 105.95), kFilterOddOrder, "", ColorFilterStyle_SVG_KnobOver), kFilterOddOrder);
    // kSpectralFilterOn
    mSpectralFilterBypass =
      pGraphics->AttachControl(new IVToggleControl_SVG(b.GetFromTop(13.5).GetMidHPadded(13.5).GetTranslated(211., 31.1), kSpectralFilterOn, "", ColorFilterStyle_SVG_KnobOver), kSpectralFilterOn);
    //  ███████╗██╗  ██╗ █████╗ ██████╗ ███████╗██████╗
    //  ██╔════╝██║  ██║██╔══██╗██╔══██╗██╔════╝██╔══██╗
    //  ███████╗███████║███████║██████╔╝█████╗  ██████╔╝
    //  ╚════██║██╔══██║██╔══██║██╔═══╝ ██╔══╝  ██╔══██╗
    //  ███████║██║  ██║██║  ██║██║     ███████╗██║  ██║
    //  ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚══════╝╚═╝  ╚═╝
    // shaper drive
    mShaperDrive =
      pGraphics->AttachControl(new SVGKnobWithValueBar(b.GetFromLeft(bigKnobSize).GetFromTop(bigKnobSize).GetTranslated(32.56, 183.4), kShaperDrive, "", ColorFilterStyle_SVG_KnobOver), kShaperDrive);
    // shaper shape
    mShaperShape = pGraphics->AttachControl(
      new SVGKnobWithValueBarMED(b.GetFromLeft(medKnobSize).GetFromTop(medKnobSize).GetTranslated(331.2 + -222.95, 40.6 + 135.3), kShaperShape, "", ColorFilterStyle_SVG_KnobOver), kShaperShape);
    // shaper bias
    mShaperBias = pGraphics->AttachControl(
      new SVGKnobWithValueBarMED(b.GetFromLeft(medKnobSize).GetFromTop(medKnobSize).GetTranslated(331.2 + -222.95, 85.8 + 135.3), kShaperBias, "", ColorFilterStyle_SVG_KnobOver), kShaperBias);
    //  ███████╗██████╗ ███████╗ ██████╗    ███████╗██╗  ██╗██████╗ ██████╗
    //  ██╔════╝██╔══██╗██╔════╝██╔════╝    ██╔════╝██║  ██║██╔══██╗██╔══██╗
    //  ███████╗██████╔╝█████╗  ██║         ███████╗███████║██████╔╝██████╔╝
    //  ╚════██║██╔═══╝ ██╔══╝  ██║         ╚════██║██╔══██║██╔═══╝ ██╔══██╗
    //  ███████║██║     ███████╗╚██████╗    ███████║██║  ██║██║     ██║  ██║
    //  ╚══════╝╚═╝     ╚══════╝ ╚═════╝    ╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝  ╚═╝
    // spectral shaper drive
    mSpectralShaperDrive = pGraphics->AttachControl(
      new SVGKnobWithValueBar(b.GetFromLeft(bigKnobSize).GetFromTop(bigKnobSize).GetTranslated(206.46, 183.4), kSpectralShaperDrive, "", ColorFilterStyle_SVG_KnobOver), kSpectralShaperDrive);
    // spectral shaper order
    mSpectralShaperOrder = pGraphics->AttachControl(
      new SVGKnobWithValueBarMED(b.GetFromLeft(medKnobSize).GetFromTop(medKnobSize).GetTranslated(331.22 + -49.2, 40.6 + 135.3), kShaperHilbertOrder, "", ColorFilterStyle_SVG_KnobOver),
      kShaperHilbertOrder);
    // spectral shaper selector
    mSpectralShaperSelector = pGraphics->AttachControl(
      new SVGKnobWithValueBarMED(b.GetFromLeft(medKnobSize).GetFromTop(medKnobSize).GetTranslated(331.22 + -49.2, 85.8 + 135.3), kSpectralShaperSelector, "", ColorFilterStyle_SVG_KnobOver),
      kSpectralShaperSelector);
    // spectral shaper ir
    mSpectralShaperIR = pGraphics->AttachControl(
      new IVRadioButtonControl_SVG(b.GetFromLeft(24).GetFromTop(90.5).GetTranslated(174.2, 173.3), kSpectralShaper_IR, {}, "", ColorFilterStyle_SVG_KnobOver, EVShape::Ellipse, EDirection::Vertical),
      kSpectralShaper_IR);
    // kSpectralShaper ODD
    mSpectralShaperOdd =
      pGraphics->AttachControl(new IVToggleControl_SVG(b.GetFromTop(13.5).GetMidHPadded(13.5).GetTranslated(6.9, 257.55), kShaperOddOrder, "", ColorFilterStyle_SVG_KnobOver), kShaperOddOrder);
    //  ██╗   ██╗████████╗██╗██╗     ██╗████████╗██╗   ██╗
    //  ██║   ██║╚══██╔══╝██║██║     ██║╚══██╔══╝╚██╗ ██╔╝
    //  ██║   ██║   ██║   ██║██║     ██║   ██║    ╚████╔╝
    //  ██║   ██║   ██║   ██║██║     ██║   ██║     ╚██╔╝
    //  ╚██████╔╝   ██║   ██║███████╗██║   ██║      ██║
    //   ╚═════╝    ╚═╝   ╚═╝╚══════╝╚═╝   ╚═╝      ╚═╝
    // utility mix
    mUtilityMix = pGraphics->AttachControl(
      new SVGKnobWithValueBarMED(b.GetFromLeft(medKnobSize).GetFromTop(medKnobSize).GetTranslated(331.25 + 86.9, 40.6 + 195.56), kDryWet, "", ColorFilterStyle_SVG_KnobOver), kDryWet);
    // oversampling
    mUtilityOversampling = pGraphics->AttachControl(
      new IVRadioButtonControl_SVG(b.GetFromLeft(24).GetFromTop(113.).GetTranslated(348.25, 165.55), kOverSampling, {}, "", ColorFilterStyle_SVG_KnobOver, EVShape::Ellipse, EDirection::Vertical),
      kOverSampling);
    // bypass
    pGraphics->AttachControl(new IVToggleControl_SVG(b.GetFromTop(13.5).GetMidHPadded(13.5).GetTranslated(211., 166.3), kBypass, "", ColorFilterStyle_SVG_KnobOver, "", "", true), kBypass);
    //  ██████╗ ██╗  ██╗ █████╗ ███████╗███████╗██████╗
    //  ██╔══██╗██║  ██║██╔══██╗██╔════╝██╔════╝██╔══██╗
    //  ██████╔╝███████║███████║███████╗█████╗  ██████╔╝
    //  ██╔═══╝ ██╔══██║██╔══██║╚════██║██╔══╝  ██╔══██╗
    //  ██║     ██║  ██║██║  ██║███████║███████╗██║  ██║
    //  ╚═╝     ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝╚══════╝╚═╝  ╚═╝
    // phaser mix
    mPhaserMix =
      pGraphics->AttachControl(new SVGKnobWithValueBar(b.GetFromLeft(bigKnobSize).GetFromTop(bigKnobSize).GetTranslated(55.34, 319.), kPhaserMix, "", ColorFilterStyle_SVG_KnobOver), kPhaserMix);
    // phaser order
    mPhaserOrder = pGraphics->AttachControl(
      new SVGKnobWithValueBarMED(b.GetFromLeft(medKnobSize).GetFromTop(medKnobSize).GetTranslated(331.25 + -200.4, 40.4 + 271.), kPhaserDepth, "", ColorFilterStyle_SVG_KnobOver), kPhaserDepth);
    // phaser Freq
    mPhaserFreq = pGraphics->AttachControl(
      new SVGKnobWithValueBarMED(b.GetFromLeft(medKnobSize).GetFromTop(medKnobSize).GetTranslated(331.25 + -200.4, 85.6 + 271.), kPhaserFreq, "", ColorFilterStyle_SVG_KnobOver), kPhaserFreq);
    // phaser selector
    mPhaserSelector = pGraphics->AttachControl(
      new IVRadioButtonControl_SVG(b.GetFromLeft(24).GetFromTop(90.5).GetTranslated(23, 308.7), kPhaserSelector, {}, "", ColorFilterStyle_SVG_KnobOver, EVShape::Ellipse, EDirection::Vertical),
      kPhaserSelector);
    //  ██████╗ ███████╗██╗      █████╗ ██╗   ██╗
    //  ██╔══██╗██╔════╝██║     ██╔══██╗╚██╗ ██╔╝
    //  ██║  ██║█████╗  ██║     ███████║ ╚████╔╝
    //  ██║  ██║██╔══╝  ██║     ██╔══██║  ╚██╔╝
    //  ██████╔╝███████╗███████╗██║  ██║   ██║
    //  ╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝   ╚═╝
    // delay mix
    mDelayMix =
      pGraphics->AttachControl(new SVGKnobWithValueBar(b.GetFromLeft(bigKnobSize).GetFromTop(bigKnobSize).GetTranslated(226.6, 318.8), kDelayMix, "", ColorFilterStyle_SVG_KnobOver), kDelayMix);
    // delay time
    mDelayTime = pGraphics->AttachControl(
      new SVGKnobWithValueBarLIL(b.GetFromLeft(medSmallerKnobSize).GetFromTop(medSmallerKnobSize).GetTranslated(331.25 + -12.06, 40.3 + 259.1), kDelayTime, "", ColorFilterStyle_SVG_KnobOver),
      kDelayTime);
    // delay feedback
    mDelayFeedback = pGraphics->AttachControl(new SVGKnobWithValueBarLIL(b.GetFromLeft(medSmallerKnobSize).GetFromTop(medSmallerKnobSize).GetTranslated(331.25 + -12.06, 40.3 + 259.1 + 37.6),
                                                                         kDelayFeedback, "", ColorFilterStyle_SVG_KnobOver),
                                              kDelayFeedback);
    // delay filter
    mDelayFilter = pGraphics->AttachControl(new SVGKnobWithValueBarLIL(b.GetFromLeft(medSmallerKnobSize).GetFromTop(medSmallerKnobSize).GetTranslated(331.25 + -12.06, 40.3 + 259.1 + 37.6 + 37.6),
                                                                       kDelayDampFilterCutoff, "", ColorFilterStyle_SVG_KnobOver),
                                            kDelayDampFilterCutoff);
    // kDelayIR
    mDelayIR = pGraphics->AttachControl(new IVToggleControl_SVG(b.GetFromTop(13.5).GetMidHPadded(13.5).GetTranslated(26.9, 392.1), kDelayIR, "", ColorFilterStyle_SVG_KnobOver), kDelayIR);
    //   ██████╗██╗     ██╗██████╗ ██████╗ ███████╗██████╗
    //  ██╔════╝██║     ██║██╔══██╗██╔══██╗██╔════╝██╔══██╗
    //  ██║     ██║     ██║██████╔╝██████╔╝█████╗  ██████╔╝
    //  ██║     ██║     ██║██╔═══╝ ██╔═══╝ ██╔══╝  ██╔══██╗
    //  ╚██████╗███████╗██║██║     ██║     ███████╗██║  ██║
    //   ╚═════╝╚══════╝╚═╝╚═╝     ╚═╝     ╚══════╝╚═╝  ╚═╝
    // clipper flavour
    mClipVisualiser = pGraphics->AttachControl(new ClipperLightButton(b.GetFromBottom(16 * 1.5).GetFromRight(20 * 1.5).GetTranslated(-13., -14.8)), kClipVisualizer);
    mClipperFlavour = pGraphics->AttachControl(
      new SVGKnobWithValueBarMED(b.GetFromLeft(medKnobSize).GetFromTop(medKnobSize).GetTranslated(331.25 + 71.75, 40.1 + 271.2), kClipSelector, "", ColorFilterStyle_SVG_KnobOver), kClipSelector);
    // clipper mix
    mClipperMix = pGraphics->AttachControl(
      new SVGKnobWithValueBarMED(b.GetFromLeft(medKnobSize).GetFromTop(medKnobSize).GetTranslated(331.25 + 71.75, 85.3 + 271.2), kClipMix, "", ColorFilterStyle_SVG_KnobOver), kClipMix);
    DecideOnReset();
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
  m_filterBypass = GetParam(kFilterBypass)->Value();
  m_spectralFilterOn = GetParam(kSpectralFilterOn)->Value();
  m_phaserSelector = GetParam(kPhaserSelector)->Value();
  g_Bypass = GetParam(kBypass)->Value();


  if (g_Bypass)
    mOverSampler.ProcessBlock(inputs, outputs, nFrames, nChans, nChans, [&](sample** inputs, sample** outputs, int nFrames) {
      for (int s = 0; s < nFrames; s++)
      {
        filterHilbertOrder = getFinalParamValue(kFilterHilbertOrder);
        bool oddFilterHilbertOrder = GetParam(kFilterOddOrder)->Value();
        shaperHilbertOrder = getFinalParamValue(kShaperHilbertOrder);
        bool oddShaperHilbertOrder = GetParam(kShaperOddOrder)->Value();

        double preGain = dBtoControlParam(getFinalParamValue(kGainIn, -24, 24, false));
        // filter
        const double filterCutoff = getFinalParamValue(kFilterCutoff);
        const double filterResonance = getFinalParamValue(kFilterResonance);
        const double filterBandwidth = getFinalParamValue(kFilterBandwidth);
        const double spectralFilterDrive = getFinalParamValue(kSpectralFilter_Drive);
        //  shaper
        const double shaperDrive = getFinalParamValue(kShaperDrive);
        const double shaperShape = getFinalParamValue(kShaperShape);
        const double shaperBias = getFinalParamValue(kShaperBias, -1);
        const double spectralShaperDrive = getFinalParamValue(kSpectralShaperDrive);
        spectralShaperSelector = getFinalParamValue(kSpectralShaperSelector, 1, (int)SpectralShaperTypes::MAX_SHAPER_TYPES, false);


        const double phaserMix = getFinalParamValue(kPhaserMix);
        const double phaserFreq = getFinalParamValue(kPhaserFreq);
        const double phaserDepth = getFinalParamValue(kPhaserDepth);


        const double delayMix = getFinalParamValue(kDelayMix);
        const double delayTime = getFinalParamValue(kDelayTime) * 0.075;
        const double delayFeedback = getFinalParamValue(kDelayFeedback, -1., 1, false);
        const double delayDampFilterCutoff = getFinalParamValue(kDelayDampFilterCutoff);

        const double softClip = getFinalParamValue(kClipMix);
        const double clipSelector = getFinalParamValue(kClipSelector, 1, 3, false);

        double postGain = dBtoControlParam(getFinalParamValue(kGainOut, -24, 24, false));
        const double dryWet = getFinalParamValue(kDryWet);
        SetVisualizationData(kClipVisualizer, peakCalc.getRoundedValueForUI());
        //SetVisualizationData(kClipVisualizer, peakCalc.getCurrentRMS());
        BatchSetModData(kDryWet, kGainIn, kGainOut, kFilterCutoff, kFilterResonance, kFilterBandwidth, kSpectralFilter_Drive, kShaperDrive, kShaperShape, kShaperBias, kSpectralShaperDrive,
                        kSpectralShaperSelector, kFilterHilbertOrder, kShaperHilbertOrder, kPhaserFreq, kPhaserMix, kPhaserDepth, kDelayMix, kDelayTime, kDelayFeedback, kDelayDampFilterCutoff,
                        kClipMix, kClipSelector);
        fParams.setFilterParameters(m_filterAlgo, m_filterType, m_filterSelector, filterCutoff, filterResonance, filterBandwidth, shaperDrive, shaperShape, shaperBias, spectralFilterDrive,
                                    filterHilbertOrder, oddFilterHilbertOrder, spectralFilter_IR, spectralFilter_Harder, spectralShaperDrive, shaperHilbertOrder, oddShaperHilbertOrder,
                                    spectralShaper_IR, spectralShaperSelector, phaserMix, phaserFreq, phaserDepth, m_phaserSelector, delayMix, delayTime, delayFeedback, delayDampFilterCutoff,
                                    m_ovrsmpFactor, sampleRate);
        const double L = inputs[0][s];
        const double R = inputs[1][s];
        double inputL = L * preGain;
        double inputR = R * preGain;
        double FilteredL = inputL;
        double FilteredR = inputR;
        if (m_filterBypass && !m_spectralFilterOn)
        {
          FilteredL = filterSelectorL.Process(inputL, fParams);
          FilteredR = filterSelectorR.Process(inputR, fParams);
        }
        else if (m_filterBypass && m_spectralFilterOn)
        {
          FilteredL = spectralFilterL.Process(inputL, fParams);
          FilteredR = spectralFilterR.Process(inputR, fParams);
        }
        if (fParams.m_drive)
        {
          FilteredL = interpolateLin(FilteredL, sigmoidalShaperL.Process(FilteredL, fParams.m_shape, fParams.m_bias), fParams.m_drive);
          FilteredR = interpolateLin(FilteredR, sigmoidalShaperR.Process(FilteredR, fParams.m_shape, fParams.m_bias), fParams.m_drive);
        }
        if (spectralShaperOn)
        {
          mSpectralShaperL.Process(FilteredL, fParams);
          mSpectralShaperR.Process(FilteredR, fParams);
        }
        // mRingBufferL.write(FilteredL);
        // mRingBufferR.write(FilteredR);
        // mRingBufferL.readChunk();
        // mRingBufferR.readChunk();
        // fftL.ProcessBuffer(mRingBufferL.getChunkBuffer(), mRingBufferL.getChunkRead(), fParams);
        // fftR.ProcessBuffer(mRingBufferR.getChunkBuffer(), mRingBufferR.getChunkRead(), fParams);
        // mRingBufferL.writeChunk();
        // mRingBufferR.writeChunk();
        // outputs[0][s] = interpolateLin(inputL, mRingBufferL.read(), smoothedDryWet) * smoothedGain;
        // outputs[1][s] = interpolateLin(inputR, mRingBufferR.read(), smoothedDryWet) * smoothedGain;
        if (phaserMix)
        {
          FilteredL = phaserL.Process(FilteredL, fParams);
          FilteredR = phaserR.Process(FilteredR, fParams);
        }
        if (delayMix)
        {
          FilteredL = interpolateLin(FilteredL, delayLineL.Process(FilteredL, fParams), delayMix);
          FilteredR = interpolateLin(FilteredR, delayLineR.Process(FilteredR, fParams), delayMix);
        }
        // FilteredL *= postGain;
        // FilteredR *= postGain;
        if (softClip)
        {
          double processedL = FilteredL;
          double processedR = FilteredR;
          double a_L{};
          double a_R{};
          double b_L{};
          double b_R{};
          if (clipSelector < 2)
          {
            a_L = filnalTanh(processedL);
            a_R = filnalTanh(processedR);
            b_L = softclip(processedL);
            b_R = softclip(processedR);
            processedL = interpolateLin(a_L, b_L, clipSelector - 1);
            processedR = interpolateLin(a_R, b_R, clipSelector - 1);
          }
          else if (clipSelector < 3)
          {
            a_L = softclip(processedL);
            a_R = softclip(processedR);
            b_L = diodeNonlinearity(processedL);
            b_R = diodeNonlinearity(processedR);
            processedL = interpolateLin(a_L, b_L, clipSelector - 2);
            processedR = interpolateLin(a_R, b_R, clipSelector - 2);
          }
          double unprocessedL = FilteredL;
          double unprocessedR = FilteredR;
          FilteredL = interpolateLin(FilteredL, processedL, softClip);
          FilteredR = interpolateLin(FilteredR, processedR, softClip);
          clipVisualisationValue = ((unprocessedL - FilteredL) + (unprocessedR - FilteredR)) / 2;
          peakCalc.processSample(clipVisualisationValue);
        }
        outputs[0][s] = interpolateLin(inputL, FilteredL, dryWet) * postGain;
        outputs[1][s] = interpolateLin(inputR, FilteredR, dryWet) * postGain;
      }
    });

  else
    for (int s = 0; s < nFrames; s++)
    {
      const double L = inputs[0][s];
      const double R = inputs[1][s];
      outputs[0][s] = L;
      outputs[1][s] = R;
    }
  if (mOversamplingFactorChanged)
  {
    mOversamplingFactorChanged = false;
    m_ovrsmpFactor = GetParam(kOverSampling)->Value();
    mOverSampler.SetOverSampling((iplug::EFactor)m_ovrsmpFactor);
    mOverSampler.Reset(GetBlockSize());
  }
}

void ColorFilterPlugin::OnReset()
{ // initialize smoothers:
  for (int i = 0; i < kNumParams; ++i)
  {
    mSmoothers[i] = iplug::LogParamSmooth<double>(paramSmoothing);
  }

  int m_factor{};
  switch (m_ovrsmpFactor)
  {
  case 1:
    m_factor = 2;
    break;
  case 2:
    m_factor = 4;
    break;
  case 3:
    m_factor = 8;
    break;
  case 4:
    m_factor = 16;
    break;
  default:
    m_factor = 1;
  }
  double s_r = GetSampleRate() * m_factor;
  delayLineL.SetUpDelayLine(s_r, s_r);
  delayLineR.SetUpDelayLine(s_r, s_r);
  CalculateLatency();
  SetLatency(m_OS_LatencySamples /* + m_RB_LatencySamples*/);
  DecideOnReset();
}
void ColorFilterPlugin::OnParamChange(int paramIdx, EParamSource, int sampleOffset)
{
  switch (paramIdx)
  {
  case kDelayIR:
    delayLineL.SetIIR(GetParam(kDelayIR)->Value());
    delayLineR.SetIIR(GetParam(kDelayIR)->Value());
    break;

  case kShaperDrive:
    DecideControlDisableStatus(GetParam(kShaperDrive)->Value() == 0, mShaperShape, mShaperBias);
    break;
  case kSpectralShaperDrive:
    DecideControlDisableStatus(GetParam(kSpectralShaperDrive)->Value() == 0, mSpectralShaperOrder, mSpectralShaperIR, mSpectralShaperSelector);
    DecideControlDisableStatus(GetParam(kSpectralShaperDrive)->Value() == 0 || GetParam(kSpectralShaper_IR)->Value() != 0, mSpectralShaperOdd);
    spectralShaperOn = GetParam(kSpectralShaperDrive)->Value();
    break;
  case kFilterSelector:
    m_filterSelector = GetParam(kFilterSelector)->Value();
    if (m_filterAlgo == (int)FilterAlgo::DF1)
      m_df1retainer = m_filterSelector;
    else if (m_filterAlgo == (int)FilterAlgo::DF2)
      m_df2retainer = m_filterSelector;
    else if (m_filterAlgo == (int)FilterAlgo::SVF1)
      m_svf1retainer = m_filterSelector;
    else if (m_filterAlgo == (int)FilterAlgo::ZDF1)
      m_zdf1retainer = m_filterSelector;
    DecideControlDisableStatus(GetParam(kFilterSelector)->Value() == 0, mReso);
    break;
  case kFilterAlgo:
    m_filterAlgo = GetParam(kFilterAlgo)->Value();
    mSelectorIsDirty = true;
    mAlgoChanged = true;
    break;
  case kFilterType:
    m_filterType = GetParam(kFilterType)->Value();
    DecideControlDisableStatus(GetParam(kFilterType)->Value() == (int)FilterType::LP || GetParam(kFilterType)->Value() == (int)FilterType::HP, mF_BW);
    break;
  case kSpectralFilterOn:
    DecideControlDisableStatus(GetParam(kSpectralFilterOn)->Value() < 0.5, mSpectralFilterOrder, mSpectralFilterDrive, mSpectralFilterOdd, mSpectralFilterHarder, mSpectralFilterIR);
    break;
  case kFilterBypass:
    DecideOnReset();
    break;
  case kBypass:
    DecideOnReset();
    break;
  case kSpectralFilter_Harder:
    spectralFilter_Harder = GetParam(kSpectralFilter_Harder)->Value();
    break;
  case kSpectralFilter_IR:
    spectralFilter_IR = GetParam(kSpectralFilter_IR)->Value();
    DecideControlDisableStatus(GetParam(kSpectralFilter_IR)->Value() != 0, mSpectralFilterOdd);
    break;
  case kSpectralShaper_IR:
    spectralShaper_IR = GetParam(kSpectralShaper_IR)->Value();
    DecideControlDisableStatus(GetParam(kSpectralShaper_IR)->Value() != 0, mSpectralShaperOdd);
    break;
  case kDelayMix:
    DecideControlDisableStatus(GetParam(kDelayMix)->Value() == 0, mDelayTime, mDelayFeedback, mDelayFilter, mDelayIR);
    break;
  case kPhaserMix:
    DecideControlDisableStatus(GetParam(kPhaserMix)->Value() == 0, mPhaserOrder, mPhaserFreq, mPhaserSelector);
    break;
  case kClipMix:
    DecideControlDisableStatus(GetParam(kClipMix)->Value() == 0, mClipperFlavour);
    break;
  case kOverSampling:
    mOversamplingFactorChanged = true;
    break;
  }
}

void ColorFilterPlugin::OnIdle()
{
  static double lastSavedZoom = 0; // Stores the last saved value to prevent unnecessary writes

  if (mDrawScaleRetainerNeedsSaving && (mDrawScaleRetainer != lastSavedZoom))
  {
    SaveGlobalSettings();
    lastSavedZoom = mDrawScaleRetainer; // Update the last saved value
    mDrawScaleRetainerNeedsSaving = false;
  }
  if (GetUI()) // Ensure UI exists before sending data
  {
    mModValueSender.TransmitData(*this); // Send modulation data to UI controls
  }
  if (mSelectorIsDirty)
  {
    mSelectorIsDirty = false;
    DefineSelector();
    SendParameterValueFromDelegate(kFilterSelector, m_filterSelector, 0);
  }
  if (mAlgoChanged)
    if (GetUI())
    {
      mAlgoChanged = false;
      if (m_filterAlgo == (int)FilterAlgo::DF2)
      {
        mFilterSelector->SetButtonDisabled(0, 1);
        mFilterSelector->SetButtonDisabled(2, 1);
      }
      else
      {
        mFilterSelector->SetButtonDisabled(0, 0);
        mFilterSelector->SetButtonDisabled(2, 0);
      }
      DecideControlDisableStatus(m_filterSelector == 0, mReso);
    }
}
void ColorFilterPlugin::DecideOnReset()
{
  DecideControlDisableStatus(
    GetParam(kBypass)->Value() < 0.5, mShaperDrive, mSpectralShaperDrive, mPhaserMix, mDelayMix, mClipperMix, mUtilityOversampling, mPreGain, mPostGain, mFilterBypass, mUtilityMix);

  DecideControlDisableStatus(GetParam(kBypass)->Value() < 0.5 || GetParam(kClipMix)->Value() == 0, mClipperFlavour);
  DecideControlDisableStatus(GetParam(kBypass)->Value() < 0.5 || GetParam(kSpectralFilterOn)->Value() < 0.5 || GetParam(kFilterBypass)->Value() < 0.5, mSpectralFilterOrder, mSpectralFilterDrive,
                             mSpectralFilterHarder, mSpectralFilterIR);
  DecideControlDisableStatus(
    GetParam(kBypass)->Value() < 0.5 || GetParam(kSpectralFilterOn)->Value() < 0.5 || GetParam(kFilterBypass)->Value() < 0.5 || GetParam(kSpectralFilter_IR)->Value() != 0, mSpectralFilterOdd);
  DecideControlDisableStatus(GetParam(kBypass)->Value() < 0.5 || GetParam(kFilterBypass)->Value() < 0.5, mCutoff, mFilterType, mFilterAlgo, mFilterSelector, mSpectralFilterBypass);
  DecideControlDisableStatus(GetParam(kBypass)->Value() < 0.5 || GetParam(kFilterBypass)->Value() < 0.5 || GetParam(kFilterSelector)->Value() == 0, mReso);
  DecideControlDisableStatus(
    GetParam(kBypass)->Value() < 0.5 || GetParam(kFilterBypass)->Value() < 0.5 || GetParam(kFilterType)->Value() == (int)FilterType::LP || GetParam(kFilterType)->Value() == (int)FilterType::HP,
    mF_BW);
  DecideControlDisableStatus(GetParam(kBypass)->Value() < 0.5 || GetParam(kShaperDrive)->Value() == 0, mShaperShape, mShaperBias);
  DecideControlDisableStatus(GetParam(kBypass)->Value() < 0.5 || GetParam(kSpectralShaperDrive)->Value() == 0, mSpectralShaperOrder, mSpectralShaperIR, mSpectralShaperSelector);
  DecideControlDisableStatus(GetParam(kBypass)->Value() < 0.5 || GetParam(kSpectralShaperDrive)->Value() == 0 || GetParam(kSpectralShaper_IR)->Value() != 0, mSpectralShaperOdd);
  DecideControlDisableStatus(GetParam(kBypass)->Value() < 0.5 || GetParam(kDelayMix)->Value() == 0, mDelayTime, mDelayFeedback, mDelayFilter, mDelayIR);
  DecideControlDisableStatus(GetParam(kBypass)->Value() < 0.5 || GetParam(kPhaserMix)->Value() == 0, mPhaserOrder, mPhaserFreq, mPhaserSelector);
}
void ColorFilterPlugin::CalculateLatency()
{
  if ((GetParam(kOverSampling)->Value() != 0) && (GetParam(kBypass)->Value() > 0.5))
    m_OS_LatencySamples = 4;
  else
    m_OS_LatencySamples = 0;
}
template <typename... Args>
void ColorFilterPlugin::DecideControlDisableStatus(bool disableCondition, Args... controls)
{
  if (GetUI())
    if (disableCondition)
      (controls->SetDisabled(true), ...);
    else
      (controls->SetDisabled(false), ...);
}
template <typename... Args>
void ColorFilterPlugin::DecideControlHideStatus(bool hideCondition, Args... controls)
{
  if (GetUI())
    if (hideCondition)
      (controls->Hide(true), ...);
    else
      (controls->Hide(false), ...);
}
void ColorFilterPlugin::DefineSelector()
{
  if (m_filterAlgo == (int)FilterAlgo::DF1)
    m_filterSelector = m_df1retainer;
  else if (m_filterAlgo == (int)FilterAlgo::DF2)
    m_filterSelector = m_df2retainer;
  else if (m_filterAlgo == (int)FilterAlgo::SVF1)
    m_filterSelector = m_svf1retainer;
  else if (m_filterAlgo == (int)FilterAlgo::ZDF1)
    m_filterSelector = m_zdf1retainer;
}
void ColorFilterPlugin::RemoveControl(int paramIdx)
{
  if (GetUI())
  {
    int idx = 0;
    while (idx < GetUI()->NControls())
    {
      IControl* pControl = GetUI()->GetControl(idx);
      if (pControl && pControl->GetParamIdx() == paramIdx)
      {
        GetUI()->RemoveControl(idx);
        // Don't increment idx since RemoveControl shifts everything down
      }
      else
      {
        idx++;
      }
    }
  }
}
bool ColorFilterPlugin::SerializeState(IByteChunk& chunk) const
{
  // Call the base class implementation to serialize parameters
  if (!SerializeParams(chunk))
    return false;
  // Add any additional state you want to save here
  // chunk.Put(&mDrawScaleRetainer);
  chunk.Put(&m_df1retainer);
  chunk.Put(&m_df2retainer);
  chunk.Put(&m_svf1retainer);
  chunk.Put(&m_zdf1retainer);
  chunk.Put(&m_filterAlgo);
  return SerializeParams(chunk);
}
// Override UnserializeParams to restore plugin state
int ColorFilterPlugin::UnserializeState(const IByteChunk& chunk, int startPos)
{
  // Call the base class implementation to unserialize parameters
  startPos = UnserializeParams(chunk, startPos);

  // Add any additional state you want to restore here
  // startPos = chunk.Get(&mDrawScaleRetainer, startPos);
  startPos = chunk.Get(&m_df1retainer, startPos);
  startPos = chunk.Get(&m_df2retainer, startPos);
  startPos = chunk.Get(&m_svf1retainer, startPos);
  startPos = chunk.Get(&m_zdf1retainer, startPos);
  startPos = chunk.Get(&m_filterAlgo, startPos);

  return UnserializeParams(chunk, startPos);
}
static std::string GetSettingsFilePath()
{
  WDL_String homePath;
  UserHomePath(homePath); // Get cross-platform user home directory
  #ifdef OS_WIN
  homePath.Append("\\ColorFilter\\settings.ini");
  #elif defined(OS_MAC)
  homePath.Append("/Library/Application Support/ColorFilter/settings.ini");
  #else // Linux
  homePath.Append("/.config/ColorFilter/settings.ini");
  #endif

  return std::string(homePath.Get());
}
void ColorFilterPlugin::SaveGlobalSettings()
{
  // std::ofstream file(GetSettingsFilePath());

  std::string filePath = GetSettingsFilePath();
  std::filesystem::path dirPath = std::filesystem::path(filePath).parent_path();
  // Ensure the directory exists before writing the file
  if (!std::filesystem::exists(dirPath))
  {
    if (!std::filesystem::create_directories(dirPath))
    {
      return;
    }
  }
  std::ofstream file(filePath);
  if (file.is_open())
  {
    file << "ZoomFactor=" << mDrawScaleRetainer << std::endl;
    file.close();
  }
}

void ColorFilterPlugin::LoadGlobalSettings()
{
  std::ifstream file(GetSettingsFilePath());
  if (file.is_open())
  {
    std::string line;
    while (std::getline(file, line))
    {
      if (line.find("ZoomFactor=") == 0)
      {
        mDrawScaleRetainer = std::stod(line.substr(11)); // Extract value
        break;
      }
    }
    file.close();
  }
}
void ColorFilterPlugin::OnUIOpen()
{
  IEditorDelegate::OnUIOpen();
  if (GetUI())
  {
    GetUI()->Resize(GetUI()->Width(), GetUI()->Height(), mDrawScaleRetainer);
    // GetUI()->SetDrawScale(mDrawScaleRetainer);
  }
}
void ColorFilterPlugin::OnUIClose()
{
  mDrawScaleRetainer = GetUI()->GetDrawScale();
  SaveGlobalSettings();
}

void ColorFilterPlugin::SetModData(int knobTag)
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
void ColorFilterPlugin::BatchSetModData(Tags... knobTag)
{
  (SetModData(knobTag), ...);
}

double ColorFilterPlugin::getFinalParamValue(int paramId, double clampBottom, double clampTop, bool scaleDown)
{
  double baseValue = GetParam(paramId)->Value();
  if (scaleDown)
    baseValue /= 100.0;
  double modValue = GetModulatedParamOffset(paramId);
  return std::clamp(mSmoothers[paramId].Process(baseValue + modValue), clampBottom, clampTop);
}
double ColorFilterPlugin::dBtoControlParam(double param)
{
  // Clamp param to the range [-24, +24]
  param = std::max(-24.0, std::min(24.0, param));

  // Convert param (in dB) to linear amplitude
  double amplitude = std::pow(10.0, param / 20.0);

  // Ensure the amplitude is clamped to [0, 1]
  return std::max(0.0, std::min(amplitude, 16.));
}

#endif
