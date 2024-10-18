#pragma once

#include "Base.h"
#include "BaseTexture.h"
#include "Resources/Image.h"

#include <vector>
#include <string>

namespace Core
{

    class CubeMapTexture : public BaseTexture
    {
    public:
        struct Configuration
        {
            std::string Left;
            std::string Right;
            std::string Top;
            std::string Bottom;
            std::string Front;
            std::string Back;
            TextureConfiguration TextureConfig;

            void From(Configuration *config);
        };

    private:
        std::vector<Image *> images;
        Configuration config;

        void ReGenImages();
        void FreeImagesData();

    public:
        CubeMapTexture();
        CubeMapTexture(Configuration &config);
        ~CubeMapTexture();

        void Load();
        void Destroy();
        void Bind();
        void Use();

        void Load(Configuration &config);

        inline Configuration &GetConfig() { return config; };
    };
}