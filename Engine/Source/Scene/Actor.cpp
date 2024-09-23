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
    }

    Actor::~Actor()
    {
    }

    void Actor::_CalculateMatrices()
    {
        localMatrix = transform.GetTransformMatrix();
        worldMatrix.From(&localMatrix);
    }

    void Actor::Start()
    {
        if (state == Running)
            return;

        state = Started;

        for (auto comp : components)
        {
            comp->Start();
        }
    }

    void Actor::Render()
    {
        if (state == Started)
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
    }

    void Actor::Update()
    {
        if (state == Started)
            return;

        state = Running;

        for (auto comp : components)
        {
            comp->Update();
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
    }
}