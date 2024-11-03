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

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <glfw/glfw3.h>

namespace Core
{
    static Engine::State state;
    Platform::DynamicLibrary lib;

    void Engine::PreInit()
    {
        Logger::Settings LogSettings; // TODO: Maybe parse a file?
        Logger::Init(LogSettings);

        // -- Cool --
        CE_CORE_INFO("VERSION: %s", CE_VERSION);

        Input::Init();

        // TODO: From configuration or project
        Window::Information WindowInformation;
        WindowInformation.SizeMode = Window::Windowed;
        WindowInformation.Title = "Hello Core";
        state.Window = new Window(WindowInformation);

        Renderer::InitializeRenderingContext();
        Renderer::Init();
        Renderer::Viewport(state.Window->GetInfo()->Width, state.Window->GetInfo()->Height);
        World::Init();
        ScriptEngine::Init();
        PhysicsEngine::Init();
        ImGuiLayer::Init(); // NOTE: Requires window

        LayerStack::Init();

        lib = Platform::CreateLibrary("GameCode.dll");
        typedef void (*TestPFN)();

        if (Platform::LoadFunction(&lib, "Test"))
        {
            auto t = (TestPFN)(lib.Functions["Test"]->PFN);
            t();
        }
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

        double currentFrameTime = glfwGetTime();
        state.DeltaTime = (float)(currentFrameTime - state.LastTime);
        state.LastTime = (float)(currentFrameTime);
    }

    void Engine::Render()
    {
        CE_PROFILE_FUNCTION();

        Renderer::BeginFrame();
        Renderer::Render();
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