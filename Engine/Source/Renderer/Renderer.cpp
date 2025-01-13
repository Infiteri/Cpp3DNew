#include "Renderer.h"
#include "Core/Engine.h"
#include "Core/Logger.h"
#include "Platform/Platform.h"

#include "Light/PointLight.h"

#include "Object/Mesh.h"
#include "Shader/ShaderSystem.h"
#include "Texture/TextureSystem.h"
#include "Material/MaterialSystem.h"

#include "Sky/Sky.h"

#include "Scene/World.h"
#include "Camera/CameraSystem.h"

#include "Light/LightID.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Core
{
    static Renderer::RendererState state;

    PostProcessor *Renderer::_GetPostProcessor()
    {
        // todo: make function that turns off the wanted post effects at runtime
        return &state.postProcessor;
    }

    void Renderer::InitializeRenderingContext()
    {
        if (state.initializedContext)
            return;

        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        state.initializedContext = true;
    }

    void Renderer::Init()
    {
        CE_DEFINE_LOG_CATEGORY("RENDER", "Renderer");
        CE_LOG("RENDER", Info, "Initialized Renderer");

        if (!state.initializedContext)
        {
            CE_LOG("RENDER", Error, "Renderer::Init exit, no context initialized.");
            return;
        }

        ShaderSystem::Init();
        CameraSystem::Init();
        TextureSystem::Init();
        MaterialSystem::Init(); // Post texture system as the material relies on the default texture. :)
        ShaderSystem::Load("EngineResources/Shaders/Object");
        state.postProcessor.Add("EngineResources/Shaders/Post/RadiantBlur.glsl", false); // TODO: Some architecture
        state.postProcessor.Add("EngineResources/Shaders/Post/Vignette.glsl", false);    // TODO: Some architecture
        state.postProcessor.Add("EngineResources/Shaders/Post/Toy.glsl", false);         // TODO: Some architecture
        state.Screen.Setup();

        state.meshRenderArray = new VertexArray();

        glEnable(GL_MULTISAMPLE);
    }

    Viewport &Renderer::GetViewport()
    {
        return state.ScreenViewport;
    }

    void Renderer::BeginFrame()
    {
        if (!state.initializedContext)
            return;

        state.Screen.Begin();

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Necessary
        glClearColor(0, 0, 255, 255); // note for debug, if the sky is this rgb and its not set to color mode then ... something is wrong
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (state.skyInstance != nullptr)
        {
            switch (state.skyInstance->GetMode())
            {
            case Sky::ColorMode:
            {
                Color c = state.skyInstance->GetColor().Normalized();
                glClearColor(c.r, c.g, c.b, c.a);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            }
            break;

            default:
                glDepthMask(false);
                state.skyInstance->Render();
                glDepthMask(true);
                break;
            }
        }
    }

    void Renderer::Render()
    {
        if (!state.initializedContext)
            return;

        Shader *objShader = ShaderSystem::GetFromEngineResource("Object");
        CE_VERIFY(objShader);

        objShader->Use();

        // Camera
        {
            auto activeCamera = CameraSystem::GetPerspectiveActive();
            if (activeCamera)
            {
                activeCamera->UpdateView();
                objShader->Vec3(activeCamera->GetPosition(), "uCameraPosition");
                objShader->Mat4(activeCamera->GetProjection(), "uProjection");
                objShader->Mat4(activeCamera->GetViewInverted(), "uView");
            }
        }
    }

    void Renderer::EndFrame()
    {
        if (!state.initializedContext)
            return;
        state.Screen.End();
    }

    void Renderer::RenderScreenImage()
    {
        // Bind texture
        // TODO: Refactor into different functions so that it can be chosen between filling the PostBuffer Framebuffer with correct textures or immediately render to the screen
        RenderPassSpecification *renderPass = state.Screen.Buffer->GetRenderPass(0);
        auto post = state.Screen.PostBuffer;

        post->Bind();
        state.Screen.SShader->Use();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, renderPass->id);
        ShaderSystem::GetFromEngineResource("Screen")->Int(0, "uScreenTexture");
        state.Screen.Array->Bind();
        state.Screen.Array->GetVertexBuffer()->Bind();
        state.Screen.Array->GetVertexBuffer()->Draw();

        // -- Post process --
        for (auto shader : state.postProcessor.GetEnabledShaders())
        {
            shader->Use();
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, post->GetRenderPass(0)->id);
            state.Screen.Array->Bind();
            state.Screen.Array->GetVertexBuffer()->Bind();
            state.Screen.Array->GetVertexBuffer()->Draw();
        }
        // ------------------

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, post->GetRenderPass(0)->id);

        state.Screen.Array->Bind();
        state.Screen.Array->GetVertexBuffer()->Bind();
        state.Screen.Array->GetVertexBuffer()->Draw();
    }

    void Renderer::Viewport(int width, int height)
    {
        state.ScreenViewport.Width = width;
        state.ScreenViewport.Height = height;

        glViewport(0, 0, width, height);

        auto c = CameraSystem::GetPerspectiveActive();
        if (!c)
            return;
        c->UpdateProjection((float)width / (float)height);

        if (state.Screen.Buffer)
            state.Screen.Buffer->Resize(width, height);

        if (state.Screen.PostBuffer)
            state.Screen.PostBuffer->Resize(width, height);
    }

    void Renderer::Viewport()
    {
        Viewport(state.ScreenViewport.Width, state.ScreenViewport.Height);
    }

    CeU32 Renderer::GetPassID(int index)
    {
        return state.Screen.PostBuffer->GetRenderPass(index)->id;
    }

    void Renderer::SetSkyInstance(Sky *sky, bool deleteOldSky)
    {
        if (deleteOldSky && state.skyInstance)
            delete state.skyInstance;

        state.skyInstance = sky;
    }

    void Renderer::InitializePostShader(const std::string &shaderName)
    {
        if (!state.postProcessor.Exists(shaderName))
            state.postProcessor.Add(shaderName, true);
        else
            state.postProcessor.Enable(shaderName);
    }

    Shader *Renderer::GetPostShader(const std::string &shaderName)
    {
        return state.postProcessor.Get(shaderName);
    }

    void Renderer::EnsureReloadingOfPostShaders()
    {
        for (auto &shader : state.postProcessor.shaders)
        {
            if (shader.enabled)
            {
                std::string name = shader.shd->GetName();
                delete shader.shd;
                shader.shd = new Shader(name);
            }
        }
    }

    void Renderer::Shutdown()
    {
        ShaderSystem::Shutdown();
        CameraSystem::Shutdown();
        TextureSystem::Shutdown();
        MaterialSystem::Shutdown();

        delete state.Screen.Array;
        delete state.Screen.Buffer;
        delete state.meshRenderArray;
    }
}
