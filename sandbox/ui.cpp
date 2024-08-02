#include "ui.h"

#include "font.h"
#include "rect.h"
#include "shader.h"
#include "input.h"

#include <iostream>

#define min(a, b) ((a) < (b)) ? (a)  : (b);

bool Ui::Button(const std::string &name, const Shader& shader, UiVec2I size, UiVec2I position)
{
    bool button_press = false;

    Rect button_rect{ position.x, position.y, size.w, size.h };
    //button_rect.SetRadius(5.0f);
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

    static Font font{ "fonts/Montserrat/Montserrat-VariableFont_wght.ttf" };
    font.SetColour(1.0f, 1.0f, 1.0f, 1.0f);
    font.SetPosition({
        position.x + size.w / 2 - font_length / 2,
        position.y + size.h / 2 - font_size / 2
    });
    font.Load(font_size);
    font.Render(name, font_size, shader);

    return button_press;
}
