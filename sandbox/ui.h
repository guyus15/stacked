#ifndef UI_H
#define UI_H

#include <string>

struct UiVec2I
{
    union
    {
        int x, w;
    };

    union
    {
        int y, h;
    };
};

class Font;
class Shader;

namespace Ui
{
    bool Button(const std::string& name, Font& font, const Shader& shader, UiVec2I size, UiVec2I position);
}

#endif // UI_H
