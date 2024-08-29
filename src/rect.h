#ifndef RECT_H
#define RECT_H

#include "stacked/types.h"

#include "shader.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class Rect
{
public:
    Rect(UiVec2I position, UiVec2I size);
    ~Rect();

    void Render(const Shader &shader);

    void SetPosition(UiVec2I position);
    void SetSize(UiVec2I size);
    void SetColour(UiVec4F colour);
    void SetRadius(UiFloat radius);

    bool IsHovered() const;
    bool IsHovered(UiVec2I offset, UiVec2I padding) const;

private:
    UiVec2I m_position;
    UiVec2I m_size;
    UiVec4F m_colour;
    UiFloat m_radius;
    uint32_t m_vao, m_vbo, m_ebo;
};

#endif // RECT_H