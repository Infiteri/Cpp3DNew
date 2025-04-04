#pragma once

#include "Base.h"
#include "Vectors.h"

namespace Core
{
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
        Vector3 GetPositionXYZ() const;

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