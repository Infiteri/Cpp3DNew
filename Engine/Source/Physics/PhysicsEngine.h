#pragma once

#include "Base.h"
#include "Body/RigidBody.h"
#include "Objects/Spring.h"
#include "Collision/Contact.h"
#include "Collision/ContactResolver.h"

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
            Spring TempSpring;
            NumericValues Numeric;
            std::vector<PhysicsBody *> Bodies;

            ContactResolver Resolver;
        };

        static void Init();
        static void Shutdown();

        static RigidBody *CreateRigid(RigidBodyConfiguration *config);

        static void UpdateRuntime();
        static void StopRuntime();

        static void DestroyBodies();

        static void CheckCollision(PhysicsBody *a, PhysicsBody *b);
        static void ResolveContacts();

        static NumericValues &GetNumericValueSet();
    };

}