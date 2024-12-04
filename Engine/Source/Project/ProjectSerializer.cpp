#include "ProjectSerializer.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace Core
{
    void ProjectSerializer::Serialize(const std::string &name)
    {
        auto &config = Target->config;
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Project";
        out << YAML::Key << YAML::BeginMap;

        CE_SERIALIZE_FIELD("Name", config.Name);
        CE_SERIALIZE_FIELD("AssetDirectory", config.AssetDirectory);
        CE_SERIALIZE_FIELD("StartScenePath", config.StartScenePath);

        out << YAML::EndMap;

        std::ofstream fout(name);
        fout << out.c_str();
    }

    void ProjectSerializer::Deserialize(const std::string &name)
    {
        std::ifstream stream(name);
        std::stringstream strStream(name);
        strStream << stream.rdbuf();
        YAML::Node data = YAML::Load(strStream.str());

        if (!data["Project"])
            return;

        auto prj = data["Project"];
        auto &config = Target->config;
        config.Name = prj["Name"].as<std::string>();
        config.AssetDirectory = prj["AssetDirectory"].as<std::string>();
        config.StartScenePath = prj["StartScenePath"].as<std::string>();
    }
}