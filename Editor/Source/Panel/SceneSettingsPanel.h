#pragma once

#include "Panel.h"

namespace Core
{
    class SceneSettingsPanel : public Panel
    {
    public:
        SceneSettingsPanel();
        ~SceneSettingsPanel();

        void OnImGuiRender(PanelInformation *info);
    };
}