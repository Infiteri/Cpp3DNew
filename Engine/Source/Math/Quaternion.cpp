#include "Quaternion.h"
#include "Math.h"

#include <float.h>
#include <cmath>

namespace Core
{
    Quaternion::Quaternion()
    {
        Set(1, 0, 0, 0);
    }

    Quaternion::Quaternion(const Quaternion &q)
    {
        Set(q);
    }

    Quaternion::Quaternion(float r, float i, float j, float k)
    {
        Set(r, i, j, k);
    }

    Quaternion::~Quaternion()
    {
    }

    void Quaternion::Set(float r, float i, float j, float k)
    {
        this->r = r;
        this->i = i;
        this->j = j;
        this->k = k;
    }

    void Quaternion::Set(const Quaternion &quat)
    {
        Set(quat.r, quat.i, quat.j, quat.k);
    }

    void Quaternion::Set(Quaternion *quat)
    {
        Set(quat->r, quat->i, quat->j, quat->k);
    }

    void Quaternion::Normalize()
    {
        float d = r * r + i * i + j * j + k * k;
        if (d < DBL_EPSILON)
        {
            r = 1;
            return;
        }

        d = 1.0 / Math::Sqrt(d);
        r *= d;
        i *= d;
        j *= d;
        k *= d;
    }

    Quaternion Quaternion::Normalized()
    {
        Quaternion q = (*this);
        q.Normalize();
        return q;
    }

    void Quaternion::SetFromEuler(const Vector3 &euler)
    {
        float cy = cos(euler.z * 0.5f);
        float sy = sin(euler.z * 0.5f);
        float cp = cos(euler.y * 0.5f);
        float sp = sin(euler.y * 0.5f);
        float cr = cos(euler.x * 0.5f);
        float sr = sin(euler.x * 0.5f);

        r = cr * cp * cy + sr * sp * sy;
        i = sr * cp * cy - cr * sp * sy;
        j = cr * sp * cy + sr * cp * sy;
        k = cr * cp * sy - sr * sp * cy;
    }

}