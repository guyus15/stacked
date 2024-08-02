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

class Shader;

namespace Ui
{
    void Initialise();
    bool Button(const std::string &name, UiVec2I size, UiVec2I position);
}

#endif // UI_H
