#ifndef RESOURCES_H
#define RESOURCES_H

#include <string>
#include <unordered_map>

class Shader;

class UiResources
{
public:
    UiResources() = default;

    void LoadShader(const std::string &name,
                    const std::string &vertex_file_path,
                    const std::string &fragment_file_path);

    Shader &GetShader(const std::string &name);

private:
    std::unordered_map<std::string, Shader> m_shaders;
};

#endif // RESOURCES_H