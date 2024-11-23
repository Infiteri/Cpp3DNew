#pragma once

#include "Base.h"

#define CE_PI 3.14159265358979323846264338327950288f
#define CE_DEG_TO_RAD (CE_PI / 180)
#define CE_RAD_TO_DEG (180 / CE_PI)

#include "Vectors.h"
#include "Matrix4.h"

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
        CE_API float Abs(float x);
        CE_API float Tan(float x);
        CE_API float Pow(float x, float y);

        CE_API float Lerp(float min, float max, float t);
        CE_API float Clamp(float min, float max, float t);
    }
}