#include "text.h"
#include "font.h"
#include "shader.h"

UiText::UiText(const std::string &content, UiInt size, UiVec2I position, UiVec4F colour)
    : m_content{std::move(content)},
      m_size{size},
      m_position{position},
      m_colour{colour}
{
}

void UiText::Render(UiFont &font, const Shader &shader)
{
    font.Load(m_size);
    font.SetPosition(m_position);
    font.SetColour(m_colour);
    font.Render(m_content, m_size, shader);
}