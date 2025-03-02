#pragma once
#include "IControls.h"
#include "SkBitmap.h"
#include "SkCanvas.h"
#include "SkColorFilter.h"
#include "SkData.h"
#include "SkImage.h"
#include "SkSVGDOM.h"
#include "SkStream.h"
#include "Smoothers.h"
#include "projects/DebugPrint.h"
#include <vector>

static double mDrawScaleRetainer{1};
static bool mDrawScaleRetainerNeedsSaving{};
// static bool mInitialized{false};
static bool g_ShowValuesToggle{};

BEGIN_IPLUG_NAMESPACE
BEGIN_IGRAPHICS_NAMESPACE
namespace Colors
{

const IColor NO_COLOR = IColor(0, 0, 0, 0);

const IColor YELLOW_GREEN = IColor(255, 0x95, 0xC6, 0x23); // bright color for knob strip or something
const IColor PLATINUM = IColor(255, 0xE5, 0xE4, 0xE2);     // light grey, fine for BG
// claude monokai:
// Vibrant accent colors (for highlights/important elements)
const IColor ACCENT_YELLOW = IColor(255, 0xE5, 0xD5, 0x6D); // Monokai yellow
const IColor ACCENT_GREEN = IColor(255, 0xA2, 0xCC, 0xA1);  // Monokai green

// Greys (from darker to lighter)
const IColor BG_DARKEST = IColor(255, 0x1E, 0x1E, 0x1E);            // Very dark grey (main background)
const IColor BG_DARKER_TRANSPARENT = IColor(120, 0x25, 0x25, 0x26); // shadow (with transparency)
const IColor FG_DIM = IColor(255, 0x51, 0x51, 0x51);                // Dimmed foreground
const IColor FG_MEDIUM = IColor(255, 0x70, 0x70, 0x70);             // Medium grey (inactive text)
const IColor FG_LIGHT = IColor(255, 0xC5, 0xC5, 0xC5);              // Light grey (main text)
const IColor FG_LIGHTER = IColor(255, 0xE0, 0xE0, 0xE0);

const IColor MONOKAI_TYPE_GREEN = IColor(255, 0x48, 0xc9, 0xb0);
const IColor MONOKAI_TYPE_GREEN_DIM = IColor(255, 0x4c, 0xad, 0x70);
const IColor MONOKAI_MID_GREY = IColor(255, 0x40, 0x40, 0x40);
const IColor MONOKAI_MID_GREY_PLUS = IColor(180, 0x66, 0x66, 0x66);
const IColor MONOKAI_MID_GREY_PLUS_PLUS = IColor(255, 0x8c, 0x8c, 0x8c);

const IColor MONOKAI_TEXT_LIGHT_GREY = IColor(255, 0xd8, 0xd8, 0xd8);
const IColor MONOKAI_TEXT_LIGHT_GREY_PLUS = IColor(255, 0xfe, 0xfe, 0xfe);
const IColor MONOKAI_TEXT_BROWN_YELLOW = IColor(255, 0xdc, 0xdc, 0xaa);
const IColor MONOKAI_TEXT_GREY = IColor(255, 0x9a, 0x9b, 0x99);
const IColor MONOKAI_STRING_ORANGE = IColor(255, 0xc9, 0x99, 0x52);

const IColor JET_BRAINS_PINK = IColor(255, 0xed, 0x94, 0xc0);
const IColor COMMENT_GREEN = IColor(255, 0x85, 0xc4, 0x6c);

const IColor REAPER_MID_GREY = IColor(255, 0x3e, 0x3e, 0x3e);

const IColor BW_DARK_BG = IColor(255, 0x2e, 0x2e, 0x2e);
const IColor BW_DIM_BG = IColor(255, 0x3c, 0x3c, 0x3c);
const IColor BW_LIGHT_BG = IColor(255, 0x53, 0x53, 0x53);
const IColor BW_TEXT = IColor(255, 0xdc, 0xde, 0xdb);
const IColor BW_LIGHT_KNOB_TOP = IColor(255, 0xfb, 0xfb, 0xfb);
const IColor BW_LIGHT_KNOB_TOP_DIM = IColor(255, 0xf5, 0xf5, 0xf5);
const IColor BW_LIGHT_KNOB_BOTTOM = IColor(255, 0xc7, 0xc7, 0xc7);
const IColor BW_LIGHT_KNOB_BOTTOM_DIM = IColor(255, 0xc2, 0xc2, 0xc2);
const IColor BW_DARK_KNOB_TOP = IColor(255, 0x71, 0x71, 0x71);
const IColor BW_DARK_KNOB_TOP_HL = IColor(255, 0x85, 0x85, 0x85);
const IColor BW_DARK_KNOB_TOP_DIM = IColor(255, 0x67, 0x67, 0x67);
const IColor BW_DARK_KNOB_BOTTOM = IColor(255, 0x47, 0x47, 0x47);
const IColor BW_DARK_KNOB_BOTTOM_HL = IColor(255, 0x61, 0x61, 0x61);
const IColor BW_DARK_KNOB_BOTTOM_DIM = IColor(255, 0x3d, 0x3d, 0x3d);
const IColor BW_IDLE_KNOB_VALUE = IColor(255, 0x58, 0x58, 0x58);
const IColor BW_ORANGE = IColor(255, 0xff, 0x7d, 0x0f);
const IColor BW_GREEN = IColor(255, 0x4a, 0xca, 0x00);
const IColor BW_YELLOW = IColor(255, 0xff, 0xe5, 0x43);
const IColor BW_RED = IColor(255, 0xff, 0x16, 0x41);
const IColor BW_CYAN = IColor(255, 0x19, 0xee, 0xf0);

const IColor SAFFRON = IColor(255, 0xe9, 0xc4, 0x6a);
const IColor SANDY_BROWN = IColor(255, 0xf4, 0xa2, 0x61);
const IColor BURNT_SIENNA = IColor(255, 0xe7, 0x6f, 0x51);

const IColor PREPROCESSOR_VIOLET = IColor(255, 0xbe, 0xb7, 0xff);
// accent colors
const IColor SHAPER_COLOR = COMMENT_GREEN;

// ColorFilterStyle_Knob IVColorSpec
const IColor BACKGROUND = NO_COLOR;
const IColor OFF_FOREGROUND = BW_DARK_KNOB_BOTTOM;
const IColor ON_PRESSED = BW_DARK_KNOB_TOP;
const IColor FRAME = BW_DIM_BG;
const IColor HIGHLIGHT = FG_MEDIUM;
const IColor SHADOW = BG_DARKER_TRANSPARENT;
const IColor XTRA_1 = MONOKAI_TYPE_GREEN;
const IColor XTRA_2 = MONOKAI_TEXT_LIGHT_GREY;
const IColor XTRA_3 = MONOKAI_TEXT_GREY;


static const std::initializer_list<IColor> ColorFilterStyleColorList_FilterPanel{BACKGROUND, OFF_FOREGROUND, ON_PRESSED, FRAME, HIGHLIGHT, SHADOW, XTRA_1, XTRA_2, XTRA_3};
static const IVColorSpec ColorFilterColorSpec_FilterPanel{ColorFilterStyleColorList_FilterPanel};

static const std::initializer_list<IPattern> ColorFilterStylePatternList_FilterPanel{BACKGROUND, OFF_FOREGROUND, ON_PRESSED, FRAME, HIGHLIGHT, SHADOW, XTRA_1, XTRA_2, XTRA_3};
static const IVPatternSpec ColorFilterPatternSpec_FilterPanel{ColorFilterStylePatternList_FilterPanel};


// ColorFilterStyle_RadioButtons IVColorSpec
const IColor BACKGROUND_RB = NO_COLOR;            // bg
const IColor OFF_FOREGROUND_RB = ACCENT_YELLOW;   // ??
const IColor ON_PRESSED_RB = MONOKAI_TYPE_GREEN;  // selected text
const IColor FRAME_RB = BW_DIM_BG;                // circle around button
const IColor HIGHLIGHT_RB = FG_MEDIUM;            // splash on button
const IColor SHADOW_RB = BG_DARKER_TRANSPARENT;   // ??
const IColor XTRA_1_RB = MONOKAI_TEXT_GREY;       // non focused text
const IColor XTRA_2_RB = MONOKAI_TEXT_LIGHT_GREY; // ??
const IColor XTRA_3_RB = ACCENT_YELLOW;           // ??

static const std::initializer_list<IColor> ColorFilterStyleColorList_FilterPanel_RB{
  BACKGROUND_RB, OFF_FOREGROUND_RB, ON_PRESSED_RB, FRAME_RB, HIGHLIGHT_RB, SHADOW_RB, XTRA_1_RB, XTRA_2_RB, XTRA_3_RB};

static const IVColorSpec ColorFilterColorSpec_FilterPanel_RB{ColorFilterStyleColorList_FilterPanel_RB};
} // namespace Colors

