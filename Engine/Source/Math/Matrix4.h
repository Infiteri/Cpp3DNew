#pragma once

#include "Base.h"
#include "Vectors.h"

namespace Core
{
    /// @brief Useful for physics math like transform and inversions. Used in physics not of use by the user
    class PhysMatrix4x3
    {
    public:
        union
        {
            struct
            {
                float _11, _12, _13, _14;
                float _21, _22, _23, _24;
                float _31, _32, _33, _34;
            };

            float data[12];
            float m[4][3];
        };

        PhysMatrix4x3();
        PhysMatrix4x3(const PhysMatrix4x3 &other);
        PhysMatrix4x3(float data11, float data12, float data13, float data14,
                      float data21, float data22, float data23, float data24,
                      float data31, float data32, float data33, float data34);
        ~PhysMatrix4x3();

        void From(const PhysMatrix4x3 &mat);
        void From(float *data);
        void From(PhysMatrix4x3 *mat);

        float GetDeterment();
        void SetInverse(const PhysMatrix4x3 &m);
        void Invert();
        PhysMatrix4x3 Inverted();

        operator const float *() { return data; };
        float &operator[](int index)
        {
            return data[index];
        };

        Vector3 operator*(const Vector3 &vector) const
        {
            return Vector3(
                vector.x * data[0] +
                    vector.y * data[1] +
                    vector.z * data[2] + data[3],

                vector.x * data[4] +
                    vector.y * data[5] +
                    vector.z * data[6] + data[7],

                vector.x * data[8] +
                    vector.y * data[9] +
                    vector.z * data[10] + data[11]);
        };

        PhysMatrix4x3 operator*(const PhysMatrix4x3 &o) const
        {
            PhysMatrix4x3 result;
            result.data[0] = (o.data[0] * data[0]) + (o.data[4] * data[1]) + (o.data[8] * data[2]);
            result.data[4] = (o.data[0] * data[4]) + (o.data[4] * data[5]) + (o.data[8] * data[6]);
            result.data[8] = (o.data[0] * data[8]) + (o.data[4] * data[9]) + (o.data[8] * data[10]);

            result.data[1] = (o.data[1] * data[0]) + (o.data[5] * data[1]) + (o.data[9] * data[2]);
            result.data[5] = (o.data[1] * data[4]) + (o.data[5] * data[5]) + (o.data[9] * data[6]);
            result.data[9] = (o.data[1] * data[8]) + (o.data[5] * data[9]) + (o.data[9] * data[10]);

            result.data[2] = (o.data[2] * data[0]) + (o.data[6] * data[1]) + (o.data[10] * data[2]);
            result.data[6] = (o.data[2] * data[4]) + (o.data[6] * data[5]) + (o.data[10] * data[6]);
            result.data[10] = (o.data[2] * data[8]) + (o.data[6] * data[9]) + (o.data[10] * data[10]);

            result.data[3] = (o.data[3] * data[0]) + (o.data[7] * data[1]) + (o.data[11] * data[2]) + data[3];
            result.data[7] = (o.data[3] * data[4]) + (o.data[7] * data[5]) + (o.data[11] * data[6]) + data[7];
            result.data[11] = (o.data[3] * data[8]) + (o.data[7] * data[9]) + (o.data[11] * data[10]) + data[11];
            return result;
        }

        Vector3 GetAxisVector3(int i)
        {
            return Vector3(data[i], data[i + 4], data[i + 8]);
        }
    };

    /// @brief Useful for inertia tensor. Used in physics not of use by the user
    class PhysMatrix3
    {
    public:
        union
        {
            struct
            {
                float _11, _12, _13;
                float _21, _22, _23;
                float _31, _32, _33;
            };

            float data[9];
            float m[3][3];
        };

        PhysMatrix3();
        PhysMatrix3(const PhysMatrix3 &m);
        PhysMatrix3(float data11, float data12, float data13,
                    float data21, float data22, float data23,
                    float data31, float data32, float data33);
        ~PhysMatrix3();

