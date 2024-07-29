#ifndef FONT_H
#define FONT_H

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "shader.h"

#include <unordered_map>

struct Character
{
    uint32_t texture_id;
    glm::ivec2 size;
    glm::ivec2 bearing;
    uint32_t advance;
};

class Font
{
public:
    Font();

    void Load(const std::string& path);
    void Render(const std::string& text, const Shader& shader);
    void SetColour(float r, float g, float b, float a);
    void SetPosition(const glm::ivec2& position);
    void SetScale(float scale);

private:
    glm::vec4 m_colour;
    glm::ivec2 m_position;
    float m_scale;
    std::unordered_map<char, Character> m_characters;
    uint32_t m_vao, m_vbo, m_ebo;
};

#endif // FONT_H