static IStrokeOptions getStrokeOptions()
{
  IStrokeOptions strokeOptions{};
  strokeOptions.mCapOption = ELineCap::Round;
  strokeOptions.mJoinOption = ELineJoin::Bevel;
  return strokeOptions;
};

static const IText labelText15 = IText(15, Colors::MONOKAI_TEXT_LIGHT_GREY);
static const IText labelText12 = IText(12, Colors::MONOKAI_TEXT_LIGHT_GREY);

static IVStyle ColorFilterStyle_Knob{true,  // show label
                                     false, // show value
                                     Colors::ColorFilterColorSpec_FilterPanel,
                                     labelText15, // text styling
                                     DEFAULT_VALUE_TEXT,
                                     DEFAULT_HIDE_CURSOR,
                                     DEFAULT_DRAW_FRAME,
                                     false, // draw shadows
                                     DEFAULT_EMBOSS,
                                     DEFAULT_ROUNDNESS,
                                     0.5, // frame thinckness
                                     0.,  // shadow offset
                                     DEFAULT_WIDGET_FRAC,
                                     DEFAULT_WIDGET_ANGLE,
                                     DEFAULT_LABEL_ORIENTATION,
                                     true, // using gradients
                                     true, // add background track
                                     Colors::ColorFilterPatternSpec_FilterPanel,
                                     getStrokeOptions()};

