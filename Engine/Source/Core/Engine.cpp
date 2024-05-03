#include "Engine.h"
#include "Logger.h"

namespace Core
{
    static Application *GApp = nullptr;

    void Engine::PreInit()
    {
        Logger::Settings LogSettings; // TODO: Maybe parse a file?
        Logger::Init(LogSettings);
    }

    void Engine::Init()
    {
        if (GApp)
            GApp->Init();
    }

    void Engine::Update()
    {
        if (GApp)
            GApp->Update();
    }

    void Engine::Render()
    {
        if (GApp)
            GApp->Render();
    }

    void Engine::Shutdown()
    {
        if (GApp)
        {
            GApp->Shutdown();
            delete GApp;
            GApp = nullptr;
        }
    }

    bool Engine::ShouldRun()
    {
        return true;
    }

    void Engine::FeedApplication(Application *_App)
    {
        GApp = _App;
    }
}