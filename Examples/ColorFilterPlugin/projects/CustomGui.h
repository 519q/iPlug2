#pragma once
#include "IControls.h"
#include "SkBitmap.h"
#include "SkCanvas.h"
#include "SkImage.h"
#include "SkSVGDOM.h"
#include "SkStream.h"
#include "projects/DebugPrint.h"
#include <vector>
static double mDrawScaleRetainer{1};
static bool mDrawScaleRetainerNeedsSaving{};

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

// class BitmapPanel : public IControl
//{
// public:
//   BitmapPanel(const IRECT& bounds, const char* svgPath)
//     : IControl(bounds)
//     , fSvgPath(svgPath)
//     , fWidth(bounds.W())  // Store width from bounds
//     , fHeight(bounds.H()) // Store height from bounds
//   {
//     // Initialize the bitmap size with the specified bounds
//     if (!RenderSVGToBitmap(fSvgPath, fBitmap))
//     {
//       // Handle the error (e.g., log or set a default image)
//     }
//
//     // Create IBitmap from the SkBitmap
//     CreateIBitmapFromSkia();
//   }
//
//   ~BitmapPanel() {}
//
//   void Draw(IGraphics& g) override
//   {
//     if (fBitmapNeedsUpdate || fBitmapIBitmap == nullptr)
//     {
//       // Render the SVG to the bitmap with the current dimensions
//       if (!RenderSVGToBitmap(fSvgPath, fBitmap))
//       {
//         // Handle the error (e.g., log or set a default image)
//       }
//
//       // Create or update the IBitmap from the SkBitmap
//       CreateIBitmapFromSkia();
//       fBitmapNeedsUpdate = false; // Only reset after a successful update
//     }
//
//     // Draw the IBitmap to the UI
//     g.DrawBitmap(*fBitmapIBitmap, g.GetBounds(), 0, 0, EBlend::Default);
//   }
//
// private:
//   bool RenderSVGToBitmap(const char* svgPath, SkBitmap& bitmap)
//   {
//     SkFILEStream svgStream(svgPath);
//     sk_sp<SkSVGDOM> svgDom = SkSVGDOM::MakeFromStream(svgStream);
//     if (!svgDom)
//     {
//       // Handle error (you might want to log or display an error message)
//       return false;
//     }
//
//     // Allocate pixels with the panel's current width and height
//     SkImageInfo info = SkImageInfo::MakeN32Premul(fWidth, fHeight);
//     bitmap.allocPixels(info);
//
//     SkCanvas canvas(bitmap);
//     svgDom->setContainerSize(SkSize::Make(fWidth, fHeight));
//     svgDom->render(&canvas);
//
//     return true;
//   }
//
//   void CreateIBitmapFromSkia()
//   {
//     // Convert SkBitmap to IBitmap
//     // Assuming you've got an appropriate constructor or method to create an IBitmap from pixel data
//     fBitmapIBitmap = new IBitmap(fBitmap.width(), fBitmap.height(), false); // Use the appropriate constructor
//     // Copy the pixel data from SkBitmap to IBitmap
//     // Note: You'll need to ensure the pixel format is compatible
//     memcpy(fBitmapIBitmap->GetPixels(), fBitmap.getPixels(), fBitmap.byteSize());
//   }
//
//   const char* fSvgPath{};
//   SkBitmap fBitmap{};
//   IBitmap* fBitmapIBitmap = nullptr; // Pointer to the IBitmap representation
//   int fWidth;                        // Store the width of the bounds
//   int fHeight;                       // Store the height of the bounds
//   bool fBitmapNeedsUpdate = true;    // Flag to indicate when to update
// };
// const char* fSvgPath;
// int fWidth;
// int fHeight;
// SkBitmap fBitmap;
// void RenderSVGToBitmap(const char* svgPath, SkBitmap& bitmap, int width, int height)
//{
//   // Load the SVG file
//   SkFILEStream svgStream(svgPath);
//   sk_sp<SkSVGDOM> svgDom = SkSVGDOM::MakeFromStream(svgStream);
//   if (!svgDom)
//   {
//     // Handle error
//     return;
//   }
//
//   // Create bitmap info
//   SkImageInfo info = SkImageInfo::MakeN32Premul(width, height);
//   // Allocate pixels for the bitmap
//   bitmap.allocPixels(info);
//
//   // Create a canvas to draw onto
//   SkCanvas canvas(bitmap);
//
//   // Set up the SVG's viewBox which enables scaling
//   svgDom->setContainerSize(SkSize::Make(width, height));
//
//   // Draw the SVG to the canvas
//   svgDom->render(&canvas);
// }

