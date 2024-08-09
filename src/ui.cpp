#include "stacked/ui.h"
#include "stacked/input.h"

#include "font.h"
#include "rect.h"
#include "resource_manager.h"

#include <glm/ext/matrix_clip_space.hpp>

#include <iostream>

#define min(a, b) ((a) < (b)) ? (a) : (b)
#define clamp(l, u, c) ((c) > (u)) ? (u) : ((c) < (l)) ? (l) \
                                                       : (c)

UiStyle::UiStyle()
{
    button_colour_normal = {0.0f, 0.0f, 1.0f, 1.0f};
    button_colour_highlight = {0.3f, 0.3f, 1.0f, 1.0f};
    button_colour_press = {0.0f, 0.0f, 0.7f, 1.0f};
    button_font_colour = {1.0f, 1.0f, 1.0f, 1.0f};
    button_font_padding = 5.0f;
    button_radius = 5.0f;

    checkbox_colour_normal = {1.0f, 0.0f, 0.0f, 1.0f};
    checkbox_colour_enabled = {0.0f, 1.0f, 0.0f, 1.0f};
    checkbox_radius = 5.0f;

    slider_colour_background = {0.6f, 0.6f, 0.6f, 1.0f};
    slider_colour_handle = {1.0f, 1.0f, 0.0f, 1.0f};
}

UiContext *g_context = nullptr;

static UiContext *GetContext()
{
    assert(g_context != nullptr);
    return g_context;
}

void Ui::Initialise()
{
    g_context = new UiContext();

    g_context->initialised = true;

    ResourceManager::LoadShader("default", "shaders/shader.vs", "shaders/shader.fs");
    Shader &shader = ResourceManager::GetShader("default");
    shader.Use();

    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    shader.SetMat4("projection", projection);
}

void Ui::Dispose()
{
    delete g_context;
}

bool Ui::Button(const std::string &name, UiVec2I size, UiVec2I position)
{
    bool button_press = false;

    Shader &shader = ResourceManager::GetShader("default");

    const UiStyle &style = GetContext()->style;

    Rect button_rect{position, size};
    button_rect.SetRadius(5.0f);
    button_rect.SetColour(style.button_colour_normal);

    if (button_rect.IsHovered())
    {
        // The button is being hovered over.
        button_rect.SetColour(style.button_colour_highlight);

        // We set the colour setting operation and the button press separately
        // so it is more obvious that the button has been pressed but the associated
        // function does not occur more than once.
        if (Input::GetMouse(MouseButton::LeftMouse))
        {
            button_rect.SetColour(style.button_colour_press);
        }

        if (Input::GetMouseDown(MouseButton::LeftMouse))
        {
            button_press = true;
        }
    }

    button_rect.Render(shader);

    static Font font{"fonts/Montserrat/Montserrat-VariableFont_wght.ttf"};
    font.SetColour(style.button_font_colour);

    int font_size = min(size.w / name.size() * 2, size.h);
    // Add padding, this will later be done via styling functions.
    int padding = 5;
    font_size = ((float)font_size / 100.0f) * (100.0f - padding);
    int font_length = font_size * name.size() / 2;
    float font_divisors = size.h / static_cast<float>(font_size);

    font.SetPosition({position.x + size.w / 2 - font_length / 2,
                      position.y + static_cast<UiInt>(static_cast<float>(font_size) * (font_divisors / 2) - (static_cast<float>(font_size) / 2) + (padding * 2))});
    font.Load(font_size);
    font.Render(name, font_size, shader);

    return button_press;
}

void Ui::Checkbox(const std::string &name, bool &enabled, UiVec2I position)
{
    Shader &shader = ResourceManager::GetShader("default");

    const UiStyle &style = GetContext()->style;

    Rect checkbox_rect{position, {50, 50}};
    checkbox_rect.SetRadius(5.0f);

    if (enabled)
        checkbox_rect.SetColour(style.checkbox_colour_enabled);
    else
        checkbox_rect.SetColour(style.checkbox_colour_normal);

    checkbox_rect.Render(shader);

    glm::ivec2 mouse_pos = Input::GetMousePosition();
    mouse_pos.y = 600 - mouse_pos.y;

    if (checkbox_rect.IsHovered())
    {
        if (Input::GetMouseDown(MouseButton::LeftMouse))
        {
            enabled = !enabled;
        }
    }
}

void Ui::SliderFloat(const std::string &name, float &current_val, float min_val, float max_val, UiVec2I position)
{
    Shader &shader = ResourceManager::GetShader("default");

    const UiStyle &style = GetContext()->style;

    Rect background_rect{position, {200, 20}};
    background_rect.SetColour(style.slider_colour_background);

    background_rect.Render(shader);

    current_val = clamp(min_val, max_val, current_val);

    const float range = max_val - min_val;

    static bool gripped = false;
    static std::string gripped_name = "";

    if (background_rect.IsHovered() && Input::GetMouseDown(MouseButton::LeftMouse))
    {
        gripped = true;
        gripped_name = name;
    }

    if (Input::GetMouseUp(MouseButton::LeftMouse))
    {
        gripped = false;
        gripped_name = "";
    }

    if (gripped && gripped_name == name)
    {
        glm::ivec2 mouse_pos = Input::GetMousePosition();
        mouse_pos.y = 600 - mouse_pos.y;

        int new_pos_x = clamp(position.x, position.x + 200 - 20, mouse_pos.x - 10);

        const float ui_range = 200 - 20;
        const float percent = (new_pos_x - position.x) / ui_range;

        current_val = (percent * range) + min_val;
    }

    const float new_percent = (current_val - min_val) / range;

    Rect handle_rect{{position.x + static_cast<int>(new_percent * (200.0f - 20.0f)), position.y}, {20, 20}};
    handle_rect.SetColour(style.slider_colour_handle);

    handle_rect.Render(shader);
}

UiStyle &Ui::GetStyle()
{
    return g_context->style;
}