#include "BaseTexture.h"
#include "Renderer/Texture/TextureSystem.h"
#include <glad/glad.h>

namespace Core
{
    void BaseTexture::GenGeneration()
    {
        if (!INTERNAL_REQUIRE_GENERATION)
            return;

        // if (!hasGeneration)
        {
            generation = TextureSystem::GetNewTextureGeneration();
            hasGeneration = true;
        }
    }

    CeU32 BaseTexture::FilterIntToGLType(CeU32 filter)
    {
        switch (filter)
        {
        case 3:
            return GL_RGB;
            break;

        default:
        case 4:
            return GL_RGBA;
            break;
        }
    }

    CeU32 BaseTexture::FilterToGLType(TextureFilter filter)
    {
        switch (filter)
        {
        case TextureFilter::Nearest:
            return GL_NEAREST;
            break;

        default:
        case TextureFilter::Linear:
            return GL_LINEAR;
            break;
        }
    }

    void BaseTexture::TextureLoadUtils(int w, int h, CeU8 *data, CeU32 channel, TextureConfiguration config)
    {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, FilterIntToGLType(channel), w, h, 0, FilterIntToGLType(channel), GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterToGLType(config.MinFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterToGLType(config.MagFilter));

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    BaseTexture::BaseTexture()
    {
    }

    BaseTexture::~BaseTexture()
    {
    }

}