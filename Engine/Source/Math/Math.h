#pragma once

#include "Base.h"

#define CE_PI 3.14f
#define CE_DEG_TO_RAD (CE_PI / 180)
#define CE_RAD_TO_DEG (180 / CE_PI)

namespace Core
{
    namespace Math
    {
        CE_API int Max(int a, int b);
        CE_API int Min(int a, int b);
        CE_API float DegToRad(float a);
        CE_API float RadToDeg(float a);

        CE_API float Sqrt(float x);
        CE_API float Sin(float x);
        CE_API float Cos(float x);
        CE_API float Tan(float x);

        CE_API float Lerp(float min, float max, float t);
        CE_API float Clamp(float min, float max, float t);
    }
}