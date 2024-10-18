#include "Texture.h"
#include "Core/Logger.h"
#include "TextureSystem.h"
#include <glad/glad.h>

namespace Core
{
    void Texture::DestroyImageIfExistent()
    {
        if (!image)
            return;
        delete image;
        image = nullptr;
    }

    void Texture::DestroyGLTexture()
    {
        glDeleteTextures(1, &id);
        id = 0;
    }

    Texture::Texture()
    {
        id = 0;
        image = nullptr;
        generation = TextureSystem::GetNewTextureGeneration();

        Load();
    }

    Texture::~Texture()
    {
        TextureSystem::DecrementTextureGeneration();
        Destroy();
    }

    void Texture::Load()
    {
        glGenTextures(1, &id);
        Bind();

        CeU8 data[4] = {255, 255, 255, 255};
        TextureLoadUtils(1, 1, data, GL_RGBA, {});
    }

    void Texture::Load(const std::string &imagePath, TextureConfiguration texConfig)
    {
        DestroyGLTexture();
        DestroyImageIfExistent();
        image = new Image(imagePath);

        if (!image->IsValid())
        {
            CE_WARN("Invalid image source: %s.", imagePath.c_str());
            return;
        }

        config.MinFilter = texConfig.MinFilter;
        config.MagFilter = texConfig.MagFilter;

        glGenTextures(1, &id);
        Bind();
        TextureLoadUtils(image->GetWidth(), image->GetHeight(), image->GetData(), image->GetChannels(), texConfig);
        image->FreeData(); // Free memory
    }

    void Texture::Destroy()
    {
        glDeleteTextures(1, &id);
    }

    void Texture::Bind()
    {
        glBindTexture(GL_TEXTURE_2D, id);
    }

    void Texture::Use()
    {
        glActiveTexture(GL_TEXTURE0 + generation);
        Bind();
    }

    void Texture::UpdateWithConfig(TextureConfiguration &config)
    {
        if (id != 0)
            DestroyGLTexture();

        this->config.MinFilter = config.MinFilter;
        this->config.MagFilter = config.MagFilter;

        glGenTextures(1, &id);
        Bind();

        if (image != nullptr)
        {
            std::string texName = image->GetPath();
            DestroyImageIfExistent();
            image = new Image(texName);
            TextureLoadUtils(image->GetWidth(), image->GetHeight(), image->GetData(), image->GetChannels(), config);
            image->FreeData();
        }
        else
        {
            CeU8 data[4] = {255, 255, 255, 255};
            TextureLoadUtils(1, 1, data, 4, {});
        }
    }
}