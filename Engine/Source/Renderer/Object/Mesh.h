#pragma once

#include "Base.h"
#include "Renderer/Buffer/VertexArray.h"

namespace Core
{
    class CE_API Mesh
    {
    private:
        VertexArray *vertex;

    public:
        Mesh();
        ~Mesh();

        void Render();
    };
}