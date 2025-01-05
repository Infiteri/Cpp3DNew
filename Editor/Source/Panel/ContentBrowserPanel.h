#pragma once

#include "Panel.h"
#include "Project/Project.h"
#include "Platform/Platform.h"

#include <fstream>
#include <string>

namespace Core
{
    class ContentBrowserPanel : public Panel
    {
    private:
        struct NewFileMenu
        {
            std::string FilePath = "";
            std::string FolderPath = "";
            bool ShouldRender = false;
            void Render();
        };

        struct ScriptCreateMenu
        {
            std::string Path;
            std::string ClassName;
            bool ShouldRender = false;
            void Render();
        };

    public:
        struct State
        {
            std::string ActivePath;
            float Padding;
            float ThumbnailSize;

            Texture *FolderTexture;
            Texture *IconTexture;

            NewFileMenu NewFileMenu;
            ScriptCreateMenu ScriptCreateMenu;

            Platform::DirectoryEntry RightClickedEntry;
        };

    public:
        State state;

        ContentBrowserPanel();
        ~ContentBrowserPanel();

        void OnImGuiRender(PanelInformation *info);

        void RenderCreateNewFile();
    };

}