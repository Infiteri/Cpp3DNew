#pragma once

#include "Base.h"
#include "Math/Matrix4.h"

namespace Core
{
    class CE_API Camera
    {
    public:
        enum MatrixMode
        {
            /// @brief Normal transformation matrix. Transform is based on:
            /// @brief Position
            /// @brief Rotation
            /// @brief Zoom
            TransformMatrix,

            /// @brief Input matrix.
            /// @brief Requires SetViewMatrix to be called for a valid matrix output. Use "TransformMatrix" to use the transform object for the matrix.
            InputMatrix
        };

    protected:
        Matrix4 view;
        Matrix4 projection;
        MatrixMode mode;

        Vector3 position;
        Vector3 rotation;

    public:
        Camera();
        virtual ~Camera();

        virtual void UpdateProjection() = 0;
        void UpdateView();

        void SetMatrixMode(MatrixMode mode);
        inline MatrixMode GetMatrixMode() { return mode; };

        void SetViewMatrix(const Matrix4 &other);

        Matrix4 GetViewInverted() { return Matrix4::Invert(&view); };
        Matrix4 &GetView() { return view; };
        Matrix4 &GetProjection() { return projection; };

        inline Vector3 &GetPosition() { return position; };
        inline Vector3 &GetRotation() { return rotation; };
    };
}