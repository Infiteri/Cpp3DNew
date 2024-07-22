#pragma once

#include "Base.h"
#include "Application.h"
#include "Window.h"

#include <string>

namespace Core
{
    class CE_API Engine
    {
    public:
        struct State
        {
            Window *Window;
            Application *GApp;
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
        static void FeedApplication(Application *App);

        static std::string ReadFileContent(const std::string &filename);
    };
}