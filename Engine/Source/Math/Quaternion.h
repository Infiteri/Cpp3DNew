#pragma once

#include "Base.h"
#include "Vectors.h"

namespace Core
{
    class CE_API Quaternion
    {
    public:
        union
        {
            struct
            {
                float r;
                float i;
                float j;
                float k;
            };

            float data[4];
        };

        Quaternion();
        Quaternion(const Quaternion &q);
        Quaternion(float r, float i, float j, float k);
        ~Quaternion();

        void Set(float r, float i, float j, float k);
        void Set(const Quaternion &quat);
        void Set(Quaternion *quat);

        void Normalize();
        Quaternion Normalized();

        void RotateBy(const Vector3 &vector);

        void operator*=(const Quaternion &multiplier)
        {
            Quaternion q = *this;
            r = q.r * multiplier.r - q.i * multiplier.i -
                q.j * multiplier.j - q.k * multiplier.k;
            i = q.r * multiplier.i + q.i * multiplier.r +
                q.j * multiplier.k - q.k * multiplier.j;
            j = q.r * multiplier.j + q.j * multiplier.r +
                q.k * multiplier.i - q.i * multiplier.k;
            k = q.r * multiplier.k + q.k * multiplier.r +
                q.i * multiplier.j - q.j * multiplier.i;
        }

        void operator+=(const Vector3 &vec)
        {
            Quaternion q(0,
                         vec.x,
                         vec.y,
                         vec.z);
            q *= *this;
            r += q.r * 0.5f;
            i += q.i * 0.5f;
            j += q.j * 0.5f;
            k += q.k * 0.5f;
        }
    };
}