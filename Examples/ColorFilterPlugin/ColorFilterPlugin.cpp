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
  GetParam(kSpectralFilterSelector)->InitInt("SpectralFilterSelector", 0, 0, (int)SpectralFilterTypes::MAX_SPECTRAL_FILTER_TYPES - 1, "", IParam::kFlagStepped, "");
  GetParam(kFilterBypass)->InitBool("FilterBypass", false, "", IParam::kFlagStepped, "", "Off", "On");

  GetParam(kShaperDrive)->InitDouble("ShaperDrive", 0., 0., 100.0, 0.01, "%");
  GetParam(kShaperShape)->InitDouble("ShaperShape", 0., 0., 100.0, 0.01, "%");
  GetParam(kShaperBias)->InitDouble("ShaperBias", 0., 0., 100.0, 0.01, "%");

  GetParam(kSpectralShaperShape)->InitDouble("SpectralShape", 0., 0., 100.0, 0.01, "%");

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

    mCutoffKnobBounds = m_FilterPanel.GetGridCell(1, 1, rows, columns).GetFromTop(80).GetTranslated(0, -10).GetMidHPadded(padding) /*GetFromLeft(180).*/;
    mFilterCutoffStyle = ColorFilterStyle_Knob
                           .WithGradient(kFG, IPattern::CreateLinearGradient(mCutoffKnobBounds.L, mCutoffKnobBounds.T, mCutoffKnobBounds.L, mCutoffKnobBounds.B,
                                                                             {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                           .WithGradient(kPR, IPattern::CreateLinearGradient(mCutoffKnobBounds.L, mCutoffKnobBounds.T, mCutoffKnobBounds.L, mCutoffKnobBounds.B,
                                                                             {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}));
    mCutoff_Knob =
      pGraphics->AttachControl(new IVKnobControl(mCutoffKnobBounds, kFilterCutoff, "Cutoff", mFilterCutoffStyle, false, false, -135.f, 135.f, -135.f, EDirection::Vertical, DEFAULT_GEARING, 2.3));

    // ██████  ███████ ███████  ██████
    // ██   ██ ██      ██      ██    ██
    // ██████  █████   ███████ ██    ██
    // ██   ██ ██           ██ ██    ██
    // ██   ██ ███████ ███████  ██████
    mResoKnobBounds = m_FilterPanel.GetGridCell(1, 2, rows, columns).GetFromTop(getFromTopFilter).GetMidHPadded(padding);
    mFilterResoStyle = ColorFilterStyle_Knob
                         .WithGradient(kFG, IPattern::CreateLinearGradient(
                                              mResoKnobBounds.L, mResoKnobBounds.T, mResoKnobBounds.L, mResoKnobBounds.B, {{Colors::BW_DARK_KNOB_TOP, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM, 1.f}}))
                         .WithGradient(kPR, IPattern::CreateLinearGradient(mResoKnobBounds.L, mResoKnobBounds.T, mResoKnobBounds.L, mResoKnobBounds.B,
                                                                           {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}));
    mReso_Knob = pGraphics->AttachControl(new IVKnobControl(mResoKnobBounds, kFilterResonance, "Reso", mFilterResoStyle));

    DecideResoStatus();

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

    DecideBandwidthStatus();

    //  ███████ ██████  ███████ ████████ ██████   █████  ██          ████████  ██████   ██████   ██████  ██      ███████
    //  ██      ██   ██ ██         ██    ██   ██ ██   ██ ██             ██    ██    ██ ██       ██       ██      ██
    //  ███████ ██████  █████      ██    ██████  ███████ ██             ██    ██    ██ ██   ███ ██   ███ ██      █████
    //       ██ ██      ██         ██    ██   ██ ██   ██ ██             ██    ██    ██ ██    ██ ██    ██ ██      ██
    //  ███████ ██      ███████    ██    ██   ██ ██   ██ ███████        ██     ██████   ██████   ██████  ███████ ███████

    pGraphics->AttachControl(new IVToggleControl(m_FilterPanel.GetGridCell(1, 4, rows, columns).GetFromTop(45).GetMidHPadded(25) /*.GetTranslated(0, 20)*/, kSpectralFilterOn, "SpectralToggle",
                                                 ColorFilterStyle_Knob.WithGradient(kPR, Colors::ACCENT_YELLOW).WithRoundness(0.1), "Off", "On"));

    // ███████ ██ ██      ████████    ██████  ██    ██ ██████   █████  ███████ ███████
    // ██      ██ ██         ██       ██   ██  ██  ██  ██   ██ ██   ██ ██      ██
    // █████   ██ ██         ██       ██████    ████   ██████  ███████ ███████ ███████
    // ██      ██ ██         ██       ██   ██    ██    ██      ██   ██      ██      ██
    // ██      ██ ███████    ██    ██ ██████     ██    ██      ██   ██ ███████ ███████
    mFilterBypassStyle = ColorFilterStyle_Knob.WithGradient(kPR, Colors::MONOKAI_TYPE_GREEN).WithRoundness(0.1);
    mFilterBypass_Bounds = m_FilterPanel.GetGridCell(1, 5, rows, columns).GetFromTop(45).GetMidHPadded(25);
    mFilterBypassSwitch = new IVToggleControl(mFilterBypass_Bounds, kFilterBypass, "Filter Bypass", mFilterBypassStyle, "Off", "On");
    pGraphics->AttachControl(mFilterBypassSwitch);

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
                                                                                                   {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))));

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
                                                                                                   {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))));

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
                                                                                                   {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))));

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
                                                                                                   {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))));

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
                                                                                                   {{Colors::BW_DARK_KNOB_TOP_HL, 0.55f}, {Colors::BW_DARK_KNOB_BOTTOM_HL, 1.f}}))));


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
    mFilter_Type_RB_Bounds = m_ButtonsPanel.GetGridCell(1, 1, rows, columns).GetFromTop(75).GetMidHPadded(padding);
    mFilter_Type_RB = new IVRadioButtonControl(mFilter_Type_RB_Bounds, kFilterType, {"LP", "BP", "BS", "HP"}, "FilterType",
                                               ColorFilterStyle_RadioButtons /*.WithGradient(kPR, Colors::BW_DARK_KNOB_TOP)*/, EVShape::Ellipse, EDirection::Vertical);
    pGraphics->AttachControl(mFilter_Type_RB);
    DecideFilterTypeStatus();

    //  █████  ██       ██████   ██████
    // ██   ██ ██      ██       ██    ██
    // ███████ ██      ██   ███ ██    ██
    // ██   ██ ██      ██    ██ ██    ██
    // ██   ██ ███████  ██████   ██████
    mFilterAlgo_Bounds = m_ButtonsPanel.GetGridCell(1, 2, rows, columns).GetFromTop(75).GetMidHPadded(padding);
    mFilterAlgoSwitch = GetUI()->AttachControl(new IVRadioButtonControl(
      mFilterAlgo_Bounds, kFilterAlgo, {"DF1", "DF2", "SVF1"}, "FilterAlgo", ColorFilterStyle_RadioButtons.WithColor(kPR, Colors::MONOKAI_TYPE_GREEN), EVShape::Ellipse, EDirection::Vertical));
    DecideFilterAlgoStatus();
    // ███████ ██      ████████ ██████      ███████ ██       ██████ ████████ ██████      ██ ███    ██ ██ ████████
    // ██      ██         ██    ██   ██     ██      ██      ██         ██    ██   ██     ██ ████   ██ ██    ██
    // █████   ██         ██    ██████      ███████ ██      ██         ██    ██████      ██ ██ ██  ██ ██    ██
    // ██      ██         ██    ██   ██          ██ ██      ██         ██    ██   ██     ██ ██  ██ ██ ██    ██
    // ██      ███████    ██    ██   ██     ███████ ███████  ██████    ██    ██   ██     ██ ██   ████ ██    ██

    pGraphics->AttachControl(new IVRadioButtonControl(m_ButtonsPanel.GetGridCell(1, 3, rows, columns).GetFromTop(75).GetMidHPadded(padding), kFilterSelector, FilterSelector::getInitList(m_filterAlgo),
                                                      "FilterSelector", ColorFilterStyle_RadioButtons, EVShape::Ellipse, EDirection::Vertical /*, 5.f*/));

    //  ███████ ██████  ███████  ██████    ███████ ██      ████████    ███████ ██       ██████ ████████ ██████
    //  ██      ██   ██ ██      ██         ██      ██         ██       ██      ██      ██         ██    ██   ██
    //  ███████ ██████  █████   ██         █████   ██         ██       ███████ ██      ██         ██    ██████
    //       ██ ██      ██      ██         ██      ██         ██            ██ ██      ██         ██    ██   ██
    //  ███████ ██      ███████  ██████ ██ ██      ███████    ██    ██ ███████ ███████  ██████    ██    ██   ██

    // pGraphics->AttachControl(new IVRadioButtonControl(m_ButtonsPanel.GetGridCell(1, 4, rows, columns).GetFromTop(75).GetMidHPadded(padding), kSpectralFilterSelector, SpectralFilterTypes,
    //                                                   "SpectralFilterSelector", ColorFilterStyle_RadioButtons, EVShape::Ellipse, EDirection::Vertical /*, 5.f*/));


    //  ██████  ██    ██ ███████ ██████  ███████  █████  ███    ███ ██████  ██
    // ██    ██ ██    ██ ██      ██   ██ ██      ██   ██ ████  ████ ██   ██ ██
    // ██    ██ ██    ██ █████   ██████  ███████ ███████ ██ ████ ██ ██████  ██
    // ██    ██  ██  ██  ██      ██   ██      ██ ██   ██ ██  ██  ██ ██      ██
    //  ██████    ████   ███████ ██   ██ ███████ ██   ██ ██      ██ ██      ███████

    pGraphics->AttachControl(new IVRadioButtonControl(m_ButtonsPanel.GetGridCell(1, 5, rows, columns).GetFromTop(75).GetMidHPadded(padding), kOverSampling, {"None", "2x", "4x", "8x", "16x"},
                                                      "FilterOversampling", ColorFilterStyle_RadioButtons, EVShape::Ellipse, EDirection::Vertical /*, 5.f*/));
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
      // gain
      const double gain = GetParam(kGain)->Value() / 100.;
      // Filter
      const double filterCutoff = GetParam(kFilterCutoff)->Value() / 100.0;
      const double filterReso = GetParam(kFilterResonance)->Value() / 100.0;
      const double filterBadwidth = GetParam(kFilterBandwidth)->Value() / 100.0;
      const bool spectralFilter = GetParam(kSpectralFilterOn)->Value();
      const double filterBypass = GetParam(kFilterBypass)->Value();
      const int filterSelector = GetParam(kFilterSelector)->Value();
      retained_kFilterSelector = filterSelector;

      const int spectralFilterSelector = GetParam(kSpectralFilterSelector)->Value();

      const int filterType = GetParam(kFilterType)->Value();
      //  shaper
      const double shaperDrive = GetParam(kShaperDrive)->Value() / 100.;
      const double shaperShape = GetParam(kShaperShape)->Value() / 100.;
      const double shaperAsym = GetParam(kShaperBias)->Value() / 100.;

      const double SH_Shape = GetParam(kSpectralShaperShape)->Value() / 100.;
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

      const double smoothedSH_Shape = mSpectralShaperShapeSmooth.Process(SH_Shape);


      fParams.setFilterParameters(filterType, filterSelector, smoothedFilterCutoff, smoothedFilterResonance, smoothedFilterBandwidth, smoothedShaperDrive, smoothedShaperShape, smoothedShaperBias,
                                  smoothedSH_Shape, oversamplingFactor, sampleRate);

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
      // double FilteredL = spectralFilterL.Process(inputL, fParams);
      // double FilteredR = spectralFilterR.Process(inputR, fParams);
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
      mSpectralShaperL.Process(FilteredL, fParams);
      mSpectralShaperR.Process(FilteredR, fParams);
      filnalTanh(FilteredL, fParams);
      filnalTanh(FilteredR, fParams);
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
  SendParameterValueFromDelegate(kFilterSelector, retained_kFilterSelector, 0);
  // int latencySamples = 2 * mRingBufferL.getChunkSize() - mRingBufferL.getHopSize();
  // SetLatency(latencySamples);
}
void ColorFilterPlugin::OnParamChange(int paramIdx, EParamSource, int sampleOffset)
{
  switch (paramIdx)
  {
  case kSpectralFilterOn:
    RemoveControl(kFilterCutoff); // deleting cutoff knob
    RemoveControl(kFilterResonance);
    RemoveControl(kFilterBypass);
    //  ███████ ██████  ███████  ██████ ████████      ██████ ████████ ███████ ███████
    //  ██      ██   ██ ██      ██         ██        ██         ██    ██      ██
    //  ███████ ██████  █████   ██         ██        ██         ██    █████   █████
    //       ██ ██      ██      ██         ██        ██         ██    ██      ██
    //  ███████ ██      ███████  ██████    ██         ██████    ██    ██      ██
    if (GetUI() && (GetParam(kSpectralFilterOn)->Value() > 0.5)) // instantiating new cutoff knob with different color depending on if spectral filt is on
    {
      mFilterCutoffStyle = mFilterCutoffStyle.WithGradient(kX1, Colors::ACCENT_YELLOW);
      mFilterResoStyle = mFilterResoStyle.WithGradient(kX1, Colors::ACCENT_YELLOW);
      mFilterBypassStyle = mFilterBypassStyle.WithGradient(kPR, Colors::ACCENT_YELLOW);


      mCutoff_Knob = GetUI()->AttachControl(new IVKnobControl(mCutoffKnobBounds, kFilterCutoff, "Cutoff", mFilterCutoffStyle));
      mReso_Knob = GetUI()->AttachControl(new IVKnobControl(mResoKnobBounds, kFilterResonance, "Reso", mFilterResoStyle));
      mFilterBypassSwitch = GetUI()->AttachControl(new IVToggleControl(mFilterBypass_Bounds, kFilterBypass, "Filter Bypass", mFilterBypassStyle, "Off", "On"));
      DecideFilterAlgoStatus();
      double filterValue = GetParam(kFilterCutoff)->Value();
      SendParameterValueFromDelegate(kFilterCutoff, filterValue, false);
      double ResoValue = GetParam(kFilterResonance)->Value();
      SendParameterValueFromDelegate(kFilterResonance, ResoValue, false);
      bool FilterBypassValue = GetParam(kFilterBypass)->Value();
      SendParameterValueFromDelegate(kFilterBypass, FilterBypassValue, false);
    }
    //  ███    ██  ██████  ██████  ███    ███      ██████ ████████ ███████ ███████
    //  ████   ██ ██    ██ ██   ██ ████  ████     ██         ██    ██      ██
    //  ██ ██  ██ ██    ██ ██████  ██ ████ ██     ██         ██    █████   █████
    //  ██  ██ ██ ██    ██ ██   ██ ██  ██  ██     ██         ██    ██      ██
    //  ██   ████  ██████  ██   ██ ██      ██      ██████    ██    ██      ██
    else if (GetUI() && (GetParam(kSpectralFilterOn)->Value() < 0.5)) // and other way around
    {
      mFilterCutoffStyle = mFilterCutoffStyle.WithGradient(kX1, Colors::MONOKAI_TYPE_GREEN);
      mFilterResoStyle = mFilterResoStyle.WithGradient(kX1, Colors::MONOKAI_TYPE_GREEN);
      mFilterBypassStyle = mFilterBypassStyle.WithGradient(kPR, Colors::MONOKAI_TYPE_GREEN);

      mCutoff_Knob = GetUI()->AttachControl(new IVKnobControl(mCutoffKnobBounds, kFilterCutoff, "Cutoff", mFilterCutoffStyle));
      mReso_Knob = GetUI()->AttachControl(new IVKnobControl(mResoKnobBounds, kFilterResonance, "Reso", mFilterResoStyle));
      mFilterBypassSwitch = GetUI()->AttachControl(new IVToggleControl(mFilterBypass_Bounds, kFilterBypass, "Filter Bypass", mFilterBypassStyle, "Off", "On"));

      DecideFilterAlgoStatus();

      double cutoffValue = GetParam(kFilterCutoff)->Value();
      SendParameterValueFromDelegate(kFilterCutoff, cutoffValue, false);
      double ResoValue = GetParam(kFilterResonance)->Value();
      SendParameterValueFromDelegate(kFilterResonance, ResoValue, false);
      bool FilterBypassValue = GetParam(kFilterBypass)->Value();
      SendParameterValueFromDelegate(kFilterBypass, FilterBypassValue, false);
    }
    DecideFilterTypeStatus();
    DecideResoStatus();

    break;
  case kFilterType:
    DefineSelector(retained_kFilterSelector);
    DecideBandwidthStatus();
    break;
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
    DecideResoStatus();
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

    GetParam(kFilterSelector)->InitInt("FilterSelector", default_value, min_value, max_value, "", IParam::kFlagStepped, "");
    IGraphics* pGraphics = GetUI();
    RemoveControl(kFilterSelector);

    //  ███████ ██      ████████ ██████      ███████ ██       ██████ ████████ ██████      ██    ██ ██████  ██████
    //  ██      ██         ██    ██   ██     ██      ██      ██         ██    ██   ██     ██    ██ ██   ██ ██   ██
    //  █████   ██         ██    ██████      ███████ ██      ██         ██    ██████      ██    ██ ██████  ██   ██
    //  ██      ██         ██    ██   ██          ██ ██      ██         ██    ██   ██     ██    ██ ██      ██   ██
    //  ██      ███████    ██    ██   ██     ███████ ███████  ██████    ██    ██   ██      ██████  ██      ██████
    if (pGraphics)
    {
      pGraphics->AttachControl(new IVRadioButtonControl(m_ButtonsPanel.GetGridCell(1, 3, rows, columns).GetFromTop(75).GetMidHPadded(padding), kFilterSelector,
                                                        FilterSelector::getInitList(m_filterAlgo), "FilterSelector", ColorFilterStyle_RadioButtons, EVShape::Ellipse, EDirection::Vertical /*, 5.f*/));
      SendParameterValueFromDelegate(kFilterSelector, default_value, 0);
    }
    break;
  }
}
void ColorFilterPlugin::DecideFilterAlgoStatus()
{
  if (GetUI())
    if (GetParam(kSpectralFilterOn)->Value() < 0.5)
      mFilterAlgoSwitch->Hide(false);
    else
      mFilterAlgoSwitch->Hide(true);
}
void ColorFilterPlugin::DecideFilterTypeStatus()
{
  if (GetUI())
    if (GetParam(kSpectralFilterOn)->Value() < 0.5)
      mFilter_Type_RB->Hide(false);
    else
      mFilter_Type_RB->Hide(true);
}

void ColorFilterPlugin::DecideBandwidthStatus()
{
  if (GetUI())
    if ((GetParam(kFilterType)->Int() == (int)FilterType::LP) || (GetParam(kFilterType)->Int() == (int)FilterType::HP))
      mF_BW_Knob->Hide(true);
    else
      mF_BW_Knob->Hide(false);
}
void ColorFilterPlugin::DecideResoStatus()
{
  if (GetUI())
    if ((GetParam(kFilterSelector)->Value() == (int)FilterTypes::DF1_1P))
      mReso_Knob->Hide(true);
    else
      mReso_Knob->Hide(false);
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

  // remove control
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
