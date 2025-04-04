#include "PanelSystem.h"

#include "SceneHierarchyPanel.h"
#include "SceneSettingsPanel.h"
#include "ContentBrowserPanel.h"
#include "LoggerPanel.h"

namespace Core
{
    PanelSystem::PanelSystem()
    {
        panels.push_back(new SceneHierarchyPanel());
        panels.push_back(new SceneSettingsPanel());
        panels.push_back(new ContentBrowserPanel());
        panels.push_back(new LoggerPanel());
    }

    PanelSystem::~PanelSystem()
    {
    }

    void PanelSystem::RenderImGui(PanelInformation *information)
    {
        for (int i = 0; i < panels.size(); i++)
        {
            auto panel = panels[i];
            panel->OnImGuiRender(information);
        }
    }
}