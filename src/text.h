#ifndef TEXT_H
#define TEXT_H

#include "stacked/types.h"

#include <string>

class UiFont;
class Shader;

class UiText
{
public:
    UiText(const std::string &content, UiInt size, UiVec2I position, UiVec4F colour);

    void Render(UiFont &font, const Shader &shader);

private:
    std::string m_content;
    UiInt m_size;
    UiVec2I m_position;
    UiVec4F m_colour;
};

#endif // TEXT_H