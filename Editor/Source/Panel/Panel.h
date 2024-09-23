#pragma once

#include "Core.h"
#include <imgui.h>

namespace Core
{
    /// @brief General Panel information, edited by the EditorLayer class, passed to all the panels
    struct PanelInformation
    {
        // TODO: Add needed data
    };

    /// @brief Abstract class for panels
    /// @note Adding useful functions here is a valid thing that should be done when different panels start doing the same thing (ImGui functions probably don't count (future me knows :))
    class Panel
    {
    public:
        Panel() {};
        virtual ~Panel() {};

        virtual void OnImGuiRender(PanelInformation *information) {};
    };

}