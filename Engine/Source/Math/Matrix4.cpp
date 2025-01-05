#include "Matrix4.h"
#include "Math.h"

namespace Core
{
    Matrix4::Matrix4()
    {
        for (int i = 0; i < 16; i++)
            data[i] = 0;

        data[0] = data[5] = data[10] = data[15] = 1.0f;
    }

    Matrix4::~Matrix4()
    {
    }

    Matrix4 Matrix4::Identity()
    {
        return Matrix4();
    }

    Matrix4 Matrix4::Empty()
    {
        Matrix4 m;
        m.data[0] = m.data[5] = m.data[10] = m.data[15] = 0.0f;

        return m;
    }

    void Matrix4::From(Matrix4 *other)
    {
        for (int i = 0; i < 16; i++)
        {
            data[i] = other->data[i];
        }
    }

    void Matrix4::From(Matrix4 other)
    {
        for (int i = 0; i < 16; i++)
        {
            data[i] = other.data[i];
        }
    }

    void Matrix4::From(float *data)
    {
        for (int i = 0; i < 16; i++)
        {
            this->data[i] = data[i];
        }
    }

    void Matrix4::Normalized()
    {
        for (int i = 0; i < 4; ++i)
        {
            Vector3 col(data[i], data[i + 4], data[i + 8]);
            col.Normalize();
            data[i] = col.x;
            data[i + 4] = col.y;
            data[i + 8] = col.z;
        }
    }

    Vector3 Matrix4::GetEulerAnglesZYX() const
    {
        Vector3 eulerAngles;

        // Normalize matrix to remove scale effects
        Vector3 scale;
        scale.x = sqrt(data[0] * data[0] + data[1] * data[1] + data[2] * data[2]);
        scale.y = sqrt(data[4] * data[4] + data[5] * data[5] + data[6] * data[6]);
        scale.z = sqrt(data[8] * data[8] + data[9] * data[9] + data[10] * data[10]);

        // Normalize rotation components
        float m00 = data[0] / scale.x;
        float m01 = data[1] / scale.x;
        float m02 = data[2] / scale.x;
        float m10 = data[4] / scale.y;
        float m11 = data[5] / scale.y;
        float m12 = data[6] / scale.y;
        float m20 = data[8] / scale.z;
        float m21 = data[9] / scale.z;
        float m22 = data[10] / scale.z;

        // Assuming the matrix is a rotation matrix with ZYX order
        if (m02 < 1)
        {
            if (m02 > -1)
            {
                eulerAngles.y = asin(-m02);      // Rotation around Y
                eulerAngles.x = atan2(m12, m22); // Rotation around X
                eulerAngles.z = atan2(m01, m00); // Rotation around Z
            }
            else
            {
                // m02 == -1 -> Gimbal lock
                eulerAngles.y = CE_PI / 2;
                eulerAngles.x = atan2(-m10, m11);
                eulerAngles.z = 0;
            }
        }
        else
        {
            // m02 == 1 -> Gimbal lock
            eulerAngles.y = -CE_PI / 2;
            eulerAngles.x = atan2(-m10, m11);
            eulerAngles.z = 0;
        }

        eulerAngles.y = -eulerAngles.y;

        return eulerAngles;
    }

    Vector3 Matrix4::GetPositionXYZ() const
    {
        return {data[12], data[13], data[14]};
    }

    Matrix4 Matrix4::Perspective(float fov, float aspect, float near, float far)
    {
        Matrix4 m;

        float f = 1.0 / Math::Tan(fov / 2);
        m.data[0] = f / aspect;
        m.data[1] = 0;
        m.data[2] = 0;
        m.data[3] = 0;
        m.data[4] = 0;
        m.data[5] = f;
        m.data[6] = 0;
        m.data[7] = 0;
        m.data[8] = 0;
        m.data[9] = 0;
        m.data[11] = -1;
        m.data[12] = 0;
        m.data[13] = 0;
        m.data[15] = 0;
        m.data[10] = -1;
        m.data[14] = -2 * near;

        return m;
    }

    Matrix4 Matrix4::Translate(float x, float y, float z)
    {
        Matrix4 m;

        m.data[12] = x;
        m.data[13] = y;
        m.data[14] = z;

        return m;
    }

