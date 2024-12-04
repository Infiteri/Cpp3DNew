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
        struct MeshRenderData
        {
            Matrix4 Transform;
            Material *Material;
            Geometry *Geometry;

            MeshRenderData() {};
            MeshRenderData(const MeshRenderData &data)
            {
                Transform.From(data.Transform);
                Material = data.Material;
                Geometry = data.Geometry;
            };
        };

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
            std::vector<MeshRenderData> meshRenderData;

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

        static void SetSkyInstance(Sky *sky, bool deleteOldSky = false);

        static void AddMeshRenderData(MeshRenderData &data);
    };
}