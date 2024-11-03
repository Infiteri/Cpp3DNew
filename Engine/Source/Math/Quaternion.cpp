#include "Quaternion.h"
#include "Math.h"

namespace Core
{
    Quaternion::Quaternion()
    {
        Set(0, 0, 0, 1);
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
        if (d == 0)
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

    void Quaternion::RotateBy(const Vector3 &vector)
    {
        Quaternion q(0, vector.x, vector.y,
                     vector.z);
        (*this) *= q;
    }
}