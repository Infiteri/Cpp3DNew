#pragma once

#include "Panel.h"

namespace Core
{
    class LoggerPanel : public Panel
    {
    public:
        LoggerPanel();
        ~LoggerPanel() = default;

        void OnImGuiRender(PanelInformation *info);
    };
}