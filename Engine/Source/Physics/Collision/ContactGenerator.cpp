#include "ContactGenerator.h"
#include "Core/Logger.h"

#include "Math/Math.h"

#include <limits>

#define CE_CHECK_AXIS(axis, index)                                \
    if (!TryAxis(one, two, (axis), toCentre, (index), pen, best)) \
    return false

namespace Core
{
    static inline float TransformToAxis(AABBCollider *box, Vector3 axis)
    {
        // TODO: Safe margin
        return (box->Size.x + 0.01f) * Math::Abs(axis.Dot(box->GetAxis(0))) +
               (box->Size.y + 0.01f) * Math::Abs(axis.Dot(box->GetAxis(1))) +
               (box->Size.z + 0.01f) * Math::Abs(axis.Dot(box->GetAxis(2)));
    }

    static inline float PenetrationOnAxis(
        AABBCollider *one,
        AABBCollider *two,
        const Vector3 &axis,
        const Vector3 &toCentre)
    {
        float oneProject = TransformToAxis(one, axis);
        float twoProject = TransformToAxis(two, axis);

        float distance = Math::Abs(toCentre.Dot(axis));

        return (oneProject + twoProject - distance);
    }

    static inline bool TryAxis(
        AABBCollider *one,
        AABBCollider *two,
        Vector3 axis,
        const Vector3 &toCentre,
        CeU32 index,

        float &smallestPenetration,
        CeU32 &smallestCase)
    {
        if (axis.SquaredMagnitude() < 0.0001f)
            return true;
        axis.Normalize();
        float penetration = PenetrationOnAxis(one, two, axis, toCentre);

        if (penetration < 0)
            return false;

        if (penetration < smallestPenetration)
        {
            smallestPenetration = penetration;
            smallestCase = index;
        }

        return true;
    };

    static void FillPointFaceBoxBox(
        AABBCollider *one,
        AABBCollider *two,
        const Vector3 &toCentre,
        Contact *contact,
        CeU32 best,
        float pen)
    {
        Vector3 normal = one->GetAxis(best);
        if (one->GetAxis(best).Dot(toCentre) > 0)
            normal = normal * -1.0f;

        Vector3 vertex = two->Size;
        if (two->GetAxis(0).Dot(normal) < 0)
            vertex.x = -vertex.x;

        if (two->GetAxis(1).Dot(normal) < 0)
            vertex.y = -vertex.y;

        if (two->GetAxis(2).Dot(normal) < 0)
            vertex.z = -vertex.z;

        contact->ContactNormal = normal;
        contact->Penetration = pen;
        contact->ContactPoint = (*two->TransformMatrix) * vertex;
        contact->One = one->Owner;
        contact->Two = two->Owner;

        // TODO: figure out
        contact->Restitution = 0.8f;
        contact->Friction = 0.8f;
    }

    static inline Vector3 ContactPoint(
        const Vector3 &pOne,
        const Vector3 &dOne,
        float oneSize,
        const Vector3 &pTwo,
        const Vector3 &dTwo,
        float twoSize,

        bool useOne)
    {
        Vector3 toSt, cOne, cTwo;
        float dpStaOne, dpStaTwo, dpOneTwo, smOne, smTwo;
        float denom, mua, mub;

        smOne = dOne.SquaredMagnitude();
        smTwo = dTwo.SquaredMagnitude();
        dpOneTwo = dTwo.Dot(dOne);

        toSt = pOne - pTwo;
        dpStaOne = dOne.Dot(toSt);
        dpStaTwo = dTwo.Dot(toSt);

        denom = smOne * smTwo - dpOneTwo * dpOneTwo;

        if (abs(denom) < 0.0001f)
            return useOne ? pOne : pTwo;

        mua = (dpOneTwo * dpStaTwo - smTwo * dpStaOne) / denom;
        mub = (smOne * dpStaTwo - dpOneTwo * dpStaOne) / denom;

        if (mua > oneSize ||
            mua < -oneSize ||
            mub > twoSize ||
            mub < -twoSize)
        {
            return useOne ? pOne : pTwo;
        }
        else
        {
            cOne = pOne + dOne * mua;
            cTwo = pTwo + dTwo * mub;

            return cOne * 0.5 + cTwo * 0.5;
        }
    }

