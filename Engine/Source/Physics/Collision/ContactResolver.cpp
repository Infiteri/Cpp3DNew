#include "ContactResolver.h"
#include "Core/Logger.h"
#include "Math/Math.h"

namespace Core
{
    void ContactResolver::Resolve()
    {
        PositionEpsilon = VelocityEpsilon = 0;

        PrepareContacts();
        AdjustPosition();
        AdjustVelocity();
    }

    void ContactResolver::PrepareContacts()
    {
        for (auto &contact : Contacts)
            contact.CalculateInternalData();
    }

    void ContactResolver::AdjustVelocity()
    {
        Vector3 velocityChange[2], rotationChange[2];
        Vector3 deltaVel;

        CeU32 velocityIterationsUsed = 0;
        while (velocityIterationsUsed < VelocityIterations)
        {
            CeU32 numContacts = Contacts.size();
            float max = VelocityEpsilon;
            CeU32 index = numContacts;
            for (CeU32 i = 0; i < numContacts; i++)
            {
                if (Contacts[i].DesiredDeltaVelocity > max)
                {
                    max = Contacts[i].DesiredDeltaVelocity;
                    index = i;
                }
                else
                    CE_CORE_TRACE("bad ddv value %f", Contacts[i].DesiredDeltaVelocity);
            }

            if (index == numContacts)
            {
                CE_CORE_TRACE("Adjusting velocity gets skipped");
                break;
            }

            Contacts[index].ApplyVelocityChange(velocityChange, rotationChange);

            // Update penetrations
            {
                for (CeU32 i = 0; i < numContacts; i++)
                {
                    for (CeU32 b = 0; b < 2; b++)
                        for (CeU32 d = 0; d < 2; d++)
                        {
                            if (Contacts[i].Body[b] == Contacts[index].Body[d])
                            {
                                // note: same as adjusting position except here is rotation change
                                int sign = b ? -1 : 1;
                                deltaVel = velocityChange[d] + rotationChange[d].VectorProduct(Contacts[i].RelativeContactPosition[b]);
                                Contacts[i].ContactVelocity += Contacts[i].ContactToWorld.TransformTranspose(deltaVel) * sign;
                                Contacts[i].CalculateDesiredDeltaVelocity();
                            }
                        }
                }
            }

            velocityIterationsUsed++;
        }
    }

    void ContactResolver::AdjustPosition()
    {
        CeU32 i, index;
        Vector3 linearChange[2], angularChange[2];
        float max;
        Vector3 deltaPosition;

        CeU32 positionIterationsUsed = 0;
        while (positionIterationsUsed < PositionIterations)
        {
            // Finds the worst contact to resolve first
            CeU32 numContacts = Contacts.size();
            max = PositionEpsilon;
            index = numContacts;

            for (i = 0; i < numContacts; i++)
            {
                // TODO: Abs?
                if (Math::Abs(Contacts[i].Penetration) > max)
                {
                    max = (Contacts[i].Penetration);
                    index = i;
                }
            }

            if (index == numContacts)
            {
                CE_CORE_TRACE("Adjusting position gets skipped");
                break;
            }

            // Resolves the worst contact
            Contacts[index].ApplyPositionChange(linearChange, angularChange, max);

            // Update penetrations
            {
                for (i = 0; i < numContacts; i++)
                {
                    for (CeU32 b = 0; b < 2; b++)
                    {
                        for (CeU32 d = 0; d < 2; d++)
                        {
                            if (Contacts[i].Body[b] == Contacts[index].Body[d])
                            {
                                // note: angular change cant be modified to much directly, this is why radian/degree conversion should be done carefully
                                deltaPosition = linearChange[d] + angularChange[d].VectorProduct(Contacts[i].RelativeContactPosition[b]);
                                Contacts[i].Penetration = (deltaPosition.Dot(Contacts[i].ContactNormal) * (b ? 1 : -1));
                            }
                        }
                    }
                }
            }

            // Update iterations used
            positionIterationsUsed++;
        }
    }
}