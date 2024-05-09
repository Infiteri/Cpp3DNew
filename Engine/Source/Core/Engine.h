#pragma once

#include "Base.h"
#include "Application.h"
#include "Window.h"

namespace Core
{
    class CE_API Engine
    {
    public:
        struct State
        {
            Window *_Window;
            Application *_GApp;
        };

        Engine() {}
        ~Engine() {}

        static void PreInit();
        static void Init();
        static void Update();
        static void Render();
        static void Shutdown();
        static bool ShouldRun();
        static Window *GetWindow();
        static void FeedApplication(Application *_App);
    };
}