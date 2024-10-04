#pragma once

#include "Base.h"
#include "Buffer/Buffer.h"
#include "Buffer/FrameBuffer.h"
#include "Buffer/VertexArray.h"
#include "Shader/Shader.h"

namespace Core
{
    struct GPUScreen
    {
        VertexArray *Array;
        FrameBuffer *Buffer;
        FrameBuffer *PostBuffer;
        Shader *SShader;

        void Setup();
        void Begin();
        void End();
    };
}