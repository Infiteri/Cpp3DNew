#pragma once

#include "Base.h"

namespace Core
{
    class LightID
    {
    public:
        LightID() {};
        ~LightID() {};

        static void Begin();

        static CeU32 GetNewPointLight();
        static CeU32 GetPointLight();
        static void AddNewPointLightCount();
        static CeU32 GetPointLightCount();
    };

}