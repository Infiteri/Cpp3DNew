#pragma once

#include "Base.h"

#include "PhysicsBody.h"
#include "Math/Vectors.h"
#include "Math/Matrix4.h"
#include "Math/Quaternion.h"

namespace Core
{
    struct StaticBodyConfiguration
    {
        Actor *Owner;
        float Mass = 1.0f;

        void From(StaticBodyConfiguration *c);
    };

    class CE_API StaticBody : public PhysicsBody
    {
    private:
        StaticBodyConfiguration state;

        PhysMatrix4x3 transformMatrix;
        void _CalculateData();

        friend class PhysicsEngine;
        friend class Contact;

    public:
        StaticBody();
        ~StaticBody();

        inline StaticBodyConfiguration &GetConfiguration() { return state; };

        void UseConfiguration(StaticBodyConfiguration *config);

        void Update();

        inline float GetMass() { return state.Mass; };
        inline float GetInverseMass() {return 1.0f / state.Mass; };
    };
}