#pragma once

#include "Base.h"
#include "Core/Application.h"

namespace Core
{
    class CE_API EngineLoop
    {
    public:
        Application *GApplication;

        EngineLoop();
        ~EngineLoop();

        void Setup();
        void Run();
        void Shutdown();
    };
}