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

void Ui::Initialise()
{
    ResourceManager::LoadShader("default", "shaders/shader.vs", "shaders/shader.fs");
    Shader &shader = ResourceManager::GetShader("default");
    shader.Use();

    glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
    shader.SetMat4("projection", projection);
}

// FIXME (guy): The radius of a Rect isn't working correctly when a button is rescaled during runtime.
// FIXME (guy): The font isn't centered vertically within the button's Rect.
bool Ui::Button(const std::string &name, UiVec2I size, UiVec2I position)
{
    bool button_press = false;

    Shader &shader = ResourceManager::GetShader("default");

    Rect button_rect{position.x, position.y, size.w, size.h};
    button_rect.SetRadius(5.0f);
    button_rect.SetColour(0.0f, 0.0f, 1.0f, 1.0f);

    if (button_rect.IsHovered())
    {
        // The button is being hovered over.
        button_rect.SetColour(0.3f, 0.3f, 1.0f, 1.0f);

        // We set the colour setting operation and the button press separately
        // so it is more obvious that the button has been pressed but the associated
        // function does not occur more than once.
        if (Input::GetMouse(MouseButton::LeftMouse))
        {
            button_rect.SetColour(0.0f, 0.0f, 0.7f, 1.0f);
        }

        if (Input::GetMouseDown(MouseButton::LeftMouse))
        {
            button_press = true;
        }
    }

    button_rect.Render(shader);

    static Font font{"fonts/Montserrat/Montserrat-VariableFont_wght.ttf"};
    font.SetColour(1.0f, 1.0f, 1.0f, 1.0f);

    int font_size = min(size.w / name.size() * 2, size.h);
    // Add padding, this will later be done via styling functions.
    int padding = 5;
    font_size = ((float)font_size / 100.0f) * (100.0f - padding);
    int font_length = font_size * name.size() / 2;
    float font_divisors = size.h / static_cast<float>(font_size);

    font.SetPosition({position.x + size.w / 2 - font_length / 2,
                      position.y + static_cast<float>(font_size) * (font_divisors / 2) - (static_cast<float>(font_size) / 2) + (padding * 2)});
    font.Load(font_size);
    font.Render(name, font_size, shader);

    return button_press;
}
void Ui::Checkbox(const std::string &name, bool &enabled, UiVec2I position)
{
    Shader &shader = ResourceManager::GetShader("default");

    Rect checkbox_rect{position.x, position.y, 50, 50};
    checkbox_rect.SetRadius(5.0f);

    if (enabled)
        checkbox_rect.SetColour(0.0f, 1.0f, 0.0f, 1.0f);
    else
        checkbox_rect.SetColour(1.0f, 0.0f, 0.0f, 1.0f);

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

    Rect background_rect{position.x, position.y, 200, 20};
    background_rect.SetColour(0.6f, 0.6f, 0.6f, 1.0f);

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

    Rect slider_rect{position.x + static_cast<int>(new_percent * (200.0f - 20.0f)), position.y, 20, 20};
    slider_rect.SetColour(1.0f, 1.0f, 0.0f, 1.0f);

    slider_rect.Render(shader);
}