#include "stacked/ui.h"

#include "stacked/input.h"

#include "font.h"
#include "rect.h"
#include "resource_manager.h"

#include <glm/ext/matrix_clip_space.hpp>

#include <iostream>

#define min(a, b) ((a) < (b)) ? (a) : (b);

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

    glm::ivec2 mouse_pos = Input::GetMousePosition();
    mouse_pos.y = 600 - mouse_pos.y;

    if (mouse_pos.x < position.x + size.w && mouse_pos.x > position.x &&
        mouse_pos.y < position.y + size.h && mouse_pos.y > position.y)
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

    int font_size = min(size.w / name.size() * 2, size.h);
    // Add padding, this will later be done via styling functions.
    int padding = 5;
    font_size = ((float)font_size / 100.0f) * (100.0f - padding);

    int font_length = font_size * name.size() / 2;

    static Font font{"fonts/Montserrat/Montserrat-VariableFont_wght.ttf"};
    font.SetColour(1.0f, 1.0f, 1.0f, 1.0f);
    font.SetPosition({position.x + size.w / 2 - font_length / 2,
                      position.y + size.h / 2 - font_size / 2});
    font.Load(font_size);
    font.Render(name, font_size, shader);

    return button_press;
}
void Ui::Checkbox(const std::string &name, bool &enabled, UiVec2I position)
{
    Shader &shader = ResourceManager::GetShader("default");

    Rect checkbox_rect{position.x, position.y, 50, 50};
    if (enabled)
        checkbox_rect.SetColour(0.0f, 1.0f, 0.0f, 1.0f);
    else
        checkbox_rect.SetColour(1.0f, 0.0f, 0.0f, 1.0f);

    checkbox_rect.Render(shader);

    glm::ivec2 mouse_pos = Input::GetMousePosition();
    mouse_pos.y = 600 - mouse_pos.y;

    if (mouse_pos.x < position.x + 50 && mouse_pos.x > position.x &&
        mouse_pos.y < position.y + 50 && mouse_pos.y > position.y)
    {
        if (Input::GetMouseDown(MouseButton::LeftMouse))
        {
            enabled = !enabled;
        }
    }
}