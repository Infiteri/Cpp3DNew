#pragma once

#include "EngineLoop.h"
#include "Core/Application.h"
#include "Core/Instrumentation.h"

extern Core::Application *Core::CreateApplication();

int main(int argc, char const *argv[])
{
    CE_PROFILE_BEGIN_SESSION_FILE("Game", "Debug/Core-ProfileEngine.json");

    Core::EngineLoop GLoop;
    Core::Application *App = Core::CreateApplication();

    GLoop.GApplication = App;

    GLoop.Setup();
    GLoop.Run();
    GLoop.Shutdown();

    CE_PROFILE_END_SESSION();

    return 0;
}
