#pragma once

#include "Base.h"
#include "Body/RigidBody.h"
#include "Objects/Spring.h"

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
            float MathFPS = 240.0f;
        };

        struct PhysicsEngineState
        {
            Spring TempSpring;
            NumericValues Numeric;
            std::vector<PhysicsBody *> Bodies;
        };

        static void Init();
        static void Shutdown();

        static RigidBody *CreateRigid(RigidBodyConfiguration *config);

        static void UpdateRuntime();
        static void StopRuntime();

        static void DestroyBodies();

        static NumericValues &GetNumericValueSet();

        CE_API static Spring *GetTempSpring();
    };

}