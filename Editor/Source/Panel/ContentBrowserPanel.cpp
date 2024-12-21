#include "ContentBrowserPanel.h"
#include "EditorUtils.h"
#include "Core/FileSystem/FileSystem.h"

// cool place to work in, can add things like create file, edit file, image inspectors and others

namespace Core
{
    // Note: overwritten int OnImGuiRender
    static std::string ProjectBasePath = "EngineResources";
    ContentBrowserPanel::ContentBrowserPanel()
    {
        state.RightClickedEntry.Name = "";
        state.RightClickedEntry.IsFolder = false;

        state.ActivePath = ProjectBasePath;
        if (Project::GetActiveConfiguration().AssetDirectory != "")
            state.ActivePath = Project::GetActiveConfiguration().AssetDirectory;

        state.Padding = 16.0f;
        state.ThumbnailSize = 64.0f;

        state.FolderTexture = EditorUtils::LoadTexture("EngineResources/Images/Icons/folder.png");
        state.IconTexture = EditorUtils::LoadTexture("EngineResources/Images/Icons/icon.png");
    }

    ContentBrowserPanel::~ContentBrowserPanel()
    {
        delete state.FolderTexture;
        delete state.IconTexture;
    }

    void ContentBrowserPanel::OnImGuiRender(PanelInformation *info)
    {
        ProjectBasePath = info->AssetDirectory;

        // NOTE: Extremely platform specific
        ImGui::Begin("Content Browser");

        if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right) && !ImGui::IsAnyItemHovered())
            ImGui::OpenPopup("ContentPanelRightClick");

        if (ImGui::BeginPopup("ContentPanelRightClick"))
        {
            if (ImGui::MenuItem("New File"))
            {
                state.NewFileMenu.FolderPath = state.ActivePath;
                state.NewFileMenu.FilePath = "";
                state.NewFileMenu.ShouldRender = true;
            }

            ImGui::EndPopup();
        }

        float panelWidth = ImGui::GetContentRegionAvail().x;
        float cellSize = state.ThumbnailSize + state.Padding;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1)
            columnCount = 1;
        ImVec2 size{state.ThumbnailSize, state.ThumbnailSize}; // Might change based on preferences
        ImGui::Columns(columnCount, 0, false);

        if (state.ActivePath.compare(ProjectBasePath) != 0 && ImGui::Button("<-"))
            state.ActivePath = ProjectBasePath;

        int it = 0;
        for (auto entry : Platform::GetDirectoryEntries(state.ActivePath))
        {
            ImGui::PushID(++it);

            Texture *texture = entry.IsFolder ? state.FolderTexture : state.IconTexture;

            if (ImGui::ImageButton((void *)(CeU64)(CeU32)texture->GetID(), size))
            {
                if (entry.IsFolder)
                    state.ActivePath += "/" + entry.Name;
            }

            // When an item is hovered and the right mouse button is clicked:
            if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
            {
                state.RightClickedEntry.Name = entry.Name;
                state.RightClickedEntry.IsFolder = entry.IsFolder;
                ImGui::OpenPopup("ContentPanelRightItem");
            }

            if (ImGui::BeginPopup("ContentPanelRightItem"))
            {
                if (ImGui::MenuItem("Remove File"))
                {
                    FileSystem::RemoveFile(state.RightClickedEntry.Name);
                    state.RightClickedEntry.Name = "";
                    state.RightClickedEntry.IsFolder = false;
                }

                ImGui::EndPopup();
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

        state.NewFileMenu.Render();
    }

    void ContentBrowserPanel::NewFileMenu::Render()
    {
        if (!ShouldRender)
            return;

        ImGui::Begin("Create New File");

        ImGui::Text("This is the 'Create File' menu.");
        ImGui::Text("This allows you to create different files with different extensions.");
        ImGui::Text("The final format is 'FolderPath/FilePath'");

        FolderPath = EditorUtils::ImGuiStringEditReturnString("Folder", FolderPath);
        FilePath = EditorUtils::ImGuiStringEditReturnString("File", FilePath);

        if (ImGui::Button("Create"))
        {
            ShouldRender = false;

            FileSystem::CreateFile(FolderPath + "/" + FilePath);
        }

        ImGui::SameLine();

        if (ImGui::Button("Cancel"))
            ShouldRender = false;

        ImGui::End();
    }

}