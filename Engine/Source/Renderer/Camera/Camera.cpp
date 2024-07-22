#include "Camera.h"

namespace Core
{
    Camera::Camera()
    {
        mode = TransformMatrix;
    }

    Camera::~Camera()
    {
    }

    void Camera::UpdateView()
    {
        switch (mode)
        {
        case TransformMatrix:
        default:
            view = Matrix4::Translate(&position);
            view *= Matrix4::RotationZYX(&rotation);
            break;
        }
    }

    void Camera::SetMatrixMode(MatrixMode mode)
    {
        this->mode = mode;
    }

    void Camera::SetViewMatrix(const Matrix4 &other)
    {
        view = other;
    }
}