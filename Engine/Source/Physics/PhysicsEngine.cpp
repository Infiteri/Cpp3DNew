#include "PhysicsEngine.h"
#include "Core/Logger.h"
#include "Body/RigidBody.h"
#include "Body/StaticBody.h"
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
        b->Update();
        return b;
    }

    StaticBody *PhysicsEngine::CreateStatic(StaticBodyConfiguration *config)
    {
        auto b = new StaticBody();
        b->UseConfiguration(config);
        state.Bodies.push_back(b);
        b->Update();
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
                body2->_CalculateData();

                CheckCollision(body2, body);
            }
        }

        ResolveContacts();
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

    void PhysicsEngine::CheckCollision(PhysicsBody *a, PhysicsBody *b)
    {
        auto aCollider = a->GetCollider();
        auto bCollider = b->GetCollider();

        if (!aCollider || !bCollider)
            return;

        switch (aCollider->GetType())
        {
        case Collider::Box:
        {
            switch (bCollider->GetType())
            {
            case Collider::Box:
            {
                CollisionDetector detector;
                Contact c;
                c.Restitution = 0.5f;
                c.Friction = 0.1f;
                if (detector.BoxAndBox(aCollider->As<AABBCollider>(), bCollider->As<AABBCollider>(), &c))
                    state.Resolver.Contacts.push_back(c);
            }
            break;
            }
        }
        break;
        }
    }

    void PhysicsEngine::ResolveContacts()
    {
        if (state.Resolver.Contacts.size() == 0)
            return;

        state.Resolver.PositionEpsilon = 0.01f; // depth
        state.Resolver.VelocityEpsilon = 5.0f;  // ddl
        state.Resolver.Resolve();

        state.Resolver.Contacts.clear();
    }

    PhysicsEngine::NumericValues &PhysicsEngine::GetNumericValueSet()
    {
        return state.Numeric;
    }
}