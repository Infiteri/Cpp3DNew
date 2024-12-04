#pragma once

#include "Base.h"
#include "Renderer/Buffer/VertexArray.h"
#include "Renderer/Material/Material.h"
#include "Renderer/Geometry/Geometry.h"

#include "Math/Matrix4.h"

namespace Core
{
    class CE_API Mesh
    {
    private:
        Material *material = nullptr;
        Geometry *geometry;

        void _ReleaseMaterial();
        void _ReleaseGeometry();

    public:
        Mesh();
        ~Mesh();

        void Render(Matrix4 transformMatrix);

        inline Material *GetMaterial() { return material; };
        inline Geometry *GetGeometry() { return geometry; };
        inline bool IsMaterialUnique() { return material->GetType() == Material::Config; };

        /// @brief Sets geometry and takes ownership.
        /// @param newGeometry The new geometry
        void SetGeometry(Geometry *newGeometry);

        void MakeMaterialDefault();
        void MakeMaterialUnique();
        void MakeMaterialUnique(Material::Configuration &config);
        void MakeMaterialFromFile();
        void MakeMaterialFromFile(const std::string &name);
        // void SetMaterial(const std::string& name);
    };
}