class IVToggleControl_SVG : public IVToggleControl
{
private:
  bool mGreen{};

public:
  IVToggleControl_SVG(
    const IRECT& bounds, int paramIdx = kNoParameter, const char* label = "", const IVStyle& style = DEFAULT_STYLE, const char* offText = "", const char* onText = "", bool green = false)
    : IVToggleControl(bounds, paramIdx, label, style, offText, onText)
    , mGreen(green)
  {
  }
  void DrawWidget(IGraphics& g)
  {
    // DrawPressableShape(g, mShape, mWidgetBounds, GetValue() > 0.5, mMouseIsOver, IsDisabled());
    if (GetValue() > 0.5)
      if (IsDisabled())
        g.DrawSVG(GetUI()->LoadSVG("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/SmllButtonActive Disabled.svg"),
                  mWidgetBounds.GetFromLeft(18.2).GetFromTop(18.2).GetTranslated(-3.624, -3.086));
      else
      {
        if (!mGreen)
          g.DrawSVG(GetUI()->LoadSVG("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/SmllButtonActive.svg"),
                    mWidgetBounds.GetFromLeft(18.2).GetFromTop(18.2).GetTranslated(-3.624, -3.086));
        else
          g.DrawSVG(GetUI()->LoadSVG("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/BypassOnButton.svg"),
                    mWidgetBounds.GetFromLeft(18.2).GetFromTop(18.2).GetTranslated(-3.624, -2.373));
      }
    else
    {
      if (IsDisabled())
        g.DrawSVG(
          GetUI()->LoadSVG("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/Small Button Disabled.svg"), mWidgetBounds.GetPadded(-.4).GetTranslated(-.64, .12));
      else
        g.DrawSVG(GetUI()->LoadSVG("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/Small Button.svg"), mWidgetBounds);
    }
  }
};


class IVRadioButtonControl_SVG : public IVRadioButtonControl
{
private:
  std::vector<bool> mDisabledStates{};

public:
  IVRadioButtonControl_SVG(const IRECT& bounds,
                           int paramIdx = kNoParameter,
                           const std::initializer_list<const char*>& options = {},
                           const char* label = "",
                           const IVStyle& style = DEFAULT_STYLE,
                           EVShape shape = EVShape::Ellipse,
                           EDirection direction = EDirection::Vertical,
                           float buttonSize = 10.f)
    : IVRadioButtonControl(bounds, paramIdx, options, label, style, shape, direction, buttonSize)
  {
    mDisabledStates.resize(mNumStates);
  }
  void SetButtonDisabled(int buttonIdx, bool disabled)
  {
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
      if (i == hit)
      {
        if (IsDisabled() || IsButtonDisabled(i))
          g.DrawSVG(GetUI()->LoadSVG("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/SmllButtonActive Disabled.svg"),
                    r.GetFromLeft(mButtonAreaWidth * 1).GetCentredInside(mButtonSize * 1.73).GetTranslated(-.77, -.38));
        else
          g.DrawSVG(GetUI()->LoadSVG("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/SmllButtonActive.svg"),
                    r.GetFromLeft(mButtonAreaWidth * .945).GetCentredInside(mButtonSize * 1.73).GetTranslated(-.15, -.48));
      }
      else
      {
        if (IsDisabled() || IsButtonDisabled(i))
          g.DrawSVG(GetUI()->LoadSVG("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/Small Button Disabled.svg"),
                    r.GetFromLeft(mButtonAreaWidth).GetCentredInside(mButtonSize * 1.13).GetTranslated(-.6, -.03));
        else
          g.DrawSVG(GetUI()->LoadSVG("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/Small Button.svg"),
                    r.GetFromLeft(mButtonAreaWidth).GetCentredInside(mButtonSize * 1.22).GetTranslated(.05, -.15));
      }


      if (mTabLabels.Get(i))
      {
        r = r.GetFromRight(r.W() - mButtonAreaWidth);
        g.DrawText(mStyle.valueText.WithFGColor(i == hit ? GetColor(kON) : GetColor(kX1)), mTabLabels.Get(i)->Get(), r, &mBlend);
      }
    }
  }
};

