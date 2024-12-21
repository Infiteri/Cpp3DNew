#include "Engine.h"
#include "Logger.h"
#include "Input.h"

#include "Platform/Platform.h"

#include "Layer/LayerStack.h"
#include "Layer/ImGuiLayer.h"
#include "Instrumentation.h"

#include "Renderer/Renderer.h"

#include "Scene/World.h"
#include "Script/ScriptEngine.h"
#include "Physics/PhysicsEngine.h"

#include "Project/Project.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <glfw/glfw3.h>

namespace Core
{
    struct State
    {
        Window *Window;
        Application *GApp;
        bool IsProjectValid = false;

        // Delta
        float DeltaTime;
        float LastTime;
    };

    static State state;

    void Engine::PreInit()
    {
        Logger::Settings LogSettings; // TODO: Maybe parse a file?
        Logger::Init(LogSettings);

        CE_CORE_INFO("VERSION: %s", CE_VERSION);

        LoadProject();
        SetupWindow();

        // Note; No project specific section
        Input::Init();
        Renderer::InitializeRenderingContext();
        Renderer::Init();
        Renderer::Viewport(state.Window->GetInfo()->Width, state.Window->GetInfo()->Height);
        World::Init();
        PhysicsEngine::Init();
        ImGuiLayer::Init(); // NOTE: Requires window
        LayerStack::Init();

        if (state.IsProjectValid)
            LoadSystemsWithProject();

        // Note: the final exe and the '.ce_proj' file MUSt be in the same directory. Otherwise game wont run, when a better architecture is in place it will be done the right way
    }

    void Engine::Init()
    {
        if (state.GApp)
            state.GApp->Init();
    }

    void Engine::Update()
    {
        CE_PROFILE_FUNCTION();

        state.Window->Update();
        LayerStack::Update();

        float currentFrameTime = (float)glfwGetTime();
        state.DeltaTime = currentFrameTime - state.LastTime;
        state.LastTime = currentFrameTime;
    }

    void Engine::Render()
    {
        CE_PROFILE_FUNCTION();

        Renderer::BeginFrame();
        World::RenderActiveScene();
        Renderer::Render();
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
        ScriptEngine::Shutdown();
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

    float Engine::GetDeltaTime()
    {
        return state.DeltaTime;
    }

    bool Engine::IsProjectValid()
    {
        return state.IsProjectValid;
    }

    void Engine::SetupWindow()
    {
        // TODO: Check if with editor (somehow)
        // If its not with editor it is going to create a window based on saved data to Project.ce_proj
        Window::Information winInfo;
        winInfo.SizeMode = Window::Windowed;
        winInfo.Title = "Hello Core";
        state.Window = new Window(winInfo);
    }

    void Engine::LoadProject()
    {
        Project::Load("Project.ce_proj");
        state.IsProjectValid = true;

        if (Project::GetActiveConfiguration().Name.empty())
        {
            CE_CORE_WARN("Project name is empty, this could happen due to project being invalid or the name being empty itself.");
            CE_CORE_WARN("Engine will continue running with no project taken into consideration.");
            CE_CORE_WARN("Make sure a 'Project.ce_proj' file exists in the same folder sa the '.exe' file begin currently run.");
            CE_CORE_WARN("Additionally make sure that there is a valid name for the project and that its file format is correct.");
            state.IsProjectValid = false;
            return;
        }
    }

    void Engine::LoadSystemsWithProject()
    {
        auto &config = Project::GetActiveConfiguration();
        ScriptEngine::Init(config.GetScriptLibPathFormatted());
    }
}