#include "PhysicsEngine.h"
#include "Body/RigidBody.h"

namespace Core
{
    static PhysicsEngine::PhysicsEngineState state;

    void PhysicsEngine::Init()
    {
        state.TempSpring.RestLength = 1.0f;
        state.TempSpring.SpringConstant = 1.0f;
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

        // state.TempSpring.OtherSpring = new Spring();
        // state.TempSpring.OtherSpring->TargetBody = (RigidBody *)state.Bodies[1];
        // state.TempSpring.TargetBody = (RigidBody *)state.Bodies[0];
        // state.TempSpring.Update();
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

    Spring *PhysicsEngine::GetTempSpring()
    {
        return &state.TempSpring;
    }
}