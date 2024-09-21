#include "Actor.h"
#include "Renderer/Object/Mesh.h"

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