static IVStyle ColorFilterStyle_RadioButtons{true,  // show label
                                             false, // show value
                                             Colors::ColorFilterColorSpec_FilterPanel_RB,
                                             labelText15, // text styling
                                             DEFAULT_VALUE_TEXT,
                                             DEFAULT_HIDE_CURSOR,
                                             DEFAULT_DRAW_FRAME,
                                             false, // draw shadows
                                             DEFAULT_EMBOSS,
                                             DEFAULT_ROUNDNESS,
                                             0.5, // frame thinckness
                                             0.,  // shadow offset
                                             DEFAULT_WIDGET_FRAC,
                                             DEFAULT_WIDGET_ANGLE,
                                             DEFAULT_LABEL_ORIENTATION,
                                             true,  // using gradients
                                             false, // add background track
                                             Colors::ColorFilterPatternSpec_FilterPanel,
                                             getStrokeOptions()};

static IVStyle ColorFilterStyle_SVG_KnobOver{false,
                                             false,
                                             Colors::ColorFilterColorSpec_FilterPanel_RB,
                                             labelText15, // text styling
                                             DEFAULT_VALUE_TEXT,
                                             DEFAULT_HIDE_CURSOR,
                                             DEFAULT_DRAW_FRAME,
                                             false, // draw shadows
                                             DEFAULT_EMBOSS,
                                             DEFAULT_ROUNDNESS,
                                             0.5, // frame thinckness
                                             0.,  // shadow offset
                                             DEFAULT_WIDGET_FRAC,
                                             DEFAULT_WIDGET_ANGLE,
                                             DEFAULT_LABEL_ORIENTATION,
                                             false, // using gradients
                                             false, // add background track
                                             Colors::ColorFilterPatternSpec_FilterPanel,
                                             getStrokeOptions()};
