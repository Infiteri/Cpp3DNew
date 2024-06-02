#include "Renderer.h"
#include "Core/Logger.h"
#include "Platform/Platform.h"

#include "Object/Mesh.h"

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

    static Renderer::RendererState State;

    void Renderer::InitializeRenderingContext()
    {
        if (State.InitializedContext)
            return;

        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        State.InitializedContext = true;
    }

    void Renderer::Init()
    {
        CE_DEFINE_LOG_CATEGORY("RENDER", "Renderer");
        CE_LOG("RENDER", Info, "Initialized Renderer");

        if (!State.InitializedContext)
        {
            CE_LOG("RENDER", Error, "Renderer::Init exit, no context initialized.");
            return;
        }

        State.Screen.Setup();
        mesh = new Mesh();
    }

    void Renderer::BeginFrame()
    {
        if (!State.InitializedContext)
            return;

        State.Screen.Begin();

        glClearColor(0.4, 0.1, 0.1, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void Renderer::Render()
    {
        if (!State.InitializedContext)
            return;

        mesh->Render();
    }

    void Renderer::EndFrame()
    {
        if (!State.InitializedContext)
            return;
        State.Screen.End();
    }

    void Renderer::RenderScreenImage()
    {
        State.Screen.Array->Bind();

        // Bind texture
        RenderPassSpecification *renderPass = State.Screen.Buffer->GetRenderPass(0);
        glActiveTexture(GL_TEXTURE0 + renderPass->index);
        glBindTexture(GL_TEXTURE_2D, renderPass->id);

        State.Screen.SShader->Use();
        State.Screen.Array->GetVertexBuffer()->Bind();
        State.Screen.Array->GetVertexBuffer()->Draw();
    }

    void Renderer::Viewport(int width, int height)
    {
        State.ScreenViewport.Width = width;
        State.ScreenViewport.Height = height;

        glViewport(0, 0, width, height);

        if (State.Screen.Buffer)
            State.Screen.Buffer->Resize(width, height);
    }

    void Renderer::Shutdown()
    {
        delete State.Screen.Array;
        delete State.Screen.Buffer;
    }

}
