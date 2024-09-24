#pragma once

#include "Base.h"

namespace Core
{
    class CE_API Application
    {
    public:
        Application();
        virtual ~Application();

        virtual void Init();
        virtual void Shutdown();
    };

    Application *CreateApplication();
}