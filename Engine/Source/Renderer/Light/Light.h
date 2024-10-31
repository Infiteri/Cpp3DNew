#pragma once

#include "Base.h"

namespace Core
{
    class Light
    {
    public:
        Light();
        ~Light();

        virtual void Render();
    };

}