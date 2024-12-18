#include "Components.h"
#include "Scene/Actor.h"
#include "Core/Logger.h"
#include "Core/Engine.h"

#include "Physics/Body/RigidBody.h"

namespace Core
{
    MeshComponent::MeshComponent()
    {
        mesh = new Mesh();
    }

    void MeshComponent::Destroy()
    {
        delete mesh;
    }

    void MeshComponent::Render()
    {
        Matrix4 t = Owner->GetTransformMatrix();
        mesh->Render(t);
    }

    void MeshComponent::From(MeshComponent *c)
    {
        switch (c->mesh->GetMaterial()->GetType())
        {
        case Material::Default:
        default:
            mesh->MakeMaterialDefault();
            break;

        case Material::Config:
            mesh->MakeMaterialUnique(c->mesh->GetMaterial()->GetState());
            break;

        case Material::File:
            mesh->MakeMaterialFromFile(c->mesh->GetMaterial()->GetFilePath());
            break;
        }

        auto geometry = c->mesh->GetGeometry();
        switch (geometry->GetType())
        {
        case Geometry::None:
        default:
            mesh->SetGeometry(new Geometry());
            break;

        case Geometry::Box:
        {
            auto *cast = geometry->As<BoxGeometry>();
            mesh->SetGeometry(new BoxGeometry(cast->Width, cast->Height, cast->Depth));
        }
        break;

        case Geometry::Sphere:
        {
            auto *cast = geometry->As<SphereGeometry>();
            mesh->SetGeometry(new SphereGeometry(cast->Radius, cast->LongitudeSegments, cast->LatitudeSegments));
        }
        break;
        }
    }

    DataSetComponent::DataSetComponent()
    {
    }

    DataSetComponent::~DataSetComponent()
    {
    }

    void DataSetComponent::From(DataSetComponent *c)
    {
        for (auto d : c->Set.GetSet())
        {
            CeData *c = new CeData();
            c->From(d);
            Set.Add(c);
        }
    }

    ScriptComponent::ScriptComponent()
    {
    }

    ScriptComponent::~ScriptComponent()
    {
    }

    void ScriptComponent::From(ScriptComponent *c)
    {
        ClassName = c->ClassName;
    }

    CameraComponent::CameraComponent()
    {
        Camera = new PerspectiveCamera();
    }

    CameraComponent::~CameraComponent()
    {
        delete Camera;
    }

    void CameraComponent::Render()
    {
        Camera->SetMatrixMode(Camera::InputMatrix);
        Camera->GetPosition().Set(Owner->GetTransform()->Position);
        Camera->GetRotation().Set(Owner->GetTransform()->Rotation);
        Camera->SetViewMatrix(Owner->GetTransformMatrix());
    }

    void CameraComponent::UpdateCameraState()
    {
        Camera->SetFOV(FOV);
        Camera->SetNear(Near);
        Camera->SetFar(Far);
        Camera->SetAspect(Engine::GetWindow()->GetWidth() / Engine::GetWindow()->GetHeight());
        Camera->UpdateProjection();
    }

    void CameraComponent::From(CameraComponent *c)
    {
        FOV = c->FOV;
        Near = c->Near;
        Far = c->Far;
        IsPrimary = c->IsPrimary;
    }

    PointLightComponent::PointLightComponent()
    {
        Light = new PointLight();
    }

    PointLightComponent::~PointLightComponent()
    {
        delete Light;
    }

    void PointLightComponent::Render()
    {
        Light->Position = Owner->GetTransform()->Position;
        Light->Render();
    }

    void PointLightComponent::From(PointLightComponent *c)
    {
        Light->Position = c->Light->Position;
        Light->Specular = c->Light->Specular;
        Light->Color = c->Light->Color;

        Light->Intensity = c->Light->Intensity;
        Light->Radius = c->Light->Radius;
        Light->Quadratic = c->Light->Quadratic;
        Light->Linear = c->Light->Linear;
        Light->Constant = c->Light->Constant;
    }

    TagComponent::TagComponent()
    {
        Tag = "";
    }

    TagComponent::~TagComponent()
    {
    }

    void TagComponent::From(TagComponent *c)
    {
        Tag = c->Tag;
    }

    RigidBodyComponent::RigidBodyComponent()
    {
        Collider = new AABBCollider();
    }

    RigidBodyComponent::~RigidBodyComponent()
    {
        delete Collider;
    }

    void RigidBodyComponent::From(RigidBodyComponent *c)
    {
        Config.From(&c->Config);
        Collider->From(c->Collider);
    }

    StaticBodyComponent::StaticBodyComponent()
    {
        Collider = new AABBCollider();
    }

    StaticBodyComponent::~StaticBodyComponent()
    {
        delete Collider;
    }

    void StaticBodyComponent::From(StaticBodyComponent *c)
    {
        Config.From(&c->Config);
        Collider->From(c->Collider);
    }
}