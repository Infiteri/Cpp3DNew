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

    Quaternion::Quaternion(const Vector3 &axis, float angle)
    {
        float half = 0.5f * angle;
        float s = sin(half);
        float c = cos(half);

        x = s * axis.x;
        y = s * axis.y;
        z = s * axis.z;
        w = c;
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
        float halfX = euler.y * (convertToRadians ? CE_DEG_TO_RAD : 1.0f) * 0.5f;
        float halfY = euler.x * (convertToRadians ? CE_DEG_TO_RAD : 1.0f) * 0.5f;
        float halfZ = euler.z * (convertToRadians ? CE_DEG_TO_RAD : 1.0f) * 0.5f;

        float cosA1 = cos(halfX);
        float sinA1 = sin(halfX);
        float cosA2 = cos(halfY);
        float sinA2 = sin(halfY);
        float cosA3 = cos(halfZ);
        float sinA3 = sin(halfZ);

        x = sinA1 * cosA2 * sinA3 + cosA1 * sinA2 * cosA3,
        y = sinA1 * cosA2 * cosA3 - cosA1 * sinA2 * sinA3,
        z = -sinA1 * sinA2 * cosA3 + cosA1 * cosA2 * sinA3,
        w = sinA1 * sinA2 * sinA3 + cosA1 * cosA2 * cosA3;
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

    Matrix4 Quaternion::GetMatrix() 
    {
        Quaternion n;
        n.Set(*this);
        Normalize();

        Matrix4 m;
        m.data[0] = 1 - 2 * (y * y + z * z);
        m.data[1] = 2 * (x * y - z * w);
        m.data[2] = 2 * (x * z + y * w);
        m.data[3] = 0;

        m.data[4] = 2 * (x * y + z * w);
        m.data[5] = 1 - 2 * (x * x + z * z);
        m.data[6] = 2 * (y * z - x * w);
        m.data[7] = 0;

        m.data[8] = 2 * (x * z - y * w);
        m.data[9] = 2 * (y * z + x * w);
        m.data[10] = 1 - 2 * (x * x + y * y);
        m.data[11] = 0;

        m.data[12] = 0;
        m.data[13] = 0;
        m.data[14] = 0;
        m.data[15] = 1;

        return m;
    }
}
