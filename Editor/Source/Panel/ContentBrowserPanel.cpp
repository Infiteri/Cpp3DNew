#include "ContentBrowserPanel.h"
#include "EditorUtils.h"
#include "Platform/Platform.h"

namespace Core
{
    static std::string TEMP_BASE_PATH = "EngineResources";
    ContentBrowserPanel::ContentBrowserPanel()
    {
        state.activePath = TEMP_BASE_PATH;
        if (Project::GetActiveConfiguration().AssetDirectory != "")
            state.activePath = Project::GetActiveConfiguration().AssetDirectory;

        state.padding = 16.0f;
        state.thumbnailSize = 64.0f;

        state.folderTexture = EditorUtils::LoadTexture("EngineResources/Images/Icons/folder.png");
        state.iconTexture = EditorUtils::LoadTexture("EngineResources/Images/Icons/icon.png");
    }

    ContentBrowserPanel::~ContentBrowserPanel()
    {
        delete state.folderTexture;
        delete state.iconTexture;
    }

    void ContentBrowserPanel::OnImGuiRender(PanelInformation *info)
    {
        TEMP_BASE_PATH = info->AssetDirectory;

        // NOTE: Extremely platform specific
        ImGui::Begin("Content Browser");

        float panelWidth = ImGui::GetContentRegionAvail().x;
        float cellSize = state.thumbnailSize + state.padding;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1)
            columnCount = 1;
        ImVec2 size{state.thumbnailSize, state.thumbnailSize}; // Might change based on preferences
        ImGui::Columns(columnCount, 0, false);

        if (state.activePath.compare(TEMP_BASE_PATH) != 0 && ImGui::Button("<-"))
            state.activePath = TEMP_BASE_PATH;

        int it = 0;
        for (auto entry : Platform::GetDirectoryEntries(state.activePath))
        {
            ImGui::PushID(++it);

            Texture *texture = entry.IsFolder ? state.folderTexture : state.iconTexture;

            if (ImGui::ImageButton((void *)(CeU64)(CeU32)texture->GetID(), size))
            {
                if (entry.IsFolder)
                    state.activePath += "/" + entry.Name;
            }

            if (ImGui::BeginDragDropSource())
            {
                std::string path = entry.Name;
                ImGui::SetDragDropPayload("CE_CONTENT_PANEL", path.c_str(), strlen(path.c_str()) + 1);
                ImGui::EndDragDropSource();
            }

            if (!entry.IsFolder)
            {
                std::string filePath;
                size_t lastSlash = entry.Name.find_last_of("/\\");
                if (lastSlash != std::string::npos)
                    filePath = entry.Name.substr(lastSlash + 1);
                ImGui::TextWrapped(filePath.c_str());
            }
            else
            {
                ImGui::TextWrapped(entry.Name.c_str());
            }

            ImGui::NextColumn();
            ImGui::PopID();
        }

        ImGui::Columns(1);
        ImGui::End();
    }
}