#pragma once

#include "Base.h"
#include "Shader/Shader.h"

#include <string>
#include <vector>
#include <unordered_map>

namespace Core
{
    class PostProcessor
    {
    private:
        struct PostProcessShader
        {
            bool enabled;
            Shader *shd;
        };
        std::vector<PostProcessShader> shaders;

        friend class Renderer;

    public:
        PostProcessor();
        ~PostProcessor();

        void Add(const std::string &shaderName, bool enabled = true);
        void Enable(const std::string &shaderName);
        void Disable(const std::string &shaderName);
        void Remove(const std::string &shaderName);

        void Rearrange(std::vector<std::string> shaderName);

        std::vector<Shader *> GetEnabledShaders();
    };
}