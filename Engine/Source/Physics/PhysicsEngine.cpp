#include "PhysicsEngine.h"
#include "Core/Logger.h"
#include "Body/RigidBody.h"
#include "Collision/ContactGenerator.h"

namespace Core
{
    static PhysicsEngine::PhysicsEngineState state;

    void PhysicsEngine::Init()
    {
        state.Numeric.MathFPS = 240.0f;
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
        for (int i = 0; i < state.Bodies.size(); i++)
        {
            auto body = state.Bodies[i];
            body->Update();

            for (int j = 0; j < state.Bodies.size(); j++)
            {
                if (i == j)
                    continue;

                auto body2 = state.Bodies[j];

                ContactGenerator gen;

                auto col1 = &body->As<RigidBody>()->collider;
                auto col2 = &body2->As<RigidBody>()->collider;

                bool res = gen.BoxAndBox(col1, col2);

                if (res)
                    CE_TRACE("YESSSSSSSSSSSSSS");
            }
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