    Matrix4 Matrix4::Translate(Vector3 *vec)
    {
        return Translate(vec->x, vec->y, vec->z);
    }

    Matrix4 Matrix4::Translate(const Vector3 &vec)
    {
        return Translate(vec.x, vec.y, vec.z);
    }

    Matrix4 Matrix4::Multiply(Matrix4 *a, Matrix4 *b)
    {
        Matrix4 m;

        float b00 = b->data[0 * 4 + 0];
        float b01 = b->data[0 * 4 + 1];
        float b02 = b->data[0 * 4 + 2];
        float b03 = b->data[0 * 4 + 3];
        float b10 = b->data[1 * 4 + 0];
        float b11 = b->data[1 * 4 + 1];
        float b12 = b->data[1 * 4 + 2];
        float b13 = b->data[1 * 4 + 3];
        float b20 = b->data[2 * 4 + 0];
        float b21 = b->data[2 * 4 + 1];
        float b22 = b->data[2 * 4 + 2];
        float b23 = b->data[2 * 4 + 3];
        float b30 = b->data[3 * 4 + 0];
        float b31 = b->data[3 * 4 + 1];
        float b32 = b->data[3 * 4 + 2];
        float b33 = b->data[3 * 4 + 3];
        float a00 = a->data[0 * 4 + 0];
        float a01 = a->data[0 * 4 + 1];
        float a02 = a->data[0 * 4 + 2];
        float a03 = a->data[0 * 4 + 3];
        float a10 = a->data[1 * 4 + 0];
        float a11 = a->data[1 * 4 + 1];
        float a12 = a->data[1 * 4 + 2];
        float a13 = a->data[1 * 4 + 3];
        float a20 = a->data[2 * 4 + 0];
        float a21 = a->data[2 * 4 + 1];
        float a22 = a->data[2 * 4 + 2];
        float a23 = a->data[2 * 4 + 3];
        float a30 = a->data[3 * 4 + 0];
        float a31 = a->data[3 * 4 + 1];
        float a32 = a->data[3 * 4 + 2];
        float a33 = a->data[3 * 4 + 3];

        m.data[0] = b00 * a00 + b01 * a10 + b02 * a20 + b03 * a30;
        m.data[1] = b00 * a01 + b01 * a11 + b02 * a21 + b03 * a31;
        m.data[2] = b00 * a02 + b01 * a12 + b02 * a22 + b03 * a32;
        m.data[3] = b00 * a03 + b01 * a13 + b02 * a23 + b03 * a33;
        m.data[4] = b10 * a00 + b11 * a10 + b12 * a20 + b13 * a30;
        m.data[5] = b10 * a01 + b11 * a11 + b12 * a21 + b13 * a31;
        m.data[6] = b10 * a02 + b11 * a12 + b12 * a22 + b13 * a32;
        m.data[7] = b10 * a03 + b11 * a13 + b12 * a23 + b13 * a33;
        m.data[8] = b20 * a00 + b21 * a10 + b22 * a20 + b23 * a30;
        m.data[9] = b20 * a01 + b21 * a11 + b22 * a21 + b23 * a31;
        m.data[10] = b20 * a02 + b21 * a12 + b22 * a22 + b23 * a32;
        m.data[11] = b20 * a03 + b21 * a13 + b22 * a23 + b23 * a33;
        m.data[12] = b30 * a00 + b31 * a10 + b32 * a20 + b33 * a30;
        m.data[13] = b30 * a01 + b31 * a11 + b32 * a21 + b33 * a31;
        m.data[14] = b30 * a02 + b31 * a12 + b32 * a22 + b33 * a32;
        m.data[15] = b30 * a03 + b31 * a13 + b32 * a23 + b33 * a33;

        return m;
    }

