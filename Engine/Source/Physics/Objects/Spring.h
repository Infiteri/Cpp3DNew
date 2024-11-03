#pragma once

#include "Base.h"
#include "Physics/Body/RigidBody.h"

namespace Core
{
    struct Spring
    {
        float SpringConstant;
        float RestLength;

        RigidBody *TargetBody;
        struct Spring *OtherSpring;

        void Update();
    };

}