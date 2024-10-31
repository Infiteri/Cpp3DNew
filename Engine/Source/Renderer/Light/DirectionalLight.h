#pragma once

#include "Base.h"
#include "Math/Vectors.h"
#include "Renderer/Color.h"
#include "Light.h"

namespace Core
{
    class DirectionalLight : public Light
    {
    public:
        Color Color{255, 255, 255, 255};
        Vector3 Direction{0, 0, -1};
        float Intensity = 1.0f;

        DirectionalLight();
        ~DirectionalLight();

        void Render();
    };

}