class BitmapPanel : public IPanelControl
{
  std::unique_ptr<IBitmap> mBitmap;

public:
  BitmapPanel(const IRECT& bounds)
    : IPanelControl(bounds, IColor(255, 0xff, 0xff, 0xff))
  {
    mIgnoreMouse = false;
  }
  void OnAttached() override
  {
    IPanelControl::OnAttached();
    if (GetUI())
    {
      mBitmap = std::make_unique<IBitmap>(GetUI()->LoadBitmap("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/BWSkin.png"));
    }
  }
  void SetBitmap(IBitmap& bitmap) { mBitmap = std::make_unique<IBitmap>(bitmap); }
  void Draw(IGraphics& g) override
  {
    if (mBitmap)
      g.DrawFittedBitmap(*mBitmap, mRECT.GetFromLeft(484).GetFromTop(438));
  }
  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    if (mod.R) // Right click
    {
      IPopupMenu menu;
      menu.AddItem("UI Scale: 50%", 0);
      menu.AddItem("UI Scale: 100%", 1);
      menu.AddItem("UI Scale: 150%", 2);
      menu.AddSeparator(3);
      menu.AddItem("Show/Hide Values", 4);


      GetUI()->CreatePopupMenu(*this, menu, x, y);
    }
  }
  void toggleControlValuesViewState() { g_ShowValuesToggle = true; }
  // Handle the menu selection
  void OnPopupMenuSelection(IPopupMenu* pSelectedMenu, int valIdx) override
  {
    if (pSelectedMenu)
    {
      switch (pSelectedMenu->GetChosenItemIdx())
      {
      case 0: // 50%
        if (GetUI())
        {
          GetUI()->Resize(GetUI()->Width(), GetUI()->Height(), 0.5);
        }
        break;
      case 1: // 100%
        if (GetUI())
        {
          GetUI()->Resize(GetUI()->Width(), GetUI()->Height(), 1);
        }
        break;
      case 2: // 150%
        if (GetUI())
        {
          GetUI()->Resize(GetUI()->Width(), GetUI()->Height(), 1.5);
        }
        break;
      case 4: // show/hide values
        toggleControlValuesViewState();
        break;
      }
    }
  }
  // void OnResize() override
  //{
  //     IControl::OnResize();
  //   if (mInitialized)
  //   {
  //     mDrawScaleRetainerNeedsSaving = true;
  //     mDrawScaleRetainer = GetUI()->GetDrawScale();
  //   }
  //   else
  //   {
  //     mInitialized = true;
  //   }
  // }
};
class InvisibleToggleControl : public IVToggleControl
{
private:
public:
  InvisibleToggleControl(const IRECT& bounds, int paramIdx = kNoParameter, const char* label = "", const IVStyle& style = DEFAULT_STYLE, const char* offText = "", const char* onText = "")
    : IVToggleControl(bounds, paramIdx, label, style, offText, onText)
  {
  }
  void DrawWidget(IGraphics& g) { /*DrawPressableShape(g, mShape, mWidgetBounds, GetValue() > 0.5, mMouseIsOver, IsDisabled());*/ }

  void DrawValue(IGraphics& g, bool mouseOver)
  {
    // if (mouseOver)
    //   g.FillRect(GetColor(kHL), mValueBounds, &mBlend);

    // if (GetValue() > 0.5)
    //   g.DrawText(mStyle.valueText, mOnText.Get(), mValueBounds, &mBlend);
    // else
    //   g.DrawText(mStyle.valueText, mOffText.Get(), mValueBounds, &mBlend);
  }
};

