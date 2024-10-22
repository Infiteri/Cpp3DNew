#pragma once

#include "Panel.h"

#include <string>

namespace Core
{
    class ContentBrowserPanel : public Panel
    {
    private:
        struct State
        {
            std::string activePath;
            float padding;
            float thumbnailSize;

            Texture *folderTexture;
            Texture *iconTexture;
        };

    public:
        State state;

        ContentBrowserPanel();
        ~ContentBrowserPanel();

        void OnImGuiRender(PanelInformation *info);
    };

}