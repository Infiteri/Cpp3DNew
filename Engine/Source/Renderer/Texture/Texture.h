#pragma once

#include "Base.h"
#include "BaseTexture.h"
#include "Resources/Image.h"
#include "Renderer/Color.h"

namespace Core
{
    class CE_API Texture : public BaseTexture
    {
    private:
        Image *image = nullptr;
        TextureConfiguration config;

        void DestroyImageIfExistent();
        void DestroyGLTexture();

    public:
        Texture();
        ~Texture();

        /// @brief Will load a white texture. (A.K.A default)
        void Load();

        /// @brief Will load from an image.
        /// @param imagePath The image path.
        /// @param texConfig The configuration to use.
        void Load(const std::string &imagePath, TextureConfiguration texConfig);
        void Destroy();
        void Bind();
        void Use();

        void UpdateWithConfig(TextureConfiguration& config);

        inline TextureConfiguration &GetConfig() { return config; };
        inline bool HasImage() { return image != nullptr; };
        inline std::string GetImagePath()
        {
            if (!HasImage())
                return "";
            return image->GetPath();
        }
        inline Image *GetImage() { return image; };
    };
}