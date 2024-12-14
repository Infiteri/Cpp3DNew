#include "EditorSetting.h"

#include <fstream>
#include <yaml-cpp/yaml.h>

namespace Core
{
    static YAML::Emitter &operator<<(YAML::Emitter &out, Color *color)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << color->r << color->g << color->b << color->a << YAML::EndSeq;
        return out;
    }

    EditorSettingsSerializer::EditorSettingsSerializer(EditorSettings *s)
    {
        Settings = s;
    }

    EditorSettingsSerializer::~EditorSettingsSerializer()
    {
    }

    void EditorSettingsSerializer::Serialize()
    {
        if (!Settings)
            return;

        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Settings";
        out << YAML::Value << "Field";

        CE_SERIALIZE_FIELD("CameraFOV", Settings->CameraFOV);
        CE_SERIALIZE_FIELD("FontSize", Settings->FontSize);

        out << YAML::Key << "Colors";
        out << YAML::Value << YAML::BeginSeq;

        for (auto it : Settings->Colors)
        {
            auto &colors = ImGui::GetStyle().Colors;
            Color color{colors[it.Color].x, colors[it.Color].y, colors[it.Color].z, colors[it.Color].w};

            out << YAML::BeginMap;
            out << YAML::Key << "Name" << YAML::Value << it.Name.c_str();
            out << YAML::Key << "Color" << YAML::Value << (int)it.Color;
            CE_SERIALIZE_FIELD("Color Value", &color);
            out << YAML::EndMap;
        }

        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(SettingsFilePath);
        fout << out.c_str();
    }

    bool EditorSettingsSerializer::Deserialize()
    {
        if (!Settings)
            return false;

        std::ifstream stream(SettingsFilePath);
        std::stringstream strStream(SettingsFilePath);

        strStream << stream.rdbuf();

        if (!stream.good())
            return false;

        YAML::Node data = YAML::Load(strStream.str());

        if (data.IsNull() || !data["Settings"])
            return false;

        Settings->CameraFOV = data["CameraFOV"].as<float>();
        Settings->FontSize = data["FontSize"].as<float>();

        auto colors = data["Colors"];
        if (colors)
        {
            for (auto c : colors)
            {
                auto &imColors = ImGui::GetStyle().Colors;
                int valid = c["Color"].as<int>();
                imColors[(ImGuiCol)c["Color"].as<int>()].x = c["Color Value"][0].as<float>();
                imColors[(ImGuiCol)c["Color"].as<int>()].y = c["Color Value"][1].as<float>();
                imColors[(ImGuiCol)c["Color"].as<int>()].z = c["Color Value"][2].as<float>();
                imColors[(ImGuiCol)c["Color"].as<int>()].w = c["Color Value"][3].as<float>();
            }
        }

        return true;
    }
}