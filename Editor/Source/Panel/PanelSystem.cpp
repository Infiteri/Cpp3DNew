#include "PanelSystem.h"

#include "SceneHierarchyPanel.h"

namespace Core
{
    PanelSystem::PanelSystem()
    {
        panels.push_back(new SceneHierarchyPanel());
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