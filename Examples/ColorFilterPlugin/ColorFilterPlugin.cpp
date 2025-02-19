#include "ColorFilterPlugin.h"
#include "IPlugConstants.h"
#include "IPlug_include_in_plug_src.h"
#ifndef CLAP_API
static double GetModulatedParamOffset(int i) { return 0; }
  #define CLAP_PARAM_IS_MODULATABLE IParam::kFlagsNone
#endif // !CLAP
#define ColorFilterStyle_Knob ColorFilterStyle_Knob.WithDrawModValue()


ColorFilterPlugin::ColorFilterPlugin(const InstanceInfo& info)
  : iplug::Plugin(info, MakeConfig(kNumParams, kNumPresets))
{
  GetParam(kDryWet)->InitDouble("Mix", 100., 0., 100.0, 0.01, "", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kGainIn)->InitDouble("PreGain", 50., 0., 100.0, 0.01, "", CLAP_PARAM_IS_MODULATABLE);
  mapParamToDB(kGainIn);
  GetParam(kGainOut)->InitDouble("PostGain", 50., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  mapParamToDB(kGainOut);
  GetParam(kFilterCutoff)->InitDouble("Cutoff", 100., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kFilterResonance)->InitDouble("Resonance", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kFilterBandwidth)->InitDouble("Bandwidth", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kFilterSelector)->InitInt("FilterSelector", 0, 0, (int)FilterTypes::MAX_FILTER_TYPES - 1, "", IParam::kFlagStepped, "");
  GetParam(kFilterType)->InitInt("FilterType", 0, 0, 3, "", IParam::kFlagStepped, "");
  GetParam(kFilterAlgo)->InitInt("FilterAlgo", 0, 0, 3, "", IParam::kFlagStepped, "");
  GetParam(kSpectralFilterOn)->InitBool("Spectral Toggle", false, "", IParam::kFlagStepped, "", "Off", "On");
  GetParam(kSpectralFilter_Drive)->InitDouble("SpectralFitlerDrive", 0., 0., 20.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kSpectralFilter_IR)->InitInt("SpectralFilterIR", 0, 0, 2, "", IParam::kFlagStepped, "");
  GetParam(kSpectralFilter_Harder)->InitBool("SpectralFilterHarder", false, "", IParam::kFlagStepped, "", "Soft", "Hard");
  GetParam(kFilterBypass)->InitBool("FilterBypass", true, "", IParam::kFlagStepped, "", "Off", "On");
  GetParam(kShaperDrive)->InitDouble("ShaperDrive", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kShaperShape)->InitDouble("ShaperShape", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kShaperBias)->InitDouble("ShaperBias", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kSpectralShaperDrive)->InitDouble("SpectralDrive", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kSpectralShaper_IR)->InitInt("SpectralShaperIR", 0, 0, 2, "", IParam::kFlagStepped, "");
  GetParam(kSpectralShaperSelector)->InitInt("ShaperSelector", 0, 0, (int)ShaperTypes::MAX_SHAPER_TYPES - 1, "", IParam::kFlagStepped, "");

  GetParam(kFilterFIR_Q)->InitDouble("FilterFIR", 0, 0, 100, 0.01, "", IParam::kFlagStepped | CLAP_PARAM_IS_MODULATABLE, "");
  GetParam(kFilterFIR_Q_Plus1)->InitBool("FilterFIR Even", false, "", IParam::kFlagStepped | CLAP_PARAM_IS_MODULATABLE, "", "Odd", "Even");

  GetParam(kShaperFIR_Q)->InitDouble("ShaperFIR", 0, 0, 100, 0.01, "", IParam::kFlagStepped | CLAP_PARAM_IS_MODULATABLE, "");
  GetParam(kShaperFIR_Q_Plus1)->InitBool("ShaperFIR Even", false, "", IParam::kFlagStepped, "", "Odd", "Even");

  GetParam(kFilterIIR_Q)->InitDouble("FilterIIR", 0, 0, 100, 0.01, "", IParam::kFlagStepped | CLAP_PARAM_IS_MODULATABLE, "");

  GetParam(kShaperIIR_Q)->InitDouble("ShaperIIR", 0, 0, 100, 0.01, "", IParam::kFlagStepped | CLAP_PARAM_IS_MODULATABLE, "");


  GetParam(kPhaserFreq)->InitDouble("PhaserFreq", 0., 0., 100.0, 0.0001, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kPhaserDepth)->InitDouble("PhaserDepth", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kPhaserSelector)->InitInt("PhaserSelector", 0, 0, 3, "", IParam::kFlagStepped, "");
  GetParam(kPhaserMix)->InitDouble("PhaserMix", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);


  GetParam(kDelayMix)->InitDouble("DelayMix", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kDelayTime)->InitDouble("DelayTime", 0., 0., 100.0, 0.01, "", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kDelayFeedback)->InitDouble("DelayFeedback", 0., -1., 1.0, 0.0001, "", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kDelayIR)->InitBool("DelayIR", false, "", IParam::kFlagStepped, "", "FIR", "IIR");
  GetParam(kDelayDampFilterCutoff)->InitDouble("DampFilter", 100., 0., 100.0, 0.01, "", CLAP_PARAM_IS_MODULATABLE);


  GetParam(kSoftClip)->InitDouble("SoftClip", 100, 0, 100, 0.01, "%", IParam::kFlagStepped | CLAP_PARAM_IS_MODULATABLE, "");

  GetParam(kOverSampling)->InitInt("Oversampler", 0, 0, 4, "OverSampler", IParam::kFlagStepped, "");
  GetParam(kBypass)->InitBool("Bypass", true, "", IParam::kFlagStepped, "", "Off", "On");

#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() { return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT)); };
  mLayoutFunc = [&](IGraphics* pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(Colors::FG_DIM);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);

    const IRECT b = pGraphics->GetBounds();
    const double getFromLeftControlPanels{300};
    m_FilterPanel = b.GetFromLeft(getFromLeftControlPanels).GetFromTop(30);
    m_ShaperPanel = b.GetFromLeft(getFromLeftControlPanels).GetFromTop(110);
    m_ButtonsPanel = b.GetFromLeft(getFromLeftControlPanels).GetFromTop(190) /*.GetTranslated(-10, 0)*/;

    const double topSpaceSize = 20;
    const double filterPanelSize = 80;
    const double shaperPanelSize = 80;
    const double buttonsPanelSize = 80;
    const double panelRoundness{3};
    const double translatedByX{40};
    const double getFromLeftRoundedPanel{340};
    // ██████   ██████  ██    ██ ███    ██ ██████  ███████ ██████      ██████   █████  ███    ██ ███████ ██      ███████
    // ██   ██ ██    ██ ██    ██ ████   ██ ██   ██ ██      ██   ██     ██   ██ ██   ██ ████   ██ ██      ██      ██
    // ██████  ██    ██ ██    ██ ██ ██  ██ ██   ██ █████   ██   ██     ██████  ███████ ██ ██  ██ █████   ██      ███████
    // ██   ██ ██    ██ ██    ██ ██  ██ ██ ██   ██ ██      ██   ██     ██      ██   ██ ██  ██ ██ ██      ██           ██
    // ██   ██  ██████   ██████  ██   ████ ██████  ███████ ██████      ██      ██   ██ ██   ████ ███████ ███████ ███████
    // One color panel
    pGraphics->AttachControl(new RoundedPanelControl(b.GetFromTop(PLUG_HEIGHT).GetFromLeft(PLUG_WIDTH), Colors::BW_LIGHT_BG, panelRoundness));
    pGraphics->AttachControl(
      new RoundedPanelControl(m_FilterPanel.GetFromTop(filterPanelSize).GetFromLeft(getFromLeftRoundedPanel).GetTranslated(translatedByX, topSpaceSize), Colors::BW_DARK_BG, panelRoundness));
    pGraphics->AttachControl(new RoundedPanelControl(
      m_FilterPanel.GetFromTop(shaperPanelSize).GetFromLeft(getFromLeftRoundedPanel).GetTranslated(translatedByX, topSpaceSize + filterPanelSize + 5), Colors::BW_DARK_BG, panelRoundness));
    pGraphics->AttachControl(
      new RoundedPanelControl(m_FilterPanel.GetFromTop(buttonsPanelSize).GetFromLeft(getFromLeftRoundedPanel).GetTranslated(translatedByX, topSpaceSize + filterPanelSize + shaperPanelSize + 10),
                              Colors::BW_DARK_BG, panelRoundness));
    // CONTROLS:
    // padding = width
    // ███████╗██╗██╗  ████████╗███████╗██████╗
    // ██╔════╝██║██║  ╚══██╔══╝██╔════╝██╔══██╗
    // █████╗  ██║██║     ██║   █████╗  ██████╔╝
    // ██╔══╝  ██║██║     ██║   ██╔══╝  ██╔══██╗
    // ██║     ██║███████╗██║   ███████╗██║  ██║
    // ╚═╝     ╚═╝╚══════╝╚═╝   ╚══════╝╚═╝  ╚═╝

    //  ██████ ██    ██ ████████  ██████  ███████ ███████
    // ██      ██    ██    ██    ██    ██ ██      ██
    // ██      ██    ██    ██    ██    ██ █████   █████
    // ██      ██    ██    ██    ██    ██ ██      ██
    //  ██████  ██████     ██     ██████  ██      ██

    IRECT mCutoffKnobBounds = m_FilterPanel.GetGridCell(1, 1, rows, columns).GetFromTop(getFromTopFilter) /*.GetTranslated(0, -10)*/.GetMidHPadded(padding) /*GetFromLeft(180).*/;
    IVStyle mFilterCutoffStyle = ColorFilterStyle_Knob
                                   .WithGradient(kFG, IPattern::CreateLinearGradient(mCutoffKnobBounds.L, mCutoffKnobBounds.T, mCutoffKnobBounds.L, mCutoffKnobBounds.B,
                                                                                     {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                   .WithGradient(kPR, IPattern::CreateLinearGradient(mCutoffKnobBounds.L, mCutoffKnobBounds.T, mCutoffKnobBounds.L, mCutoffKnobBounds.B,
                                                                                     {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}));

    mCutoff_Knob = new IVKnobControl(mCutoffKnobBounds, kFilterCutoff, "Cutoff", mFilterCutoffStyle, false, false, -135.f, 135.f, -135.f, EDirection::Vertical, DEFAULT_GEARING, 2.3);
    pGraphics->AttachControl(mCutoff_Knob, kFilterCutoff);
    // ██████  ███████ ███████  ██████
    // ██   ██ ██      ██      ██    ██
    // ██████  █████   ███████ ██    ██
    // ██   ██ ██           ██ ██    ██
    // ██   ██ ███████ ███████  ██████
    IRECT mResoKnobBounds = m_FilterPanel.GetGridCell(1, 2, rows, columns).GetFromTop(getFromTopFilter).GetMidHPadded(padding);
    IVStyle mFilterResoStyle = ColorFilterStyle_Knob
                                 .WithGradient(kFG, IPattern::CreateLinearGradient(mResoKnobBounds.L, mResoKnobBounds.T, mResoKnobBounds.L, mResoKnobBounds.B,
                                                                                   {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                 .WithGradient(kPR, IPattern::CreateLinearGradient(mResoKnobBounds.L, mResoKnobBounds.T, mResoKnobBounds.L, mResoKnobBounds.B,
                                                                                   {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
      /*.WithLabelText(labelText12)*/;

    mReso_Knob = pGraphics->AttachControl(new IVKnobControl(mResoKnobBounds, kFilterResonance, "Reso", mFilterResoStyle), kFilterResonance);

    // ██████   █████  ███    ██ ██████  ██     ██ ██ ██████  ████████ ██   ██
    // ██   ██ ██   ██ ████   ██ ██   ██ ██     ██ ██ ██   ██    ██    ██   ██
    // ██████  ███████ ██ ██  ██ ██   ██ ██  █  ██ ██ ██   ██    ██    ███████
    // ██   ██ ██   ██ ██  ██ ██ ██   ██ ██ ███ ██ ██ ██   ██    ██    ██   ██
    // ██████  ██   ██ ██   ████ ██████   ███ ███  ██ ██████     ██    ██   ██

    IRECT bwBounds = m_FilterPanel.GetGridCell(1, 3, rows, columns).GetFromTop(getFromTopFilter).GetMidHPadded(padding);
    mF_BW_Knob = pGraphics->AttachControl(
      new IVKnobControl(
        bwBounds, kFilterBandwidth, "Band",
        ColorFilterStyle_Knob.WithGradient(kFG, IPattern::CreateLinearGradient(bwBounds.L, bwBounds.T, bwBounds.L, bwBounds.B, {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
          .WithGradient(kPR, IPattern::CreateLinearGradient(
                               bwBounds.L, bwBounds.T, bwBounds.L, bwBounds.B, {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}})) /*, false, false, -135, 135, 0*/),
      kFilterBandwidth);

    //  ███████ ██████  ███████ ████████ ██████   █████  ██          ████████  ██████   ██████   ██████  ██      ███████
    //  ██      ██   ██ ██         ██    ██   ██ ██   ██ ██             ██    ██    ██ ██       ██       ██      ██
    //  ███████ ██████  █████      ██    ██████  ███████ ██             ██    ██    ██ ██   ███ ██   ███ ██      █████
    //       ██ ██      ██         ██    ██   ██ ██   ██ ██             ██    ██    ██ ██    ██ ██    ██ ██      ██
    //  ███████ ██      ███████    ██    ██   ██ ██   ██ ███████        ██     ██████   ██████   ██████  ███████ ███████

    mSpectralFilterOnToggle = pGraphics->AttachControl(new IVToggleControl(m_FilterPanel.GetGridCell(1, 4, rows, columns).GetFromTop(45).GetMidHPadded(25).GetTranslated(0, 5), kSpectralFilterOn,
                                                                           "SpectralToggle", ColorFilterStyle_Knob.WithGradient(kPR, Colors::ACCENT_YELLOW).WithRoundness(0.1), "Off", "On"));

    IRECT F_FIR_Q_Bounds = m_FilterPanel.GetGridCell(1, 4, rows, columns).GetFromTop(21 * SmallKnobScale).GetMidHPadded(9.45 * SmallKnobScale).GetTranslated(-17, 54);
    mSpectralFilter_IR = new IVKnobControl(
      m_FilterPanel.GetGridCell(1, 4, rows, columns).GetFromTop(21 * SmallKnobScale).GetMidHPadded(9.45 * SmallKnobScale).GetTranslated(0, 54), kSpectralFilter_IR, "SpectralIR",
      ColorFilterStyle_Knob.WithLabelText(false)
        .WithGradient(
          kFG, IPattern::CreateLinearGradient(F_FIR_Q_Bounds.L, F_FIR_Q_Bounds.T, F_FIR_Q_Bounds.L, F_FIR_Q_Bounds.B, {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
        .WithGradient(
          kPR, IPattern::CreateLinearGradient(F_FIR_Q_Bounds.L, F_FIR_Q_Bounds.T, F_FIR_Q_Bounds.L, F_FIR_Q_Bounds.B, {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
        .WithGradient(kX1, Colors::ACCENT_YELLOW),
      false, false, -135, 135, -135, EDirection::Vertical, 10, 1.);
    pGraphics->AttachControl(mSpectralFilter_IR, kSpectralFilter_IR);
    mSpectralFilter_IR->SetPointerThickness(1.2);

    mSpectralFilter_Harder = pGraphics->AttachControl(
      new IVToggleControl(m_FilterPanel.GetGridCell(1, 4, rows, columns).GetFromTop(21 * 0.5).GetMidHPadded(9.45 * 0.5).GetTranslated(17, 50 + 5), kSpectralFilter_Harder, "SpectralSofter",
                          ColorFilterStyle_Knob.WithGradient(kPR, Colors::BW_ORANGE).WithGradient(kFG, Colors::ACCENT_YELLOW).WithRoundness(1).WithLabelText(false), "", ""));
    mFilterFirQ = new IVKnobControl(F_FIR_Q_Bounds, kFilterFIR_Q, "FilterFirQ",
                                    ColorFilterStyle_Knob.WithLabelText(false)
                                      .WithGradient(kFG, IPattern::CreateLinearGradient(F_FIR_Q_Bounds.L, F_FIR_Q_Bounds.T, F_FIR_Q_Bounds.L, F_FIR_Q_Bounds.B,
                                                                                        {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                      .WithGradient(kPR, IPattern::CreateLinearGradient(F_FIR_Q_Bounds.L, F_FIR_Q_Bounds.T, F_FIR_Q_Bounds.L, F_FIR_Q_Bounds.B,
                                                                                        {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                                      .WithGradient(kX1, Colors::ACCENT_YELLOW),
                                    false, false, -135, 135, -135, EDirection::Vertical, 10, 1.);
    pGraphics->AttachControl(mFilterFirQ, kFilterFIR_Q);
    mFilterFirQ->SetPointerThickness(1.2);

    mFilterFirQ_Plus1 = pGraphics->AttachControl(
      new IVToggleControl(m_FilterPanel.GetGridCell(1, 4, rows, columns).GetFromTop(21 * m_Plus1_Scale).GetMidHPadded(9.45 * m_Plus1_Scale).GetTranslated(-26, 57), kFilterFIR_Q_Plus1, "odd/even",
                          ColorFilterStyle_Knob.WithGradient(kPR, Colors::MONOKAI_STRING_ORANGE).WithGradient(kFG, Colors::ACCENT_YELLOW).WithRoundness(1).WithLabelText(false), "", ""));


    mFilterIirQ = new IVKnobControl(F_FIR_Q_Bounds, kFilterIIR_Q, "FilterIirQ",
                                    ColorFilterStyle_Knob.WithLabelText(false)
                                      .WithGradient(kFG, IPattern::CreateLinearGradient(F_FIR_Q_Bounds.L, F_FIR_Q_Bounds.T, F_FIR_Q_Bounds.L, F_FIR_Q_Bounds.B,
                                                                                        {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                      .WithGradient(kPR, IPattern::CreateLinearGradient(F_FIR_Q_Bounds.L, F_FIR_Q_Bounds.T, F_FIR_Q_Bounds.L, F_FIR_Q_Bounds.B,
                                                                                        {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                                      .WithGradient(kX1, Colors::BW_ORANGE),
                                    false, false, -135, 135, -135, EDirection::Vertical, 10, 1.);
    pGraphics->AttachControl(mFilterIirQ, kFilterIIR_Q);
    mFilterIirQ->SetPointerThickness(1.2);

    //  ███████╗██████╗  ██████╗████████╗    ███████╗    ██████╗ ██████╗ ██╗   ██╗███████╗
    //  ██╔════╝██╔══██╗██╔════╝╚══██╔══╝    ██╔════╝    ██╔══██╗██╔══██╗██║   ██║██╔════╝
    //  ███████╗██████╔╝██║        ██║       █████╗      ██║  ██║██████╔╝██║   ██║█████╗
    //  ╚════██║██╔═══╝ ██║        ██║       ██╔══╝      ██║  ██║██╔══██╗╚██╗ ██╔╝██╔══╝
    //  ███████║██║     ╚██████╗   ██║       ██║         ██████╔╝██║  ██║ ╚████╔╝ ███████╗
    //  ╚══════╝╚═╝      ╚═════╝   ╚═╝       ╚═╝         ╚═════╝ ╚═╝  ╚═╝  ╚═══╝  ╚══════╝

    mSpectralFilterDrive = (new IVKnobControl(
      m_FilterPanel.GetGridCell(1, 2, rows, columns) /*.GetFromTop(20)*/.GetMidHPadded(padding).GetTranslated(-30, 53).GetFromTop(21 * SmallKnobScale).GetMidHPadded(9.45 * SmallKnobScale),
      kSpectralFilter_Drive, "Drive",
      ColorFilterStyle_Knob.WithGradient(kFG, IPattern::CreateLinearGradient(bwBounds.L, bwBounds.T, bwBounds.L, bwBounds.B, {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
        .WithGradient(kPR, IPattern::CreateLinearGradient(bwBounds.L, bwBounds.T, bwBounds.L, bwBounds.B, {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
        .WithGradient(kX1, Colors::ACCENT_YELLOW)
        .WithLabelText(false),
      false, false, -135, 135, -135, EDirection::Vertical, 10, 1.));
    pGraphics->AttachControl(mSpectralFilterDrive, kSpectralFilter_Drive);
    mSpectralFilterDrive->SetPointerThickness(1.2);

    // ███████ ██ ██      ████████    ██████  ██    ██ ██████   █████  ███████ ███████
    // ██      ██ ██         ██       ██   ██  ██  ██  ██   ██ ██   ██ ██      ██
    // █████   ██ ██         ██       ██████    ████   ██████  ███████ ███████ ███████
    // ██      ██ ██         ██       ██   ██    ██    ██      ██   ██      ██      ██
    // ██      ██ ███████    ██    ██ ██████     ██    ██      ██   ██ ███████ ███████
    IVStyle mFilterBypassStyle = ColorFilterStyle_Knob.WithGradient(kPR, Colors::MONOKAI_TYPE_GREEN).WithRoundness(0.1);
    IRECT mFilterBypass_Bounds = m_FilterPanel.GetGridCell(1, 5, rows, columns).GetFromTop(45).GetMidHPadded(25).GetTranslated(0, 5);

    mFilterBypassSwitch = pGraphics->AttachControl(new IVToggleControl(mFilterBypass_Bounds, kFilterBypass, "Filter Bypass", mFilterBypassStyle, "Off", "On"));

    // ███████╗██╗  ██╗ █████╗ ██████╗ ███████╗██████╗
    // ██╔════╝██║  ██║██╔══██╗██╔══██╗██╔════╝██╔══██╗
    // ███████╗███████║███████║██████╔╝█████╗  ██████╔╝
    // ╚════██║██╔══██║██╔══██║██╔═══╝ ██╔══╝  ██╔══██╗
    // ███████║██║  ██║██║  ██║██║     ███████╗██║  ██║
    // ╚══════╝╚═╝  ╚═╝╚═╝  ╚═╝╚═╝     ╚══════╝╚═╝  ╚═╝

    // ██████  ██████  ██ ██    ██ ███████
    // ██   ██ ██   ██ ██ ██    ██ ██
    // ██   ██ ██████  ██ ██    ██ █████
    // ██   ██ ██   ██ ██  ██  ██  ██
    // ██████  ██   ██ ██   ████   ███████
    IRECT shaperShapeKnobBounds = m_ShaperPanel.GetGridCell(1, 1, rows, columns_Shaper).GetFromTop(getFromTopShaper).GetMidHPadded(padding);
    pGraphics->AttachControl(new IVKnobControl(shaperShapeKnobBounds, kShaperDrive, "Drive",
                                               ColorFilterStyle_Knob
                                                 .WithGradient(kFG, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                   {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                                 .WithGradient(kPR, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                   {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                                                 .WithGradient(kX1, Colors::SHAPER_COLOR)),
                             kShaperDrive);

    // ███████ ██   ██  █████  ██████  ███████
    // ██      ██   ██ ██   ██ ██   ██ ██
    // ███████ ███████ ███████ ██████  █████
    //      ██ ██   ██ ██   ██ ██      ██
    // ███████ ██   ██ ██   ██ ██      ███████

    mShaperShape =
      pGraphics->AttachControl(new IVKnobControl(m_ShaperPanel.GetGridCell(1, 2, rows, columns_Shaper).GetFromTop(getFromTopShaper).GetMidHPadded(padding), kShaperShape, "Shape",
                                                 ColorFilterStyle_Knob
                                                   .WithGradient(kFG, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                     {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                                   .WithGradient(kPR, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                     {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                                                   .WithGradient(kX1, Colors::SHAPER_COLOR)),
                               kShaperShape);

    // ██████  ██  █████  ███████
    // ██   ██ ██ ██   ██ ██
    // ██████  ██ ███████ ███████
    // ██   ██ ██ ██   ██      ██
    // ██████  ██ ██   ██ ███████

    mShaperBias =
      pGraphics->AttachControl(new IVKnobControl(m_ShaperPanel.GetGridCell(1, 3, rows, columns_Shaper).GetFromTop(getFromTopShaper).GetMidHPadded(padding), kShaperBias, "Bias",
                                                 ColorFilterStyle_Knob
                                                   .WithGradient(kFG, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                     {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                                   .WithGradient(kPR, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                     {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                                                   .WithGradient(kX1, Colors::SHAPER_COLOR)),
                               kShaperBias);

    // ███████╗██████╗ ███████╗ ██████╗████████╗██████╗  █████╗ ██╗         ███████╗██╗  ██╗██████╗ ██████╗
    // ██╔════╝██╔══██╗██╔════╝██╔════╝╚══██╔══╝██╔══██╗██╔══██╗██║         ██╔════╝██║  ██║██╔══██╗██╔══██╗
    // ███████╗██████╔╝█████╗  ██║        ██║   ██████╔╝███████║██║         ███████╗███████║██████╔╝██████╔╝
    // ╚════██║██╔═══╝ ██╔══╝  ██║        ██║   ██╔══██╗██╔══██║██║         ╚════██║██╔══██║██╔═══╝ ██╔══██╗
    // ███████║██║     ███████╗╚██████╗   ██║   ██║  ██║██║  ██║███████╗    ███████║██║  ██║██║     ██║  ██║
    // ╚══════╝╚═╝     ╚══════╝ ╚═════╝   ╚═╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝    ╚══════╝╚═╝  ╚═╝╚═╝     ╚═╝  ╚═╝
    //
    // ███████ ██   ██  █████  ██████  ███████
    // ██      ██   ██ ██   ██ ██   ██ ██
    // ███████ ███████ ███████ ██████  █████
    //      ██ ██   ██ ██   ██ ██      ██
    // ███████ ██   ██ ██   ██ ██      ███████

    pGraphics->AttachControl(new IVKnobControl(m_ShaperPanel.GetGridCell(1, 4, rows, columns_Shaper).GetFromTop(getFromTopShaper).GetMidHPadded(padding), kSpectralShaperDrive, "SpectralDrive",
                                               ColorFilterStyle_Knob
                                                 .WithGradient(kFG, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                   {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                                 .WithGradient(kPR, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                   {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                                                 .WithGradient(kX1, Colors::JET_BRAINS_PINK)),
                             kSpectralShaperDrive);

    double S_FIR_Q_Scale = 0.6;
    IRECT S_FIR_Q_Bounds = m_ShaperPanel.GetGridCell(1, 4, rows, columns_Shaper).GetFromTop(21 * S_FIR_Q_Scale).GetMidHPadded(9.45 * S_FIR_Q_Scale).GetTranslated(-17, 58.5);

    mShaperFirQ = new IVKnobControl(S_FIR_Q_Bounds, kShaperFIR_Q, "ShaperFirQ",
                                    ColorFilterStyle_Knob.WithLabelText(false)
                                      .WithGradient(kFG, IPattern::CreateLinearGradient(S_FIR_Q_Bounds.L, S_FIR_Q_Bounds.T, S_FIR_Q_Bounds.L, S_FIR_Q_Bounds.B,
                                                                                        {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                      .WithGradient(kPR, IPattern::CreateLinearGradient(S_FIR_Q_Bounds.L, S_FIR_Q_Bounds.T, S_FIR_Q_Bounds.L, S_FIR_Q_Bounds.B,
                                                                                        {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                                      .WithGradient(kX1, Colors::JET_BRAINS_PINK /*Colors::MONOKAI_STRING_ORANGE*/),
                                    false, false, -135, 135, -135, EDirection::Vertical, 10, 1.);
    pGraphics->AttachControl(mShaperFirQ, kShaperFIR_Q);
    mShaperFirQ->SetPointerThickness(1.2);

    mShaperFirQ_Plus1 = pGraphics->AttachControl(
      new IVToggleControl(m_ShaperPanel.GetGridCell(1, 4, rows, columns_Shaper).GetFromTop(21 * m_Plus1_Scale).GetMidHPadded(9.45 * m_Plus1_Scale).GetTranslated(-26, 61.5), kShaperFIR_Q_Plus1,
                          "odd/even", ColorFilterStyle_Knob.WithGradient(kPR, Colors::MONOKAI_STRING_ORANGE).WithGradient(kFG, Colors::JET_BRAINS_PINK).WithRoundness(1).WithLabelText(false), "", ""));

    mShaperIirQ = new IVKnobControl(S_FIR_Q_Bounds, kShaperIIR_Q, "ShaperIirQ",
                                    ColorFilterStyle_Knob.WithLabelText(false)
                                      .WithGradient(kFG, IPattern::CreateLinearGradient(S_FIR_Q_Bounds.L, S_FIR_Q_Bounds.T, S_FIR_Q_Bounds.L, S_FIR_Q_Bounds.B,
                                                                                        {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                      .WithGradient(kPR, IPattern::CreateLinearGradient(S_FIR_Q_Bounds.L, S_FIR_Q_Bounds.T, S_FIR_Q_Bounds.L, S_FIR_Q_Bounds.B,
                                                                                        {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                                      .WithGradient(kX1, Colors::BW_ORANGE),
                                    false, false, -135, 135, -135, EDirection::Vertical, 10, 1.);
    pGraphics->AttachControl(mShaperIirQ, kShaperIIR_Q);
    mShaperIirQ->SetPointerThickness(1.2);
    //  ██ ██████
    //  ██ ██   ██
    //  ██ ██████
    //  ██ ██   ██
    //  ██ ██   ██

    mSpectralShaper_IR = new IVKnobControl(
      m_ShaperPanel.GetGridCell(1, 4, rows, columns_Shaper).GetFromTop(21 * SmallKnobScale).GetMidHPadded(9.45 * SmallKnobScale).GetTranslated(0, 58.5), kSpectralShaper_IR, "SpectralShaperIR",
      ColorFilterStyle_Knob.WithLabelText(false)
        .WithGradient(
          kFG, IPattern::CreateLinearGradient(S_FIR_Q_Bounds.L, S_FIR_Q_Bounds.T, S_FIR_Q_Bounds.L, S_FIR_Q_Bounds.B, {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
        .WithGradient(
          kPR, IPattern::CreateLinearGradient(S_FIR_Q_Bounds.L, S_FIR_Q_Bounds.T, S_FIR_Q_Bounds.L, S_FIR_Q_Bounds.B, {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
        .WithGradient(kX1, Colors::BW_ORANGE),
      false, false, -135, 135, -135, EDirection::Vertical, 10, 1.);
    pGraphics->AttachControl(mSpectralShaper_IR, kSpectralShaper_IR);
    mSpectralShaper_IR->SetPointerThickness(1.2);

    //  ███████ ██   ██  █████  ██████  ██████      ███████ ███████ ██       ██████ ████████ ██████
    //  ██      ██   ██ ██   ██ ██   ██ ██   ██     ██      ██      ██      ██         ██    ██   ██
    //  ███████ ███████ ███████ ██████  ██████      ███████ █████   ██      ██         ██    ██████
    //       ██ ██   ██ ██   ██ ██      ██   ██          ██ ██      ██      ██         ██    ██   ██
    //  ███████ ██   ██ ██   ██ ██      ██   ██     ███████ ███████ ███████  ██████    ██    ██   ██
    IRECT ssKnobBounds = m_ShaperPanel.GetGridCell(1, 4, rows, columns_Shaper).GetFromTop(21 * SmallKnobScale).GetMidHPadded(9.45 * SmallKnobScale).GetTranslated(17, 58.5);
    mSpectralShaperSelector = new IVKnobControl(
      ssKnobBounds, kSpectralShaperSelector, "SpectralShaperSelector",
      ColorFilterStyle_Knob.WithLabelText(false)
        .WithGradient(kFG, IPattern::CreateLinearGradient(ssKnobBounds.L, ssKnobBounds.T, ssKnobBounds.L, ssKnobBounds.B, {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
        .WithGradient(
          kPR, IPattern::CreateLinearGradient(ssKnobBounds.L, ssKnobBounds.T, ssKnobBounds.L, ssKnobBounds.B, {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
        .WithGradient(kX1, Colors::JET_BRAINS_PINK),
      false, false, -135, 135, -135, EDirection::Vertical, 6, 1.);
    mSpectralShaperSelector->SetPointerThickness(1.2);
    pGraphics->AttachControl(mSpectralShaperSelector);
    //  ██████╗ ███████╗██╗      █████╗ ██╗   ██╗
    //  ██╔══██╗██╔════╝██║     ██╔══██╗╚██╗ ██╔╝
    //  ██║  ██║█████╗  ██║     ███████║ ╚████╔╝
    //  ██║  ██║██╔══╝  ██║     ██╔══██║  ╚██╔╝
    //  ██████╔╝███████╗███████╗██║  ██║   ██║
    //  ╚═════╝ ╚══════╝╚══════╝╚═╝  ╚═╝   ╚═╝
    IRECT delayMixKnobBounds = m_ShaperPanel.GetGridCell(1, 5, rows, columns_Shaper).GetFromTop(getFromTopShaper).GetMidHPadded(padding);
    // IRECT delayTimeKnobBounds = delayMixKnobBounds.GetGridCell(2, 1, 2, 2).GetFromTop(25);
    IRECT delayTimeKnobBounds = m_ShaperPanel.GetGridCell(1, 5, rows, columns_Shaper).GetFromTop(21 * S_FIR_Q_Scale).GetMidHPadded(9.45 * S_FIR_Q_Scale).GetTranslated(-17, 58.5);
    IRECT delayFeedbackKnobBounds = m_ShaperPanel.GetGridCell(1, 5, rows, columns_Shaper).GetFromTop(21 * S_FIR_Q_Scale).GetMidHPadded(9.45 * S_FIR_Q_Scale).GetTranslated(17, 58.5);
    IRECT delayDampFilterCutoffBounds = m_ShaperPanel.GetGridCell(1, 5, rows, columns_Shaper).GetFromTop(21 * S_FIR_Q_Scale).GetMidHPadded(9.45 * S_FIR_Q_Scale).GetTranslated(34, 58.5);
    IVStyle delayMixKnobStyle = ColorFilterStyle_Knob
                                  .WithGradient(kFG, IPattern::CreateLinearGradient(delayMixKnobBounds.L, delayMixKnobBounds.T, delayMixKnobBounds.L, delayMixKnobBounds.B,
                                                                                    {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                  .WithGradient(kPR, IPattern::CreateLinearGradient(delayMixKnobBounds.L, delayMixKnobBounds.T, delayMixKnobBounds.L, delayMixKnobBounds.B,
                                                                                    {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}));
    IVStyle delayTimeKnobStyle = ColorFilterStyle_Knob
                                   .WithGradient(kFG, IPattern::CreateLinearGradient(delayTimeKnobBounds.L, delayTimeKnobBounds.T, delayTimeKnobBounds.L, delayTimeKnobBounds.B,
                                                                                     {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                   .WithGradient(kPR, IPattern::CreateLinearGradient(delayTimeKnobBounds.L, delayTimeKnobBounds.T, delayTimeKnobBounds.L, delayTimeKnobBounds.B,
                                                                                     {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                                   .WithLabelText(false);
    IVStyle delayDampFilterCutoffStyle =
      ColorFilterStyle_Knob
        .WithGradient(kFG, IPattern::CreateLinearGradient(delayDampFilterCutoffBounds.L, delayDampFilterCutoffBounds.T, delayDampFilterCutoffBounds.L, delayDampFilterCutoffBounds.B,
                                                          {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
        .WithGradient(kPR, IPattern::CreateLinearGradient(delayDampFilterCutoffBounds.L, delayDampFilterCutoffBounds.T, delayDampFilterCutoffBounds.L, delayDampFilterCutoffBounds.B,
                                                          {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
        .WithLabelText(false);

    IVKnobControl* delayMixKnob = new IVKnobControl(delayMixKnobBounds, kDelayMix, "DelayMix", delayMixKnobStyle);
    IVKnobControl* delayTimeKnob = new IVKnobControl(delayTimeKnobBounds, kDelayTime, "DelayTime", delayTimeKnobStyle, false, false, -135, 135, -135, EDirection::Vertical, 10, 1.);
    IVKnobControl* delayFeedbackKnob = new IVKnobControl(delayFeedbackKnobBounds, kDelayFeedback, "DelayFeedback", delayTimeKnobStyle, false, false, -135, 135, 0, EDirection::Vertical, 10, 1.);
    IVKnobControl* delayDampFilterCutoff =
      new IVKnobControl(delayDampFilterCutoffBounds, kDelayDampFilterCutoff, "DampFilter", delayDampFilterCutoffStyle, false, false, -135, 135, -135, EDirection::Vertical, 10, 1.);
    pGraphics->AttachControl(delayMixKnob, kDelayMix);
    pGraphics->AttachControl(delayTimeKnob, kDelayTime);
    pGraphics->AttachControl(delayFeedbackKnob, kDelayFeedback);
    pGraphics->AttachControl(delayDampFilterCutoff, kDelayDampFilterCutoff);
    delayTimeKnob->SetPointerThickness(1.2);
    delayFeedbackKnob->SetPointerThickness(1.2);
    delayDampFilterCutoff->SetPointerThickness(1.2);


    IControl* delayIR =
      pGraphics->AttachControl(new IVToggleControl(m_ShaperPanel.GetGridCell(1, 5, rows, columns).GetFromTop(21 * 0.5).GetMidHPadded(9.45 * 0.5).GetTranslated(0, 59), kDelayIR, "DelayIR",
                                                   ColorFilterStyle_Knob.WithGradient(kPR, Colors::BW_ORANGE).WithGradient(kFG, Colors::ACCENT_YELLOW).WithRoundness(1).WithLabelText(false), "", ""));
    //   ██████╗  █████╗ ██╗███╗   ██╗
    //  ██╔════╝ ██╔══██╗██║████╗  ██║
    //  ██║  ███╗███████║██║██╔██╗ ██║
    //  ██║   ██║██╔══██║██║██║╚██╗██║
    //  ╚██████╔╝██║  ██║██║██║ ╚████║
    //   ╚═════╝ ╚═╝  ╚═╝╚═╝╚═╝  ╚═══╝
    double translateGainKnobDown = 45;
    IRECT postGainKnobBounds = m_ButtonsPanel.GetGridCell(1, 5, rows, columns_Shaper).GetFromTop(50).GetMidHPadded(smallKnobPadding).GetTranslated(70, translateGainKnobDown);
    IRECT preGainKnobBounds = m_ButtonsPanel.GetGridCell(1, 1, rows, columns_Shaper).GetFromTop(50).GetMidHPadded(smallKnobPadding).GetTranslated(-70, translateGainKnobDown);
    IVStyle gainKnobStyle = ColorFilterStyle_Knob
                              .WithGradient(kFG, IPattern::CreateLinearGradient(postGainKnobBounds.L, postGainKnobBounds.T, postGainKnobBounds.L, postGainKnobBounds.B,
                                                                                {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                              .WithGradient(kPR, IPattern::CreateLinearGradient(postGainKnobBounds.L, postGainKnobBounds.T, postGainKnobBounds.L, postGainKnobBounds.B,
                                                                                {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                              .WithGradient(kX1, Colors::PLATINUM)
                              .WithLabelText(labelText12);
    mPostGain = new IVKnobControl(postGainKnobBounds, kGainOut, "PostGain", gainKnobStyle, false, false, -135., 135., 0, EDirection::Vertical, 8, 1.5);
    pGraphics->AttachControl(mPostGain, kGainOut);
    mPostGain->SetPointerThickness(1.8);
    mPreGain = new IVKnobControl(preGainKnobBounds, kGainIn, "PreGain", gainKnobStyle, false, false, -135, 135, 0, EDirection::Vertical, 8, 1.5);
    pGraphics->AttachControl(mPreGain, kGainIn);
    mPreGain->SetPointerThickness(1.8);

    //  ███████╗ ██████╗ ███████╗████████╗     ██████╗██╗     ██╗██████╗
    //  ██╔════╝██╔═══██╗██╔════╝╚══██╔══╝    ██╔════╝██║     ██║██╔══██╗
    //  ███████╗██║   ██║█████╗     ██║       ██║     ██║     ██║██████╔╝
    //  ╚════██║██║   ██║██╔══╝     ██║       ██║     ██║     ██║██╔═══╝
    //  ███████║╚██████╔╝██║        ██║       ╚██████╗███████╗██║██║
    //  ╚══════╝ ╚═════╝ ╚═╝        ╚═╝        ╚═════╝╚══════╝╚═╝╚═╝

    IRECT softclipKnobBounds = m_ButtonsPanel.GetGridCell(1, 5, rows, columns_Shaper).GetFromTop(50).GetMidHPadded(smallKnobPadding).GetTranslated(70, 0);
    IVStyle softClipKnobStyle = ColorFilterStyle_Knob
                                  .WithGradient(kFG, IPattern::CreateLinearGradient(softclipKnobBounds.L, softclipKnobBounds.T, softclipKnobBounds.L, softclipKnobBounds.B,
                                                                                    {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                  .WithGradient(kPR, IPattern::CreateLinearGradient(softclipKnobBounds.L, softclipKnobBounds.T, softclipKnobBounds.L, softclipKnobBounds.B,
                                                                                    {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                                  .WithGradient(kX1, Colors::PLATINUM)
                                  .WithLabelText(labelText12);
    IVKnobControl* softClipKnob = new IVKnobControl(softclipKnobBounds, kSoftClip, "SoftClip", softClipKnobStyle, false, false, -135, 135, -135, EDirection::Vertical, 8, 1.5);
    pGraphics->AttachControl(softClipKnob, kSoftClip);
    softClipKnob->SetPointerThickness(1.8);

    // ██████╗ ██╗   ██╗████████╗████████╗ ██████╗ ███╗   ██╗███████╗
    // ██╔══██╗██║   ██║╚══██╔══╝╚══██╔══╝██╔═══██╗████╗  ██║██╔════╝
    // ██████╔╝██║   ██║   ██║      ██║   ██║   ██║██╔██╗ ██║███████╗
    // ██╔══██╗██║   ██║   ██║      ██║   ██║   ██║██║╚██╗██║╚════██║
    // ██████╔╝╚██████╔╝   ██║      ██║   ╚██████╔╝██║ ╚████║███████║
    // ╚═════╝  ╚═════╝    ╚═╝      ╚═╝    ╚═════╝ ╚═╝  ╚═══╝╚══════╝

    // ███████    ████████ ██    ██ ██████  ███████
    // ██            ██     ██  ██  ██   ██ ██
    // █████         ██      ████   ██████  █████
    // ██            ██       ██    ██      ██
    // ██      ██    ██       ██    ██      ███████
    IRECT mFilter_Type_RB_Bounds = m_ButtonsPanel.GetGridCell(1, 1, rows, columns_BP).GetFromTop(75).GetMidHPadded(buttonsPadding);
    mFilter_Type_RadioButton = pGraphics->AttachControl(new IVRadioButtonControl(
      mFilter_Type_RB_Bounds, kFilterType, {"LP", "BP", "BS", "HP"}, "FilterType", ColorFilterStyle_RadioButtons.WithLabelText(false), EVShape::Ellipse, EDirection::Vertical));

    //  █████  ██       ██████   ██████
    // ██   ██ ██      ██       ██    ██
    // ███████ ██      ██   ███ ██    ██
    // ██   ██ ██      ██    ██ ██    ██
    // ██   ██ ███████  ██████   ██████
    // normal algo
    IRECT mFilterAlgo_Bounds = m_ButtonsPanel.GetGridCell(1, 2, rows, columns_BP).GetFromTop(75).GetMidHPadded(buttonsPadding);
    mFilterAlgoSwitch =
      GetUI()->AttachControl(new IVRadioButtonControl(mFilterAlgo_Bounds, kFilterAlgo, {"DF1", "DF2", "SVF", "ZDF"}, "FilterAlgo",
                                                      ColorFilterStyle_RadioButtons.WithColor(kPR, Colors::MONOKAI_TYPE_GREEN).WithLabelText(false), EVShape::Ellipse, EDirection::Vertical));


    // ███████ ██      ████████ ██████      ███████ ██       ██████ ████████ ██████
    // ██      ██         ██    ██   ██     ██      ██      ██         ██    ██   ██
    // █████   ██         ██    ██████      ███████ ██      ██         ██    ██████
    // ██      ██         ██    ██   ██          ██ ██      ██         ██    ██   ██
    // ██      ███████    ██    ██   ██     ███████ ███████  ██████    ██    ██   ██


    IRECT mFilterSelector_Bounds = m_ButtonsPanel.GetGridCell(1, 3, rows, columns_BP).GetFromTop(75).GetMidHPadded(buttonsPadding);
    mFilterSelectorSwitch = pGraphics->AttachControl(new IVRadioButtonControl(mFilterSelector_Bounds, kFilterSelector, FilterSelector::getInitList(m_filterAlgo), "FilterSelector",
                                                                              ColorFilterStyle_RadioButtons.WithLabelText(false), EVShape::Ellipse, EDirection::Vertical /*, 5.f*/));


    //  ██████  ██    ██ ███████ ██████  ███████  █████  ███    ███ ██████  ██
    // ██    ██ ██    ██ ██      ██   ██ ██      ██   ██ ████  ████ ██   ██ ██
    // ██    ██ ██    ██ █████   ██████  ███████ ███████ ██ ████ ██ ██████  ██
    // ██    ██  ██  ██  ██      ██   ██      ██ ██   ██ ██  ██  ██ ██      ██
    //  ██████    ████   ███████ ██   ██ ███████ ██   ██ ██      ██ ██      ███████

    pGraphics->AttachControl(new IVRadioButtonControl(m_ButtonsPanel.GetGridCell(1, 4, rows, columns_BP).GetFromTop(75).GetMidHPadded(buttonsPadding), kOverSampling, {"None", "2x", "4x", "8x", "16x"},
                                                      "FilterOversampling", ColorFilterStyle_RadioButtons.WithColor(kPR, Colors::PREPROCESSOR_VIOLET).WithLabelText(false), EVShape::Ellipse,
                                                      EDirection::Vertical));

    //  ██████  ██   ██  █████  ███████ ███████ ██████
    //  ██   ██ ██   ██ ██   ██ ██      ██      ██   ██
    //  ██████  ███████ ███████ ███████ █████   ██████
    //  ██      ██   ██ ██   ██      ██ ██      ██   ██
    //  ██      ██   ██ ██   ██ ███████ ███████ ██   ██

    IRECT phaserMixBounds = m_ButtonsPanel.GetGridCell(1, 5, rows, columns_BP).GetFromTop(getFromTopShaper).GetMidHPadded(padding);
    IRECT phaserFreqBounds = m_ButtonsPanel.GetGridCell(1, 5, rows, columns_Shaper).GetFromTop(21 * S_FIR_Q_Scale).GetMidHPadded(9.45 * S_FIR_Q_Scale).GetTranslated(-17, 58.5);
    IRECT phaserDepthBounds = m_ButtonsPanel.GetGridCell(1, 5, rows, columns_Shaper).GetFromTop(21 * S_FIR_Q_Scale).GetMidHPadded(9.45 * S_FIR_Q_Scale).GetTranslated(17, 58.5);
    IRECT phaserSelectorBounds = m_ButtonsPanel.GetGridCell(1, 5, rows, columns_Shaper).GetFromTop(21 * S_FIR_Q_Scale).GetMidHPadded(9.45 * S_FIR_Q_Scale).GetTranslated(0, 58.5);
    IVStyle phaserFreqKnobStyle = ColorFilterStyle_Knob
                                    .WithGradient(kFG, IPattern::CreateLinearGradient(phaserFreqBounds.L, phaserFreqBounds.T, phaserFreqBounds.L, phaserFreqBounds.B,
                                                                                      {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                    .WithGradient(kPR, IPattern::CreateLinearGradient(phaserFreqBounds.L, phaserFreqBounds.T, phaserFreqBounds.L, phaserFreqBounds.B,
                                                                                      {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                                    .WithLabelText(false);
    IVKnobControl* phaserFreqKnob = new IVKnobControl(phaserFreqBounds, kPhaserFreq, "PhaserFreq", phaserFreqKnobStyle, false, false, -135, 135, -135, EDirection::Vertical, 10, 1.);
    IVKnobControl* phaserDepthKnob = new IVKnobControl(phaserDepthBounds, kPhaserDepth, "PhaserDepth", phaserFreqKnobStyle, false, false, -135, 135, -135, EDirection::Vertical, 10, 1.);
    IVKnobControl* phaserSelectorKnob = new IVKnobControl(phaserSelectorBounds, kPhaserSelector, "PhaserSelector", phaserFreqKnobStyle, false, false, -135, 135, -135, EDirection::Vertical, 10, 1.);
    pGraphics->AttachControl(phaserFreqKnob, kPhaserFreq);
    pGraphics->AttachControl(phaserDepthKnob, kPhaserDepth);
    pGraphics->AttachControl(phaserSelectorKnob, kPhaserSelector);
    pGraphics->AttachControl(new IVKnobControl(phaserMixBounds, kPhaserMix, "PhaserMix",
                                               ColorFilterStyle_Knob
                                                 .WithGradient(kFG, IPattern::CreateLinearGradient(phaserMixBounds.L, phaserMixBounds.T, phaserMixBounds.L, phaserMixBounds.B,
                                                                                                   {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                                 .WithGradient(kPR, IPattern::CreateLinearGradient(phaserMixBounds.L, phaserMixBounds.T, phaserMixBounds.L, phaserMixBounds.B,
                                                                                                   {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                                                 .WithGradient(kX1, Colors::JET_BRAINS_PINK)),
                             kPhaserMix);
    phaserFreqKnob->SetPointerThickness(1.2);
    phaserDepthKnob->SetPointerThickness(1.2);
    phaserSelectorKnob->SetPointerThickness(1.2);


    //  ██████╗ ██╗   ██╗██████╗  █████╗ ███████╗███████╗
    //  ██╔══██╗╚██╗ ██╔╝██╔══██╗██╔══██╗██╔════╝██╔════╝
    //  ██████╔╝ ╚████╔╝ ██████╔╝███████║███████╗███████╗
    //  ██╔══██╗  ╚██╔╝  ██╔═══╝ ██╔══██║╚════██║╚════██║
    //  ██████╔╝   ██║   ██║     ██║  ██║███████║███████║
    //  ╚═════╝    ╚═╝   ╚═╝     ╚═╝  ╚═╝╚══════╝╚══════╝
    pGraphics->AttachControl(new IVToggleControl(m_FilterPanel.GetGridCell(1, 5, rows, columns_BP).GetFromTop(16).GetMidHPadded(24 * 0.5).GetTranslated(70, -20), kBypass, "Bypass",
                                                 ColorFilterStyle_Knob.WithGradient(kPR, Colors::PLATINUM).WithGradient(kFG, Colors::BW_DARK_KNOB_BOTTOM_DIM).WithRoundness(1).WithLabelText(false), "",
                                                 ""));
    m_filterAlgo = GetParam(kFilterAlgo)->Int();

    //  ██████╗ ██████╗ ██╗   ██╗ ██╗██╗    ██╗███████╗████████╗
    //  ██╔══██╗██╔══██╗╚██╗ ██╔╝██╔╝██║    ██║██╔════╝╚══██╔══╝
    //  ██║  ██║██████╔╝ ╚████╔╝██╔╝ ██║ █╗ ██║█████╗     ██║
    //  ██║  ██║██╔══██╗  ╚██╔╝██╔╝  ██║███╗██║██╔══╝     ██║
    //  ██████╔╝██║  ██║   ██║██╔╝   ╚███╔███╔╝███████╗   ██║
    //  ╚═════╝ ╚═╝  ╚═╝   ╚═╝╚═╝     ╚══╝╚══╝ ╚══════╝   ╚═╝

    IVKnobControl* dryWetKnob =
      (new IVKnobControl(m_FilterPanel.GetGridCell(1, 5, rows, columns_BP).GetFromTop(50).GetMidHPadded(smallKnobPadding).GetTranslated(70, 0), kDryWet, "Dry/Wet",
                         ColorFilterStyle_Knob
                           .WithGradient(kFG, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                             {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                           .WithGradient(kPR, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                             {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                           .WithGradient(kX1, Colors::PLATINUM)
                           .WithLabelText(labelText12),
                         false, false, -135, 135, -135, EDirection::Vertical, 8, 1.5));
    pGraphics->AttachControl(dryWetKnob, kDryWet);
    dryWetKnob->SetPointerThickness(1.8);

    // int selector = GetParam(kFilterSelector)->Int();
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

  if (g_Bypass)
    mOverSampler.ProcessBlock(inputs, outputs, nFrames, nChans, nChans, [&](sample** inputs, sample** outputs, int nFrames) {
      for (int s = 0; s < nFrames; s++)
      {
        filterFIR_Q = Map01To2_200_Stepped(getFinalParamValue(kFilterFIR_Q), GetParam(kFilterFIR_Q_Plus1)->Value());
        filterIIR_Q = Map01To1_40_Stepped(getFinalParamValue(kFilterIIR_Q));
        shaperFIR_Q = Map01To2_200_Stepped(getFinalParamValue(kShaperFIR_Q), GetParam(kShaperFIR_Q_Plus1)->Value());
        shaperIIR_Q = Map01To1_40_Stepped(getFinalParamValue(kShaperIIR_Q));

        double preGain = mapGain(getFinalParamValue(kGainIn));
        // filter
        const double filterCutoff = getFinalParamValue(kFilterCutoff);
        const double filterResonance = getFinalParamValue(kFilterResonance);
        const double filterBandwidth = getFinalParamValue(kFilterBandwidth);
        const double spectralFilterDrive = getFinalParamValue(kSpectralFilter_Drive);
        //  shaper
        const double shaperDrive = getFinalParamValue(kShaperDrive);
        const double shaperShape = getFinalParamValue(kShaperShape);
        const double shaperBias = getFinalParamValue(kShaperBias);
        const double spectralShaperDrive = getFinalParamValue(kSpectralShaperDrive);


        const double phaserMix = getFinalParamValue(kPhaserMix);
        const double phaserFreq = getFinalParamValue(kPhaserFreq);
        const double phaserDepth = getFinalParamValue(kPhaserDepth);


        const double delayMix = getFinalParamValue(kDelayMix);
        const double delayTime = getFinalParamValue(kDelayTime) * 0.075;
        const double delayFeedback = getFinalParamValue(kDelayFeedback, true, false) * 0.98;
        const double delayDampFilterCutoff = getFinalParamValue(kDelayDampFilterCutoff);

        const double softClip = getFinalParamValue(kSoftClip);
        double postGain = mapGain(getFinalParamValue(kGainOut));
        const double dryWet = getFinalParamValue(kDryWet);

        BatchSetModData(kDryWet, kGainIn, kGainOut, kFilterCutoff, kFilterResonance, kFilterBandwidth, kSpectralFilter_Drive, kShaperDrive, kShaperShape, kShaperBias, kSpectralShaperDrive,
                        kFilterFIR_Q, kShaperFIR_Q, kFilterIIR_Q, kShaperIIR_Q, kPhaserFreq, kPhaserMix, kPhaserDepth, kDelayMix, kDelayTime, kDelayFeedback, kDelayDampFilterCutoff, kSoftClip);
        fParams.setFilterParameters(m_filterType, m_filterSelector, filterCutoff, filterResonance, filterBandwidth, shaperDrive, shaperShape, shaperBias, spectralFilterDrive, filterFIR_Q, filterIIR_Q,
                                    spectralFilter_IR, spectralFilter_Harder, spectralShaperDrive, shaperFIR_Q, shaperIIR_Q, spectralShaper_IR, spectralShaperSelector, phaserMix, phaserFreq,
                                    phaserDepth, m_phaserSelector, delayMix, delayTime, delayFeedback, delayDampFilterCutoff, m_ovrsmpFactor, sampleRate);
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

        FilteredL = sigmoidalShaperL.Process(FilteredL, fParams);
        FilteredR = sigmoidalShaperR.Process(FilteredR, fParams);
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
        if (softClip)
        {
          FilteredL = interpolateLin(FilteredL, filnalTanh(FilteredL, fParams), softClip);
          FilteredR = interpolateLin(FilteredR, filnalTanh(FilteredR, fParams), softClip);
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
    // case kDelayTime: {
    //   double time = GetParam(kDelayTime)->Value() * 30;
    //   delayLineL.SetDelayTime(time);
    //   delayLineR.SetDelayTime(time);
    // }
    // break;
    // case kDelayFeedback: {
    //   double fb = GetParam(kDelayFeedback)->Value();
    //   delayLineL.SetFeedback(fb);
    //   delayLineR.SetFeedback(fb);
    // }
    // break;

  // case kShaperDrive:
  //  DecideControlDisableStatus(GetParam(kShaperDrive)->Value() == 0, mShaperShape, mShaperBias);
  // break;

  // case kPhaserSelector:
  // break;
  case kSpectralShaperDrive:
    // DecideControlDisableStatus(GetParam(kSpectralShaperDrive)->Value() == 0, /*mShaperFirQ_Odd,*/ mShaperFirQ, mShaperFirQ_Plus1, mShaperIirQ, mSpectralShaper_IR, mSpectralShaperSelector);
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
    DecideOnReset();
    break;
  case kFilterAlgo:
    m_filterAlgo = GetParam(kFilterAlgo)->Int();
    DecideOnReset();
    mSelectorIsDirty = true;
    break;
  // case kFilterBypass:
  case kFilterType:
    m_filterType = GetParam(kFilterType)->Value();
    DecideOnReset();
    break;
  case kSpectralFilterOn:
    DecideOnReset();
    break;
  case kFilterBypass:
    DecideOnReset();
    break;
  case kBypass:
    g_Bypass = GetParam(kBypass)->Value();
    break;
  case kSpectralFilter_Harder:
    spectralFilter_Harder = GetParam(kSpectralFilter_Harder)->Value();
    break;
  case kSpectralFilter_IR:
    spectralFilter_IR = GetParam(kSpectralFilter_IR)->Value();
    DecideOnReset();

    break;
  case kSpectralShaper_IR:
    spectralShaper_IR = GetParam(kSpectralShaper_IR)->Value();
    DecideOnReset();

    break;
  case kSpectralShaperSelector:
    spectralShaperSelector = GetParam(kSpectralShaperSelector)->Value();
    break;
  case kOverSampling:
    mOversamplingFactorChanged = true;
    break;
  }
}

void ColorFilterPlugin::DecideOnReset()
{
  DecideControlDisableStatus((GetParam(kFilterSelector)->Value() == (int)FilterTypes::SVF1_1P) || (GetParam(kFilterSelector)->Value() == (int)FilterTypes::DF1_1P)
                               || (GetParam(kFilterSelector)->Value() == (int)FilterTypes::ZDF1_1P) || (GetParam(kFilterBypass)->Value() < 0.5),
                             mReso_Knob);

  DecideControlDisableStatus(GetParam(kFilterBypass)->Value() < 0.5, mCutoff_Knob, mSpectralFilterOnToggle, mSpectralFilter_IR, /*mFilterFirQ_Odd, */ mFilterFirQ, mFilterFirQ_Plus1, mFilterIirQ,
                             mSpectralFilter_Harder, mFilterAlgoSwitch, mFilter_Type_RadioButton, mFilterSelectorSwitch, mSpectralFilterDrive);

  DecideControlDisableStatus(
    (GetParam(kFilterType)->Value() == (int)FilterType::LP) || (GetParam(kFilterType)->Value() == (int)FilterType::HP) || (GetParam(kFilterBypass)->Value() < 0.5), mF_BW_Knob);

  // DecideControlDisableStatus(GetParam(kShaperDrive)->Value() == 0, mShaperShape, mShaperBias);
  // DecideControlDisableStatus(GetParam(kSpectralShaperDrive)->Value() == 0, /*mShaperFirQ_Odd,*/ mShaperFirQ, mShaperFirQ_Plus1, mShaperIirQ, mSpectralShaper_IR, mSpectralShaperSelector);


  // DecideControlHideStatus((GetParam(kFilterType)->Value() == (int)FilterType::BS), mFilterSelectorSwitch);

  DecideControlHideStatus((GetParam(kSpectralFilterOn)->Value() < 0.5) || (GetParam(kSpectralFilter_IR)->Value()), mFilterFirQ);

  DecideControlHideStatus((GetParam(kSpectralFilter_IR)->Value()) || (GetParam(kSpectralFilterOn)->Value() < 0.5), mFilterFirQ_Plus1);
  DecideControlHideStatus(!(GetParam(kSpectralFilter_IR)->Value()) || (GetParam(kSpectralFilterOn)->Value() < 0.5), mFilterIirQ);
  DecideControlHideStatus((GetParam(kSpectralFilterOn)->Value() < 0.5), mSpectralFilterDrive, mSpectralFilter_Harder, mSpectralFilter_IR);

  DecideControlHideStatus((GetParam(kSpectralShaper_IR)->Value()), mShaperFirQ_Plus1);

  DecideControlHideStatus((GetParam(kSpectralShaper_IR)->Value()), mShaperFirQ);

  DecideControlHideStatus(!(GetParam(kSpectralShaper_IR)->Value()), mShaperIirQ);
};
void ColorFilterPlugin::CalculateLatency()
{
  if ((GetParam(kOverSampling)->Value() != 0) && (GetParam(kBypass)->Value() > 0.5))
    m_OS_LatencySamples = 4;
  else
    m_OS_LatencySamples = 0;
  // #define buffer RingBuffer::getChunkSize()
  // if (GetParam(kOverSampling)->Value() == 0)
  //   m_RB_LatencySamples = buffer;
  // else if (GetParam(kOverSampling)->Value() == 1)
  //   m_RB_LatencySamples = buffer / 2;
  // else if (GetParam(kOverSampling)->Value() == 2)
  //   m_RB_LatencySamples = buffer / 4;
  // else if (GetParam(kOverSampling)->Value() == 3)
  //   m_RB_LatencySamples = buffer / 8;
  // else
  //   m_RB_LatencySamples = buffer / 16;
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

#endif
