#include "ShaderSystem.h"
#include "Core/Logger.h"

namespace Core
{
    static ShaderMap Shaders;
    static std::string ResourceBase;

    void ShaderSystem::Init()
    {
        ResourceBase = "EngineResources/Shaders";
        CE_DEFINE_LOG_CATEGORY("CE_SHD", "Shader System");
    }

    void ShaderSystem::Shutdown()
    {
        for (auto it : Shaders)
        {
            delete it.second;
        }

        Shaders.clear();
    }

    Shader *ShaderSystem::GetFromEngineResource(const std::string &name)
    {
        return Get(ResourceBase + "/" + name);
    }

    Shader *ShaderSystem::Get(const std::string &name)
    {
        if (!Exists(name))
        {
            Load(name);
            if (!Exists(name))
            {
                // ! NO SHADER VALID
                CE_LOG("CE_SHD", Error, "Shader '%s' not valid.");
                return nullptr;
            }
        }

        return Shaders[name];
    }

    bool ShaderSystem::Load(const std::string &name)
    {
        if (Exists(name))
            return true;

        Shaders[name] = new Shader(name + ".glsl");
        if (!Shaders[name]->GetValid())
        {
            CE_LOG("CE_SHD", Error, "Shader '%s' not valid.");
            return false;
        }

        return true;
    }

    bool ShaderSystem::Exists(const std::string &name)
    {
        return Shaders[name] != nullptr;
    }

    void ShaderSystem::Destroy(const std::string &name)
    {
        if (Exists(name))
        {
            Shader *shd = Shaders[name];
            delete shd;
            Shaders.erase(name);
        }
    }
}