#include "ui.h"

#include "font.h"
#include "rect.h"
#include "shader.h"
#include "input.h"

bool Ui::Button(const std::string &name, Font& font, const Shader& shader, UiVec2I size, UiVec2I position)
{
    Rect button_rect{ position.x, position.y, size.w, size.h };
    button_rect.Render(shader);

    font.SetPosition({ position.x, position.y });
    font.Render(name, shader);

    

    return false;
}
