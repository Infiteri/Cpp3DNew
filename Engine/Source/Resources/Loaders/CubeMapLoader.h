#pragma once

#include "Base.h"
#include "Renderer/Texture/CubeMapTexture.h"

#include <string>

namespace Core
{
    class CE_API CubeMapLoader
    {
    public:
        CubeMapLoader() {};
        ~CubeMapLoader() {};

        void Deserialize(const std::string &name, CubeMapTexture::Configuration *config);
        void Serialize(const std::string &name, CubeMapTexture::Configuration *config);
    };
}