    Matrix4 Matrix4::Invert(Matrix4 *a)
    {
        Matrix4 m;

        float a00 = a->data[0];
        float a01 = a->data[1];
        float a02 = a->data[2];
        float a03 = a->data[3];
        float a10 = a->data[4];
        float a11 = a->data[5];
        float a12 = a->data[6];
        float a13 = a->data[7];
        float a20 = a->data[8];
        float a21 = a->data[9];
        float a22 = a->data[10];
        float a23 = a->data[11];
        float a30 = a->data[12];
        float a31 = a->data[13];
        float a32 = a->data[14];
        float a33 = a->data[15];
        float b00 = a00 * a11 - a01 * a10;
        float b01 = a00 * a12 - a02 * a10;
        float b02 = a00 * a13 - a03 * a10;
        float b03 = a01 * a12 - a02 * a11;
        float b04 = a01 * a13 - a03 * a11;
        float b05 = a02 * a13 - a03 * a12;
        float b06 = a20 * a31 - a21 * a30;
        float b07 = a20 * a32 - a22 * a30;
        float b08 = a20 * a33 - a23 * a30;
        float b09 = a21 * a32 - a22 * a31;
        float b10 = a21 * a33 - a23 * a31;
        float b11 = a22 * a33 - a23 * a32;
        // Calculate the determinant
        float det =
            b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

        det = 1.0 / det;

        m.data[0] = (a11 * b11 - a12 * b10 + a13 * b09) * det;
        m.data[1] = (a02 * b10 - a01 * b11 - a03 * b09) * det;
        m.data[2] = (a31 * b05 - a32 * b04 + a33 * b03) * det;
        m.data[3] = (a22 * b04 - a21 * b05 - a23 * b03) * det;
        m.data[4] = (a12 * b08 - a10 * b11 - a13 * b07) * det;
        m.data[5] = (a00 * b11 - a02 * b08 + a03 * b07) * det;
        m.data[6] = (a32 * b02 - a30 * b05 - a33 * b01) * det;
        m.data[7] = (a20 * b05 - a22 * b02 + a23 * b01) * det;
        m.data[8] = (a10 * b10 - a11 * b08 + a13 * b06) * det;
        m.data[9] = (a01 * b08 - a00 * b10 - a03 * b06) * det;
        m.data[10] = (a30 * b04 - a31 * b02 + a33 * b00) * det;
        m.data[11] = (a21 * b02 - a20 * b04 - a23 * b00) * det;
        m.data[12] = (a11 * b07 - a10 * b09 - a12 * b06) * det;
        m.data[13] = (a00 * b09 - a01 * b07 + a02 * b06) * det;
        m.data[14] = (a31 * b01 - a30 * b03 - a32 * b00) * det;
        m.data[15] = (a20 * b03 - a21 * b01 + a22 * b00) * det;

        return m;
    }

    Matrix4 Matrix4::Multiply(Matrix4 a, Matrix4 b)
    {
        Matrix4 m;

        float b00 = b.data[0 * 4 + 0];
        float b01 = b.data[0 * 4 + 1];
        float b02 = b.data[0 * 4 + 2];
        float b03 = b.data[0 * 4 + 3];
        float b10 = b.data[1 * 4 + 0];
        float b11 = b.data[1 * 4 + 1];
        float b12 = b.data[1 * 4 + 2];
        float b13 = b.data[1 * 4 + 3];
        float b20 = b.data[2 * 4 + 0];
        float b21 = b.data[2 * 4 + 1];
        float b22 = b.data[2 * 4 + 2];
        float b23 = b.data[2 * 4 + 3];
        float b30 = b.data[3 * 4 + 0];
        float b31 = b.data[3 * 4 + 1];
        float b32 = b.data[3 * 4 + 2];
        float b33 = b.data[3 * 4 + 3];
        float a00 = a.data[0 * 4 + 0];
        float a01 = a.data[0 * 4 + 1];
        float a02 = a.data[0 * 4 + 2];
        float a03 = a.data[0 * 4 + 3];
        float a10 = a.data[1 * 4 + 0];
        float a11 = a.data[1 * 4 + 1];
        float a12 = a.data[1 * 4 + 2];
        float a13 = a.data[1 * 4 + 3];
        float a20 = a.data[2 * 4 + 0];
        float a21 = a.data[2 * 4 + 1];
        float a22 = a.data[2 * 4 + 2];
        float a23 = a.data[2 * 4 + 3];
        float a30 = a.data[3 * 4 + 0];
        float a31 = a.data[3 * 4 + 1];
        float a32 = a.data[3 * 4 + 2];
        float a33 = a.data[3 * 4 + 3];

        m.data[0] = b00 * a00 + b01 * a10 + b02 * a20 + b03 * a30;
        m.data[1] = b00 * a01 + b01 * a11 + b02 * a21 + b03 * a31;
        m.data[2] = b00 * a02 + b01 * a12 + b02 * a22 + b03 * a32;
        m.data[3] = b00 * a03 + b01 * a13 + b02 * a23 + b03 * a33;
        m.data[4] = b10 * a00 + b11 * a10 + b12 * a20 + b13 * a30;
        m.data[5] = b10 * a01 + b11 * a11 + b12 * a21 + b13 * a31;
        m.data[6] = b10 * a02 + b11 * a12 + b12 * a22 + b13 * a32;
        m.data[7] = b10 * a03 + b11 * a13 + b12 * a23 + b13 * a33;
        m.data[8] = b20 * a00 + b21 * a10 + b22 * a20 + b23 * a30;
        m.data[9] = b20 * a01 + b21 * a11 + b22 * a21 + b23 * a31;
        m.data[10] = b20 * a02 + b21 * a12 + b22 * a22 + b23 * a32;
        m.data[11] = b20 * a03 + b21 * a13 + b22 * a23 + b23 * a33;
        m.data[12] = b30 * a00 + b31 * a10 + b32 * a20 + b33 * a30;
        m.data[13] = b30 * a01 + b31 * a11 + b32 * a21 + b33 * a31;
        m.data[14] = b30 * a02 + b31 * a12 + b32 * a22 + b33 * a32;
        m.data[15] = b30 * a03 + b31 * a13 + b32 * a23 + b33 * a33;
        return m;
    }

