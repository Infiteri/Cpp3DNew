#include "MaterialSystem.h"

#include "Core/Logger.h"
#include <unordered_map>

namespace Core
{

    struct MaterialReference
    {
        int count = 0;
        Material *material;
    };

    static inline void _CheckAndDeleteMaterialReference(MaterialReference *reference)
    {
        if (!reference)
        {
            CE_LOG("CE_MATERIAL", Warn, "Undefined material reference.");
            return;
        }

        if (!reference->material)
        {
            CE_LOG("CE_MATERIAL", Warn, "Undefined material in material reference.");
            return;
        }

        if (reference->count > 0)
        {
            CE_LOG("CE_MATERIAL", Warn, "Material reference count is over 0 (%i).0", reference->count);
            return;
        }

        delete reference->material;
    };

    /// @brief Will also decrement count
    static bool _ReleaseMaterialNoLogging(MaterialReference *reference)
    {
        if (!reference || !reference->material)
            return false;

        reference->count--;
        if (reference->count > 0)
            return true;

        delete reference->material;
        delete reference;
        return true;
    }

    static std::unordered_map<std::string, MaterialReference *> references;
    static MaterialReference defaultMaterial;

    void MaterialSystem::Init()
    {
        CE_DEFINE_LOG_CATEGORY("CE_MATERIAL", "Material System");

        defaultMaterial.count = 0;
        defaultMaterial.material = new Material();
        defaultMaterial.material->SetColorTexture(); // To default
        defaultMaterial.material->SetType(Material::Default);
    }

    void MaterialSystem::Shutdown()
    {
        for (auto it = references.begin(); it != references.end(); it++)
        {
            _CheckAndDeleteMaterialReference(it->second);
        }

        _CheckAndDeleteMaterialReference(&defaultMaterial);
        references.clear();
    }

    // void MaterialSystem::Load(Material::Configuration &config)
    // {
    //     if (references.find(config.Name)->second != nullptr)
    //         return;

    //     auto reference = (references[config.Name] = new MaterialReference);
    //     reference->count = 0;
    //     reference->material = new Material();
    //     reference->material->Create(&config);
    //     reference->material->SetType(Materials::Config);
    // }

    void MaterialSystem::ReleaseDefault(Material *mat)
    {
        defaultMaterial.count--;
        mat = nullptr;
    }

    Material *MaterialSystem::Get(const std::string &name)
    {
        if (references.find(name)->second == nullptr)
            return nullptr;

        references[name]->count++;
        return references[name]->material;
    }

    Material *MaterialSystem::GetDefault()
    {
        defaultMaterial.count++;
        return defaultMaterial.material;
    }

    void MaterialSystem::Release(const std::string &name)
    {
        if (name.empty())
            return;

        if (references.find(name)->second == nullptr)
            return;

        _CheckAndDeleteMaterialReference(references[name]);
    }
}