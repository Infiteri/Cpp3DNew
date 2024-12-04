#include "Mesh.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Material/MaterialSystem.h"
#include "Renderer/Renderer.h"

namespace Core
{
    void Mesh::_ReleaseMaterial()
    {
        if (!material)
            return;

        switch (material->GetType())
        {
        case Material::Default:
        default:
            MaterialSystem::ReleaseDefault(material);
            break;

        case Material::Config:
            delete material;
            break;

        case Material::File: // Will matter at some point
            MaterialSystem::Release(material->GetState().Name);
            break;
        }

        material = nullptr;
    }

    void Mesh::_ReleaseGeometry()
    {
        if (geometry)
            delete geometry;
        geometry = nullptr;
    }

    Mesh::Mesh()
    {
        geometry = new Geometry();

        MakeMaterialDefault();
    }

    Mesh::~Mesh()
    {
        _ReleaseMaterial();
        _ReleaseGeometry();
    }

    void Mesh::Render(Matrix4 transformMatrix)
    {
        Renderer::MeshRenderData data;

        data.Transform = transformMatrix;
        data.Geometry = geometry;
        data.Material = material;

        Renderer::AddMeshRenderData(data);
    }

    void Mesh::SetGeometry(Geometry *newGeometry)
    {
        _ReleaseGeometry();

        geometry = newGeometry;
    }

    void Mesh::MakeMaterialDefault()
    {
        if (!material)
        {
            material = MaterialSystem::GetDefault();
            return;
        }

        if (material->GetType() == Material::Default)
            return;

        _ReleaseMaterial();
        material = MaterialSystem::GetDefault();
    }

    void Mesh::MakeMaterialUnique()
    {
        if (material)
            _ReleaseMaterial();

        material = new Material();
        material->SetType(Material::Config);
    }

    void Mesh::MakeMaterialUnique(Material::Configuration &config)
    {
        if (material)
            _ReleaseMaterial();

        material = new Material();
        material->SetType(Material::Config);
        material->Create(&config);
    }

    void Mesh::MakeMaterialFromFile()
    {
        if (material)
            _ReleaseMaterial();

        material = new Material();
        material->MakeDefault();
        material->SetType(Material::File);
    }

    void Mesh::MakeMaterialFromFile(const std::string &name)
    {
        if (material)
            _ReleaseMaterial();

        material = new Material();
        material->Create(name);
    }
}