    Matrix4 Matrix4::Invert(Matrix4 a)
    {
        Matrix4 m;

        float a00 = a.data[0];
        float a01 = a.data[1];
        float a02 = a.data[2];
        float a03 = a.data[3];
        float a10 = a.data[4];
        float a11 = a.data[5];
        float a12 = a.data[6];
        float a13 = a.data[7];
        float a20 = a.data[8];
        float a21 = a.data[9];
        float a22 = a.data[10];
        float a23 = a.data[11];
        float a30 = a.data[12];
        float a31 = a.data[13];
        float a32 = a.data[14];
        float a33 = a.data[15];
        float b00 = a00 * a11 - a01 * a10;
        float b01 = a00 * a12 - a02 * a10;
        float b02 = a00 * a13 - a03 * a10;
        float b03 = a01 * a12 - a02 * a11;
        float b04 = a01 * a13 - a03 * a11;
        float b05 = a02 * a13 - a03 * a12;
        float b06 = a20 * a31 - a21 * a30;
        float b07 = a20 * a32 - a22 * a30;
        float b08 = a20 * a33 - a23 * a30;
        float b09 = a21 * a32 - a22 * a31;
        float b10 = a21 * a33 - a23 * a31;
        float b11 = a22 * a33 - a23 * a32;
        // Calculate the determinant
        float det =
            b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

        det = 1.0 / det;

        m.data[0] = (a11 * b11 - a12 * b10 + a13 * b09) * det;
        m.data[1] = (a02 * b10 - a01 * b11 - a03 * b09) * det;
        m.data[2] = (a31 * b05 - a32 * b04 + a33 * b03) * det;
        m.data[3] = (a22 * b04 - a21 * b05 - a23 * b03) * det;
        m.data[4] = (a12 * b08 - a10 * b11 - a13 * b07) * det;
        m.data[5] = (a00 * b11 - a02 * b08 + a03 * b07) * det;
        m.data[6] = (a32 * b02 - a30 * b05 - a33 * b01) * det;
        m.data[7] = (a20 * b05 - a22 * b02 + a23 * b01) * det;
        m.data[8] = (a10 * b10 - a11 * b08 + a13 * b06) * det;
        m.data[9] = (a01 * b08 - a00 * b10 - a03 * b06) * det;
        m.data[10] = (a30 * b04 - a31 * b02 + a33 * b00) * det;
        m.data[11] = (a21 * b02 - a20 * b04 - a23 * b00) * det;
        m.data[12] = (a11 * b07 - a10 * b09 - a12 * b06) * det;
        m.data[13] = (a00 * b09 - a01 * b07 + a02 * b06) * det;
        m.data[14] = (a31 * b01 - a30 * b03 - a32 * b00) * det;
        m.data[15] = (a20 * b03 - a21 * b01 + a22 * b00) * det;

        return m;
    }

