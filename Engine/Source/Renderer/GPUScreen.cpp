#include "GPUScreen.h"
#include "Shader/ShaderSystem.h"
#include "Platform/Platform.h"
#include <glad/glad.h>

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

    void GPUScreen::Setup()
    {
        SShader = ShaderSystem::GetFromEngineResource("Screen");

        {
            FrameBufferSpecification spec;
            spec.width = 1920;
            spec.height = 1080;
            spec.renderPassSpecificationCount = 2;
            spec.renderPassSpecifications = (RenderPassSpecification *)Platform::MemALloc(sizeof(RenderPassSpecification) * spec.renderPassSpecificationCount);
            spec.renderPassSpecifications[0].textureType = RenderPassTextureType::Rgb;
            spec.renderPassSpecifications[1].textureType = RenderPassTextureType::Depth;
            Buffer = new FrameBuffer(spec);
        }

        {
            FrameBufferSpecification spec;
            spec.width = 1920;
            spec.height = 1080;
            spec.renderPassSpecificationCount = 1;
            spec.renderPassSpecifications = (RenderPassSpecification *)Platform::MemALloc(sizeof(RenderPassSpecification) * spec.renderPassSpecificationCount);
            spec.renderPassSpecifications[0].textureType = RenderPassTextureType::Rgb;
            PostBuffer = new FrameBuffer(spec);
        }

        Array = new VertexArray();
        Array->GenVertexBuffer(screenQuadVertices, sizeof(screenQuadVertices)); // ! NO sizeof(float) cuz it makes the final image broken;
        Array->GetVertexBuffer()->AddLayout(0, 0, 2);
        Array->GetVertexBuffer()->AddLayout(1, 2, 2);
    }

    void GPUScreen::Begin()
    {
        Buffer->Bind();
        glEnable(GL_DEPTH_TEST);
    }

    void GPUScreen::End()
    {
        Buffer->Unbind();

        glDisable(GL_DEPTH_TEST);
        glClearColor(0, 1, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}