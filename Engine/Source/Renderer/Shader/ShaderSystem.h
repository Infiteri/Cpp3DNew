#pragma once

#include "Base.h"
#include "Shader.h"

#include <unordered_map>
#include <string>

namespace Core
{
    typedef std::unordered_map<std::string, Shader *> ShaderMap;

    class CE_API ShaderSystem
    {
    public:
        ShaderSystem(){};
        ~ShaderSystem(){};

        static void Init();
        static void Shutdown();

        static Shader* GetFromEngineResource(const std::string &name);

        static Shader *Get(const std::string &name);
        static bool Load(const std::string &name);
        static bool Exists(const std::string &name);
        static void Destroy(const std::string &name);
    };

}