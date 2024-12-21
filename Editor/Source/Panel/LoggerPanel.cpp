#include "LoggerPanel.h"
#include "Core/Logger.h"
#include "EditorUtils.h"

namespace Core
{
    static int LastLogCount;
    static std::string NameString;
    static std::string LevelToString = "All";

    LoggerPanel::LoggerPanel()
    {
        LastLogCount = Logger::GetLogInfos().size();
    }

    void LoggerPanel::OnImGuiRender(PanelInformation *info)
    {
        ImGui::Begin("Logger");

        NameString = EditorUtils::ImGuiStringEditReturnString("Category Lookup", NameString);

        ImGui::SameLine();

        if (ImGui::Button("Clear"))
        {
            NameString = "";
            Logger::ClearLogInfos();
        }

        ImGui::SameLine();

        // -- DROP DOWN -----------
        const char *LevelToStringStrings[] = {"All", "Info", "Warn", "Error", "Trace", "Debug"};
        const int selCount = 6;
        if (ImGui::BeginCombo("Level", LevelToString.c_str()))
        {
            for (int i = 0; i < selCount; i++)
            {
                bool isSelected = (LevelToString == LevelToStringStrings[i]);
                if (ImGui::Selectable(LevelToStringStrings[i], isSelected))
                    LevelToString = LevelToStringStrings[i];
                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }

            ImGui::EndCombo();
        } // ------------------------

        for (auto info : Logger::GetLogInfos())
        {
            ImVec4 color;

            switch (info.Level)
            {
            case Logger::Level::Info:
                color = {0, 1, 0.5, 1};
                break;

            case Logger::Level::Warn:
                color = {1, 1, 0, 1};
                break;

            case Logger::Level::Error:
                color = {1, 0, 0, 1};
                break;

            case Logger::Level::Trace:
                color = {0.9f, 0.9f, 0.9f, 1};
                break;

            case Logger::Level::Debug:
                color = {0, 0.49, 1, 1};
                break;

            default:
                color = {1, 1, 1, 1};
                break;
            }

            if (LevelToString == LevelToStringStrings[1])
                if (info.Level != Logger::Level::Info)
                    continue;

            if (LevelToString == LevelToStringStrings[2])
                if (info.Level != Logger::Level::Warn)
                    continue;

            if (LevelToString == LevelToStringStrings[3])
                if (info.Level != Logger::Level::Error)
                    continue;

            if (LevelToString == LevelToStringStrings[4])
                if (info.Level != Logger::Level::Trace)
                    continue;

            if (LevelToString == LevelToStringStrings[5])
                if (info.Level != Logger::Level::Debug)
                    continue;

            if (NameString.empty())
            {
                ImGui::PushStyleColor(ImGuiCol_Text, color);
                ImGui::Text(info.Message.c_str());
                ImGui::PopStyleColor();
            }
            else
            {
                if (info.Pending.find(NameString) != std::string::npos)
                {
                    ImGui::PushStyleColor(ImGuiCol_Text, color);
                    ImGui::Text(info.Message.c_str());
                    ImGui::PopStyleColor();
                }
            }
        }

        if (LastLogCount != Logger::GetLogInfos().size())
        {
            LastLogCount = Logger::GetLogInfos().size();
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::End();
    }
}
