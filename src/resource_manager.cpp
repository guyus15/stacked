#include "resource_manager.h"

#include "shader.h"

#include <fstream>
#include <iostream>
#include <sstream>

ResourceManager ResourceManager::s_instance;

void ResourceManager::LoadShader(const std::string &name, const std::string &vertex_file_path, const std::string &fragment_file_path)
{
    if (Get().m_shaders.find(name) != Get().m_shaders.end())
        std::cout << "Writing over shader with name '" << name << "'.\n";

    std::string vertex_shader_path = "shaders/shader.vs";
    std::string fragment_shader_path = "shaders/shader.fs";

    std::string vertex_code, fragment_code;
    std::ifstream vertex_shader_file, fragment_shader_file;

    vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        vertex_shader_file.open(vertex_shader_path);
        fragment_shader_file.open(fragment_shader_path);

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

    Get().m_shaders[name] = shader;
}

Shader &ResourceManager::GetShader(const std::string &name)
{
    if (Get().m_shaders.find(name) == Get().m_shaders.end())
    {
        std::cerr << "Error: Could not find shader with name '" << name << "'\n.";
        throw std::runtime_error{"Failed to get shader."};
    }

    return Get().m_shaders[name];
}

ResourceManager &ResourceManager::Get()
{
    return s_instance;
}
