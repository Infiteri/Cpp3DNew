#include "Renderer.h"
#include "Core/Logger.h"
#include "Platform/Platform.h"

#include "Object/Mesh.h"
#include "Shader/ShaderSystem.h"

#include "Camera/CameraSystem.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Core
{
    static Mesh *mesh;

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
        state.objectShader = ShaderSystem::GetFromEngineResource("Object");

        CameraSystem::CreatePerspective("Main Camera");
        CameraSystem::Activate("Main Camera");

        state.Screen.Setup();
        mesh = new Mesh();
    }

    void Renderer::BeginFrame()
    {
        if (!state.initializedContext)
            return;

        state.Screen.Begin();

        glClearColor(0.4, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::Render()
    {
        if (!state.initializedContext)
            return;

        state.objectShader->Use();

        auto c = CameraSystem::GetPerspectiveActive();

        c->GetPosition().Set(0, 0, -10);
        c->UpdateView();

        state.objectShader->Mat4(c->GetProjection(), "uProjection");
        state.objectShader->Mat4(c->GetView(), "uView");

        mesh->Render();
    }

    void Renderer::EndFrame()
    {
        if (!state.initializedContext)
            return;
        state.Screen.End();
    }

    void Renderer::RenderScreenImage()
    {
        state.Screen.Array->Bind();

        // Bind texture
        RenderPassSpecification *renderPass = state.Screen.Buffer->GetRenderPass(0);
        glActiveTexture(GL_TEXTURE0 + renderPass->index);
        glBindTexture(GL_TEXTURE_2D, renderPass->id);

        state.Screen.SShader->Use();
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
    }

    void Renderer::Shutdown()
    {
        delete state.Screen.Array;
        delete state.Screen.Buffer;
    }

}
