#pragma once

#include "Base.h"

namespace Core
{
    enum class TextureFilter
    {   
        Linear,
        Nearest,
    };

    struct TextureConfiguration
    {
        TextureFilter MinFilter = TextureFilter::Linear;
        TextureFilter MagFilter = TextureFilter::Linear;
    };

    class BaseTexture
    {
    public:
    protected:
        CeU32 id;
        CeU32 generation;

        CeU32 FilterIntToGLType(CeU32 filter);
        CeU32 FilterToGLType(TextureFilter filter);
        void TextureLoadUtils(int w, int h, CeU8 *data, CeU32 channel, TextureConfiguration config);

    public:
        BaseTexture();
        virtual ~BaseTexture();

        virtual void Load() = 0;
        virtual void Destroy() = 0;
        virtual void Bind() = 0;

        inline CeU32 GetID() { return id; };
        inline CeU32 GetGeneration() { return generation; };
    };
}