class ClipperLightButton : public IVToggleControl
{
  IVStyle style{};
  std::atomic<float> mModValue{};

public:
  ClipperLightButton(
    const IRECT& bounds, int paramIdx = kNoParameter, const char* label = "", const IVStyle& style = DEFAULT_STYLE, const char* offText = "", const char* onText = "", bool green = false)
    : IVToggleControl(bounds, paramIdx, label, style, offText, onText)
  {
    style.WithColor(kX2, IColor(0xff, 0xff, 0xff, 0xff));
  }
  void OnMsgFromDelegate(int msgTag, int dataSize, const void* pData) override
  {
    auto data = static_cast<const ISenderData<1, float>*>(pData);
    float value = data->vals[0];
    mModValue.store(value);
    SetDirty(false);
    IControl::OnMsgFromDelegate(msgTag, dataSize, pData); // base call if needed
  }
  void DrawLight(IGraphics& g, const IRECT& bounds)
  {
    const float alpha = mModValue.load();
    const float centerX = bounds.MW(); // Middle width
    const float centerY = bounds.MH(); // Middle height
    const float radius = std::min(bounds.W(), bounds.H()) * 0.5f;

    // Create a radial pattern
    IPattern pattern = IPattern::CreateRadialGradient(centerX, centerY, 4.31,
                                                      {
                                                        /*{IColor(55, 0x8b, 0x30, 0x30), 0}, */   // white dot
                                                        {IColor(255, 0xcc, 0x38, 0x38), 0.15},  // red light
                                                        {IColor(250, 0x6b, 0x1c, 0x1c), 0.7}, // glow (255 alpha)
                                                        {IColor(0, 0x38, 0x1c, 0x1c), 1.0}     // White transparent
                                                      });
    IBlend blend(EBlend::Default, alpha);
    g.PathClear();
    g.PathEllipse(bounds);
    g.PathFill(pattern, IFillOptions(), &blend);
  }
  void DrawWidget(IGraphics& g) { DrawLight(g, mWidgetBounds); }
};

class IVToggleControl_Bitmap : public IVToggleControl
{
private:
  bool mGreen{};
  std::unique_ptr<IBitmap> SmallButtonDisabled{};
  std::unique_ptr<IBitmap> SmallButton{};
  std::unique_ptr<IBitmap> SmallButtonActiveDisabled{};
  std::unique_ptr<IBitmap> SmallButtonActive{};
  std::unique_ptr<IBitmap> GreenButton{};

public:
  IVToggleControl_Bitmap(
    const IRECT& bounds, int paramIdx = kNoParameter, const char* label = "", const IVStyle& style = DEFAULT_STYLE, const char* offText = "", const char* onText = "", bool green = false)
    : IVToggleControl(bounds, paramIdx, label, style, offText, onText)
    , mGreen(green)
  {
  }
  void OnAttached() override
  {
    IVToggleControl::OnAttached();
    if (GetUI())
    {
      SmallButtonDisabled = std::make_unique<IBitmap>(GetUI()->LoadBitmap("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/Small Button Disabled.png"));
      SmallButton = std::make_unique<IBitmap>(GetUI()->LoadBitmap("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/Small Button.png"));
      SmallButtonActiveDisabled = std::make_unique<IBitmap>(GetUI()->LoadBitmap("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/SmllButtonActive Disabled.png"));
      SmallButtonActive = std::make_unique<IBitmap>(GetUI()->LoadBitmap("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/SmllButtonActive.png"));
      GreenButton = std::make_unique<IBitmap>(GetUI()->LoadBitmap("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/BypassOnButton.png"));
    }
  }

