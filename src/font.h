#ifndef FONT_H
#define FONT_H

#include "stacked/types.h"

#include "shader.h"

#include <glm/vec2.hpp>

#include <unordered_map>

constexpr int DEFAULT_FONT_SIZE = 48;

struct Character
{
    uint32_t texture_id;
    glm::ivec2 size;
    glm::ivec2 bearing;
    uint32_t advance;
};

class UiFont
{
public:
    UiFont() = default;
    UiFont(const std::string &path);
    ~UiFont();

    void Load(int size = DEFAULT_FONT_SIZE);
    void Render(const std::string &text, int size, const Shader &shader);
    void SetColour(UiVec4F colour);
    void SetPosition(UiVec2I position);

private:
    std::string m_path;
    UiVec4F m_colour;
    UiVec2I m_position;
    std::unordered_map<int, std::unordered_map<char, Character>> m_characters;
    uint32_t m_vao, m_vbo, m_ebo;
};

#endif // FONT_H