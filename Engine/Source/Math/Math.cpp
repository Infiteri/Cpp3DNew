#include "Math.h"

#include <cmath>

namespace Core
{
    namespace Math
    {
        int Max(int a, int b)
        {
            if (a > b)
                return a;
            else
                return b;
        }

        int Min(int a, int b)
        {
            if (a < b)
                return a;
            else
                return b;
        }

        float DegToRad(float a)
        {
            return a * CE_DEG_TO_RAD;
        }

        float RadToDeg(float a)
        {
            return a * CE_RAD_TO_DEG;
        }

        float Sqrt(float x)
        {
            return sqrtf(x);
        }

        float Sin(float x)
        {
            return sinf(x);
        }

        float Cos(float x)
        {
            return cosf(x);
        }

        float Abs(float x)
        {
            return abs(x);
        }

        float Tan(float x)
        {
            return tanf(x);
        }

        float Pow(float x, float y)
        {
            return powf(x, y);
        }

        float Lerp(float min, float max, float t)
        {
            return min + (max - min) * t;
        }

        float Clamp(float min, float max, float t)
        {
            if (t < min)
                return min;
            else if (t > max)
                return max;
            else
                return t;
        }

    }
}