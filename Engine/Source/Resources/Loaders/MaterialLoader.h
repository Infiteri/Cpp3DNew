#pragma once

#include "Base.h"
#include "Renderer/Material/Material.h"

#include <string>

namespace YAML
{
    class Emitter;
    class Node;
};

namespace Core
{
    class CE_API MaterialLoader
    {
    public:
        MaterialLoader() {};
        ~MaterialLoader() {};

        void Deserialize(const std::string &name, Material::Configuration *config);
        void Serialize(const std::string &name, Material *mat);

        void DeserializeNonSpecific(YAML::Node data, Material::Configuration *mat);
        void SerializeNonSpecific(YAML::Emitter &out, Material *mat);
    };
}