    // Box and box collision detecter
    bool CollisionDetector::BoxAndBox(AABBCollider *one, AABBCollider *two, Contact *c)
    {
        if (!one || !two)
            return false;

        if (!one->TransformMatrix || !two->TransformMatrix)
            return false;

        float pen = std::numeric_limits<float>::max();
        Vector3 toCentre = two->GetAxis(3) - one->GetAxis(3);
        CeU32 best = 0xffffff;

        CE_CHECK_AXIS(one->GetAxis(0), 0);
        CE_CHECK_AXIS(one->GetAxis(1), 1);
        CE_CHECK_AXIS(one->GetAxis(2), 2);

        CE_CHECK_AXIS(two->GetAxis(0), 3);
        CE_CHECK_AXIS(two->GetAxis(1), 4);
        CE_CHECK_AXIS(two->GetAxis(2), 5);

        CeU32 bestSingleAxis = best;

        CE_CHECK_AXIS(one->GetAxis(0) % two->GetAxis(0), 6);
        CE_CHECK_AXIS(one->GetAxis(0) % two->GetAxis(1), 7);
        CE_CHECK_AXIS(one->GetAxis(0) % two->GetAxis(2), 8);
        CE_CHECK_AXIS(one->GetAxis(1) % two->GetAxis(0), 9);
        CE_CHECK_AXIS(one->GetAxis(1) % two->GetAxis(1), 10);
        CE_CHECK_AXIS(one->GetAxis(1) % two->GetAxis(2), 11);
        CE_CHECK_AXIS(one->GetAxis(2) % two->GetAxis(0), 12);
        CE_CHECK_AXIS(one->GetAxis(2) % two->GetAxis(1), 13);
        CE_CHECK_AXIS(one->GetAxis(2) % two->GetAxis(2), 14);

        if (best < 3)
        {
            FillPointFaceBoxBox(one, two, toCentre, c, best, pen);
            return true;
        }
        else if (best < 6)
        {
            // crazy, one and two begin swapped and ' -3 ' not being present might have cost me a few days lol
            FillPointFaceBoxBox(two, one, toCentre * -1.0f, c, best - 3, pen);
            return true;
        }
        else
        {
            best -= 6;
            CeU32 oneAxisIndex = best / 3;
            CeU32 twoAxisIndex = best % 3;
            Vector3 oneAxis = one->GetAxis(oneAxisIndex);
            Vector3 twoAxis = two->GetAxis(twoAxisIndex);
            Vector3 axis = oneAxis % twoAxis;
            axis.Normalize();

            if (axis.Dot(toCentre) > 0)
                axis = axis * -1.0f;

            Vector3 ptOnOneEdge = one->Size;
            Vector3 ptOnTwoEdge = two->Size;

            for (CeU32 i = 0; i < 3; i++)
            {
                if (i == oneAxisIndex)
                    ptOnOneEdge[i] = 0;
                else if (one->GetAxis(i).Dot(axis) > 0)
                    ptOnOneEdge[i] = -ptOnOneEdge[i];

                if (i == twoAxisIndex)
                    ptOnTwoEdge[i] = 0;
                else if (two->GetAxis(i).Dot(axis) < 0)
                    ptOnTwoEdge[i] = -ptOnTwoEdge[i];
            }

            ptOnOneEdge = (*one->TransformMatrix) * ptOnOneEdge;
            ptOnTwoEdge = (*two->TransformMatrix) * ptOnTwoEdge;
            Vector3 vertex = ContactPoint(
                ptOnOneEdge, oneAxis, one->Size[oneAxisIndex],
                ptOnTwoEdge, twoAxis, two->Size[twoAxisIndex],
                bestSingleAxis > 2);

            c->ContactNormal = axis;
            c->Penetration = pen;
            c->ContactPoint = vertex;
            c->One = one->Owner;
            c->Two = two->Owner;

            // TODO: figure out
            c->Restitution = 1.0f;
            c->Friction = 1.0f;

            return true;
        }

        return true; // how tf you reach this point lmao
    }
}