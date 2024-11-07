#pragma once

#include "Base.h"
#include "Buffer/Buffer.h"
#include "Buffer/FrameBuffer.h"
#include "Buffer/VertexArray.h"
#include "Shader/Shader.h"
#include "Sky/Sky.h"
#include "PostProcessor.h"

#include "Camera/PerspectiveCamera.h"

#include "GPUScreen.h"

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

            bool initializedContext = false;
        };

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

        static void SetSkyInstance(Sky* sky, bool deleteOldSky = false);
    };
}