#include "font.h"
#include "vertex.h"

#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <iostream>

UiFont::UiFont(const std::string &path)
    : m_path{std::move(path)},
      m_colour{0.0f, 0.0f, 0.0f, 1.0f},
      m_position{}
{
}

UiFont::~UiFont()
{
    glDeleteVertexArrays(1, &m_vao);
    glDeleteBuffers(1, &m_vbo);
    glDeleteBuffers(1, &m_ebo);
}

void UiFont::Load(const int size)
{
    if (m_characters.find(size) != m_characters.end())
        return;

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cerr << "Error: Failed to initialise FreeType.\n";

    FT_Face face;
    if (FT_New_Face(ft, m_path.c_str(), 0, &face))
        std::cerr << "Error: Failed to load font.\n";

    FT_Set_Pixel_Sizes(face, 0, size);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    for (unsigned char c = 0; c < 128; c++)
    {
        // Load character glyph.
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cerr << "Error: Failed to load glyph.\n";
            continue;
        }

        uint32_t texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        Character character{};
        character.texture_id = texture;
        character.size = glm::ivec2{face->glyph->bitmap.width, face->glyph->bitmap.rows};
        character.bearing = glm::ivec2{face->glyph->bitmap_left, face->glyph->bitmap_top};
        character.advance = face->glyph->advance.x;

        m_characters[size][c] = character;
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    constexpr uint32_t indices[6] = {
        0, 1, 2,
        0, 2, 3};

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, nullptr, GL_DYNAMIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 6, indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, position));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texture_coordinate));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void UiFont::Render(const std::string &text, const int size, const Shader &shader)
{
    if (m_characters.find(size) == m_characters.end())
    {
        std::cerr << "Error: Font has not been loaded with size " << size << "\n";
        return;
    }

    shader.Use();

    shader.SetVec4("u_colour", m_colour);
    shader.SetFloat("u_radius", 0.0f);

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_vao);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float current_x = m_position.x;

    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = m_characters[size][*c];

        float xpos = current_x + ch.bearing.x;
        float ypos = m_position.y - (ch.size.y - ch.bearing.y);

        float w = ch.size.x;
        float h = ch.size.y;

        Vertex vertices[4] = {
            {{xpos, ypos + h}, {0.0f, 0.0f}},
            {{xpos, ypos}, {0.0f, 1.0f}},
            {{xpos + w, ypos}, {1.0f, 1.0f}},
            {{xpos + w, ypos + h}, {1.0f, 0.0f}}};

        glBindTexture(GL_TEXTURE_2D, ch.texture_id);

        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        current_x += (ch.advance >> 6);
    }

    glDisable(GL_BLEND);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void UiFont::SetColour(const UiVec4F colour)
{
    m_colour = colour;
}

void UiFont::SetPosition(const UiVec2I position)
{
    m_position = position;
}