        void From(const PhysMatrix3 &mat);
        void From(float *data);
        void From(PhysMatrix3 *mat);

        void SetTranspose(const PhysMatrix3 &m);
        void Transpose();

        void SetSkewSymmetric(const Vector3 &vector)
        {
            data[0] = data[4] = data[8] = 0;
            data[1] = -vector.z;
            data[2] = vector.y;
            data[3] = vector.z;
            data[5] = -vector.x;
            data[6] = -vector.y;
            data[7] = vector.x;
        };

        void SetComponents(const Vector3 &compOne, const Vector3 &compTwo,
                           const Vector3 &compThree);

        operator const float *() { return data; };
        float &operator[](int index)
        {
            return data[index];
        };

        Vector3 operator*(const Vector3 &vector) const
        {
            return Vector3(
                vector.x * data[0] + vector.y * data[1] + vector.z * data[2],
                vector.x * data[3] + vector.y * data[4] + vector.z * data[5],
                vector.x * data[6] + vector.y * data[7] + vector.z * data[8]);
        }

        PhysMatrix3 operator*(const PhysMatrix3 &o) const
        {
            return PhysMatrix3(data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6],
                               data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7],
                               data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8],

                               data[3] * o.data[0] + data[4] * o.data[3] + data[5] * o.data[6],
                               data[3] * o.data[1] + data[4] * o.data[4] + data[5] * o.data[7],
                               data[3] * o.data[2] + data[4] * o.data[5] + data[5] * o.data[8],

