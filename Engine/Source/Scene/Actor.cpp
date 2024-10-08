#include "Actor.h"
#include "Renderer/Object/Mesh.h"
#include "Renderer/Shader/ShaderSystem.h"

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
    }

    Actor *Actor::From(Actor *other)
    {

        if (!other)
            return nullptr;

        Actor *outActor = new Actor();
        outActor->SetName(other->GetName());
        outActor->parent = other->GetParent();

        auto transform = other->GetTransform();
        auto mesh = other->GetComponent<MeshComponent>();

        outActor->GetTransform()->From(transform);

        if (mesh)
            outActor->AddComponent<MeshComponent>()->From(mesh);

        for (Actor *a : other->GetChildren())
            outActor->AddChild(Actor::From(a));

        return other;
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
        if ((state != Started) && (state != Running))
            return;

        state = Running;

        //? what
        _CalculateMatrices();
        auto shader = ShaderSystem::GetFromEngineResource("Object");
        if (shader)
            shader->Mat4(&worldMatrix, "uTransform");

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
        if (state != Started)
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

    Actor *Actor::FindChildByUUID(const UUID &uuid)
    {
        if (id == uuid)
            return this;

        for (auto c : children)
            if (c->GetUUID() == uuid)
                return c;

        return nullptr;
    }

    Actor *Actor::FindChildInHierarchyByUUID(const UUID &uuid)
    {
        if (this->id == uuid)
            return this;

        for (auto child : children)
            if (child->GetUUID() == id)
                return child;

        for (Actor *child : children)
        {
            Actor *result = child->FindChildInHierarchyByUUID(uuid);
            if (result != nullptr)
                return result;
        }

        return nullptr;
    }

    void Actor::RemoveActorByUUID(const UUID &uuid)
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
}