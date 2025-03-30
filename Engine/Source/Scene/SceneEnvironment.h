#pragma once

#include "Base.h"
#include "Renderer/Sky/Sky.h"
#include "Renderer/Light/DirectionalLight.h"

#include <string>

namespace Core
{
    struct SceneEnvironment
    {
        SceneEnvironment() = default;
        ~SceneEnvironment();
        Sky *SkyInst;
        DirectionalLight DirectionalLight;

        void Setup();
        void From(SceneEnvironment* env);
    };
}