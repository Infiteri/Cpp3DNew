#include "ProjectSerializer.h"
#include "Core/Logger.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace Core
{
    void ProjectSerializer::Serialize(const std::string &name)
    {
        if (!Target)
        {
            CE_CORE_ERROR("Unable to serialize project as its invalid.");
            return;
        }
        auto &config = Target->config;
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Project";
        out << YAML::Key << YAML::BeginMap;

        CE_SERIALIZE_FIELD("Name", config.Name);
        CE_SERIALIZE_FIELD("AssetDirectory", config.AssetDirectory);
        CE_SERIALIZE_FIELD("StartScenePath", config.StartScenePath);
        CE_SERIALIZE_FIELD("ScriptLibraryPath", config.ScriptLibraryPath);
        CE_SERIALIZE_FIELD("ScriptBuildTask", config.ScriptBuildTask);

        out << YAML::EndMap;

        std::ofstream fout(name);
        fout << out.c_str();
    }

    bool ProjectSerializer::Deserialize(const std::string &name)
    {
        if (!Target)
        {
            CE_CORE_ERROR("Unable to deserialize project as its invalid.");
            return false;
        }

        // TODO: Necessary?
        Target->config.Name = "";

        std::ifstream stream(name);
        std::stringstream strStream(name);
        strStream << stream.rdbuf();
        if (!strStream.good())
        {
            CE_CORE_ERROR("Unable to deserialize project as file is invalid at path '%s'.", name.c_str());
            return false;
        }

        YAML::Node data = YAML::Load(strStream.str());

        if (!data || !data["Project"])
        {
            CE_CORE_ERROR("Unable to load project '%s', file format not valid or inexistent.", name.c_str());
            return false;
        }

        auto prj = data["Project"];
        auto &config = Target->config;
        config.Name = prj["Name"].as<std::string>();
        config.AssetDirectory = prj["AssetDirectory"].as<std::string>();
        config.StartScenePath = prj["StartScenePath"].as<std::string>();
        config.ScriptLibraryPath = prj["ScriptLibraryPath"].as<std::string>();
        config.ScriptBuildTask = prj["ScriptBuildTask"].as<std::string>();

        return true;
    }
}