  void DrawWidget(IGraphics& g)
  {
    // DrawPressableShape(g, mShape, mWidgetBounds, GetValue() > 0.5, mMouseIsOver, IsDisabled());
    if (GetValue() > 0.5)
      if (IsDisabled())
      {
        if (SmallButtonActiveDisabled)
          g.DrawFittedBitmap(*SmallButtonActiveDisabled, mWidgetBounds.GetFromLeft(18.2).GetFromTop(18.2).GetTranslated(-4.44, -3.086));
      }
      else
      {
        if (!mGreen)
        {
          if (SmallButtonActive)
            g.DrawFittedBitmap(*SmallButtonActive, mWidgetBounds.GetFromLeft(18.2).GetFromTop(18.2).GetTranslated(-3.624, -3.086));
        }
        else if (GreenButton)
        {
          g.DrawFittedBitmap(*GreenButton, mWidgetBounds.GetFromLeft(18.28).GetFromTop(18.28).GetTranslated(-4.3, -3.1));
        }
      }
    else
    {
      if (IsDisabled())
      {
        if (SmallButtonDisabled)
          g.DrawFittedBitmap(*SmallButtonDisabled, mWidgetBounds.GetFromLeft(14.17).GetFromTop(14.17).GetTranslated(-2.14, -1.015));
      }
      else
      {
        if (SmallButton)
          g.DrawFittedBitmap(*SmallButton, mWidgetBounds.GetFromLeft(14.04).GetFromTop(14.04).GetTranslated(-2.165, -0.989));
      }
    }
  }
};


class IVRadioButtonControl_Bitmap : public IVRadioButtonControl
{
private:
  std::vector<bool> mDisabledStates{};
  std::unique_ptr<IBitmap> SmallButtonDisabled{};
  std::unique_ptr<IBitmap> SmallButton{};
  std::unique_ptr<IBitmap> SmallButtonActiveDisabled{};
  std::unique_ptr<IBitmap> SmallButtonActive{};

public:
  IVRadioButtonControl_Bitmap(const IRECT& bounds,
                              int paramIdx = kNoParameter,
                              const std::initializer_list<const char*>& options = {},
                              const char* label = "",
                              const IVStyle& style = DEFAULT_STYLE,
                              EVShape shape = EVShape::Ellipse,
                              EDirection direction = EDirection::Vertical,
                              float buttonSize = 10.3f)
    : IVRadioButtonControl(bounds, paramIdx, options, label, style, shape, direction, buttonSize)
  {
    mDisabledStates.resize(mNumStates);
  }
  void OnAttached() override
  {
    IVRadioButtonControl::OnAttached();
    if (GetUI())
    {
      SmallButtonDisabled = std::make_unique<IBitmap>(GetUI()->LoadBitmap("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/Small Button Disabled.png"));
      SmallButton = std::make_unique<IBitmap>(GetUI()->LoadBitmap("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/Small Button.png"));
      SmallButtonActiveDisabled = std::make_unique<IBitmap>(GetUI()->LoadBitmap("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/SmllButtonActive Disabled.png"));
      SmallButtonActive = std::make_unique<IBitmap>(GetUI()->LoadBitmap("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/SmllButtonActive.png"));
    }
  }
  void SetButtonDisabled(int buttonIdx, bool disabled)
  {
    if (GetUI())
      if (buttonIdx >= 0 && buttonIdx < mNumStates && buttonIdx < mDisabledStates.size())
      {
        mDisabledStates[buttonIdx] = disabled;
        SetDirty(false);
      }
  }

