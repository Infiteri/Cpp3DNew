#include "MaterialLoader.h"
#include "Core/Serializer/CeSerializerUtils.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

#define CE_SERIALIZE_FIELD(name, value) out << YAML::Key << name << YAML::Value << value

namespace Core
{
    void MaterialLoader::Deserialize(const std::string &name, Material::Configuration *config)
    {
        std::ifstream stream(name);
        std::stringstream strStream(name);
        strStream << stream.rdbuf();
        YAML::Node data = YAML::Load(strStream.str());
        if (!data["Material"])
            return;
        DeserializeNonSpecific(data, config);
    }

    void MaterialLoader::Serialize(const std::string &name, Material *mat)
    {
        if (!mat)
            return;

        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Material";
        out << YAML::Value << "Value";
        SerializeNonSpecific(out, mat);
        out << YAML::EndMap;

        std::ofstream fout(name);
        fout << out.c_str();
    }

    void MaterialLoader::DeserializeNonSpecific(YAML::Node data, Material::Configuration *config)
    {

        config->Name = data["Name"].as<std::string>();
        SerializerUtils::YAMLToColor(data["Color"], &config->Color);
        // TODO: Texture
        config->TextureConfig.FilePath = data["TextureName"].as<std::string>();
        config->TextureConfig.MinFilter = (TextureFilter)data["TextureMin"].as<int>();
        config->TextureConfig.MagFilter = (TextureFilter)data["TextureMag"].as<int>();
    }

    void MaterialLoader::SerializeNonSpecific(YAML::Emitter &out, Material *mat)
    {
        Color c = mat->GetState().Color;
        CE_SERIALIZE_FIELD("Name", mat->GetState().Name);
        SerializerUtils::ColorToYAML(out, "Color", &c);
        CE_SERIALIZE_FIELD("TextureName", mat->GetColorTexture()->GetImagePath().c_str());
        CE_SERIALIZE_FIELD("TextureMin", (int)mat->GetColorTexture()->GetConfig().MinFilter);
        CE_SERIALIZE_FIELD("TextureMag", (int)mat->GetColorTexture()->GetConfig().MagFilter);
    }
}