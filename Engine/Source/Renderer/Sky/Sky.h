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
        VertexArray *array = nullptr;
        Color color;

        struct CubemapState
        {
            CubeMapTexture *texture = nullptr;
            std::string path;
        } cubemap;

        struct ShaderState
        {
            std::string name = "";
            Shader *shader = nullptr;
            CeDataSet data;
        } shader;

        void _DestroyCubeMap();
        void _DestroyShader();
        void _DestroyFromMode();
        void _ReloadNewModeWithPreviousData();

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
        void SetColor(const Color &color);

        inline Color &GetColor() { return color; };
        inline std::string GetShaderName() { return shader.name; };
        inline std::string GetCubemapPath() { return cubemap.path; };
        inline CeDataSet &GetShaderDataSet() { return shader.data; };
        
        void From(Sky *other);
    };
}