#pragma once

#include "Panel.h"
#include <vector>

namespace Core
{
    class PanelSystem
    {
    public:
        std::vector<Panel*> panels;
        /// @brief Will add default panels
        PanelSystem();
        ~PanelSystem();

        void RenderImGui(PanelInformation *information);
    };
}