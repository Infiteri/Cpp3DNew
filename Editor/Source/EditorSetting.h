#pragma once

#include "Core.h"

#include <string>
#include <vector>

#include <ImGui.h>

namespace Core
{
    struct EditorSettings
    {
        struct ImGuiEditableColor
        {
            std::string Name;
            ImGuiCol Color;

            ImGuiEditableColor(const std::string &n, ImGuiCol c) : Name(n), Color(c) {};
        };

        // Colors
        std::vector<ImGuiEditableColor> Colors;

        // Camera
        float CameraFOV = 120.0f;

        // Font styles
        float FontSize = 12.0f;
    };

    class EditorSettingsSerializer : public CeSerializer
    {
    public:
        std::string SettingsFilePath = "CeEditor.ce_ed_set";
        EditorSettings *Settings;

        EditorSettingsSerializer(EditorSettings *s);
        ~EditorSettingsSerializer();

        void Serialize();
        void Deserialize();
    };
}