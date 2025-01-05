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

    Quaternion::~Quaternion() {}

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
        if (d < FLT_EPSILON)
        {
            Set(1, 0, 0, 0);
            return;
        }

        d = 1.0f / sqrt(d);
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

    void Quaternion::SetFromEuler(const Vector3 &euler, bool convertToRadians)
    {
        float halfX = euler.x * (convertToRadians ? CE_DEG_TO_RAD : 1.0f) * 0.5f;
        float halfY = euler.y * (convertToRadians ? CE_DEG_TO_RAD : 1.0f) * 0.5f;
        float halfZ = euler.z * (convertToRadians ? CE_DEG_TO_RAD : 1.0f) * 0.5f;

        float cr = cos(halfX);
        float sr = sin(halfX);
        float cp = cos(halfY);
        float sp = sin(halfY);
        float cy = cos(halfZ);
        float sy = sin(halfZ);

        r = cr * cp * cy + sr * sp * sy;
        i = sr * cp * cy - cr * sp * sy;
        j = cr * sp * cy + sr * cp * sy;
        k = cr * cp * sy - sr * sp * cy;

        Normalize();
    }

    Vector3 Quaternion::GetEulerAngles(bool convertToDegree)
    {
        Vector3 angles;

        float norm = sqrt(r * r + i * i + j * j + k * k);
        float qx = i / norm;
        float qy = j / norm;
        float qz = k / norm;
        float qw = r / norm;

        float sinr_cosp = 2 * (qw * qx + qy * qz);
        float cosr_cosp = 1 - 2 * (qx * qx + qy * qy);
        angles.x = atan2(sinr_cosp, cosr_cosp);

        float sinp = 2 * (qw * qy - qz * qx);
        if (abs(sinp) >= 1)
            angles.y = (sinp >= 0 ? 1 : -1) * CE_PI / 2;
        else
            angles.y = asin(sinp);

        float siny_cosp = 2 * (qw * qz + qx * qy);
        float cosy_cosp = 1 - 2 * (qy * qy + qz * qz);
        angles.z = atan2(siny_cosp, cosy_cosp);

        angles *= convertToDegree ? CE_RAD_TO_DEG : 1.0f;

        return angles;
    }
}
