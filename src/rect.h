#ifndef RECT_H
#define RECT_H

#include "shader.h"

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

class Rect
{
public:
    Rect(int x, int y, int w, int h);
    ~Rect();

    void Render(const Shader &shader);

    void SetPosition(int x, int y);
    void SetDimensions(int w, int h);
    void SetColour(float r, float g, float b, float a);
    void SetRadius(float radius);

    bool IsHovered() const;

private:
    glm::ivec2 m_position;
    glm::ivec2 m_dimensions;
    glm::vec4 m_colour;
    float m_radius;
    uint32_t m_vao, m_vbo, m_ebo;
};

#endif // RECT_H