  bool IsButtonDisabled(int buttonIdx) const
  {
    if (buttonIdx >= 0 && buttonIdx < mNumStates && buttonIdx < mDisabledStates.size())
      return mDisabledStates[buttonIdx];

    return false;
  }
  void OnMouseDown(float x, float y, const IMouseMod& mod)
  {
    int index = GetButtonForPoint(x, y);
    if (index > -1 && !IsButtonDisabled(index))
      SetValue(((double)index * (1. / (double)(mNumStates - 1))));

    SetDirty(true);
  }
  void DrawWidget(IGraphics& g)
  {
    int hit = GetSelectedIdx();

    for (int i = 0; i < mNumStates; i++)
    {
      IRECT r = mButtons.Get()[i];
      // DrawButton(g, r.GetFromLeft(mButtonAreaWidth).GetCentredInside(mButtonSize), i == hit, mMouseOverButton == i, ETabSegment::Mid, IsDisabled() || GetStateDisabled(i));
      double moveX = 0.4;
      if (i == hit)
      {
        if (IsDisabled() || IsButtonDisabled(i))
          g.DrawFittedBitmap(*SmallButtonActiveDisabled, r.GetFromLeft(mButtonAreaWidth * 1).GetCentredInside(mButtonSize * 1.665).GetTranslated(-2.4 + moveX, 0));
        else
          g.DrawFittedBitmap(*SmallButtonActive, r.GetFromLeft(mButtonAreaWidth * .945).GetCentredInside(mButtonSize * 1.665).GetTranslated(-.15 - .5 + moveX, 0));
      }
      else
      {
        if (IsDisabled() || IsButtonDisabled(i))
          g.DrawFittedBitmap(*SmallButtonDisabled, r.GetFromLeft(mButtonAreaWidth).GetCentredInside(mButtonSize * 1.28).GetTranslated(-2 + moveX, 0.02));
        else
          g.DrawFittedBitmap(*SmallButton, r.GetFromLeft(mButtonAreaWidth).GetCentredInside(mButtonSize * 1.28).GetTranslated(-2 + moveX, 0.02));
      }


      // if (mTabLabels.Get(i))
      //{
      //   r = r.GetFromRight(r.W() - mButtonAreaWidth);
      //   g.DrawText(mStyle.valueText.WithFGColor(IColor(255, 0xda, 0xda, 0xda)), mTabLabels.Get(i)->Get(), r, &mBlend);
      // }
    }
  }
};

