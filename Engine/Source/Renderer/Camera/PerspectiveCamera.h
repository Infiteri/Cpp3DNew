#pragma once

#include "Camera.h"
#include "Math/Math.h"

namespace Core
{
    class CE_API PerspectiveCamera : public Camera
    {
    private:
        float fov;
        float near;
        float far;
        float aspect;

    public:
        /// @brief Parameters for the perspective camera.
        /// @param fov The field-of-view, in degrees.(90 degrees by default) (Internal fov float is in radians)
        /// @param aspect The aspect to use. (1280/720 by default)
        /// @param near The near view of the camera. (0.01 by default)
        /// @param far The farthest
        PerspectiveCamera(float fov = 90, float aspect = 1280 / 720, float near = 0.01f, float far = 1000.0f);
        ~PerspectiveCamera();

        void UpdateProjection();
        void UpdateProjection(float aspect);

        inline float GetFOV() { return fov; };
        inline void SetFOV(float fov = 90.0f) { this->fov = fov * CE_DEG_TO_RAD; };

        inline float GetAspect() { return aspect; };
        inline void SetAspect(float aspect) { this->aspect = aspect; };

        inline float GetNear() { return near; };
        inline void SetNear(float near) { this->near = near; };

        inline float GetFar() { return far; };
        inline void SetFar(float far) { this->far = far; };
    };
}