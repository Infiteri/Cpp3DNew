#pragma once

#include "Base.h"
#include "Math/Matrix4.h"
#include "Renderer/Color.h"

#include <string>

namespace Core
{
    class CE_API Shader
    {
    public:
        struct ShaderConstructor
        {
            std::string VertexSource;
            std::string FragmentSource;
        };

    private:
        CeU32 id;
        bool valid = false;
        std::string name; // any of them, even if its 2 sources, // TODO: Better system for 2 shader sources

        void Compile(const std::string &vertexSource, const std::string &fragmentSource);
        void Compile(const std::string &shaderSource);
        void CompileFromSource(const std::string &vertexSource, const std::string &fragmentSource);

        CeU32 GetUniLoc(const char *n);

    public:
        Shader(const ShaderConstructor &constructor);
        Shader(const std::string &shaderSource);
        Shader(const std::string &vertexSource, const std::string &fragmentSource);
        ~Shader();

        void Use();

        inline CeU32 GetID() { return id; };
        inline bool GetValid() { return valid; };
        inline std::string GetName() { return name; };

        void Int(int i, const char *name);

        void Vec2(Vector2 *v, const char *name);
        void Vec2(const Vector2 &v, const char *name);
        void Vec2(float x, float y, const char *name);

        void Vec3(Vector3 *v, const char *name);
        void Vec3(const Vector3 &v, const char *name);
        void Vec3(float x, float y, float z, const char *name);

        void Vec4(const Color &color, const char *name);

        void Mat4(const Matrix4 &matrix, const char *name);
        void Mat4(Matrix4 *matrix, const char *name);
        void Mat4(const float *matrix, const char *name);

        void Float(float x, const char *name);
    };
}