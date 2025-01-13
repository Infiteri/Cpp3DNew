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
    public:
        PhysicsEngine() {};
        ~PhysicsEngine() {};

        struct NumericValues
        {
            float MathFPS = 600.0f;
        };

        struct PhysicsEngineState
        {
            NumericValues Numeric;
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

        static NumericValues &GetNumericValueSet();
    };

}