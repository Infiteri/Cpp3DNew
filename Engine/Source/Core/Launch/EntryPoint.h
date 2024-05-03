#pragma once

#include "EngineLoop.h"
#include "Core/Application.h"

extern Core::Application *Core::CreateApplication();

int main(int argc, char const *argv[])
{
    Core::EngineLoop GLoop;
    Core::Application *App = Core::CreateApplication();

    GLoop.GApplication = App;

    GLoop.Setup();
    GLoop.Run();
    GLoop.Shutdown();

    return 0;
}
