#include "PhysicsEngine.h"
#include "Core/Logger.h"
#include "Core/Engine.h"
#include "Scene/Actor.h"
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

    void PhysicsEngine::_RigidCollisionCallbackRoutine()
    {
        int numManifolds = dispatcher.getNumManifolds();
        for (int i = 0; i < numManifolds; i++)
        {
            btPersistentManifold *contactManifold = dispatcher.getManifoldByIndexInternal(i);
            const btCollisionObject *objA = contactManifold->getBody0();
            const btCollisionObject *objB = contactManifold->getBody1();

            int numContacts = contactManifold->getNumContacts();
            for (int j = 0; j < numContacts; j++)
            {
                btManifoldPoint &pt = contactManifold->getContactPoint(j);
                if (pt.getDistance() < 0.0f)
                {
                    Actor *a1 = (Actor *)objA->getUserPointer();
                    Actor *a2 = (Actor *)objB->getUserPointer();
                    _OnCollision(a1, a2);
                }
            }
        }
    }

    void PhysicsEngine::_OnCollision(Actor *a1, Actor *a2)
    {
        CE_VERIFY(a1 && a2);

        if (a1->GetUUID().Get() == a2->GetUUID().Get())
            return;
    }

    void PhysicsEngine::Init()
    {
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
        btWorld->addRigidBody(b->handle);
        btWorld->addCollisionObject(b->ghost, btBroadphaseProxy::CharacterFilter, btBroadphaseProxy::AllFilter);
        return b;
    }

    void PhysicsEngine::UpdateRuntime()
    {
        btWorld->stepSimulation(Engine::GetDeltaTime());

        _RigidCollisionCallbackRoutine();

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
                btWorld->removeRigidBody(body->As<KinematicBody>()->handle);
                btWorld->removeCollisionObject(body->As<KinematicBody>()->ghost);
                break;
            }

            delete body;
        }

        state.Bodies.clear();
    }
}