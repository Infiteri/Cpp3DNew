#pragma once

#include "Base.h"
#include "Renderer/Sky/Sky.h"

#include <string>

namespace Core
{
    struct SceneEnvironment
    {
        Sky *SkyInst;

        void Setup();
    };
}