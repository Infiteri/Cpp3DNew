#include "EngineLoop.h"
#include "Core/Engine.h"

namespace Core
{
    EngineLoop::EngineLoop()
    {
    }

    EngineLoop::~EngineLoop()
    {
    }

    void EngineLoop::Setup()
    {
        Engine::FeedApplication(GApplication);
        Engine::PreInit();
        Engine::Init();
    }

    void EngineLoop::Run()
    {
        while (Engine::ShouldRun())
        {
            Engine::Update();
            Engine::Render();
        }
    }

    void EngineLoop::Shutdown()
    {
        Engine::Shutdown();
    }
}