    Matrix4 Matrix4::RotationXYZ(Vector3 vec)
    {
        return RotationXYZ(vec.x, vec.y, vec.z);
    }

    Matrix4 Matrix4::RotationXYZ(Vector3 *vec)
    {
        return RotationXYZ(vec->x, vec->y, vec->z);
    }

    Matrix4 Matrix4::RotationXYZ(float rotationX, float rotationY, float rotationZ)
    {
        Matrix4 x = RotationX(rotationX);
        Matrix4 y = RotationY(rotationY);
        Matrix4 z = RotationZ(rotationZ);

        return Matrix4::Multiply(Matrix4::Multiply(x, y), z);
    }

    Matrix4 Matrix4::RotationZYX(Vector3 vec)
    {
        return RotationZYX(vec.x, vec.y, vec.z);
    }

    Matrix4 Matrix4::RotationZYX(Vector3 *vec)
    {
        return RotationZYX(vec->x, vec->y, vec->z);
    }

    Matrix4 Matrix4::RotationZYX(float rotationX, float rotationY, float rotationZ)
    {
        Matrix4 x = RotationX(rotationX);
        Matrix4 y = RotationY(rotationY);
        Matrix4 z = RotationZ(rotationZ);

        return Matrix4::Multiply(Matrix4::Multiply(z, y), x);
    }

    Matrix4 Matrix4::RotationX(float rotation)
    {
        Matrix4 m;

        float c = Math::Cos(rotation);
        float s = Math::Sin(rotation);

        m.data[5] = c;
        m.data[6] = s;
        m.data[9] = -s;
        m.data[10] = c;

        return m;
    }

    Matrix4 Matrix4::RotationY(float rotation)
    {
        Matrix4 m;

        float c = Math::Cos(rotation);
        float s = Math::Sin(rotation);

        m.data[0] = c;
        m.data[2] = s;
        m.data[8] = -s;
        m.data[10] = c;

        return m;
    }

    Matrix4 Matrix4::RotationZ(float rotation)
    {
        Matrix4 m;

        float c = Math::Cos(rotation);
        float s = Math::Sin(rotation);

        m.data[0] = c;
        m.data[1] = s;
        m.data[4] = -s;
        m.data[5] = c;

        return m;
    }

    Vector3 Matrix4::Forward(Matrix4 mat)
    {
        Vector3 vec;

        vec.x = -mat.data[8];  // Change this line
        vec.y = -mat.data[9];  // Change this line
        vec.z = -mat.data[10]; // Change this line

        return vec;
    }

    Vector3 Matrix4::Forward(Matrix4 *mat)
    {
        Vector3 vec;

        vec.x = -mat->data[8];  // Change this line
        vec.y = -mat->data[9];  // Change this line
        vec.z = -mat->data[10]; // Change this line

        return vec;
    }

    Vector3 Matrix4::Right(Matrix4 mat)
    {
        Vector3 vec;

        vec.x = mat.data[0]; // Change this line
        vec.y = mat.data[1]; // Change this line
        vec.z = mat.data[2]; // Change this line

        return vec;
    }

    Vector3 Matrix4::Right(Matrix4 *mat)
    {
        Vector3 vec;

        vec.x = mat->data[0]; // Change this line
        vec.y = mat->data[1]; // Change this line
        vec.z = mat->data[2]; // Change this line

        return vec;
    }

    Matrix4 Matrix4::Scale(Vector3 *vec)
    {
        return Scale(vec->x, vec->y, vec->z);
    }

    Matrix4 Matrix4::Scale(float x, float y, float z)
    {
        Matrix4 m;

        m.data[0] = x;
        m.data[5] = y;
        m.data[10] = z;

        return m;
    }
    Matrix4 Matrix4::Create(float *data)
    {
        Matrix4 m;
        m.From(data);
        return m;
    }
}