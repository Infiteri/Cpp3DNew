#include "Renderer.h"
#include "Core/Logger.h"
#include "Platform/Platform.h"

#include "Object/Mesh.h"
#include "Shader/ShaderSystem.h"
#include "Texture/TextureSystem.h"
#include "Material/MaterialSystem.h"

#include "Camera/CameraSystem.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Core
{
    static float screenQuadVertices[] = {
        // positions   // texCoords
        -1.0f, 1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f, 1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 1.0f};

    static Renderer::RendererState state;

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
        state.objectShader = ShaderSystem::GetFromEngineResource("Object");

        CameraSystem::CreatePerspective("Main Camera");
        CameraSystem::Activate("Main Camera");

        state.Screen.Setup();
    }

    void Renderer::BeginFrame()
    {
        if (!state.initializedContext)
            return;

        state.Screen.Begin();

        glClearColor(0.4, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void Renderer::Render()
    {
        if (!state.initializedContext)
            return;

        state.objectShader->Use();

        auto activeCamera = CameraSystem::GetPerspectiveActive();
        activeCamera->UpdateView();

        state.objectShader->Mat4(activeCamera->GetProjection(), "uProjection");
        state.objectShader->Mat4(activeCamera->GetViewInverted(), "uView");
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

    CeU32 Renderer::GetPassID(int index)
    {
        return state.Screen.PostBuffer->GetRenderPass(index)->id;
    }

    void Renderer::Shutdown()
    {
        delete state.Screen.Array;
        delete state.Screen.Buffer;
    }

}
