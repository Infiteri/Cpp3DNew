#include "Actor.h"
#include "Core/Logger.h"
#include "Renderer/Object/Mesh.h"

#define CE_COPY_COMPONENT(type)                          \
    auto type##COMPONENT = other->GetComponents<type>(); \
    for (auto c : type##COMPONENT)                       \
    outActor->AddComponent<type>()->From(c)

namespace Core
{
    // Any default initialization will be done here
    Actor::Actor()
    {
        name = "Actor";
        state = Initialized;
        id = UUID();
    }

    Actor::~Actor()
    {
        for (auto child : children)
            delete child;

        for (auto component : components)
            delete component;

        children.clear();
        components.clear();
    }

    Actor *Actor::From(Actor *other, bool copyUUID)
    {
        if (!other)
            return nullptr;

        Actor *outActor = new Actor();
        outActor->SetName(other->GetName());
        outActor->parent = other->GetParent();

        if (copyUUID)
            outActor->id = other->id;

        auto transform = other->GetTransform();
        outActor->GetTransform()->From(transform);

        // ADD WHEN NEW COMPONENTS
        CE_COPY_COMPONENT(TagComponent);
        CE_COPY_COMPONENT(MeshComponent);
        CE_COPY_COMPONENT(DataSetComponent);
        CE_COPY_COMPONENT(ScriptComponent);
        CE_COPY_COMPONENT(CameraComponent);
        CE_COPY_COMPONENT(PointLightComponent);
        CE_COPY_COMPONENT(RigidBodyComponent);
        CE_COPY_COMPONENT(StaticBodyComponent);
        CE_COPY_COMPONENT(KinematicBodyComponent);
        CE_COPY_COMPONENT(BoxColliderComponent);

        for (Actor *a : other->GetChildren())
            outActor->AddChild(Actor::From(a, copyUUID));

        return outActor;
    }

    void Actor::_CalculateMatrices()
    {
        worldMatrix = localMatrix = transform.GetTransformMatrix();

        if (parent)
            worldMatrix = parent->worldMatrix * localMatrix;
    }

    void Actor::Start()
    {
        if (state == Running || state == Started)
            return;

        state = Started;

        for (auto comp : components)
        {
            comp->Start();
        }

        for (auto a : children)
        {
            a->Start();
        }
    }

    void Actor::Render()
    {
        // Clamp transform rotation
        for (int i = 0; i < 3; i++)
        {
            if (transform.Rotation[i] > 360)
                transform.Rotation[i] -= 360;

            if (transform.Rotation[i] < 0)
                transform.Rotation[i] += 360;
        }

        _CalculateMatrices();

        for (auto comp : components)
        {
            comp->Render();
        }

        for (auto a : children)
        {
            a->Render();
        }
    }

    void Actor::Update()
    {
        if (state != Started && (state != Running))
            return;

        state = Running;

        for (auto comp : components)
        {
            comp->Update();
        }

        for (auto a : children)
        {
            a->Update();
        }
    }

    void Actor::Stop()
    {
        if (state == Running)
            return;

        state = Stopped;

        for (auto comp : components)
        {
            comp->Stop();
        }

        for (auto a : children)
        {
            a->Stop();
        }
    }

    Transform Actor::GetWorldTransform()
    {
        Transform t;

        t.Position = {worldMatrix[12], worldMatrix[13], worldMatrix[14]};
        t.Rotation = worldMatrix.GetEulerAnglesZYX();
        t.Scale = {worldMatrix[0], worldMatrix[5], worldMatrix[10]};

        return t;
    }

    void Actor::AddChild(Actor *actor)
    {
        if (!actor)
            return;

        if (state != Initialized || state != Stopped)
            actor->Start();

        actor->parent = this;
        children.push_back(actor);
    }

    Actor *Actor::SpawnChild()
    {
        Actor *a = new Actor();
        AddChild(a);
        return a;
    }

    Actor *Actor::FindChild(const UUID &uuid)
    {
        if (id == uuid)
            return this;

        for (auto c : children)
            if (c->GetUUID() == uuid)
                return c;

        return nullptr;
    }

    Actor *Actor::FindChild(const std::string &n)
    {
        if (name == n)
            return this;

        for (auto c : children)
            if (c->name == n)
                return c;

        return nullptr;
    }

    Actor *Actor::FindChildInHierarchy(const UUID &uuid)
    {
        if (this->id == uuid)
            return this;

        for (auto child : children)
            if (child->GetUUID() == id)
                return child;

        for (Actor *child : children)
        {
            Actor *result = child->FindChildInHierarchy(uuid);
            if (result != nullptr)
                return result;
        }

        return nullptr;
    }

    void Actor::RemoveActor(const UUID &uuid)
    {
        Actor *a;

        int index = -1;
        for (auto child : children)
        {
            index++;
            if (child->id == uuid)
            {
                child->parent = nullptr;
                children.erase(children.begin() + index);
                break;
            }
        }
    }

    CameraComponent *Actor::FindCameraComponentInChildren(bool primaryMatters)
    {
        CameraComponent *wantedCamera = nullptr;
        auto thisActorCameras = GetComponents<CameraComponent>();
        for (auto cam : thisActorCameras)
        {
            if (primaryMatters)
            {
                if (cam->IsPrimary)
                    wantedCamera = cam;
                return cam;
            }
            else
            {
                return cam; // don't no care if its primary
            }
        }

        // recursive
        for (auto child : children)
        {
            wantedCamera = child->FindCameraComponentInChildren(primaryMatters);
        }

        return wantedCamera;
    }
}