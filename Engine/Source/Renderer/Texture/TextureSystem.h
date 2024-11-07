#pragma once

#include "Base.h"
#include "Texture.h"
#include <string>

namespace Core
{
    class TextureSystem
    {
    public:
        TextureSystem() {}
        ~TextureSystem() {}

        static void Init();
        static void Shutdown();

        static bool Exist(const std::string& name);

        static void Release(const std::string &name);
        static void ReleaseDefault(Texture *texture);

        static Texture *GetDefault();

        static void Load(const std::string &name, const TextureConfiguration &ref = {});
        static Texture *Get(const std::string &name);
        static Texture *Get(TextureConfiguration &config);

        static CeU32 GetNewTextureGeneration();
        static void DecrementTextureGeneration();
        static void StartNewSceneGeneration();
    };

}