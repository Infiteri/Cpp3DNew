#pragma once

#include "Base.h"
#include "Vectors.h"
#include "Matrix4.h"

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

            struct
            {
                float w;
                float x;
                float y;
                float z;
            };

            float data[4];
        };

        Quaternion();
        Quaternion(const Vector3 &axis, float angle);
        Quaternion(const Quaternion &q);
        Quaternion(float r, float i, float j, float k);
        ~Quaternion();

        void Set(float r, float i, float j, float k);
        void Set(const Quaternion &quat);
        void Set(Quaternion *quat);

        void Normalize();
        Quaternion Normalized();

        void SetFromEuler(const Vector3 &euler, bool convertToRadians = true);
        Vector3 GetEulerAngles(bool convertToDegree = true);

        Quaternion operator*(const Quaternion &q)
        {
            float W = (q.w * w) - (q.x * x) - (q.y * y) - (q.z * z);
            float X = (q.w * x) + (q.x * w) + (q.y * z) - (q.z * y);
            float Y = (q.w * y) - (q.x * z) - (q.y * w) - (q.z * x);
            float Z = (q.w * z) - (q.x * y) - (q.y * x) - (q.z * w);
            Quaternion ret;
            ret.w = W;
            ret.x = X;
            ret.y = Y;
            ret.z = Z;
            return ret;
        }

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

        Matrix4 GetMatrix();
    };
}