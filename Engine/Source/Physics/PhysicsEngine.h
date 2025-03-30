#pragma once

#include "Base.h"
#include "Body/RigidBody.h"
#include "Body/StaticBody.h"
#include "Body/KinematicBody.h"

#include <vector>

namespace Core
{
    class PhysicsEngine
    {
    private:
        static void _RigidCollisionCallbackRoutine();
        static void _OnCollision(Actor *a1, Actor *a2);

        friend class RigidBody;
        friend class StaticBody;
        friend class KinematicBody;

    public:
        PhysicsEngine() {};
        ~PhysicsEngine() {};

        struct PhysicsEngineState
        {
            std::vector<PhysicsBody *> Bodies;
        };

        static void Init();
        static void Shutdown();

        static RigidBody *CreateRigid(RigidBodyConfiguration *config);
        static StaticBody *CreateStatic(StaticBodyConfiguration *config);
        static KinematicBody *CreateKinematic(KinematicBodyConfiguration *config);

        static void UpdateRuntime();
        static void StopRuntime();

        static void DestroyBodies();
    };
}