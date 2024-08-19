#include "Mesh.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Material/MaterialSystem.h"

namespace Core
{
    float vertices[] = {
        0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.0f, 0.0f, 1.0f

    };

    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

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

    void Mesh::_BufferArrayWithGeometry()
    {
        if (vertex)
            delete vertex;

        if (!geometry)
            return;

        vertex = new VertexArray();
        vertex->GenVertexBuffer(geometry->Vertices.data(), geometry->Vertices.size() * sizeof(Vertex3D));
        vertex->GetVertexBuffer()->AddLayout(0, 0, 3);
        vertex->GetVertexBuffer()->AddLayout(1, 3, 2);
        vertex->GetVertexBuffer()->AddLayout(2, 5, 2);
        vertex->GenIndexBuffer(geometry->Indices.data(), geometry->Indices.size());
    }

    Mesh::Mesh()
    {
        vertex = nullptr;
        geometry = new BoxGeometry(2, 1, 1);

        MakeMaterialDefault();

        _BufferArrayWithGeometry();
    }

    Mesh::~Mesh()
    {
        _ReleaseMaterial();
        _ReleaseGeometry();
    }

    void Mesh::Render()
    {
        material->Use();

        vertex->Bind();
        vertex->GetVertexBuffer()->Bind();
        vertex->GetIndexBuffer()->Draw();
    }

    void Mesh::SetGeometry(Geometry *newGeometry)
    {
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
}