#pragma once

#include "Base.h"
#include "Application.h"
#include "Window.h"

#include <string>

namespace Core
{
    class CE_API Engine
    {
    private:
        static void SetupWindow();
        static void LoadProject();
        static void LoadSystemsWithProject();

    public:
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

        static float GetDeltaTime();

        static bool IsProjectValid();
    };
}