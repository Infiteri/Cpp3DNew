#include "PhysicsEngine.h"
#include "Body/RigidBody.h"

namespace Core
{
    static PhysicsEngine::PhysicsEngineState state;

    void PhysicsEngine::Init()
    {
    }

    void PhysicsEngine::Shutdown()
    {
    }

    RigidBody *PhysicsEngine::CreateRigid(RigidBodyConfiguration *config)
    {
        auto b = new RigidBody();
        b->UseConfiguration(config);
        state.Bodies.push_back(b);
        return b;
    }

    void PhysicsEngine::UpdateRuntime()
    {
        for (auto body : state.Bodies)
        {
            body->Update();
        }
    }

    void PhysicsEngine::StopRuntime()
    {
        DestroyBodies();
    }

    void PhysicsEngine::DestroyBodies()
    {
        for (auto body : state.Bodies)
            delete body;

        state.Bodies.clear();
    }

    PhysicsEngine::NumericValues &PhysicsEngine::GetNumericValueSet()
    {
        return state.Numeric;
    }
}