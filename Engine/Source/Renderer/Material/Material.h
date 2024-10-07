#pragma once

#include "Base.h"
#include "Renderer/Color.h"
#include "Renderer/Texture/Texture.h"

namespace Core
{
    class CE_API Material
    {
    public:
        enum MaterialTypes
        {
            /// @brief White material
            Default,

            /// @brief From configuration, a.k.a "Unique"
            Config,

            /// @brief From file, TODO: Implement
            // File
        };

        struct Configuration
        {
            std::string Name;
            Color Color;
            TextureConfiguration TextureConfig;

            void From(Configuration *config);
        };

    private:
        Configuration state; // Note: Configuration is just a state.
        MaterialTypes type = Default;

        struct TexturePair
        {
            Texture *texture = nullptr;
            bool isDefault = true;
        };

        TexturePair colorTexture;
        void ReleaseTexture(TexturePair *pair);

    public:
        Material();
        ~Material();

        void Use();

        void Create();
        void Create(Configuration *config);
        void MakeDefault();

        inline Configuration &GetState() { return state; };
        inline MaterialTypes GetType() { return type; };

        inline Texture *GetColorTexture() { return colorTexture.texture; };

        /// @brief Do not call if you dont know what you doing (ultra bad btw)
        /// @param t The new type
        void SetType(MaterialTypes t) { type = t; };

        /// @brief Will set the texture to the default white.
        void SetColorTexture();

        /// @brief Will set the texture from the name, name being its path with no configuration.
        /// @note For using any kind of configuration, use the other overload, FilePath being equal to name.
        /// @param name The name of the texture / path of the image.
        void SetColorTexture(const std::string &name /* const TextureConfiguration &ref = {}*/);

        /// @brief Will set the texture from configuration.
        /// @param configuration The configuration to use.
        void SetColorTexture(TextureConfiguration &configuration);

        /// @brief Will set the color of the material.
        /// @param color The color to use.
        void SetColor(const Color& color);
    };
}