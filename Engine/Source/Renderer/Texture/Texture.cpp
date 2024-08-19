#include "Texture.h"
#include "Core/Logger.h"
#include <glad/glad.h>

namespace Core
{
    static CeU32 GGen = 0;
    void Texture::DestroyImageIfExistent()
    {
        if (!image)
            return;
        delete image;
        image = nullptr;
    }

    Texture::Texture()
    {
        id = 0;
        image = nullptr;
        generation = GGen;
        GGen++;

        Load();
    }

    Texture::~Texture()
    {
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
        DestroyImageIfExistent();
        image = new Image(imagePath);

        if (!image->IsValid())
        {
            CE_WARN("Invalid image source: %s.", imagePath.c_str());
            return;
        }

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

}