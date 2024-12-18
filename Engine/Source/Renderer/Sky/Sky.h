#pragma once

#include "Base.h"
#include "Renderer/Buffer/VertexArray.h"
#include "Renderer/Texture/CubeMapTexture.h"
#include "Renderer/Color.h"
#include "Renderer/Shader/Shader.h"

#include "Core/Data/CeDataSet.h"

#include <string>

namespace Core
{
    class CE_API Sky
    {
    public:
        enum Mode
        {
            ColorMode,
            CubeMapMode,
            ShaderMode
        };

    private:
        Mode mode;

        CubeMapTexture *cubeTexture = nullptr;
        std::string cubeMapPath;

        VertexArray *array = nullptr;
        Color color;
        std::string shaderName = "";
        Shader *shader = nullptr;

        CeDataSet shaderData;

        void DestroyCubeMap();
        void DestroyShader();
        void DestroyFromMode();

        void ReloadNewModeWithPreviousData();

    public:
        Sky();
        ~Sky();

        void Render();

        inline Mode GetMode() { return mode; };
        void SetMode(Mode newMode);
        void SetModeToColor(const Color &c);
        void SetModeToCubeMap(CubeMapTexture::Configuration &config);
        void SetModeToCubeMap(const std::string &configPath);
        void SetModeToShader(const std::string &shader);

        inline Color &GetColor() { return color; };
        inline std::string GetShaderName() { return shaderName; };
        inline std::string GetCubemapPath() { return cubeMapPath; };
        void SetColor(const Color &color);

        void From(Sky *other);

        inline CeDataSet &GetShaderDataSet() { return shaderData; };
    };
}