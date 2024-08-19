#pragma once

#include "Base.h"
#include "Material.h"
#include <string>

namespace Core
{
    class MaterialSystem
    {
    public:
        MaterialSystem() {}
        ~MaterialSystem() {}

        static void Init();
        static void Shutdown();

        // static void Load(const std::string &path); // TODO: Implement (when material from files exist) NOTE: Materials cannot be form config here (for now)

        static void ReleaseDefault(Material* mat);

        static Material *Get(const std::string &name);
        static Material *GetDefault();

        static void Release(const std::string &name);
    };
}
