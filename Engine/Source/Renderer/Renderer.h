#pragma once

#include "Base.h"
#include "Buffer/Buffer.h"
#include "Buffer/FrameBuffer.h"
#include "Buffer/VertexArray.h"
#include "Math/Transform.h"
#include "Shader/Shader.h"
#include "Sky/Sky.h"
#include "PostProcessor.h"
#include "Geometry/Geometry.h"
#include "Material/Material.h"
#include "Camera/PerspectiveCamera.h"
#include "GPUScreen.h"

#include <vector>

namespace Core
{
    class CE_API Renderer
    {
    public:

        struct Viewport
        {
            int Width;
            int Height;
        };

        struct RendererState
        {
            GPUScreen Screen;
            Viewport ScreenViewport;
            PostProcessor postProcessor;

            Sky *skyInstance;
            VertexArray *meshRenderArray;
            bool initializedContext = false;
        };

        static PostProcessor* _GetPostProcessor();
        friend class Scene;

    public:
        Renderer() {};
        ~Renderer() {};

        static void InitializeRenderingContext();

        static void Init();
        static void BeginFrame();
        static void Render();
        static void EndFrame();
        static void Shutdown();
        static void RenderScreenImage();
        static void Viewport(int width, int height);
        static void Viewport();
        static CeU32 GetPassID(int index);

        static void SetSkyInstance(Sky *sky, bool deleteOldSky = false);

        static void InitializePostShader(const std::string& shaderName);
        static Shader* GetPostShader(const std::string& shaderName);
        static void EnsureReloadingOfPostShaders();
    };
}