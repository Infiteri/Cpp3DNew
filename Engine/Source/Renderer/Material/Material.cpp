#include "Material.h"
#include "Renderer/Shader/ShaderSystem.h"
#include "Renderer/Texture/TextureSystem.h"

#include "Core/Logger.h"

namespace Core
{
    void Material::Configuration::From(Configuration *config)
    {
        Name = config->Name;
        Color.Set(config->Color);
        TextureConfig.FilePath = config->TextureConfig.FilePath;
        TextureConfig.MinFilter = config->TextureConfig.MinFilter;
        TextureConfig.MagFilter = config->TextureConfig.MagFilter;
    }

    void Material::ReleaseTexture(TexturePair *pair)
    { // TODO: Better system?
        if (pair->texture)
        {
            if (pair->isDefault)
                TextureSystem::ReleaseDefault(pair->texture);
            else
                TextureSystem::Release(pair->texture->GetImagePath());
        }
    }

    Material::Material()
    {
        MakeDefault();
    }

    Material::~Material()
    {
        ReleaseTexture(&colorTexture);
    }

    void Material::Use()
    {
        auto shader = ShaderSystem::GetFromEngineResource("Object");
        if (!shader)
            return;

        shader->Use();
        shader->Vec4(state.Color, "uColor");
        colorTexture.texture->Use();
        shader->Int(colorTexture.texture->GetGeneration(), "uColorTexture");
    }

    void Material::Create()
    {
        MakeDefault();
    }

    void Material::Create(Configuration *config)
    {
        this->state.From(config);
        SetType(Config);
        SetColorTexture(config->TextureConfig);
    }

    void Material::MakeDefault()
    {
        SetType(Default);
        state.Color.Set(255, 255, 255, 255);
        SetColorTexture();
    }

    void Material::SetColorTexture()
    {
        if (colorTexture.texture)
            ReleaseTexture(&colorTexture);

        colorTexture.texture = TextureSystem::GetDefault();
    }

    void Material::SetColorTexture(const std::string &name /* const TextureConfiguration &ref */)
    {
        if (colorTexture.texture)
            ReleaseTexture(&colorTexture);

        colorTexture.texture = TextureSystem::Get(name); // TODO: use configuration
    }

    void Material::SetColorTexture(TextureConfiguration &configuration)
    {
        if (configuration.INTERNAL_IGNORE)
            return;

        if (colorTexture.texture)
            ReleaseTexture(&colorTexture);

        colorTexture.texture = TextureSystem::Get(configuration); // TODO: use configuration
    }

    void Material::SetColor(const Color &color)
    {
        state.Color.Set(color);
    }
}