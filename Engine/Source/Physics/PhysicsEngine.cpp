#include "PhysicsEngine.h"
#include "Core/Logger.h"
#include "Core/Engine.h"
#include "Body/RigidBody.h"
#include "Body/StaticBody.h"

#include "btBulletDynamicsCommon.h"

namespace Core
{
    static PhysicsEngine::PhysicsEngineState state;
    static btDefaultCollisionConfiguration collisionConfiguration;
    static btCollisionDispatcher dispatcher(&collisionConfiguration);
    static btDbvtBroadphase broadphase;
    static btSequentialImpulseConstraintSolver solver;
    static btDiscreteDynamicsWorld *btWorld;

    void PhysicsEngine::Init()
    {
        state.Numeric.MathFPS = 240.0f;
        btWorld = new btDiscreteDynamicsWorld(&dispatcher, &broadphase, &solver, &collisionConfiguration);
        btWorld->setGravity(btVector3(0.0f, -9.81f, 0.0f));
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
        btWorld->addRigidBody(b->GetHandle());
        return b;
    }

    StaticBody *PhysicsEngine::CreateStatic(StaticBodyConfiguration *config)
    {
        auto b = new StaticBody();
        b->UseConfiguration(config);
        state.Bodies.push_back(b);
        b->Update();
        btWorld->addRigidBody(b->GetHandle());
        return b;
    }

    KinematicBody *PhysicsEngine::CreateKinematic(KinematicBodyConfiguration *config)
    {
        auto b = new KinematicBody();
        b->UseConfiguration(config);
        state.Bodies.push_back(b);
        b->Update();
        btWorld->addRigidBody(b->GetHandle());
        return b;
    }

    void PhysicsEngine::UpdateRuntime()
    {
        btWorld->stepSimulation(Engine::GetDeltaTime());

        for (int i = 0; i < state.Bodies.size(); i++)
        {
            auto body = state.Bodies[i];
            body->Update();
        }
    }

    void PhysicsEngine::StopRuntime()
    {
        DestroyBodies();
        // TODO: delete btWorld;
    }

    void PhysicsEngine::DestroyBodies()
    {
        for (auto body : state.Bodies)
        {
            switch (body->GetType())
            {
            case PhysicsBody::Rigid:
                btWorld->removeRigidBody(body->As<RigidBody>()->GetHandle());
                break;

            case PhysicsBody::Static:
                btWorld->removeRigidBody(body->As<StaticBody>()->GetHandle());
                break;

            case PhysicsBody::Kinematic:
                btWorld->removeRigidBody(body->As<KinematicBody>()->GetHandle());
                break;
            }

            delete body;
        }

        state.Bodies.clear();
    }

    PhysicsEngine::NumericValues &PhysicsEngine::GetNumericValueSet()
    {
        return state.Numeric;
    }
}