class TransluscentKnobWithValueBar : public IVKnobControl
{
protected:
  double inicatorTrackOffset = 0.92;
  double knobPadding = 7.5;

public:
  TransluscentKnobWithValueBar(const IRECT& bounds,
                               int paramIdx,
                               const char* label = "",
                               const IVStyle& style = DEFAULT_STYLE,
                               bool valueIsEditable = false,
                               bool valueInWidget = false,
                               float a1 = -135.f,
                               float a2 = 135.f,
                               float aAnchor = -135.f,
                               EDirection direction = EDirection::Vertical,
                               double gearing = DEFAULT_GEARING,
                               float trackSize = 2.f)
    : IVKnobControl(bounds, paramIdx, label, style, valueIsEditable, valueInWidget, a1, a2, aAnchor, direction, gearing, trackSize)
  {
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
      g.DrawSVG(GetUI()->LoadSVG("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/BigKnob.svg"), knobHandleBounds.GetPadded(knobPadding));
    else
      g.DrawSVG(GetUI()->LoadSVG("C:/Users/salva/Documents/GitHub/iPlug2/Examples/ColorFilterPlugin/resources/Skins/BigKnobDisabled.svg"), knobHandleBounds.GetPadded(knobPadding));
    DrawIndicatorTrack(g, angle, cx, cy, widgetRadius * inicatorTrackOffset);
    if (mTickCount > 0)
      DrawTicks(g, cx, cy, widgetRadius);
    // DrawHandle(g, knobHandleBounds);
    DrawPointer(g, angle, cx, cy, knobHandleBounds.W() / 2.f);
  }
};
class TransluscentKnobWithValueBarMED : public TransluscentKnobWithValueBar
{

public:
  TransluscentKnobWithValueBarMED(const IRECT& bounds,
                                  int paramIdx,
                                  const char* label = "",
                                  const IVStyle& style = DEFAULT_STYLE,
                                  bool valueIsEditable = false,
                                  bool valueInWidget = false,
                                  float a1 = -135.f,
                                  float a2 = 135.f,
                                  float aAnchor = -135.f,
                                  EDirection direction = EDirection::Vertical,
                                  double gearing = DEFAULT_GEARING,
                                  float trackSize = 2.f)
    : TransluscentKnobWithValueBar(bounds, paramIdx, label, style, valueIsEditable, valueInWidget, a1, a2, aAnchor, direction, gearing, trackSize)
  {
    inicatorTrackOffset = 0.84;
    knobPadding = 3.7;
  }
};

class TransluscentKnobWithValueBarLIL : public TransluscentKnobWithValueBar
{

public:
  TransluscentKnobWithValueBarLIL(const IRECT& bounds,
                                  int paramIdx,
                                  const char* label = "",
                                  const IVStyle& style = DEFAULT_STYLE,
                                  bool valueIsEditable = false,
                                  bool valueInWidget = false,
                                  float a1 = -135.f,
                                  float a2 = 135.f,
                                  float aAnchor = -135.f,
                                  EDirection direction = EDirection::Vertical,
                                  double gearing = DEFAULT_GEARING,
                                  float trackSize = 2.f)
    : TransluscentKnobWithValueBar(bounds, paramIdx, label, style, valueIsEditable, valueInWidget, a1, a2, aAnchor, direction, gearing, trackSize)
  {
    inicatorTrackOffset = 0.77;
    knobPadding = 2.4;
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

  void OnMouseDown(float x, float y, const IMouseMod& mod) override
  {
    if (mod.R) // Right click
    {
      IPopupMenu menu;
      menu.AddItem("UI Scale: 50%", 0);
      menu.AddItem("UI Scale: 100%", 1);
      menu.AddItem("UI Scale: 150%", 2);
      menu.AddSeparator(3);

      GetUI()->CreatePopupMenu(*this, menu, x, y);
    }
  }

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
      }
    }
  }
  bool mInitialized{false};
  void OnResize() override
  {
    if (mInitialized)
    {
      mDrawScaleRetainerNeedsSaving = true;
      IControl::OnResize();
      mDrawScaleRetainer = GetUI()->GetDrawScale();
    }
    else
    {
      mInitialized = true;
    }
  }

private:
  IColor mColor;
  float mCornerRadius;
};


END_IGRAPHICS_NAMESPACE
END_IPLUG_NAMESPACE