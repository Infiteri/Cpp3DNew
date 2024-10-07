#include "TextureSystem.h"
#include "Core/Logger.h"
#include <unordered_map>

namespace Core
{

    struct TextureReference
    {
        int count;
        Texture *texture;
    };

    static inline void _CheckAndDeleteTextureReference(TextureReference *reference)
    {
        if (!reference)
        {
            CE_LOG("CE_TEXTURE", Warn, "Undefined texture reference.");
            return;
        }

        if (!reference->texture)
        {
            CE_LOG("CE_TEXTURE", Warn, "Undefined texture in texture reference.");
            return;
        }

        if (reference->count > 0)
        {
            CE_LOG("CE_TEXTURE", Warn, "Texture reference count is over 0 (%i).0", reference->count);
            return;
        }

        delete reference->texture;
        delete reference;
    };

    /// @brief Will also decrement count
    static bool _ReleaseTextureNoLogging(TextureReference *reference)
    {

        if (!reference || !reference->texture)
            return false;

        reference->count--;
        if (reference->count > 0)
            return true;

        delete reference->texture;
        delete reference;
        return true;
    }

    static std::unordered_map<std::string, TextureReference *> textures;
    static TextureReference defaultTexture;

    void TextureSystem::Init()
    {
        CE_DEFINE_LOG_CATEGORY("CE_TEXTURE", "Texture System");

        defaultTexture.count = 0;
        defaultTexture.texture = new Texture();
    }

    void TextureSystem::Shutdown()
    {
        for (auto it : textures)
        {
            _CheckAndDeleteTextureReference(it.second);
        }

        _CheckAndDeleteTextureReference(&defaultTexture);

        textures.clear();
    }

    void TextureSystem::Release(const std::string &name)
    {
        auto ref = textures[name];
        if (!ref)
        {
            CE_LOG("CE_TEXTURE", Warn, "Texture not found, '%s'.", name.c_str());
            return;
        }

        const int refCount = ref->count--;
        if (refCount <= 0)
        {
            delete ref->texture;
            textures.erase(name);
        }
    }

    void TextureSystem::ReleaseDefault(Texture *texture)
    {
        defaultTexture.count--;
        texture = nullptr;
    }

    Texture *TextureSystem::GetDefault()
    {
        defaultTexture.count++;

        if (!defaultTexture.texture)
        {
            CE_LOG("CE_TEXTURE", Warn, "Default texture not created.");
            defaultTexture.count--; // Decrease, todo: is good or nah.
        }

        return defaultTexture.texture;
    }

    void TextureSystem::Load(const std::string &name, const TextureConfiguration &ref)
    {
        if (!textures[name])
        {
            textures[name] = new TextureReference;
            textures[name]->count = 0;
            textures[name]->texture = new Texture();
            textures[name]->texture->Load(name, ref);

            CE_LOG("CE_TEXTURE", Info, "New texture loaded: %s.", name.c_str());
        }
    }

    Texture *TextureSystem::Get(const std::string &name)
    {
        if (textures.find(name) == textures.end())
            Load(name);

        textures[name]->count++;
        return textures[name]->texture;
    }

    Texture *TextureSystem::Get(TextureConfiguration &config)
    {
        if (textures.find(config.FilePath) == textures.end())
            Load(config.FilePath, config);

        textures[config.FilePath]->count++;
        return textures[config.FilePath]->texture;
    }
}