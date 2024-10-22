#include "CubeMapLoader.h"

#include <yaml-cpp/yaml.h>
#include <fstream>

namespace Core
{
    void CubeMapLoader::Deserialize(const std::string &name, CubeMapTexture::Configuration *config)
    {
        if (!config)
            return;

        std::ifstream stream(name);
        std::stringstream strStream(name);
        strStream << stream.rdbuf();
        YAML::Node data = YAML::Load(strStream.str());

        if (!data["CubeMap"])
            return;

        config->Left = data["Left"].as<std::string>();
        config->Right = data["Right"].as<std::string>();
        config->Top = data["Top"].as<std::string>();
        config->Bottom = data["Bottom"].as<std::string>();
        config->Front = data["Front"].as<std::string>();
        config->Back = data["Back"].as<std::string>();
        config->TextureConfig.MinFilter = (TextureFilter)data["MinFilter"].as<int>();
        config->TextureConfig.MagFilter = (TextureFilter)data["MagFilter"].as<int>();
    }

    void CubeMapLoader::Serialize(const std::string &name, CubeMapTexture::Configuration *config)
    {
        if (!config)
            return;

        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "CubeMap";
        out << YAML::Value << "Value";

        out << YAML::Key << "Left" << YAML::Value << config->Left.c_str();
        out << YAML::Key << "Right" << YAML::Value << config->Right.c_str();
        out << YAML::Key << "Top" << YAML::Value << config->Top.c_str();
        out << YAML::Key << "Bottom" << YAML::Value << config->Bottom.c_str();
        out << YAML::Key << "Front" << YAML::Value << config->Front.c_str();
        out << YAML::Key << "Back" << YAML::Value << config->Back.c_str();
        out << YAML::Key << "MinFilter" << YAML::Value << (int)config->TextureConfig.MinFilter;
        out << YAML::Key << "MagFilter" << YAML::Value << (int)config->TextureConfig.MagFilter;

        out << YAML::EndMap;

        std::ofstream fout(name);
        fout << out.c_str();
    }
}