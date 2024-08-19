#include "Shader.h"
#include "Core/Logger.h"
#include "Core/Engine.h"

#include <glad/glad.h>
#include <vector>

namespace Core
{
    static CeU32 LoadShader(const char *source, CeU32 type)
    {
        // New source casted
        const GLchar *newSrc = (const GLchar *)source;

        CeU32 shader = glCreateShader(type);
        glShaderSource(shader, 1, &newSrc, 0);
        glCompileShader(shader);

        GLint isCompiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

        if (isCompiled == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

            std::vector<GLchar> infoLog(maxLength);
            glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

            CE_CORE_ERROR("Shader compilation failed.");
            CE_CORE_ERROR("(%s %s) %s", type == GL_VERTEX_SHADER ? "Vertex" : "Fragment", source, infoLog.data());

            glDeleteShader(shader);
            return (GLuint)0;
        }

        return shader;
    };

    void Shader::Compile(const std::string &vertexSource, const std::string &fragmentSource)
    {
        id = 0;
        valid = false;
        CeU32 vertexShader = LoadShader(Engine::ReadFileContent(vertexSource).c_str(), GL_VERTEX_SHADER);
        CeU32 fragmentShader = LoadShader(Engine::ReadFileContent(fragmentSource).c_str(), GL_FRAGMENT_SHADER);

        id = glCreateProgram();
        glAttachShader(id, vertexShader);
        glAttachShader(id, fragmentShader);
        glLinkProgram(id);

        GLint isLinked = 0;
        glGetProgramiv(id, GL_LINK_STATUS, (int *)&isLinked);
        if (isLinked == GL_FALSE)
        {
            GLint maxLength = 0;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &maxLength);
            std::vector<GLchar> infoLog(maxLength);
            glGetProgramInfoLog(id, maxLength, &maxLength, &infoLog[0]);
            glDeleteProgram(id);

            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);

            CE_CORE_ERROR("ProgramError: %s", infoLog.data());
        }
        valid = true;

        // Delete shaders
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    CeU32 Shader::GetUniLoc(const char *n)
    {
        if (!valid)
            return 0;

        Use();
        return glGetUniformLocation(id, n);
    }

    Shader::Shader(const ShaderConstructor &constructor)
    {
        Compile(constructor.VertexSource, constructor.FragmentSource);
    }

    Shader::Shader(const std::string &vertexSource, const std::string &fragmentSource)
    {
        Compile(vertexSource, fragmentSource);
    }

    Shader::~Shader()
    {
        glDeleteShader(id);
    }

    void Shader::Use()
    {
        glUseProgram(id);
    }

    void Shader::Int(int i, const char *name)
    {
        if (!valid)
            return;
        Use();
        glUniform1i(GetUniLoc(name), i);
    }

    void Shader::Vec4(const Color &color, const char *name)
    {
        if (!valid)
            return;
        Use();
        glUniform4f(GetUniLoc(name), color.r / 255, color.g / 255, color.b / 255, color.a / 255);
    }

    void Shader::Mat4(const Matrix4 &matrix, const char *name)
    {
        Mat4(matrix.data, name);
    }

    void Shader::Mat4(Matrix4 *matrix, const char *name)
    {
        Mat4(matrix->data, name);
    }

    void Shader::Mat4(const float *matrix, const char *name)
    {
        if (!valid)
            return;

        Use();
        glUniformMatrix4fv(GetUniLoc(name), 1, false, matrix);
    }
}