                               data[6] * o.data[0] + data[7] * o.data[3] + data[8] * o.data[6],
                               data[6] * o.data[1] + data[7] * o.data[4] + data[8] * o.data[7],
                               data[6] * o.data[2] + data[7] * o.data[5] + data[8] * o.data[8]);
        }

        void operator*=(const PhysMatrix3 &o)
        {
            float t1;
            float t2;
            float t3;

            t1 = data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6];
            t2 = data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7];
            t3 = data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8];
            data[0] = t1;
            data[1] = t2;
            data[2] = t3;

            t1 = data[3] * o.data[0] + data[4] * o.data[3] + data[5] * o.data[6];
            t2 = data[3] * o.data[1] + data[4] * o.data[4] + data[5] * o.data[7];
            t3 = data[3] * o.data[2] + data[4] * o.data[5] + data[5] * o.data[8];
            data[3] = t1;
            data[4] = t2;
            data[5] = t3;

            t1 = data[6] * o.data[0] + data[7] * o.data[3] + data[8] * o.data[6];
            t2 = data[6] * o.data[1] + data[7] * o.data[4] + data[8] * o.data[7];
            t3 = data[6] * o.data[2] + data[7] * o.data[5] + data[8] * o.data[8];
            data[6] = t1;
            data[7] = t2;
            data[8] = t3;
        }

        void operator*=(float scalar)
        {
            data[0] *= scalar;
            data[1] *= scalar;
            data[2] *= scalar;
            data[3] *= scalar;
            data[4] *= scalar;
            data[5] *= scalar;
            data[6] *= scalar;
            data[7] *= scalar;
            data[8] *= scalar;
        }

        void operator+=(const PhysMatrix3 &o)
        {
            data[0] += o.data[0];
            data[1] += o.data[1];
            data[2] += o.data[2];
            data[3] += o.data[3];
            data[4] += o.data[4];
            data[5] += o.data[5];
            data[6] += o.data[6];
            data[7] += o.data[7];
            data[8] += o.data[8];
        }

        PhysMatrix3 Transposed()
        {
            PhysMatrix3 result;
            result.SetTranspose(*this);
            return result;
        }

        Vector3 TransformTranspose(const Vector3 &vector) const
        {
            return Vector3(
                vector.x * data[0] + vector.y * data[3] + vector.z * data[6],
                vector.x * data[1] + vector.y * data[4] + vector.z * data[7],
                vector.x * data[2] + vector.y * data[5] + vector.z * data[8]);
        }

        Vector3 Transform(const Vector3 &vector) const
        {
            return (*this) * vector;
        }

        void SetInverse(const PhysMatrix3 &m);
        PhysMatrix3 Inverted();
        void Invert();

        void SetInertiaTensorCoeffs(float ix, float iy, float iz,
                                    float ixy = 0, float ixz = 0, float iyz = 0);

        void SetBlockInertiaTensor(const Vector3 &sizes, float mass);
    };

    /// @brief Brief structure of the matrix
    /// 0 1 2 3
    /// 4 5 6 7
    /// 8 9 10 11
    /// 12 13 14 15
    class CE_API Matrix4
    {
    public:
        union
        {
            float data[16];
            float m[4][4];
        };

        Matrix4();
        ~Matrix4();

        Vector3 TransformVector(const Vector3 &vec) const
        {
            // Homogeneous vector with w = 1 (for position transformation)
            float x = vec.x * m[0][0] + vec.y * m[0][1] + vec.z * m[0][2] + m[0][3];
            float y = vec.x * m[1][0] + vec.y * m[1][1] + vec.z * m[1][2] + m[1][3];
            float z = vec.x * m[2][0] + vec.y * m[2][1] + vec.z * m[2][2] + m[2][3];
            float w = vec.x * m[3][0] + vec.y * m[3][1] + vec.z * m[3][2] + m[3][3];

            // Convert back to 3D space (perspective divide for w ≠ 1)
            if (w != 0.0f)
            {
                x /= w;
                y /= w;
                z /= w;
            }

            return Vector3(x, y, z);
        }

        void operator*=(const Matrix4 &m)
        {
            From(Matrix4::Multiply(*this, m));
        }

        operator const float *() { return data; };
        float &operator[](int index)
        {
            return data[index];
        };

        Matrix4 operator*(const Matrix4 &m)
        {
            Matrix4 a;
            a.From(this);
            a *= m;
            return a;
        }

        void From(Matrix4 *other);
        void From(Matrix4 other);
        void From(float *data);
        void Normalized();
        Vector3 GetEulerAnglesZYX() const;

        static Matrix4 Identity();
        static Matrix4 Empty();

        // WIP: Projections and transformations

        static Matrix4 Perspective(float fov, float aspect, float near, float far);
        static Matrix4 Translate(float x, float y, float z);
        static Matrix4 Translate(Vector3 *vec);
        static Matrix4 Translate(const Vector3 &vec);

        static Matrix4 Multiply(Matrix4 *a, Matrix4 *b);
        static Matrix4 Invert(Matrix4 *a);

        static Matrix4 Multiply(Matrix4 a, Matrix4 b);
        static Matrix4 Invert(Matrix4 a);

        static Matrix4 RotationXYZ(Vector3 vec);
        static Matrix4 RotationXYZ(Vector3 *vec);
        static Matrix4 RotationXYZ(float rotationX, float rotationY, float rotationZ);

        static Matrix4 RotationZYX(Vector3 vec);
        static Matrix4 RotationZYX(Vector3 *vec);
        static Matrix4 RotationZYX(float rotationX, float rotationY, float rotationZ);

        static Matrix4 RotationX(float rotation);
        static Matrix4 RotationY(float rotation);
        static Matrix4 RotationZ(float rotation);

        static Vector3 Forward(Matrix4 mat);
        static Vector3 Forward(Matrix4 *mat);

        static Vector3 Right(Matrix4 mat);
        static Vector3 Right(Matrix4 *mat);

        static Matrix4 Scale(Vector3 *vec);
        static Matrix4 Scale(float x, float y, float z);

        static Matrix4 Create(float *data);
    };
}