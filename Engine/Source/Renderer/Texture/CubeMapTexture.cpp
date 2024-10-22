#include "CubeMapTexture.h"
#include "Renderer/Texture/TextureSystem.h"
#include <glad/glad.h>

namespace Core
{
    void CubeMapTexture::Configuration::From(Configuration *config)
    {
        Left = config->Left;
        Right = config->Right;
        Top = config->Top;
        Bottom = config->Bottom;
        Front = config->Front;
        Back = config->Back;

        TextureConfig.MinFilter = config->TextureConfig.MinFilter;
        TextureConfig.MagFilter = config->TextureConfig.MagFilter;
    }

    void CubeMapTexture::ReGenImages()
    {
        if (images.size() != 0)
            for (auto image : images)
                delete image;

        images.clear();

        std::vector<std::string> paths = {
            config.Left, config.Right, config.Top, config.Bottom, config.Front, config.Back};

        for (int i = 0; i < 6; i++)
        {
            images.push_back(new Image(paths[i]));
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB8, images[i]->GetWidth(), images[i]->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, images[i]->GetData());
        }
    }

    void CubeMapTexture::FreeImagesData()
    {
        for (auto image : images)
            image->FreeData();
    }

    CubeMapTexture::CubeMapTexture()
    {
        Load();
    }

    CubeMapTexture::CubeMapTexture(Configuration &config)
    {
        Load(config);
    }

    CubeMapTexture::~CubeMapTexture()
    {
        Destroy();
    }

    void CubeMapTexture::Load()
    {
        glGenTextures(1, &id);
        Bind();

        CeU8 data[3] = {0, 0, 0};

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    }

    void CubeMapTexture::Load(Configuration &config)
    {
        glGenTextures(1, &id);
        Bind();

        this->config = config;

        ReGenImages();

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, FilterToGLType(config.TextureConfig.MinFilter));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, FilterToGLType(config.TextureConfig.MagFilter));
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        FreeImagesData();
    }

    void CubeMapTexture::Bind()
    {
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);
    }

    void CubeMapTexture::Use()
    {
        GenGeneration();
        glActiveTexture(GL_TEXTURE0 + generation);
        Bind();
    }

    void CubeMapTexture::Destroy()
    {
        for (Image *img : images)
            if (img != nullptr)
                delete img;

        glDeleteTextures(1, &id);
    }
}