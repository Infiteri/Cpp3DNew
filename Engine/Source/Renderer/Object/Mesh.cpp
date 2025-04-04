#include "Mesh.h"
#include "Core/Logger.h"
#include "Renderer/Shader/Shader.h"
#include "Renderer/Shader/ShaderSystem.h"
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

    void Mesh::_BufferGeometryArray()
    {
        if (array)
            delete array;

        if (!geometry)
            return;

        array = new VertexArray();
        array->GenVertexBuffer(geometry->Vertices.data(), geometry->Vertices.size() * sizeof(Vertex3D));
        array->GenIndexBuffer(geometry->Indices.data(), geometry->Indices.size() * sizeof(CeU32));
        array->GetVertexBuffer()->AddLayout(0, 0, 3);
        array->GetVertexBuffer()->AddLayout(1, 3, 2);
        array->GetVertexBuffer()->AddLayout(2, 5, 3);
    }

    Mesh::Mesh()
    {
        array = nullptr;
        geometry = new Geometry();

        MakeMaterialDefault();
        _BufferGeometryArray();
    }

    Mesh::~Mesh()
    {
        _ReleaseMaterial();
        _ReleaseGeometry();
        delete array;
    }

    void Mesh::Render(Matrix4 transformMatrix)
    {
        auto objShader = ShaderSystem::GetFromEngineResource("Object");
        CE_VERIFY(objShader);

        objShader->Use();
        objShader->Mat4(transformMatrix, "uTransform");
        material->Use();

        array->Bind();
        array->GetVertexBuffer()->Bind();
        array->GetIndexBuffer()->Draw();
    }

    void Mesh::SetGeometry(Geometry *newGeometry)
    {
        _ReleaseGeometry();

        geometry = newGeometry;

        _BufferGeometryArray();
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