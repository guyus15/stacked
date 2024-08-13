#include "rect.h"
#include "vertex.h"

#include "stacked/input.h"

#include <glad/glad.h>

static constexpr std::array<Vertex, 4> GetVertices(const UiVec2I position, const UiVec2I dimensions)
{
    return {Vertex{{position.x, position.y + dimensions.h}, {0.0f, 0.0f}},
            Vertex{{position.x, position.y}, {0.0f, 1.0f}},
            Vertex{{position.x + dimensions.w, position.y}, {1.0f, 1.0f}},
            Vertex{{position.x + dimensions.w, position.y + dimensions.h}, {1.0f, 0.0f}}};
}

Rect::Rect(const UiVec2I position, const UiVec2I dimensions)
    : m_position{position},
      m_dimensions{dimensions},
      m_colour{1.0f, 1.0f, 1.0f, 1.0f},
      m_radius{0.0f}
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    const std::array<Vertex, 4> vertices = GetVertices(position, dimensions);

    constexpr uint32_t indices[6] = {
        0, 1, 2,
        0, 2, 3};

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texture_coordinate));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Rect::~Rect()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void Rect::Render(const Shader &shader)
{
    shader.Use();

    shader.SetVec4("u_colour", m_colour);
    shader.SetVec2("u_dimensions", m_dimensions.x, m_dimensions.y);
    shader.SetFloat("u_radius", m_radius);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Rect::SetPosition(const UiVec2I position)
{
    m_position = position;

    const std::array<Vertex, 4> vertices = GetVertices(position, m_dimensions);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Rect::SetDimensions(const UiVec2I dimensions)
{
    m_dimensions = dimensions;
}

void Rect::SetColour(const UiVec4F colour)
{
    m_colour = colour;
}

void Rect::SetRadius(const UiFloat radius)
{
    m_radius = radius;
}

bool Rect::IsHovered() const
{
    UiVec2I mouse_pos = Input::GetMousePosition();
    // FIXME: Change this so that we aren't using a hard-coded value, but are using some
    // screen dimension value.
    mouse_pos.y = 600 - mouse_pos.y;

    return mouse_pos.x < m_position.x + m_dimensions.x && mouse_pos.x > m_position.x &&
           mouse_pos.y < m_position.y + m_dimensions.y && mouse_pos.y > m_position.y;
}
