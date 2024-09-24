#include "resources.h"
#include "shader.h"

#include "stacked/ui.h"

#include <fstream>
#include <iostream>
#include <sstream>

void UiResources::LoadShader(const std::string &name,
                             const std::string &vertex_file_path,
                             const std::string &fragment_file_path)
{
    if (m_shaders.find(name) != m_shaders.end())
        std::cout << "Writing over shader with name '" << name << "'.\n";

    std::string vertex_code, fragment_code;
    std::ifstream vertex_shader_file, fragment_shader_file;

    vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vertex_shader_file.open(vertex_file_path);
        fragment_shader_file.open(fragment_file_path);

        std::stringstream vertex_shader_stream, fragment_shader_stream;

        vertex_shader_stream << vertex_shader_file.rdbuf();
        fragment_shader_stream << fragment_shader_file.rdbuf();

        vertex_shader_file.close();
        fragment_shader_file.close();

        vertex_code = vertex_shader_stream.str();
        fragment_code = fragment_shader_stream.str();
    }
    catch (std::ifstream::failure &)
    {
        std::cerr << "Failed to read shader file.\n";
    }

    Shader shader{};
    shader.Compile(vertex_code, fragment_code);

    m_shaders[name] = shader;
}

Shader &UiResources::GetShader(const std::string &name)
{
    if (m_shaders.find(name) == m_shaders.end())
    {
        std::cerr << "Error: Could not find shader with name '" << name << "'\n.";
        throw std::runtime_error{"Failed to get shader."};
    }

    return m_shaders[name];
}

void UiResources::UpdateProjectionMatrices(UiIO &io)
{
    const float projection[16] = {
        2.0f / io.display_size.x, 0.0f, 0.0f, 0.0f,
        0.0f, 2.0f / -io.display_size.y, 0.0f, 0.0f,
        0.0f, 0.0f, -2.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f};

    for (const auto &[_, shader] : m_shaders)
    {
        shader.SetMat4("projection", projection);
    }
}
