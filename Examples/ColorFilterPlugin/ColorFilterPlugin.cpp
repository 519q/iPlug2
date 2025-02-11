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
  GetParam(kDryWet)->InitDouble("DryWet", 100., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kGain)->InitDouble("PostGain", 50., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kFilterCutoff)->InitDouble("Cutoff", 100., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kFilterResonance)->InitDouble("Resonance", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kFilterBandwidth)->InitDouble("Bandwidth", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kFilterSelector)->InitInt("FilterSelector", 0, 0, (int)FilterTypes::MAX_FILTER_TYPES - 1, "", IParam::kFlagStepped, "");
  GetParam(kFilterSelector_BS)->InitInt("FilterSelector", 0, 0, (int)FilterTypes::MAX_FILTER_TYPES - 2, "", IParam::kFlagStepped, "");
  GetParam(kFilterType)->InitInt("FilterType", 0, 0, 3, "", IParam::kFlagStepped, "");
  GetParam(kFilterAlgo)->InitInt("FilterAlgo", 0, 0, 2, "FilterAlgo", IParam::kFlagStepped, "");
  GetParam(kSpectralFilterOn)->InitBool("Spectral Toggle", false, "", IParam::kFlagStepped, "", "Off", "On");
  GetParam(kSpectralFilter_Drive)->InitDouble("SpectralFitlerDrive", 0., 0., 20.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kSpectralFilter_IR)->InitBool("SpectralFilterIR", false, "", IParam::kFlagStepped, "", "FIR", "IIR");
  GetParam(kSpectralFilter_Harder)->InitBool("SpectralFilterHarder", false, "", IParam::kFlagStepped, "", "Soft", "Hard");
  GetParam(kFilterBypass)->InitBool("FilterBypass", true, "", IParam::kFlagStepped, "", "Off", "On");
  GetParam(kShaperDrive)->InitDouble("ShaperDrive", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kShaperShape)->InitDouble("ShaperShape", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kShaperBias)->InitDouble("ShaperBias", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kSpectralShaperDrive)->InitDouble("SpectralDrive", 0., 0., 100.0, 0.01, "%", CLAP_PARAM_IS_MODULATABLE);
  GetParam(kSpectralShaper_IR)->InitBool("SpectralShaperIR", false, "", IParam::kFlagStepped, "", "FIR", "IIR");
  GetParam(kSpectralShaperSelector)->InitInt("ShaperSelector", 0, 0, (int)ShaperTypes::MAX_SHAPER_TYPES - 1, "", IParam::kFlagStepped, "");

  GetParam(kFilterFIR_Q)->InitDouble("FilterFIR", 0, 0, 100, 0.01, "", IParam::kFlagStepped | CLAP_PARAM_IS_MODULATABLE, "");
  GetParam(kFilterFIR_Q_Plus1)->InitBool("FilterFIR Even", false, "", IParam::kFlagStepped | CLAP_PARAM_IS_MODULATABLE, "", "Odd", "Even");

  GetParam(kShaperFIR_Q)->InitDouble("ShaperFIR", 0, 0, 100, 0.01, "", IParam::kFlagStepped | CLAP_PARAM_IS_MODULATABLE, "");
  GetParam(kShaperFIR_Q_Plus1)->InitBool("ShaperFIR Even", false, "", IParam::kFlagStepped, "", "Odd", "Even");

  GetParam(kFilterIIR_Q)->InitDouble("FilterIIR", 0, 0, 100, 0.01, "", IParam::kFlagStepped | CLAP_PARAM_IS_MODULATABLE, "");

  GetParam(kShaperIIR_Q)->InitDouble("ShaperIIR", 0, 0, 100, 0.01, "", IParam::kFlagStepped | CLAP_PARAM_IS_MODULATABLE, "");

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

    mSpectralFilter_IR = pGraphics->AttachControl(
      new IVToggleControl(m_FilterPanel.GetGridCell(1, 4, rows, columns).GetFromTop(21 * 0.5).GetMidHPadded(9.45 * 0.5).GetTranslated(0, 50 + 5), kSpectralFilter_IR, "SpectralIR",
                          ColorFilterStyle_Knob.WithGradient(kPR, Colors::BW_ORANGE).WithGradient(kFG, Colors::ACCENT_YELLOW).WithRoundness(1).WithLabelText(false), "", ""));
    mSpectralFilter_Harder = pGraphics->AttachControl(
      new IVToggleControl(m_FilterPanel.GetGridCell(1, 4, rows, columns).GetFromTop(21 * 0.5).GetMidHPadded(9.45 * 0.5).GetTranslated(17, 50 + 5), kSpectralFilter_Harder, "SpectralSofter",
                          ColorFilterStyle_Knob.WithGradient(kPR, Colors::BW_ORANGE).WithGradient(kFG, Colors::ACCENT_YELLOW).WithRoundness(1).WithLabelText(false), "", ""));


    double F_FIR_Q_Scale = 0.6;
    IRECT F_FIR_Q_Bounds = m_FilterPanel.GetGridCell(1, 4, rows, columns).GetFromTop(21 * F_FIR_Q_Scale).GetMidHPadded(9.45 * F_FIR_Q_Scale).GetTranslated(-17, 54);
    // mFilterFirQ_Odd = new IVKnobControl(F_FIR_Q_Bounds, kFilterFIR_Q, "FilterFirQ",
    //                                     ColorFilterStyle_Knob.WithLabelText(false)
    //                                       .WithGradient(kFG, IPattern::CreateLinearGradient(F_FIR_Q_Bounds.L, F_FIR_Q_Bounds.T, F_FIR_Q_Bounds.L, F_FIR_Q_Bounds.B,
    //                                                                                         {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
    //                                       .WithGradient(kPR, IPattern::CreateLinearGradient(F_FIR_Q_Bounds.L, F_FIR_Q_Bounds.T, F_FIR_Q_Bounds.L, F_FIR_Q_Bounds.B,
    //                                                                                         {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
    //                                       .WithGradient(kX1, Colors::ACCENT_YELLOW),
    //                                     false, false, -135, 135, -135, EDirection::Vertical, 10, 1.);
    // pGraphics->AttachControl(mFilterFirQ_Odd);
    // mFilterFirQ_Odd->SetPointerThickness(1.2);

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
      m_FilterPanel.GetGridCell(1, 2, rows, columns) /*.GetFromTop(20)*/.GetMidHPadded(padding).GetTranslated(-30, 53).GetFromTop(21 * F_FIR_Q_Scale).GetMidHPadded(9.45 * F_FIR_Q_Scale),
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
    // mShaperFirQ_Odd = new IVKnobControl(S_FIR_Q_Bounds, kShaperFIR_Q, "ShaperFirQ",
    //                                     ColorFilterStyle_Knob.WithLabelText(false)
    //                                       .WithGradient(kFG, IPattern::CreateLinearGradient(S_FIR_Q_Bounds.L, S_FIR_Q_Bounds.T, S_FIR_Q_Bounds.L, S_FIR_Q_Bounds.B,
    //                                                                                         {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
    //                                       .WithGradient(kPR, IPattern::CreateLinearGradient(S_FIR_Q_Bounds.L, S_FIR_Q_Bounds.T, S_FIR_Q_Bounds.L, S_FIR_Q_Bounds.B,
    //                                                                                         {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
    //                                       .WithGradient(kX1, Colors::JET_BRAINS_PINK),
    //                                     false, false, -135, 135, -135, EDirection::Vertical, 10, 1.);
    // pGraphics->AttachControl(mShaperFirQ_Odd);
    // mShaperFirQ_Odd->SetPointerThickness(1.2);

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

    mSpectralShaper_IR = pGraphics->AttachControl(
      new IVToggleControl(m_ShaperPanel.GetGridCell(1, 4, rows, columns_Shaper).GetFromTop(21 * 0.5).GetMidHPadded(9.45 * 0.5).GetTranslated(0, 59), kSpectralShaper_IR, "SpectralShaperIR",
                          ColorFilterStyle_Knob.WithGradient(kPR, Colors::BURNT_SIENNA).WithGradient(kFG, Colors::JET_BRAINS_PINK).WithRoundness(1).WithLabelText(false), "", ""));

    //  ███████ ██   ██  █████  ██████  ██████      ███████ ███████ ██       ██████ ████████ ██████
    //  ██      ██   ██ ██   ██ ██   ██ ██   ██     ██      ██      ██      ██         ██    ██   ██
    //  ███████ ███████ ███████ ██████  ██████      ███████ █████   ██      ██         ██    ██████
    //       ██ ██   ██ ██   ██ ██      ██   ██          ██ ██      ██      ██         ██    ██   ██
    //  ███████ ██   ██ ██   ██ ██      ██   ██     ███████ ███████ ███████  ██████    ██    ██   ██
    double ssKnobScale = 0.6;
    IRECT ssKnobBounds = m_ShaperPanel.GetGridCell(1, 4, rows, columns_Shaper).GetFromTop(21 * ssKnobScale).GetMidHPadded(9.45 * ssKnobScale).GetTranslated(17, 58.5);
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
    // shaperSelector->SetAnimation();

    // ██████   ██████  ███████ ████████      ██████   █████  ██ ███    ██
    // ██   ██ ██    ██ ██         ██        ██       ██   ██ ██ ████   ██
    // ██████  ██    ██ ███████    ██        ██   ███ ███████ ██ ██ ██  ██
    // ██      ██    ██      ██    ██        ██    ██ ██   ██ ██ ██  ██ ██
    // ██       ██████  ███████    ██         ██████  ██   ██ ██ ██   ████
    mPostGain =
      pGraphics->AttachControl(new IVKnobControl(m_ShaperPanel.GetGridCell(1, 5, rows, columns_Shaper).GetFromTop(getFromTopShaper).GetMidHPadded(padding), kGain, "Gain",
                                                 ColorFilterStyle_Knob
                                                   .WithGradient(kFG, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                     {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                                   .WithGradient(kPR, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                     {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                                                   .WithGradient(kX1, Colors::PLATINUM), false, false, -135, 135, 0),
                               kGain);

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
      GetUI()->AttachControl(new IVRadioButtonControl(mFilterAlgo_Bounds, kFilterAlgo, {"DF1", "DF2", "SVF1"}, "FilterAlgo",
                                                      ColorFilterStyle_RadioButtons.WithColor(kPR, Colors::MONOKAI_TYPE_GREEN).WithLabelText(false), EVShape::Ellipse, EDirection::Vertical));


    // ███████ ██      ████████ ██████      ███████ ██       ██████ ████████ ██████      ██ ███    ██ ██ ████████
    // ██      ██         ██    ██   ██     ██      ██      ██         ██    ██   ██     ██ ████   ██ ██    ██
    // █████   ██         ██    ██████      ███████ ██      ██         ██    ██████      ██ ██ ██  ██ ██    ██
    // ██      ██         ██    ██   ██          ██ ██      ██         ██    ██   ██     ██ ██  ██ ██ ██    ██
    // ██      ███████    ██    ██   ██     ███████ ███████  ██████    ██    ██   ██     ██ ██   ████ ██    ██


    IRECT mFilterSelector_Bounds = m_ButtonsPanel.GetGridCell(1, 3, rows, columns_BP).GetFromTop(75).GetMidHPadded(buttonsPadding);
    mFilterSelectorSwitch = pGraphics->AttachControl(new IVRadioButtonControl(mFilterSelector_Bounds, kFilterSelector, FilterSelector::getInitList(m_filterAlgo), "FilterSelector",
                                                                              ColorFilterStyle_RadioButtons.WithLabelText(false), EVShape::Ellipse, EDirection::Vertical /*, 5.f*/));


    mFilterSelectorSwitch_BS = pGraphics->AttachControl(new IVRadioButtonControl(mFilterSelector_Bounds, kFilterSelector_BS, FilterSelector::getInitList_BS(m_filterAlgo), "FilterSelectorBS",
                                                                                 ColorFilterStyle_RadioButtons.WithLabelText(false), EVShape::Ellipse, EDirection::Vertical /*, 5.f*/));

    //  ██████  ██    ██ ███████ ██████  ███████  █████  ███    ███ ██████  ██
    // ██    ██ ██    ██ ██      ██   ██ ██      ██   ██ ████  ████ ██   ██ ██
    // ██    ██ ██    ██ █████   ██████  ███████ ███████ ██ ████ ██ ██████  ██
    // ██    ██  ██  ██  ██      ██   ██      ██ ██   ██ ██  ██  ██ ██      ██
    //  ██████    ████   ███████ ██   ██ ███████ ██   ██ ██      ██ ██      ███████

    pGraphics->AttachControl(new IVRadioButtonControl(m_ButtonsPanel.GetGridCell(1, 4, rows, columns_BP).GetFromTop(75).GetMidHPadded(buttonsPadding), kOverSampling, {"None", "2x", "4x", "8x", "16x"},
                                                      "FilterOversampling", ColorFilterStyle_RadioButtons.WithColor(kPR, Colors::PREPROCESSOR_VIOLET).WithLabelText(false), EVShape::Ellipse,
                                                      EDirection::Vertical));


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
      (new IVKnobControl(m_FilterPanel.GetGridCell(1, 5, rows, columns_BP).GetFromTop(50).GetMidHPadded(24 * 0.5).GetTranslated(70, 0), kDryWet, "Dry/Wet",
                         ColorFilterStyle_Knob
                           .WithGradient(kFG, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                             {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                           .WithGradient(kPR, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                             {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                           .WithGradient(kX1, Colors::PLATINUM),
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
  const bool g_Bypass = GetParam(kBypass)->Value();

  if (g_Bypass)
    mOverSampler.ProcessBlock(inputs, outputs, nFrames, nChans, nChans, [&](sample** inputs, sample** outputs, int nFrames) {
      for (int s = 0; s < nFrames; s++)
      {
        m_filterAlgo = GetParam(kFilterAlgo)->Value();
        // Filter
        const bool spectralFilter = GetParam(kSpectralFilterOn)->Value();
        const bool spectralFilter_IR = GetParam(kSpectralFilter_IR)->Value();
        const bool spectralFilter_Softer = GetParam(kSpectralFilter_Harder)->Value();
        const double filterBypass = GetParam(kFilterBypass)->Value();
        const int filterSelector = GetParam(kFilterSelector)->Value();
        const int filterType = GetParam(kFilterType)->Value();
        const bool spectralShaper_IR = GetParam(kSpectralShaper_IR)->Value();
        const int spectralShaperSelector = GetParam(kSpectralShaperSelector)->Value();

        // int filterFIR_Q = GetParam(kFilterFIR_Q)->Value() + GetModulatedParamOffset(kFilterFIR_Q);
        // if (GetParam(kFilterFIR_Q_Plus1)->Value())
        //   filterFIR_Q -= 1;
        // int shaperFIR_Q = GetParam(kShaperFIR_Q)->Value();
        // if (GetParam(kShaperFIR_Q_Plus1)->Value())
        //   shaperFIR_Q -= 1;
        int filterFIR_Q = Map01To2_200_Stepped(getFinalParamValue(kFilterFIR_Q), GetParam(kFilterFIR_Q_Plus1)->Value());
        int shaperFIR_Q = Map01To2_200_Stepped(getFinalParamValue(kShaperFIR_Q), GetParam(kShaperFIR_Q_Plus1)->Value());
        // int filterIIR_Q = GetParam(kFilterIIR_Q)->Value();
        int filterIIR_Q = Map01To1_40_Stepped(getFinalParamValue(kFilterIIR_Q));
        // int shaperIIR_Q = GetParam(kShaperIIR_Q)->Value();
        int shaperIIR_Q = Map01To1_40_Stepped(getFinalParamValue(kShaperIIR_Q));
        const int oversamplingFactor = GetParam(kOverSampling)->Value();

        m_ovrsmpFactor = oversamplingFactor;

        const double dryWet = getFinalParamValue(kDryWet);
        // gain
        double gain = mapGain(getFinalParamValue(kGain));

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

        BatchSetModData(kDryWet, kGain, kFilterCutoff, kFilterResonance, kFilterBandwidth, kSpectralFilter_Drive, kShaperDrive, kShaperShape, kShaperBias, kSpectralShaperDrive, kFilterFIR_Q,
                        kShaperFIR_Q, kFilterIIR_Q, kShaperIIR_Q);
        fParams.setFilterParameters(filterType, filterSelector, spectralFilter_IR, filterCutoff, filterResonance, filterBandwidth, shaperDrive, shaperShape, shaperBias, spectralShaperDrive,
                                    spectralShaper_IR, spectralFilterDrive, spectralFilter_Softer, spectralShaperSelector, filterFIR_Q, shaperFIR_Q, filterIIR_Q, shaperIIR_Q, oversamplingFactor,
                                    sampleRate);

        const double L = inputs[0][s];
        const double R = inputs[1][s];
        double inputL = L;
        double inputR = R;
        int selector{};
        double FilteredL = inputL;
        double FilteredR = inputR;
        if (filterBypass && !spectralFilter)
        {
          FilteredL = filterSelectorL.Process(inputL, fParams);
          FilteredR = filterSelectorR.Process(inputR, fParams);
        }
        else if (filterBypass && spectralFilter)
        {
          FilteredL = spectralFilterL.Process(inputL, fParams);
          FilteredR = spectralFilterR.Process(inputR, fParams);
        }
        // double interpolateFilterBypassL = interpolateExp(inputL, FilteredL, smoothedFilterBypass);
        // double interpolateFilterBypassR = interpolateExp(inputR, FilteredR, smoothedFilterBypass);


        FilteredL = sigmoidalShaperL.Process(FilteredL, fParams);
        FilteredR = sigmoidalShaperR.Process(FilteredR, fParams);
        if (fParams.m_spectralShaperDrive > 0)
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

        // FilteredL = filnalTanh(FilteredL, fParams);
        // FilteredR = filnalTanh(FilteredR, fParams);
        outputs[0][s] = interpolateLin(inputL, FilteredL, dryWet) * gain;
        outputs[1][s] = interpolateLin(inputR, FilteredR, dryWet) * gain;
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
  if (mFactorChanged)
  {
    mFactorChanged = false;
    mOverSampler.SetOverSampling((iplug::EFactor)m_ovrsmpFactor);
    mOverSampler.Reset(GetBlockSize());
  }
}

void ColorFilterPlugin::OnReset()
{
  for (int i = 0; i < kNumParams; ++i)
  {
    mSmoothers[i] = iplug::LogParamSmooth<double>(paramSmoothing);
  }
  mOverSampler.Reset(GetBlockSize());
  CalculateLatency();
  SetLatency(m_OS_LatencySamples /* + m_RB_LatencySamples*/);
  DecideOnReset();
}
void ColorFilterPlugin::OnParamChange(int paramIdx, EParamSource, int sampleOffset)
{
  switch (paramIdx)
  {
  case kBypass:
    CalculateLatency();
    SetLatency(m_OS_LatencySamples);
    break;
  case kFilterBypass:
    DecideControlDisableStatus((GetParam(kFilterBypass)->Value() < 0.5), mCutoff_Knob, mSpectralFilterOnToggle, mSpectralFilter_IR, mSpectralFilter_Harder, /*mFilterFirQ_Odd,*/ mFilterFirQ,
                               mFilterFirQ_Plus1, mFilterIirQ, mSpectralFilterDrive);
    DecideControlDisableStatus(
      (GetParam(kFilterBypass)->Value() < 0.5) || (GetParam(kFilterType)->Value() == (int)FilterType::LP) || (GetParam(kFilterType)->Value() == (int)FilterType::HP), mF_BW_Knob);
    DecideControlDisableStatus((GetParam(kFilterSelector)->Value() == (int)FilterTypes::DF1_1P) || (GetParam(kFilterBypass)->Value() < 0.5), mReso_Knob);
    break;
  case kShaperDrive:
    DecideControlDisableStatus(GetParam(kShaperDrive)->Value() == 0, mShaperShape, mShaperBias);
    break;
  case kSpectralFilterOn:
    DecideControlHideStatus(GetParam(kSpectralFilterOn)->Value() < 0.5, mSpectralFilter_IR, mSpectralFilter_Harder);
    DecideControlHideStatus((GetParam(kSpectralFilterOn)->Value() < 0.5), mSpectralFilterDrive);

    DecideControlDisableStatus((GetParam(kFilterSelector)->Value() == (int)FilterTypes::DF1_1P) && (GetParam(kFilterType)->Value() != (int)FilterType::BS), mReso_Knob);
    DecideControlHideStatus(!(GetParam(kSpectralFilter_IR)->Value()) || (GetParam(kSpectralFilterOn)->Value() < 0.5), mFilterIirQ);
    DecideControlHideStatus((GetParam(kSpectralFilter_IR)->Value()) || (GetParam(kSpectralFilterOn)->Value() < 0.5), mFilterFirQ_Plus1, mFilterFirQ);
    break;
  case kFilterType:
    DecideControlDisableStatus((GetParam(kFilterType)->Value() == (int)FilterType::LP) || (GetParam(kFilterType)->Value() == (int)FilterType::HP), mF_BW_Knob);
    DecideControlDisableStatus((GetParam(kFilterSelector)->Value() == (int)FilterTypes::DF1_1P) && (GetParam(kFilterType)->Value() != (int)FilterType::BS), mReso_Knob);

    DecideControlHideStatus((GetParam(kFilterType)->Value() != (int)FilterType::BS), mFilterSelectorSwitch_BS);
    DecideControlHideStatus((GetParam(kFilterType)->Value() == (int)FilterType::BS), mFilterSelectorSwitch);
    break;

  case kFilterSelector:

    m_filterAlgo = GetParam(kFilterAlgo)->Int();
    if (m_filterAlgo == (int)FilterAlgo::DF1)
      m_df1retainer = GetParam(kFilterSelector)->Int();
    else if (m_filterAlgo == (int)FilterAlgo::DF2)
      m_df2retainer = GetParam(kFilterSelector)->Int();
    else if (m_filterAlgo == (int)FilterAlgo::SVF1)
      m_svf1retainer = GetParam(kFilterSelector)->Int();
    DecideControlDisableStatus((GetParam(kFilterSelector)->Value() == (int)FilterTypes::DF1_1P), mReso_Knob);
    break;
  case kFilterAlgo: {
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

    RemoveControl(kFilterSelector);

    GetParam(kFilterSelector)->InitInt("FilterSelector", default_value, min_value, max_value, "", IParam::kFlagStepped, "");

    if (GetUI())
    {
      GetUI()->AttachControl(new IVRadioButtonControl(m_ButtonsPanel.GetGridCell(1, 3, rows, columns_BP).GetFromTop(75).GetMidHPadded(buttonsPadding), kFilterSelector,
                                                      FilterSelector::getInitList(m_filterAlgo), "FilterSelector", ColorFilterStyle_RadioButtons.WithLabelText(false), EVShape::Ellipse,
                                                      EDirection::Vertical));
      SendParameterValueFromDelegate(kFilterSelector, default_value, 0);
    }


    int min_value_BS{};
    int max_value_BS{};
    double default_value_BS{};
    if (m_filterAlgo == (int)FilterAlgo::DF1)
    {
      default_value_BS = m_df1retainer_BS;
      min_value_BS = 0;
      max_value_BS = 3;
    }
    else if (m_filterAlgo == (int)FilterAlgo::DF2)
    {
      default_value_BS = m_df2retainer_BS;
      min_value_BS = 4;
      max_value_BS = 5;
    }
    else if (m_filterAlgo == (int)FilterAlgo::SVF1)
    {
      default_value_BS = m_svf1retainer_BS;
      min_value_BS = 6;
      max_value_BS = 8;
    }
    RemoveControl(kFilterSelector_BS);

    GetParam(kFilterSelector_BS)->InitInt("FilterSelector", default_value_BS, min_value_BS, max_value_BS, "", IParam::kFlagStepped, "");
    if (GetUI())
    {
      GetUI()->AttachControl(new IVRadioButtonControl(m_ButtonsPanel.GetGridCell(1, 3, rows, columns_BP).GetFromTop(75).GetMidHPadded(buttonsPadding), kFilterSelector_BS,
                                                      FilterSelector::getInitList_BS(m_filterAlgo), "FilterSelector_BS", ColorFilterStyle_RadioButtons.WithLabelText(false), EVShape::Ellipse,
                                                      EDirection::Vertical));
      SendParameterValueFromDelegate(kFilterSelector_BS, default_value_BS, 0);
    }

    DecideControlDisableStatus(((GetParam(kFilterSelector)->Value() == (int)FilterTypes::DF1_1P) && (GetParam(kFilterType)->Value() != (int)FilterType::BS)), mReso_Knob);

    DecideControlHideStatus((GetParam(kFilterType)->Value() != (int)FilterType::BS), mFilterSelectorSwitch_BS);
    DecideControlHideStatus((GetParam(kFilterType)->Value() == (int)FilterType::BS), mFilterSelectorSwitch);
  }

  break;
  case kSpectralFilter_IR:
    DecideControlHideStatus(!(GetParam(kSpectralFilter_IR)->Value()) || (GetParam(kSpectralFilterOn)->Value() < 0.5), mFilterIirQ);
    DecideControlHideStatus((GetParam(kSpectralFilter_IR)->Value()) || (GetParam(kSpectralFilterOn)->Value() < 0.5), mFilterFirQ_Plus1, mFilterFirQ);
    break;
  case kFilterFIR_Q_Plus1:
    break;
  case kSpectralShaperDrive:
    DecideControlDisableStatus(GetParam(kSpectralShaperDrive)->Value() == 0, /*mShaperFirQ_Odd,*/ mShaperFirQ, mShaperFirQ_Plus1, mShaperIirQ, mSpectralShaper_IR, mSpectralShaperSelector);
    break;
  case kSpectralShaper_IR:
    DecideControlHideStatus((GetParam(kSpectralShaper_IR)->Value()), mShaperFirQ_Plus1);

    DecideControlHideStatus(!(GetParam(kSpectralShaper_IR)->Value()), mShaperIirQ);
    break;
  case kShaperFIR_Q_Plus1: {
    break;
  case kOverSampling:
    mFactorChanged = true;
    CalculateLatency();
    SetLatency(m_OS_LatencySamples);
    break;
  }
  }
}
void ColorFilterPlugin::DecideOnReset()
{
  DecideControlDisableStatus((GetParam(kFilterSelector)->Value() == (int)FilterTypes::DF1_1P) || (GetParam(kFilterBypass)->Value() < 0.5), mReso_Knob);

  DecideControlDisableStatus(GetParam(kFilterBypass)->Value() < 0.5, mCutoff_Knob, mSpectralFilterOnToggle, mSpectralFilter_IR, /*mFilterFirQ_Odd, */ mFilterFirQ, mFilterFirQ_Plus1, mFilterIirQ,
                             mSpectralFilter_Harder, mFilterSelectorSwitch_BS, mFilterAlgoSwitch, mFilterSelectorSwitch, mSpectralFilterDrive);

  DecideControlDisableStatus(
    (GetParam(kFilterType)->Value() == (int)FilterType::LP) || (GetParam(kFilterType)->Value() == (int)FilterType::HP) || (GetParam(kFilterBypass)->Value() < 0.5), mF_BW_Knob);

  DecideControlDisableStatus(GetParam(kShaperDrive)->Value() == 0, mShaperShape, mShaperBias);
  DecideControlDisableStatus(GetParam(kSpectralShaperDrive)->Value() == 0, /*mShaperFirQ_Odd,*/ mShaperFirQ, mShaperFirQ_Plus1, mShaperIirQ, mSpectralShaper_IR, mSpectralShaperSelector);


  DecideControlHideStatus((GetParam(kFilterType)->Value() != (int)FilterType::BS), mFilterSelectorSwitch_BS);
  DecideControlHideStatus((GetParam(kFilterType)->Value() == (int)FilterType::BS), mFilterSelectorSwitch);

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
void ColorFilterPlugin::DefineSelector(int selector) const
{
  if (m_filterAlgo == (int)FilterAlgo::DF1)
    selector = m_df1retainer;
  else if (m_filterAlgo == (int)FilterAlgo::DF2)
    selector = m_df2retainer;
  else if (m_filterAlgo == (int)FilterAlgo::SVF1)
    selector = m_svf1retainer;
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
