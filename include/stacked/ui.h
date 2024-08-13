#ifndef UI_H
#define UI_H

#include "types.h"

#include <string>

struct UiStyle
{
    // Window
    UiVec4F window_colour_background;

    // Button
    UiVec4F button_colour_normal;
    UiVec4F button_colour_highlight;
    UiVec4F button_colour_press;
    UiVec4F button_font_colour;
    UiFloat button_font_padding;
    UiFloat button_radius;

    // Checkbox
    UiVec4F checkbox_colour_normal;
    UiVec4F checkbox_colour_enabled;
    UiFloat checkbox_radius;

    // Slider
    UiVec4F slider_colour_background;
    UiVec4F slider_colour_handle;

    UiStyle();
};

// TODO: Rename Font class in font.h to UiFont and reference here.
struct UiFont
{
};

namespace Ui
{
    void Initialise();
    void Dispose();

    // Window
    void BeginWindow(const std::string &name, UiVec2I size, UiVec2I position);
    void EndWindow();

    // Widgets
    bool Button(const std::string &name, UiVec2I size, UiVec2I position);
    void Checkbox(const std::string &name, bool &enabled, UiVec2I position);
    void SliderFloat(const std::string &name, float &current_val, float min_val, float max_val, UiVec2I position);

    // Styling
    UiStyle &GetStyle();
}

#endif // UI_H