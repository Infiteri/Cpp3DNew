#include "Engine.h"
#include "Logger.h"
#include "Input.h"

#include "Layer/LayerStack.h"

#include "Renderer/Renderer.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

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

        Renderer::InitializeRenderingContext();
        Renderer::Init();

        Renderer::Viewport(state._Window->GetInfo()->Width, state._Window->GetInfo()->Height);
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
        Renderer::BeginFrame();
        Renderer::Render();
        if (state._GApp)
            state._GApp->Render();
        Renderer::EndFrame();
        Renderer::RenderScreenImage();
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
        Renderer::Shutdown();
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

    std::string Engine::ReadFileContent(const std::string &filename)
    {
        std::string vertResult;
        std::ifstream inVert(filename, std::ios::in | std::ios::binary);
        if (inVert)
        {

            inVert.seekg(0, std::ios::end);
            vertResult.resize(inVert.tellg());
            inVert.seekg(0, std::ios::beg);
            inVert.read(&vertResult[0], vertResult.size());
            inVert.close();
        }
        else
        {
            CE_CORE_ERROR("Unable to read file content:  %s", filename.c_str());
            vertResult = "";
        }
        return vertResult;
    }
}