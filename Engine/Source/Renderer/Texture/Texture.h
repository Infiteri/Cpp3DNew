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
        /// @brief Basic texture constructor.
        /// @param autoload Weather or not to autoload the texture, if you know what you are doing, making this false is better.
        Texture(bool autoload = true);
        ~Texture();

        /// @brief Will load a white texture. (A.K.A default)
        void Load();

        /// @brief Will load from an image.
        /// @param imagePath The image path.
        /// @param texConfig The configuration to use.
        void Load(const std::string &imagePath, const TextureConfiguration &texConfig);
        void Destroy();
        void Bind();
        void Use();

        void UpdateWithConfig(TextureConfiguration &config);

        inline TextureConfiguration &GetConfig() { return config; };
        inline bool HasImage() { return image != nullptr; };
        inline std::string GetImagePath()
        {
            if (!HasImage())
                return "";
            return image->GetPath();
        }
        inline Image *GetImage() { return image; };

        inline float GetWidth()
        {
            if (!image)
                return 0.0f;
            return image->GetWidth();
        };

        inline float GetHeight()
        {
            if (!image)
                return 0.0f;
            return image->GetHeight();
        };
    };
}