#include "ContactGenerator.h"

#include "Math/Math.h"

#define CE_TEST_OVERLAP(axis) OverlapOnAxis(one, two, (axis), toCentre)

namespace Core
{
    static inline float TransformToAxis(AABBCollider *box, Vector3 axis)
    {
        return box->Size.x * Math::Abs(axis.Dot(box->GetAxis(0))) +
               box->Size.y * Math::Abs(axis.Dot(box->GetAxis(1))) +
               box->Size.z * Math::Abs(axis.Dot(box->GetAxis(2))); // TODO: Presume the box depth, NOTE: Required
    }

    static inline bool OverlapOnAxis(
        AABBCollider *one,
        AABBCollider *two,
        const Vector3 &axis,
        const Vector3 &toCentre)
    {
        float oneProject = TransformToAxis(one, axis);
        float twoProject = TransformToAxis(two, axis);
        float distance = Math::Abs(toCentre.Dot(axis));
        return (distance < oneProject + twoProject);
    }

    ContactGenerator::ContactGenerator()
    {
    }

    ContactGenerator::~ContactGenerator()
    {
    }

    bool ContactGenerator::BoxAndBox(AABBCollider *one, AABBCollider *two)
    {
        if (!one || !two)
            return false;

        if (!one->TransformMatrix || !two->TransformMatrix)
            return false;

        Vector3 toCentre = two->GetAxis(3) - one->GetAxis(3);

        return (
            CE_TEST_OVERLAP(one->GetAxis(0)) &&
            CE_TEST_OVERLAP(one->GetAxis(1)) &&
            CE_TEST_OVERLAP(one->GetAxis(2)) &&
            // And on two's
            CE_TEST_OVERLAP(two->GetAxis(0)) &&
            CE_TEST_OVERLAP(two->GetAxis(1)) &&
            CE_TEST_OVERLAP(two->GetAxis(2)));
    }
}