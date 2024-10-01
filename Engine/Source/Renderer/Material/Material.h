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
        void SetColorTexture(const std::string &name /* const TextureConfiguration &ref = {}*/);
    };
}