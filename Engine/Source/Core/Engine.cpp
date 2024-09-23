#include "Engine.h"
#include "Logger.h"
#include "Input.h"

#include "Layer/LayerStack.h"
#include "Layer/ImGuiLayer.h"

#include "Renderer/Renderer.h"

#include "Scene/World.h"

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

        // TODO: From configuration or project
        Window::Information WindowInformation;
        WindowInformation.SizeMode = Window::Windowed;
        WindowInformation.Title = "Hello Core";
        state.Window = new Window(WindowInformation);

        Renderer::InitializeRenderingContext();
        Renderer::Init();
        Renderer::Viewport(state.Window->GetInfo()->Width, state.Window->GetInfo()->Height);

        World::Init();

        ImGuiLayer::Init(); // NOTE: Requires window
    }

    void Engine::Init()
    {
        if (state.GApp)
            state.GApp->Init();
    }

    void Engine::Update()
    {
        // LayerStack::Update(); TODO: Add back (funny bugs with post processing) NOOOOOOOOOOOOOOO
        state.Window->Update();

        if (state.GApp)
            state.GApp->Update();
    }

    void Engine::Render()
    {
        Renderer::BeginFrame();
        Renderer::Render();
        if (state.GApp)
            state.GApp->Render();

        World::RenderActiveScene();
        Renderer::EndFrame();
        Renderer::RenderScreenImage();

        ImGuiLayer::BeginFrame();
        LayerStack::RenderImGui();
        ImGuiLayer::EndFrame();
    }

    void Engine::Shutdown()
    {
        if (state.GApp)
        {
            state.GApp->Shutdown();
            delete state.GApp;
            state.GApp = nullptr;
        }

        delete state.Window;
        LayerStack::Shutdown();
        Input::Shutdown();
        Renderer::Shutdown();
        World::Shutdown();
        Logger::Shutdown();
    }

    bool Engine::ShouldRun()
    {
        return state.Window->ShouldRun();
    }

    Window *Engine::GetWindow()
    {
        return state.Window;
    }

    void Engine::FeedApplication(Application *_App)
    {
        state.GApp = _App;
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