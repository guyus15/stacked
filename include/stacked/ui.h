#ifndef UI_H
#define UI_H

#include "input.h"
#include "types.h"

#include <string>

struct UiIO
{
    UiVec2I display_size;
    float delta_time;

    const char *platform_name;
    void *platform_data;

    UiInput input;
};

struct UiStyle
{
    // Window
    UiVec4F window_colour_background;

    UiVec4F window_border_colour;
    UiInt window_border_width;
    UiFloat window_border_radius;

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

namespace Ui
{
    void Initialise();
    void Dispose();

    void BeginFrame();
    void EndFrame();

    // Window
    void BeginWindow(const std::string &name, UiVec2I size, UiVec2I position);
    void EndWindow();

    // Widgets
    bool Button(const std::string &name, UiVec2I size, UiVec2I position);
    // TODO: Cleanup widget functions in the same way as Button()
    void Checkbox(const std::string &name, bool &enabled, UiVec2I position);
    void SliderFloat(const std::string &name, float &current_val, float min_val, float max_val, UiVec2I position);
    void TextBox(const std::string &name, std::string &text, UiVec2I size, UiVec2I position);

    // IO
    UiIO &GetIO();

    // Styling
    UiStyle &GetStyle();
}

#endif // UI_H