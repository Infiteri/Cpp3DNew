#include "Collider.h"

namespace Core
{
    Collider::Collider()
    {
        type = None;
    }

    Vector3 Collider::GetAxis(int index)
    {
        if (!TransformMatrix)
            return {0, 0, 0};

        return TransformMatrix->GetAxisVector3(index);
    }

    AABBCollider::AABBCollider()
    {
        type = Box;
    }

    void AABBCollider::From(AABBCollider *col)
    {
        Size.Set(col->Size);
    }
}