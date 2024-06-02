#pragma once

#include "Base.h"

#include <string>

namespace Core
{
    class Shader
    {
    public:
        struct ShaderConstructor
        {
            std::string VertexSource;
            std::string FragmentSource;
        };

    private:
        CeU32 id;

        void Compile(const std::string &vertexSource, const std::string &fragmentSource);

    public:
        Shader(const ShaderConstructor &constructor);
        Shader(const std::string &vertexSource, const std::string &fragmentSource);
        ~Shader();

        void Use();

        inline CeU32 GetID() { return id; };
    };
}