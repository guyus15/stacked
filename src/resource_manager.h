#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <unordered_map>

class Shader;

class ResourceManager
{
public:
    ResourceManager(const ResourceManager &) = delete;
    ResourceManager operator=(const ResourceManager &) = delete;

    static void LoadShader(const std::string &name,
                           const std::string &vertex_file_path,
                           const std::string &fragment_file_path);

    static Shader &GetShader(const std::string &name);

private:
    ResourceManager() = default;

    static ResourceManager s_instance;
    static ResourceManager &Get();

    std::unordered_map<std::string, Shader> m_shaders;
};

#endif // RESOURCE_MANAGER_H