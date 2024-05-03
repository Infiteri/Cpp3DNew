#pragma once

#include "Base.h"
#include "Application.h"

namespace Core
{
    class CE_API Engine
    {
    public:
        Engine() {}
        ~Engine() {}

        static void PreInit();
        static void Init();
        static void Update();
        static void Render();
        static void Shutdown();

        static bool ShouldRun();

        static void FeedApplication(Application* _App);
    };
}