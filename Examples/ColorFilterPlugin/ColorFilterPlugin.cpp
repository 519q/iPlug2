#include "ColorFilterPlugin.h"
#include "IPlugConstants.h"
#include "IPlug_include_in_plug_src.h"
ColorFilterPlugin::ColorFilterPlugin(const InstanceInfo& info)
  : iplug::Plugin(info, MakeConfig(kNumParams, kNumPresets))
{

  GetParam(kGain)->InitDouble("PostGain", 100., 0., 200.0, 0.01, "%");

  GetParam(kFilterCutoff)->InitDouble("Cutoff", 100., 0., 100.0, 0.01, "%");
  GetParam(kFilterResonance)->InitDouble("Resonance", 0., 0., 100.0, 0.01, "%");
  GetParam(kFilterBandwidth)->InitDouble("Bandwidth", 50., 0., 100.0, 0.01, "%");
  GetParam(kFilterSelector)->InitInt("FilterSelector", 0, 0, (int)FilterTypes::MAX_FILTER_TYPES - 1, "", IParam::kFlagStepped, "");
  GetParam(kFilterType)->InitInt("FilterType", 0, 0, 3, "", IParam::kFlagStepped, "");
  GetParam(kFilterAlgo)->InitInt("FilterAlgo", 0, 0, 2, "FilterAlgo", IParam::kFlagStepped, "");
  GetParam(kSpectralFilterOn)->InitBool("Spectral Toggle", false, "", IParam::kFlagStepped, "", "Off", "On");
  GetParam(kSpectralFilter_IR)->InitBool("SpectralFilterIR", false, "", IParam::kFlagStepped, "", "FIR", "IIR");
  GetParam(kSpectralFilterAlgo)->InitInt("SpectralFilterAlgo", 0, 0, 1, "FilterAlgo", IParam::kFlagStepped, "");
  GetParam(kSpectralFilterSelector)->InitInt("SpectralFilterSelector", 0, 0, (int)SpectralFilterTypes::MAX_SPECTRAL_FILTER_TYPES - 1, "", IParam::kFlagStepped, "");
  GetParam(kFilterBypass)->InitBool("FilterBypass", false, "", IParam::kFlagStepped, "", "Off", "On");

  GetParam(kShaperDrive)->InitDouble("ShaperDrive", 0., 0., 100.0, 0.01, "%");
  GetParam(kShaperShape)->InitDouble("ShaperShape", 0., 0., 100.0, 0.01, "%");
  GetParam(kShaperBias)->InitDouble("ShaperBias", 0., 0., 100.0, 0.01, "%");

  GetParam(kSpectralShaperShape)->InitDouble("SpectralShape", 0., 0., 100.0, 0.01, "%");
  GetParam(kSpectralShaper_IR)->InitBool("SpectralShaperIR", false, "", IParam::kFlagStepped, "", "FIR", "IIR");
  GetParam(kSpectralShaperSelector)->InitInt("ShaperSelector", 0, 0, (int)ShaperTypes::MAX_SHAPER_TYPES - 1, "", IParam::kFlagStepped, "");

  GetParam(kOverSampling)->InitInt("Oversampler", 0, 0, 4, "OverSampler", IParam::kFlagStepped, "");

#if IPLUG_EDITOR // http://bit.ly/2S64BDd
  mMakeGraphicsFunc = [&]() { return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT)); };
  mLayoutFunc = [&](IGraphics* pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(Colors::FG_DIM);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);

    const IRECT b = pGraphics->GetBounds();
    m_FilterPanel = b.GetFromLeft(425.f).GetFromTop(60.f);
    m_ShaperPanel = b.GetFromLeft(425.f).GetFromTop(150.f);
    m_ButtonsPanel = b.GetFromLeft(425.f).GetFromTop(235.f);

    double topSpaceSize = 40;
    double filterPanelSize = 100;
    double shaperPanelSize = 80;
    double buttonsPanelSize = 100;
    double panelRoundness{3};

    // ██████   ██████  ██    ██ ███    ██ ██████  ███████ ██████      ██████   █████  ███    ██ ███████ ██      ███████
    // ██   ██ ██    ██ ██    ██ ████   ██ ██   ██ ██      ██   ██     ██   ██ ██   ██ ████   ██ ██      ██      ██
    // ██████  ██    ██ ██    ██ ██ ██  ██ ██   ██ █████   ██   ██     ██████  ███████ ██ ██  ██ █████   ██      ███████
    // ██   ██ ██    ██ ██    ██ ██  ██ ██ ██   ██ ██      ██   ██     ██      ██   ██ ██  ██ ██ ██      ██           ██
    // ██   ██  ██████   ██████  ██   ████ ██████  ███████ ██████      ██      ██   ██ ██   ████ ███████ ███████ ███████
    // One color panel
    pGraphics->AttachControl(new RoundedPanelControl(b.GetFromTop(PLUG_HEIGHT).GetFromLeft(PLUG_WIDTH), Colors::BW_LIGHT_BG, panelRoundness));
    pGraphics->AttachControl(new RoundedPanelControl(m_FilterPanel.GetFromTop(filterPanelSize).GetFromLeft(450.f).GetTranslated(75, topSpaceSize), Colors::BW_DARK_BG, panelRoundness));
    pGraphics->AttachControl(
      new RoundedPanelControl(m_FilterPanel.GetFromTop(shaperPanelSize).GetFromLeft(450.f).GetTranslated(75, topSpaceSize + filterPanelSize + 5), Colors::BW_DARK_BG, panelRoundness));
    pGraphics->AttachControl(new RoundedPanelControl(
      m_FilterPanel.GetFromTop(buttonsPanelSize).GetFromLeft(450.f).GetTranslated(75, topSpaceSize + filterPanelSize + shaperPanelSize + 10), Colors::BW_DARK_BG, panelRoundness));
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

    IRECT mCutoffKnobBounds = m_FilterPanel.GetGridCell(1, 1, rows, columns).GetFromTop(80).GetTranslated(0, -10).GetMidHPadded(padding) /*GetFromLeft(180).*/;
    IVStyle mFilterCutoffStyle = ColorFilterStyle_Knob
                                   .WithGradient(kFG, IPattern::CreateLinearGradient(mCutoffKnobBounds.L, mCutoffKnobBounds.T, mCutoffKnobBounds.L, mCutoffKnobBounds.B,
                                                                                     {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                   .WithGradient(kPR, IPattern::CreateLinearGradient(mCutoffKnobBounds.L, mCutoffKnobBounds.T, mCutoffKnobBounds.L, mCutoffKnobBounds.B,
                                                                                     {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}));

    mCutoff_Knob =
      pGraphics->AttachControl(new IVKnobControl(mCutoffKnobBounds, kFilterCutoff, "Cutoff", mFilterCutoffStyle, false, false, -135.f, 135.f, -135.f, EDirection::Vertical, DEFAULT_GEARING, 2.3));

    mCutoff_Knob_Spectral = pGraphics->AttachControl(new IVKnobControl(
      mCutoffKnobBounds, kFilterCutoff, "Cutoff", mFilterCutoffStyle.WithGradient(kX1, Colors::ACCENT_YELLOW), false, false, -135.f, 135.f, -135.f, EDirection::Vertical, DEFAULT_GEARING, 2.3));

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
                                                                                   {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}));

    mReso_Knob = pGraphics->AttachControl(new IVKnobControl(mResoKnobBounds, kFilterResonance, "Reso", mFilterResoStyle));
    mReso_Knob_Spectral = pGraphics->AttachControl(new IVKnobControl(mResoKnobBounds, kFilterResonance, "Reso", mFilterResoStyle.WithGradient(kX1, Colors::ACCENT_YELLOW)));


    // ██████   █████  ███    ██ ██████  ██     ██ ██ ██████  ████████ ██   ██
    // ██   ██ ██   ██ ████   ██ ██   ██ ██     ██ ██ ██   ██    ██    ██   ██
    // ██████  ███████ ██ ██  ██ ██   ██ ██  █  ██ ██ ██   ██    ██    ███████
    // ██   ██ ██   ██ ██  ██ ██ ██   ██ ██ ███ ██ ██ ██   ██    ██    ██   ██
    // ██████  ██   ██ ██   ████ ██████   ███ ███  ██ ██████     ██    ██   ██
    IRECT bwBounds = m_FilterPanel.GetGridCell(1, 3, rows, columns).GetFromTop(getFromTopFilter).GetMidHPadded(padding);
    mF_BW_Knob = pGraphics->AttachControl(new IVKnobControl(
      bwBounds, kFilterBandwidth, "Band",
      ColorFilterStyle_Knob.WithGradient(kFG, IPattern::CreateLinearGradient(bwBounds.L, bwBounds.T, bwBounds.L, bwBounds.B, {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
        .WithGradient(kPR, IPattern::CreateLinearGradient(bwBounds.L, bwBounds.T, bwBounds.L, bwBounds.B, {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))));


    //  ███████ ██████  ███████ ████████ ██████   █████  ██          ████████  ██████   ██████   ██████  ██      ███████
    //  ██      ██   ██ ██         ██    ██   ██ ██   ██ ██             ██    ██    ██ ██       ██       ██      ██
    //  ███████ ██████  █████      ██    ██████  ███████ ██             ██    ██    ██ ██   ███ ██   ███ ██      █████
    //       ██ ██      ██         ██    ██   ██ ██   ██ ██             ██    ██    ██ ██    ██ ██    ██ ██      ██
    //  ███████ ██      ███████    ██    ██   ██ ██   ██ ███████        ██     ██████   ██████   ██████  ███████ ███████

    mSpectralFilterOnToggle = pGraphics->AttachControl(new IVToggleControl(m_FilterPanel.GetGridCell(1, 4, rows, columns).GetFromTop(45).GetMidHPadded(25), kSpectralFilterOn, "SpectralToggle",
                                                                           ColorFilterStyle_Knob.WithGradient(kPR, Colors::ACCENT_YELLOW).WithRoundness(0.1), "Off", "On"));

    mSpectralFilter_IR =
      pGraphics->AttachControl(new IVToggleControl(m_FilterPanel.GetGridCell(1, 4, rows, columns).GetFromTop(21 * 0.5).GetMidHPadded(9.45 * 0.5).GetTranslated(0, 50), kSpectralFilter_IR, "SpectralIR",
                                                   ColorFilterStyle_Knob.WithGradient(kPR, Colors::BW_ORANGE).WithGradient(kFG, Colors::ACCENT_YELLOW).WithRoundness(1).WithLabelText(false), "", ""));
    // ███████ ██ ██      ████████    ██████  ██    ██ ██████   █████  ███████ ███████
    // ██      ██ ██         ██       ██   ██  ██  ██  ██   ██ ██   ██ ██      ██
    // █████   ██ ██         ██       ██████    ████   ██████  ███████ ███████ ███████
    // ██      ██ ██         ██       ██   ██    ██    ██      ██   ██      ██      ██
    // ██      ██ ███████    ██    ██ ██████     ██    ██      ██   ██ ███████ ███████
    IVStyle mFilterBypassStyle = ColorFilterStyle_Knob.WithGradient(kPR, Colors::MONOKAI_TYPE_GREEN).WithRoundness(0.1);
    IRECT mFilterBypass_Bounds = m_FilterPanel.GetGridCell(1, 5, rows, columns).GetFromTop(45).GetMidHPadded(25);

    mFilterBypassSwitch = pGraphics->AttachControl(new IVToggleControl(mFilterBypass_Bounds, kFilterBypass, "Filter Bypass", mFilterBypassStyle, "Off", "On"));
    mFilterBypassSwitch_Spectral =
      pGraphics->AttachControl(new IVToggleControl(mFilterBypass_Bounds, kFilterBypass, "Filter Bypass", mFilterBypassStyle.WithGradient(kPR, Colors::ACCENT_YELLOW), "Off", "On"));

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
    IRECT shaperShapeKnobBounds = m_ShaperPanel.GetGridCell(1, 1, rows, columns).GetFromTop(getFromTopShaper).GetMidHPadded(padding);
    pGraphics->AttachControl(new IVKnobControl(shaperShapeKnobBounds, kShaperDrive, "Drive",
                                               ColorFilterStyle_Knob
                                                 .WithGradient(kFG, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                   {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                                 .WithGradient(kPR, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                   {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                                                 .WithGradient(kX1, Colors::SHAPER_COLOR)));

    // ███████ ██   ██  █████  ██████  ███████
    // ██      ██   ██ ██   ██ ██   ██ ██
    // ███████ ███████ ███████ ██████  █████
    //      ██ ██   ██ ██   ██ ██      ██
    // ███████ ██   ██ ██   ██ ██      ███████

    pGraphics->AttachControl(new IVKnobControl(m_ShaperPanel.GetGridCell(1, 2, rows, columns).GetFromTop(getFromTopShaper).GetMidHPadded(padding), kShaperShape, "Shape",
                                               ColorFilterStyle_Knob
                                                 .WithGradient(kFG, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                   {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                                 .WithGradient(kPR, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                   {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                                                 .WithGradient(kX1, Colors::SHAPER_COLOR)));

    // ██████  ██  █████  ███████
    // ██   ██ ██ ██   ██ ██
    // ██████  ██ ███████ ███████
    // ██   ██ ██ ██   ██      ██
    // ██████  ██ ██   ██ ███████

    pGraphics->AttachControl(new IVKnobControl(m_ShaperPanel.GetGridCell(1, 3, rows, columns).GetFromTop(getFromTopShaper).GetMidHPadded(padding), kShaperBias, "Bias",
                                               ColorFilterStyle_Knob
                                                 .WithGradient(kFG, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                   {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                                 .WithGradient(kPR, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                   {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                                                 .WithGradient(kX1, Colors::SHAPER_COLOR)));

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

    pGraphics->AttachControl(new IVKnobControl(m_ShaperPanel.GetGridCell(1, 4, rows, columns).GetFromTop(getFromTopShaper).GetMidHPadded(padding), kSpectralShaperShape, "Spectral Shape",
                                               ColorFilterStyle_Knob
                                                 .WithGradient(kFG, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                   {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                                 .WithGradient(kPR, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                   {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                                                 .WithGradient(kX1, Colors::JET_BRAINS_PINK)));
    //  ██ ██████
    //  ██ ██   ██
    //  ██ ██████
    //  ██ ██   ██
    //  ██ ██   ██

    pGraphics->AttachControl(
      new IVToggleControl(m_ShaperPanel.GetGridCell(1, 4, rows, columns).GetFromTop(21 * 0.5).GetMidHPadded(9.45 * 0.5).GetTranslated(0, 59), kSpectralShaper_IR, "SpectralShaperIR",
                          ColorFilterStyle_Knob.WithGradient(kPR, Colors::BURNT_SIENNA).WithGradient(kFG, Colors::JET_BRAINS_PINK).WithRoundness(1).WithLabelText(false), "", ""));

    //  ███████ ██   ██  █████  ██████  ██████      ███████ ███████ ██       ██████ ████████ ██████
    //  ██      ██   ██ ██   ██ ██   ██ ██   ██     ██      ██      ██      ██         ██    ██   ██
    //  ███████ ███████ ███████ ██████  ██████      ███████ █████   ██      ██         ██    ██████
    //       ██ ██   ██ ██   ██ ██      ██   ██          ██ ██      ██      ██         ██    ██   ██
    //  ███████ ██   ██ ██   ██ ██      ██   ██     ███████ ███████ ███████  ██████    ██    ██   ██
    double ssKnobScale = 0.6;
    IRECT ssKnobBounds = m_ShaperPanel.GetGridCell(1, 4, rows, columns).GetFromTop(21 * ssKnobScale).GetMidHPadded(9.45 * ssKnobScale).GetTranslated(17, 58.5);
    IVKnobControl* shaperSelector = new IVKnobControl(
      ssKnobBounds, kSpectralShaperSelector, "SpectralShaperSelector",
      ColorFilterStyle_Knob.WithLabelText(false)
        .WithGradient(kFG, IPattern::CreateLinearGradient(ssKnobBounds.L, ssKnobBounds.T, ssKnobBounds.L, ssKnobBounds.B, {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
        .WithGradient(
          kPR, IPattern::CreateLinearGradient(ssKnobBounds.L, ssKnobBounds.T, ssKnobBounds.L, ssKnobBounds.B, {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
        .WithGradient(kX1, Colors::JET_BRAINS_PINK),
      false, false, -135, 135, -135, EDirection::Vertical, 6, 1.);
    shaperSelector->SetPointerThickness(1.2);
    pGraphics->AttachControl(shaperSelector);
    // shaperSelector->SetAnimation();

    // ██████   ██████  ███████ ████████      ██████   █████  ██ ███    ██
    // ██   ██ ██    ██ ██         ██        ██       ██   ██ ██ ████   ██
    // ██████  ██    ██ ███████    ██        ██   ███ ███████ ██ ██ ██  ██
    // ██      ██    ██      ██    ██        ██    ██ ██   ██ ██ ██  ██ ██
    // ██       ██████  ███████    ██         ██████  ██   ██ ██ ██   ████

    pGraphics->AttachControl(new IVKnobControl(m_ShaperPanel.GetGridCell(1, 5, rows, columns).GetFromTop(getFromTopShaper).GetMidHPadded(padding), kGain, "Gain",
                                               ColorFilterStyle_Knob
                                                 .WithGradient(kFG, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                   {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                                                 .WithGradient(kPR, IPattern::CreateLinearGradient(shaperShapeKnobBounds.L, shaperShapeKnobBounds.T, shaperShapeKnobBounds.L, shaperShapeKnobBounds.B,
                                                                                                   {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))
                                                 .WithGradient(kX1, Colors::PLATINUM)));


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
    IRECT mFilter_Type_RB_Bounds = m_ButtonsPanel.GetGridCell(1, 1, rows, columns).GetFromTop(75).GetMidHPadded(buttonsPadding);
    mFilter_Type_RB = pGraphics->AttachControl(
      new IVRadioButtonControl(mFilter_Type_RB_Bounds, kFilterType, {"LP", "BP", "BS", "HP"}, "FilterType", ColorFilterStyle_RadioButtons, EVShape::Ellipse, EDirection::Vertical));

    //  █████  ██       ██████   ██████
    // ██   ██ ██      ██       ██    ██
    // ███████ ██      ██   ███ ██    ██
    // ██   ██ ██      ██    ██ ██    ██
    // ██   ██ ███████  ██████   ██████
    // normal algo
    IRECT mFilterAlgo_Bounds = m_ButtonsPanel.GetGridCell(1, 2, rows, columns).GetFromTop(75).GetMidHPadded(buttonsPadding);
    mFilterAlgoSwitch = GetUI()->AttachControl(new IVRadioButtonControl(
      mFilterAlgo_Bounds, kFilterAlgo, {"DF1", "DF2", "SVF1"}, "FilterAlgo", ColorFilterStyle_RadioButtons.WithColor(kPR, Colors::MONOKAI_TYPE_GREEN), EVShape::Ellipse, EDirection::Vertical));

    // spectral algo

    mSpectral_FilterAlgoSwitch = GetUI()->AttachControl(new IVRadioButtonControl(
      mFilterAlgo_Bounds, kSpectralFilterAlgo, {"DF1", "SVF1"}, "SpectralFilterAlgo", ColorFilterStyle_RadioButtons.WithColor(kPR, Colors::ACCENT_YELLOW), EVShape::Ellipse, EDirection::Vertical));


    // ███████ ██      ████████ ██████      ███████ ██       ██████ ████████ ██████      ██ ███    ██ ██ ████████
    // ██      ██         ██    ██   ██     ██      ██      ██         ██    ██   ██     ██ ████   ██ ██    ██
    // █████   ██         ██    ██████      ███████ ██      ██         ██    ██████      ██ ██ ██  ██ ██    ██
    // ██      ██         ██    ██   ██          ██ ██      ██         ██    ██   ██     ██ ██  ██ ██ ██    ██
    // ██      ███████    ██    ██   ██     ███████ ███████  ██████    ██    ██   ██     ██ ██   ████ ██    ██


    IRECT mFilterSelector_Bounds = m_ButtonsPanel.GetGridCell(1, 3, rows, columns).GetFromTop(75).GetMidHPadded(buttonsPadding);
    mFilterSelectorSwitch = pGraphics->AttachControl(new IVRadioButtonControl(
      mFilterSelector_Bounds, kFilterSelector, FilterSelector::getInitList(m_filterAlgo), "FilterSelector", ColorFilterStyle_RadioButtons, EVShape::Ellipse, EDirection::Vertical /*, 5.f*/));

    mSpectral_FilterSelectorSwitch = pGraphics->AttachControl(new IVRadioButtonControl(
      m_ButtonsPanel.GetGridCell(1, 3, rows, columns).GetFromTop(75).GetMidHPadded(buttonsPadding), kSpectralFilterSelector, FilterSelector::getInitListSpectral(m_spectral_FilterAlgo),
      "SpectralFilterSelector", ColorFilterStyle_RadioButtons.WithColor(kPR, Colors::ACCENT_YELLOW), EVShape::Ellipse, EDirection::Vertical /*, 5.f*/));


    //  ██████  ██    ██ ███████ ██████  ███████  █████  ███    ███ ██████  ██
    // ██    ██ ██    ██ ██      ██   ██ ██      ██   ██ ████  ████ ██   ██ ██
    // ██    ██ ██    ██ █████   ██████  ███████ ███████ ██ ████ ██ ██████  ██
    // ██    ██  ██  ██  ██      ██   ██      ██ ██   ██ ██  ██  ██ ██      ██
    //  ██████    ████   ███████ ██   ██ ███████ ██   ██ ██      ██ ██      ███████

    pGraphics->AttachControl(new IVRadioButtonControl(m_ButtonsPanel.GetGridCell(1, 5, rows, columns).GetFromTop(75).GetMidHPadded(buttonsPadding), kOverSampling, {"None", "2x", "4x", "8x", "16x"},
                                                      "FilterOversampling", ColorFilterStyle_RadioButtons, EVShape::Ellipse, EDirection::Vertical));

    DecideControlDisableStatus(GetParam(kFilterSelector)->Value() == (int)FilterTypes::DF1_1P, mReso_Knob);
    DecideControlDisableStatus(GetParam(kSpectralFilterSelector)->Value() == (int)SpectralFilterTypes::DF1_1P, mReso_Knob_Spectral);

    DecideControlDisableStatus(GetParam(kFilterBypass)->Value() < 0.5, mCutoff_Knob_Spectral, mReso_Knob_Spectral, mReso_Knob, mCutoff_Knob, mSpectralFilterOnToggle, mSpectralFilter_IR);

    DecideControlDisableStatus((GetParam(kFilterType)->Value() == (int)FilterType::LP) || (GetParam(kFilterType)->Value() == (int)FilterType::HP), mF_BW_Knob);

    DecideControlHideStatus(GetParam(kSpectralFilterOn)->Value() > 0.5, mCutoff_Knob, mFilterSelectorSwitch, mFilterAlgoSwitch, mFilterBypassSwitch, mF_BW_Knob, mReso_Knob);
    DecideControlHideStatus(GetParam(kSpectralFilterOn)->Value() < 0.5, mCutoff_Knob_Spectral, mSpectral_FilterSelectorSwitch, mSpectral_FilterAlgoSwitch, mFilterBypassSwitch_Spectral,
                            mSpectralFilter_IR, mReso_Knob_Spectral);
    DecideControlDisableStatus(GetParam(kSpectralFilterOn)->Value() > 0.5, mFilter_Type_RB);
    m_filterAlgo = GetParam(kFilterAlgo)->Int();
    // int selector = GetParam(kFilterSelector)->Int();
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
      m_filterAlgo = GetParam(kFilterAlgo)->Value();
      m_spectral_FilterAlgo = GetParam(kSpectralFilterAlgo)->Value();
      // gain
      const double gain = GetParam(kGain)->Value() / 100.;
      // Filter
      const double filterCutoff = GetParam(kFilterCutoff)->Value() / 100.0;
      const double filterReso = GetParam(kFilterResonance)->Value() / 100.0;
      const double filterBadwidth = GetParam(kFilterBandwidth)->Value() / 100.0;
      const bool spectralFilter = GetParam(kSpectralFilterOn)->Value();
      const bool spectralFilter_IR = GetParam(kSpectralFilter_IR)->Value();
      const double filterBypass = GetParam(kFilterBypass)->Value();
      const int filterSelector = GetParam(kFilterSelector)->Value();
      const int spectralFilterSelector = GetParam(kSpectralFilterSelector)->Value();

      const int filterType = GetParam(kFilterType)->Value();
      //  shaper
      const double shaperDrive = GetParam(kShaperDrive)->Value() / 100.;
      const double shaperShape = GetParam(kShaperShape)->Value() / 100.;
      const double shaperAsym = GetParam(kShaperBias)->Value() / 100.;

      const double spectralShaperShape = GetParam(kSpectralShaperShape)->Value() / 100.;
      const bool spectralShaper_IR = GetParam(kSpectralShaper_IR)->Value();
      const int spectralShaperSelector = GetParam(kSpectralShaperSelector)->Value();

      const int oversamplingFactor = GetParam(kOverSampling)->Value();

      m_ovrsmpFactor = oversamplingFactor;

      // gain
      const double smoothedGain = mGainSmooth.Process(gain);
      // filter
      const double smoothedFilterCutoff = mFilterCutoffSmooth.Process(filterCutoff);
      const double smoothedFilterResonance = mFilterResonanceSmooth.Process(filterReso);
      // const double smoothedFilterBypass = mFilterBypassSmooth.Process(filterBypass);
      const double smoothedFilterBandwidth = mFilterBandwidthSmooth.Process(filterBadwidth);
      // const double smoothedFilterSelector = mFilterSelectorSmooth.Process(filterSelector);
      //  shaper
      const double smoothedShaperDrive = mShaperDriveSmooth.Process(shaperDrive);
      const double smoothedShaperShape = mShaperShapeSmooth.Process(shaperShape);
      const double smoothedShaperBias = mShaperBiasSmooth.Process(shaperAsym);
      // const double smoothedShaperBypass = mShaperBypassSmooth.Process(shaperBypassMix);

      const double smoothedSpectralShaperShape = mSpectralShaperShapeSmooth.Process(spectralShaperShape);


      fParams.setFilterParameters(filterType, filterSelector, spectralFilterSelector, spectralFilter_IR, smoothedFilterCutoff, smoothedFilterResonance, smoothedFilterBandwidth, smoothedShaperDrive,
                                  smoothedShaperShape, smoothedShaperBias, smoothedSpectralShaperShape, spectralShaper_IR, spectralShaperSelector, oversamplingFactor, sampleRate);

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
      //  mRingBufferL.write(inputL);
      //  mRingBufferR.write(inputR);
      //  mRingBufferL.readChunk();
      //  mRingBufferR.readChunk();
      //  mDomeShaperL.ProcessBuffer(mRingBufferL.getChunkBuffer(), mRingBufferL.getChunkRead(), fParams, sampleRate);
      //  mDomeShaperL.ProcessBuffer(mRingBufferL.getOverlapBuffer(), mRingBufferL.getChunkRead(), fParams, sampleRate);
      //  mDomeShaperR.ProcessBuffer(mRingBufferR.getChunkBuffer(), mRingBufferR.getChunkRead(), fParams, sampleRate);
      //  mDomeShaperR.ProcessBuffer(mRingBufferR.getOverlapBuffer(), mRingBufferR.getChunkRead(), fParams, sampleRate);
      //  mRingBufferL.writeChunk();
      //  mRingBufferR.writeChunk();
      if (fParams.m_spectralShaperShape > 0.001)
      {
        mSpectralShaperL.Process(FilteredL, fParams);
        mSpectralShaperR.Process(FilteredR, fParams);
      }
      FilteredL = filnalTanh(FilteredL, fParams);
      FilteredR = filnalTanh(FilteredR, fParams);
      outputs[0][s] = /*mRingBufferL.read()*/ FilteredL * smoothedGain;
      outputs[1][s] = /*mRingBufferR.read()*/ FilteredR * smoothedGain;
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
  // SendParameterValueFromDelegate(kFilterSelector, retained_kFilterSelector, 0);
  //  int latencySamples = 2 * mRingBufferL.getChunkSize() - mRingBufferL.getHopSize();
  //  SetLatency(latencySamples);
}
void ColorFilterPlugin::OnParamChange(int paramIdx, EParamSource, int sampleOffset)
{
  switch (paramIdx)
  {
  // case kSpectralShaper_IR:
  //   //DebugPrint(GetParam(kSpectralShaper_IR)->Value());
  //   break;
  case kFilterBypass:
    DecideControlDisableStatus((GetParam(kFilterBypass)->Value() < 0.5), mCutoff_Knob_Spectral, mCutoff_Knob, mReso_Knob, mReso_Knob_Spectral, mSpectralFilterOnToggle, mF_BW_Knob, mSpectralFilter_IR);
    break;
  case kSpectralFilterOn:
    DecideControlHideStatus(GetParam(kSpectralFilterOn)->Value() < 0.5, mCutoff_Knob_Spectral, mSpectral_FilterSelectorSwitch, mFilterBypassSwitch_Spectral, mSpectral_FilterAlgoSwitch,
                            mSpectral_FilterSelectorSwitch, mSpectralFilter_IR);
    DecideControlHideStatus(GetParam(kSpectralFilterOn)->Value() > 0.5, mFilterSelectorSwitch, mFilterAlgoSwitch, mCutoff_Knob, mFilterBypassSwitch, mFilterSelectorSwitch);
    DecideControlDisableStatus(GetParam(kSpectralFilterOn)->Value() > 0.5, mFilter_Type_RB);

    DecideControlHideStatus(GetParam(kSpectralFilterOn)->Value() > 0.5, mReso_Knob);
    DecideControlDisableStatus(GetParam(kFilterSelector)->Value() == (int)FilterTypes::DF1_1P, mReso_Knob);
    DecideControlHideStatus(GetParam(kSpectralFilterOn)->Value() < 0.5, mReso_Knob_Spectral);
    DecideControlDisableStatus(GetParam(kSpectralFilterSelector)->Value() == (int)SpectralFilterTypes::DF1_1P, mReso_Knob_Spectral);

    if (GetUI())
      if (GetParam(kSpectralFilterOn)->Value() > 0.5)
        mF_BW_Knob->Hide(true);
      else if (GetParam(kSpectralFilterOn)->Value() < 0.5)
        mF_BW_Knob->Hide(false);
    break;
  case kFilterType:
    // DefineSelector(retained_kFilterSelector);
    DecideControlDisableStatus((GetParam(kFilterType)->Value() == (int)FilterType::LP) || (GetParam(kFilterType)->Value() == (int)FilterType::HP), mF_BW_Knob);
    break;
  case kOverSampling:
    mFactorChanged = true;
    break;
  case kFilterSelector:
    m_filterAlgo = GetParam(kFilterAlgo)->Int();
    if (m_filterAlgo == (int)FilterAlgo::DF1)
      m_df1retainer = GetParam(kFilterSelector)->Int();
    else if (m_filterAlgo == (int)FilterAlgo::DF2)
      m_df2retainer = GetParam(kFilterSelector)->Int();
    else if (m_filterAlgo == (int)FilterAlgo::SVF1)
      m_svf1retainer = GetParam(kFilterSelector)->Int();
    DecideControlDisableStatus(GetParam(kFilterSelector)->Value() == (int)FilterTypes::DF1_1P, mReso_Knob);
    break;
  case kSpectralFilterSelector:
    m_spectral_FilterAlgo = GetParam(kSpectralFilterAlgo)->Int();
    if (m_spectral_FilterAlgo == (int)SpectralFilterAlgo::DF1)
      m_spectral_df1retainer = GetParam(kSpectralFilterSelector)->Int();
    else if (m_spectral_FilterAlgo == (int)SpectralFilterAlgo::SVF1)
      m_spectral_svf1retainer = GetParam(kSpectralFilterSelector)->Int();
    // DecideControlHideStatus(GetParam(kSpectralFilterOn)->Value() < 0.5, mReso_Knob_Spectral);
    DecideControlDisableStatus(GetParam(kSpectralFilterSelector)->Value() == (int)SpectralFilterTypes::DF1_1P, mReso_Knob_Spectral);
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

    GetParam(kFilterSelector)->InitInt("FilterSelector", default_value, min_value, max_value, "", IParam::kFlagStepped, "");
    RemoveControl(kFilterSelector);

    if (GetUI())
    {
      GetUI()->AttachControl(new IVRadioButtonControl(m_ButtonsPanel.GetGridCell(1, 3, rows, columns).GetFromTop(75).GetMidHPadded(padding), kFilterSelector, FilterSelector::getInitList(m_filterAlgo),
                                                      "FilterSelector", ColorFilterStyle_RadioButtons, EVShape::Ellipse, EDirection::Vertical));
      SendParameterValueFromDelegate(kFilterSelector, default_value, 0);
    }
  }

  break;
  case kSpectralFilterAlgo: {
    m_spectral_FilterAlgo = GetParam(kSpectralFilterAlgo)->Int();
    int spec_min_value{};
    int spec_max_value{};
    double spec_default_value{};
    if (m_spectral_FilterAlgo == (int)SpectralFilterAlgo::DF1)
    {
      spec_default_value = m_spectral_df1retainer;
      spec_min_value = 0;
      spec_max_value = 4;
    }
    else if (m_spectral_FilterAlgo == (int)SpectralFilterAlgo::SVF1)
    {
      spec_default_value = m_spectral_svf1retainer;
      spec_min_value = 5;
      spec_max_value = 7;
    }

    GetParam(kSpectralFilterSelector)->InitInt("SpectralFilterSelector", spec_default_value, spec_min_value, spec_max_value, "", IParam::kFlagStepped, "");
    RemoveControl(kSpectralFilterSelector);

    if (GetUI())
    {
      GetUI()->AttachControl(new IVRadioButtonControl(m_ButtonsPanel.GetGridCell(1, 3, rows, columns).GetFromTop(75).GetMidHPadded(buttonsPadding), kSpectralFilterSelector,
                                                      FilterSelector::getInitListSpectral(m_spectral_FilterAlgo), "FilterSelector", ColorFilterStyle_RadioButtons.WithColor(kPR, Colors::ACCENT_YELLOW),
                                                      EVShape::Ellipse, EDirection::Vertical));
      SendParameterValueFromDelegate(kSpectralFilterSelector, spec_default_value, 0);
    }
  }
  break;
  }
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
