#include "rect.h"

#include <glad/glad.h>

// TODO: move this Vertex struct into its own file/some common file so it is not duplicated.
struct Vertex
{
    glm::vec2 position;
    glm::vec2 texture_coordinate;
};

Rect::Rect(const int x, const int y, const int w, const int h)
    : m_position{ x, y },
      m_dimensions{ w, h },
      m_colour{ 1.0f, 1.0f, 1.0f, 1.0f }
{
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    const Vertex vertices[4] = {
        { { x, y + h }, { 0.0f, 0.0f } },
        { { x, y }, { 0.0f, 1.0f } },
        { { x + w, y }, { 1.0f, 1.0f } },
        { { x + w, y + h }, { 1.0f, 0.0f } }
    };

    constexpr uint32_t indices[6] = {
        0, 1, 2,
        0, 2, 3
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texture_coordinate));
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

void Rect::Render(const Shader& shader)
{
    shader.Use();

    shader.SetVec4("u_colour", m_colour);
    shader.SetFloat("u_radius", m_radius);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void Rect::SetPosition(const int x, const int y)
{
    m_position = { x, y };
}

void Rect::SetDimensions(const int w, const int h)
{
    m_dimensions = { w, h };
}

void Rect::SetColour(const float r, const float g, const float b, const float a)
{
    m_colour = { r, g, b, a };
}

void Rect::SetRadius(const float radius)
{
    m_radius = radius;
}