class BitmapKnobWithValueBar : public IVKnobControl
{
protected:
  double inicatorTrackOffset = 0.92;
  double knobPaddingActive = 7.5;
  double knobPaddingDisabled = 7.5;
  double vPadding = -9;
  double hPadding = 0;
  std::unique_ptr<IBitmap> mSvg_Knob{};
  std::unique_ptr<IBitmap> mSvg_KnobDisabled{};
  double translateX{};
  double translateY{};
  double fontSize{};
  double textScalingFactor{3};
  bool mShowValue{true};

public:
  BitmapKnobWithValueBar(const IRECT& bounds,
                         int paramIdx,
                         const char* label = "",
                         const IVStyle& style = DEFAULT_STYLE,
                         bool valueIsEditable = false,
                         bool valueInWidget = false,
                         float a1 = -135.f,
                         float a2 = 135.f,
                         float aAnchor = -135.f,
                         EDirection direction = EDirection::Vertical,
                         double gearing = 6,
                         float trackSize = 2.f)
    : IVKnobControl(bounds, paramIdx, label, style, valueIsEditable, valueInWidget, a1, a2, aAnchor, direction, gearing, trackSize)
  {
    fontSize = 12;
  }
  virtual void OnMouseDown(float x, float y, const IMouseMod& mod) 
  {
    IVKnobControl::OnMouseDown(x, y, mod);
    if (mod.L && mod.A) // Right click
    {
      if (GetUI())
      {
        float widgetRadius = GetRadius();
        IRECT knobHandleBounds = mWidgetBounds.GetCentredInside(widgetRadius).GetVPadded(vPadding).GetHPadded(hPadding);
        GetUI()->PromptUserInput(*this, knobHandleBounds);
        SetDirty(false);
      }
    }
  }
  void setShowValue(bool showValue) { mShowValue = showValue; }
  void OnAttached() override
  {
    IVKnobControl::OnAttached();
    if (GetUI())
    {
      mSvg_KnobDisabled = std::make_unique<IBitmap>(GetUI()->LoadBitmap("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/BigKnobDisabled.png"));
      mSvg_Knob = std::make_unique<IBitmap>(GetUI()->LoadBitmap("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/BigKnob.png"));
    }
  }
  void DrawWidget(IGraphics& g)
  {
    float widgetRadius = GetRadius(); // The radius out to the indicator track arc
    const float cx = mWidgetBounds.MW(), cy = mWidgetBounds.MH();
    IRECT knobHandleBounds = mWidgetBounds.GetCentredInside((widgetRadius - mTrackToHandleDistance) * 2.f);
    const float angle = mAngle1 + (static_cast<float>(GetValue()) * (mAngle2 - mAngle1));
    if (mStyle.addBackgroundTrack)
      DrawBackgroundTrack(g, cx, cy, widgetRadius);
    if (!IsDisabled())
      g.DrawFittedBitmap(*mSvg_Knob, knobHandleBounds.GetPadded(knobPaddingActive).GetTranslated(translateX, translateY));
    else
      g.DrawFittedBitmap(*mSvg_KnobDisabled, knobHandleBounds.GetPadded(knobPaddingDisabled).GetTranslated(translateX, translateY));
    DrawIndicatorTrack(g, angle, cx, cy, widgetRadius * inicatorTrackOffset);
    if (mTickCount > 0)
      DrawTicks(g, cx, cy, widgetRadius);
    // DrawHandle(g, knobHandleBounds);
    DrawPointer(g, angle, cx, cy, knobHandleBounds.W() / 2.f);
  }
  void DrawValue(IGraphics& g, bool mouseOver) override
  {
    float blendValue = 0.85;
    if (IsDisabled())
      blendValue = 0.4;
    if (mShowValue)
    {
      float widgetRadius = GetRadius();
      IRECT knobHandleBounds = mWidgetBounds.GetCentredInside(widgetRadius);
      IBlend blend = IBlend(EBlend::Default, blendValue);
      g.DrawText(mStyle.valueText.WithSize(fontSize).WithFGColor(IColor(255, 0xf8, 0xf8, 0xfa)), mValueStr.Get(), knobHandleBounds.GetTranslated(0, -knobHandleBounds.H() / textScalingFactor), &blend);
    }
  }
};
class BitmapKnobWithValueBarMED : public BitmapKnobWithValueBar
{

public:
  BitmapKnobWithValueBarMED(const IRECT& bounds,
                            int paramIdx,
                            const char* label = "",
                            const IVStyle& style = DEFAULT_STYLE,
                            bool valueIsEditable = false,
                            bool valueInWidget = false,
                            float a1 = -135.f,
                            float a2 = 135.f,
                            float aAnchor = -135.f,
                            EDirection direction = EDirection::Vertical,
                            double gearing = 6,
                            float trackSize = 2.f)
    : BitmapKnobWithValueBar(bounds, paramIdx, label, style, valueIsEditable, valueInWidget, a1, a2, aAnchor, direction, gearing, trackSize)
  {
    inicatorTrackOffset = 0.84;
    knobPaddingActive = 4.5;
    knobPaddingDisabled = 4.5;
    translateX = -.3;
    fontSize = 8;
    textScalingFactor = 3.3;
    vPadding = -1;
    hPadding = 3;
  }
  void OnAttached() override
  {
    IVKnobControl::OnAttached();
    if (GetUI())
    {
      mSvg_KnobDisabled = std::make_unique<IBitmap>(GetUI()->LoadBitmap("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/MedKnobDisabled.png"));
      mSvg_Knob = std::make_unique<IBitmap>(GetUI()->LoadBitmap("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/MedKnob.png"));
    }
  }
};

class BitmapKnobWithValueBarLIL : public BitmapKnobWithValueBarMED
{

public:
  BitmapKnobWithValueBarLIL(const IRECT& bounds,
                            int paramIdx,
                            const char* label = "",
                            const IVStyle& style = DEFAULT_STYLE,
                            bool valueIsEditable = false,
                            bool valueInWidget = false,
                            float a1 = -135.f,
                            float a2 = 135.f,
                            float aAnchor = -135.f,
                            EDirection direction = EDirection::Vertical,
                            double gearing = 10,
                            float trackSize = 2.f)
    : BitmapKnobWithValueBarMED(bounds, paramIdx, label, style, valueIsEditable, valueInWidget, a1, a2, aAnchor, direction, gearing, trackSize)
  {
    inicatorTrackOffset = 0.77;
    knobPaddingActive = 3;
    knobPaddingDisabled = 3;
    fontSize = 7;
    textScalingFactor = 3.45;
  }
};

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
  }


private:
  IColor mColor;
  float mCornerRadius;
};


END_IGRAPHICS_NAMESPACE
END_IPLUG_NAMESPACE