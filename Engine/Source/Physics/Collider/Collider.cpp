#include "Collider.h"

namespace Core
{
    Vector3 Collider::GetAxis(int index)
    {
        if (!TransformMatrix)
            return {0, 0, 0};

        return TransformMatrix->GetAxisVector3(index);
    }
}