#include "Collider.h"
#include "Core/Logger.h"

namespace Core
{
    // must be implemented this way as a 'Collider' type collider (could be AABB or other kinds) doesn't call into the appropriate function when calling from as its not known
    void Collider::FromWithBase(Collider *target, Collider *from) 
    {
        CE_VERIFY(target);
        CE_VERIFY(from);

        switch (target->GetType())
        {
        case Box:
            target->As<AABBCollider>()->From(from->As<AABBCollider>());
            break;

        case None:
        default:
            CE_CORE_WARN("Unknown body type, can't copy.");
            break;
        };
    }

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