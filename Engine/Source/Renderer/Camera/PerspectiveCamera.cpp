#include "PerspectiveCamera.h"

namespace Core
{
    PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
    {
        SetFOV(fov);
        SetAspect(aspect);
        SetNear(near);
        SetFar(far);
    }

    PerspectiveCamera::~PerspectiveCamera()
    {
    }

    void PerspectiveCamera::UpdateProjection()
    {
        projection = Matrix4::Perspective(fov, aspect, near, far);
    }

    void PerspectiveCamera::UpdateProjection(float aspect)
    {
        SetAspect(aspect);
        UpdateProjection();
    }
}