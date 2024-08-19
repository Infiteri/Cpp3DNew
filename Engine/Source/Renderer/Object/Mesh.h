#pragma once

#include "Base.h"
#include "Renderer/Buffer/VertexArray.h"
#include "Renderer/Material/Material.h"
#include "Renderer/Geometry/Geometry.h"

namespace Core
{
    class CE_API Mesh
    {
    private:
        VertexArray *vertex = nullptr;
        Material *material = nullptr;
        Geometry* geometry;

        void _ReleaseMaterial();
        void _ReleaseGeometry();
        void _BufferArrayWithGeometry();

    public:
        Mesh();
        ~Mesh();

        void Render();

        inline Material *GetMaterial() { return material; };
        inline Geometry *GetGeometry() { return geometry; };

        /// @brief Sets geometry and takes ownership.
        /// @param newGeometry The new geometry
        void SetGeometry(Geometry* newGeometry);

        void MakeMaterialDefault();
        void MakeMaterialUnique();
        // void SetMaterial(const std::string& name);
    };
}