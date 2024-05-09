#include "Engine.h"
#include "Logger.h"
#include "Input.h"

#include "Layer/LayerStack.h"

namespace Core
{
    static Engine::State state;

    void Engine::PreInit()
    {
        Logger::Settings LogSettings; // TODO: Maybe parse a file?
        Logger::Init(LogSettings);
        Input::Init();
        LayerStack::Init();

        Window::Information WindowInformation;
        WindowInformation.X = WindowInformation.Y = 100;
        WindowInformation.Width = 1280;
        WindowInformation.Height = 720;
        WindowInformation.Title = "Hello Core";
        state._Window = new Window(WindowInformation);
    }

    void Engine::Init()
    {
        if (state._GApp)
            state._GApp->Init();
    }

    void Engine::Update()
    {
        LayerStack::Update();
        state._Window->Update();

        if (state._GApp)
            state._GApp->Update();
    }

    void Engine::Render()
    {
        if (state._GApp)
            state._GApp->Render();
    }

    void Engine::Shutdown()
    {
        if (state._GApp)
        {
            state._GApp->Shutdown();
            delete state._GApp;
            state._GApp = nullptr;
        }

        delete state._Window;
        Logger::Shutdown();
        LayerStack::Shutdown();
        Input::Shutdown();
    }

    bool Engine::ShouldRun()
    {
        return state._Window->ShouldRun();
    }

    Window *Engine::GetWindow()
    {
        return state._Window;
    }

    void Engine::FeedApplication(Application *_App)
    {
        state._GApp = _App;
    }
}