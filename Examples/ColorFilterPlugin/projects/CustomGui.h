#pragma once
#include "IControls.h"
#include "projects/DebugLogger.h"
static double mDrawScaleRetainer{1};

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
const IColor BACKGROUND_RB = NO_COLOR; // bg
const IColor OFF_FOREGROUND_RB = ACCENT_YELLOW;  // ??
const IColor ON_PRESSED_RB = MONOKAI_TYPE_GREEN; // selected text
const IColor FRAME_RB = BW_DIM_BG; // circle around button
const IColor HIGHLIGHT_RB = FG_MEDIUM; // splash on button
const IColor SHADOW_RB = BG_DARKER_TRANSPARENT; // ??
const IColor XTRA_1_RB = MONOKAI_TEXT_GREY;       // non focused text
const IColor XTRA_2_RB = MONOKAI_TEXT_LIGHT_GREY; // ??
const IColor XTRA_3_RB = ACCENT_YELLOW; // ??

static const std::initializer_list<IColor> ColorFilterStyleColorList_FilterPanel_RB{
  BACKGROUND_RB, OFF_FOREGROUND_RB, ON_PRESSED_RB, FRAME_RB, HIGHLIGHT_RB, SHADOW_RB, XTRA_1_RB, XTRA_2_RB, XTRA_3_RB};

static const IVColorSpec ColorFilterColorSpec_FilterPanel_RB{ColorFilterStyleColorList_FilterPanel_RB};
} // namespace Colors

/* Pattern Guide:
//Linear Gradient:
//CreateLinearGradient(x1, y1, x2, y2, {{color1, stop1}, {color2, stop2}, ...})
//- (x1,y1)->(x2,y2): Defines gradient direction vector
//- Stops: 0.0=start, 1.0=end position
//- Example horizontal: (0,0, 1,0, ...)
//- Example vertical: (0,0, 0,1, ...)
//- Example diagonal: (0,0, 1,1, ...)
//
//Radial Gradient:
//CreateRadialGradient(cx,cy, r, {{color1, stop1}, {color2, stop2}, ...})
//- (cx,cy): Center point
//- r: Radius
//- Stops: 0.0=center, 1.0=edge
//
//Common operations:
//.AddStop(color, position) - Add extra color stop
//
//.ScaleToRange()
//- Makes pattern coordinates relative (0->1) instead of absolute
//- Pattern will automatically stretch to fit any target drawing bounds
//- Essential when pattern is defined once but used on different sized elements
//- Example: gradient(0,0, 0,1).ScaleToRange() will always stretch top-to-bottom regardless of height
//
//.WithTransform(transform)
//- Takes IMatrix argument to modify pattern's appearance
//- Common transforms:
//  .WithTransform(IMatrix().Translate(x,y)) - Move pattern
//  .WithTransform(IMatrix().Scale(sx,sy)) - Stretch/shrink
//  .WithTransform(IMatrix().Rotate(degrees)) - Rotate around origin
//  .WithTransform(IMatrix().Skew(sx,sy)) - Shear/slant
//- Can chain transforms: .Scale().Rotate().Translate()
//- Transforms apply relative to pattern's coordinate space
//
//Tips:
//- Can use unlimited color stops
//- Stops can overlap for sharp transitions
//- Use .SetExtend() for repeat/reflect/pad behaviors
//
//
//1. Radial Gradient
//CreateRadialGradient(cx,cy, r, stops)
//- Colors radiate outward from center point
//- Stops: 0.0=center, 1.0=radius edge
//
//2. Sweep Gradient (aka Conic)
//CreateSweepGradient(cx,cy, {{color1, angle1}, {color2, angle2}, ...})
//- Colors rotate around center point
//- Angles in radians (0=right, π/2=bottom, π=left, 3π/2=top)
//- Perfect for knob arcs
//- Example 270° sweep:
//  CreateSweepGradient(cx,cy,
//    {{color1, -3π/4},  // -135°
//     {color2, 3π/4}})  //  135°
//
//Use .WithTransform() to rotate/position if needed
//*/

static IStrokeOptions getStrokeOptions()
{
  IStrokeOptions strokeOptions{};
  strokeOptions.mCapOption = ELineCap::Round;
  strokeOptions.mJoinOption = ELineJoin::Bevel;
  return strokeOptions;
};

static const IText labelText20 = IText(20, Colors::MONOKAI_TEXT_LIGHT_GREY);
static const IText labelText12 = IText(12, Colors::MONOKAI_TEXT_LIGHT_GREY);

static IVStyle ColorFilterStyle_Knob{true, // show label
                                false, // show value
                                Colors::ColorFilterColorSpec_FilterPanel,
                                labelText20, // text styling
                                DEFAULT_VALUE_TEXT,
                                DEFAULT_HIDE_CURSOR,
                                DEFAULT_DRAW_FRAME,
                                false, // draw shadows
                                DEFAULT_EMBOSS,
                                DEFAULT_ROUNDNESS,
                                0.5, // frame thinckness
                                0., // shadow offset
                                DEFAULT_WIDGET_FRAC,
                                DEFAULT_WIDGET_ANGLE,
                                DEFAULT_LABEL_ORIENTATION,
                                true, // using gradients
                                true, // add background track
                                Colors::ColorFilterPatternSpec_FilterPanel,
                                getStrokeOptions()};


static IVStyle ColorFilterStyle_RadioButtons{true, // show label
                                false, // show value
                                Colors::ColorFilterColorSpec_FilterPanel_RB,
                                labelText20, // text styling
                                DEFAULT_VALUE_TEXT,
                                DEFAULT_HIDE_CURSOR,
                                DEFAULT_DRAW_FRAME,
                                false, // draw shadows
                                DEFAULT_EMBOSS,
                                DEFAULT_ROUNDNESS,
                                0.5, // frame thinckness
                                0., // shadow offset
                                DEFAULT_WIDGET_FRAC,
                                DEFAULT_WIDGET_ANGLE,
                                DEFAULT_LABEL_ORIENTATION,
                                true, // using gradients
                                false, // add background track
                                Colors::ColorFilterPatternSpec_FilterPanel,
                                getStrokeOptions()};


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