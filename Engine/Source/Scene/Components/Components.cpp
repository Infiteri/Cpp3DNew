
#include "Components.h"
#include "Scene/Actor.h"
#include "Core/Logger.h"
#include "Core/Engine.h"

#include "Renderer/Renderer.h"

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
            mesh->SetGeometry(new SphereGeometry(cast->Radius, cast->Subdivision));
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
        Camera->GetRotation().Set(Owner->GetTransform()->Rotation * CE_DEG_TO_RAD);

        if (!IgnoreMatrixUpload)
            Camera->SetViewMatrix(Owner->GetTransformMatrix());
    }

    void CameraComponent::UpdateCameraState()
    {
        Camera->SetFOV(FOV);
        Camera->SetNear(Near);
        Camera->SetFar(Far);
        Camera->SetAspect(Renderer::GetViewport().Aspect());
        Camera->UpdateProjection();
    }

    float CameraComponent::ClampRotation(float angle, float degree)
    {
        float rot = angle;
        if (rot > 180.0f)
            rot -= 360.0f;

        if (rot < -degree)
            rot = -degree;
        else if (rot > degree)
            rot = degree;

        if (rot < 0.0f)
            rot += 360.0f;

        return rot;
    }

    void CameraComponent::ClampRotation(Angles angles, float degree, bool isOnActor)
    {
        Vector3 *rot = isOnActor ? &Owner->GetTransform()->Rotation : &Camera->GetRotation();

        if (angles & X)
            rot->x = ClampRotation(rot->x, degree);

        if (angles & Y)
            rot->y = ClampRotation(rot->y, degree);

        if (angles & Z)
            rot->z = ClampRotation(rot->z, degree);
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
    }

    RigidBodyComponent::~RigidBodyComponent()
    {
    }

    void RigidBodyComponent::From(RigidBodyComponent *c)
    {
        Config.From(&c->Config);
    }

    StaticBodyComponent::StaticBodyComponent()
    {
    }

    StaticBodyComponent::~StaticBodyComponent()
    {
    }

    void StaticBodyComponent::From(StaticBodyComponent *c)
    {
        Config.From(&c->Config);
    }

    KinematicBodyComponent::KinematicBodyComponent()
    {
    }

    KinematicBodyComponent::~KinematicBodyComponent()
    {
    }

    void KinematicBodyComponent::From(KinematicBodyComponent *c)
    {
        Config.From(&c->Config);
    }

    void BoxColliderComponent::From(BoxColliderComponent *c)
    {
        Width = c->Width;
        Height = c->Height;
        Depth = c->Depth;
    }
}