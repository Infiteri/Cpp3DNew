#pragma once

#include "Panel.h"
#include <vector>

namespace Core
{
    class PanelSystem
    {
    private:
        std::vector<Panel*> panels;

    public:
        /// @brief Will add default panels
        PanelSystem();
        ~PanelSystem();

        void RenderImGui(PanelInformation *information);
    };
}