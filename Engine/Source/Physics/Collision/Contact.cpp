#include "Contact.h"
#include "Core/Logger.h"
#include "Physics/Body/RigidBody.h"
#include "Scene/Actor.h"

#include "Math/Math.h"
#include "Physics/PhysCore.h"

/**
 *
        switch (b->GetType())
        {
        case PhysicsBody::None:
        default:
        {
            CE_CORE_WARN("%s, unknown body type", CE_FUNCTION_SIGNATURE);
            break;
        }

        case PhysicsBody::Static:
        {
            auto bt = b->As<StaticBody>();
            break;
        }

        case PhysicsBody::Rigid:
        {
            auto bt = b->As<RigidBody>();
            break;
        }
        }
 */

namespace Core
{
    static inline void _CalculateLocalVelocityDataWithBodyType(PhysicsBody *b, Vector3 *v, Vector3 *t, Vector3 *lfa)
    {
        switch (b->GetType())
        {
        case PhysicsBody::None:
        default:
        {
            CE_CORE_WARN("%s, unknown body type", CE_FUNCTION_SIGNATURE);
            break;
        }

        case PhysicsBody::Static:
        {
            v->Set(0, 0, 0);
            t->Set(0, 0, 0);
            lfa->Set(0, 0, 0);
            break;
        }

        case PhysicsBody::Rigid:
        {
            auto bt = b->As<RigidBody>();
            v->Set(bt->GetVelocity());
            t->Set(bt->GetTorque());
            lfa->Set(bt->GetLastFrameAcceleration());
            break;
        }
        }
    }

    static inline float _GetMassWithBodyType(PhysicsBody *b, bool inverse = false)
    {
        switch (b->GetType())
        {
        case PhysicsBody::None:
        default:
        {
            CE_CORE_WARN("%s, unknown body type", CE_FUNCTION_SIGNATURE);
            break;
        }

        case PhysicsBody::Static:
        {
            auto bt = b->As<StaticBody>();
            return inverse ? bt->GetInverseMass() : bt->GetMass(); // TODO: MASS
            break;
        }

        case PhysicsBody::Rigid:
        {
            auto bt = b->As<RigidBody>();
            return inverse ? bt->GetInverseMass() : bt->GetMass();
            break;
        }
        }
        return 0;
    }

    static inline void _ImplementValueChangeWithBodyType(PhysicsBody *b, Vector3 vel, Vector3 rot)
    {
        switch (b->GetType())
        {
        case PhysicsBody::None:
        default:
        {
            CE_CORE_WARN("%s, unknown body type", CE_FUNCTION_SIGNATURE);
            break;
        }

        case PhysicsBody::Static:
        {
            break;
        }

        case PhysicsBody::Rigid:
        {
            auto bt = b->As<RigidBody>();
            bt->AddVelocity(vel);
            bt->AddRotation(rot);
            break;
        }
        }
    }

    static inline void _GetLastFrameAccelerationWithBodyType(PhysicsBody *b, Vector3 *lfa)
    {
        switch (b->GetType())
        {
        case PhysicsBody::None:
        default:
        {
            CE_CORE_WARN("%s, unknown body type", CE_FUNCTION_SIGNATURE);
            break;
        }

        case PhysicsBody::Static:
        {
            lfa->Set(0, 0, 0);
            break;
        }

        case PhysicsBody::Rigid:
        {
            auto bt = b->As<RigidBody>();
            lfa->Set(bt->GetLastFrameAcceleration());
            break;
        }
        }
    }

    Contact::Contact(const Contact &c)
    {
        One = c.One;
        Two = c.Two;

        Friction = c.Friction;
        Restitution = c.Restitution;
        Penetration = c.Penetration;
        ContactNormal.Set(c.ContactNormal);
        ContactPoint.Set(c.ContactPoint);
    }

    bool Contact::BodySame(PhysicsBody *a, PhysicsBody *b)
    {
        return a->GetOwner()->SameUUID(b->GetOwner());
    }

    void Contact::CalculateInternalData()
    {
        CalculateContactBasis();

        RelativeContactPosition[0] = ContactPoint - One->GetPosition();
        RelativeContactPosition[1] = ContactPoint - Two->GetPosition();

        ContactVelocity = CalculateLocalVelocity(One, 0);
        ContactVelocity -= CalculateLocalVelocity(Two, 1);

        CalculateDesiredDeltaVelocity();
    }

    void Contact::CalculateContactBasis()
    {
        Vector3 contactTangent[2];

        float cx = ContactNormal.x;
        float cy = ContactNormal.y;
        float cz = ContactNormal.z;

        if (Math::Abs(cx) > Math::Abs(cy))
        {
            // Scaling factor, for normalization
            const float s = 1.0f / Math::Sqrt(cz * cz + cx * cx);

            // The new X axis is at right angles to the world Y axis.
            contactTangent[0].x = cz * s;
            contactTangent[0].y = 0;
            contactTangent[0].z = -cx * s;

            // The new Y axis is at right angles to the new X and Z axes.
            contactTangent[1].x = cy * contactTangent[0].x;
            contactTangent[1].y = cz * contactTangent[0].x - cx * contactTangent[0].z;
            contactTangent[1].z = -cy * contactTangent[0].x;
        }
        else
        {
            // Scaling factor, for normalization
            const float s = 1.0f / Math::Sqrt(cz * cz + cy * cy);

            // The new X axis is at right angles to the world X axis.
            contactTangent[0].x = 0;
            contactTangent[0].y = -cz * s;
            contactTangent[0].z = cy * s;

            // The new Y axis is at right angles to the new X and Z axes.
            contactTangent[1].x = cy * contactTangent[0].z - cz * contactTangent[0].y;
            contactTangent[1].y = -cx * contactTangent[0].z;
            contactTangent[1].z = cx * contactTangent[0].y;
        }

        ContactToWorld.SetComponents(ContactNormal, contactTangent[0], contactTangent[1]);
    }

    Vector3 Contact::CalculateLocalVelocity(PhysicsBody *body, int index)
    {
        Vector3 torque;
        Vector3 bVelocity;
        Vector3 lfa; // Last frame acceleration

        _CalculateLocalVelocityDataWithBodyType(body, &bVelocity, &torque, &lfa);

        // Work out the velocity of the contact point
        Vector3 velocity = torque % RelativeContactPosition[index];
        velocity += bVelocity;

        Vector3 contactVelocity = ContactToWorld.TransformTranspose(velocity);

        // Velocity due to forces without reactions
        Vector3 accVelocity = ContactToWorld.TransformTranspose(lfa * CE_PHYSICS_DELTA_TIME);
        accVelocity.x = 0;

        contactVelocity += accVelocity;
        return contactVelocity;
    }

    void Contact::CalculateDesiredDeltaVelocity()
    {
        const static float velocityLimit = 0.20;
        float velocityFromAcc = 0;

        Vector3 lastAAccel;
        Vector3 lastBAccel;
        _GetLastFrameAccelerationWithBodyType(One, &lastAAccel);
        _GetLastFrameAccelerationWithBodyType(Two, &lastBAccel);

        velocityFromAcc += (lastAAccel * CE_PHYSICS_DELTA_TIME).Dot(ContactNormal);
        velocityFromAcc -= (lastBAccel * CE_PHYSICS_DELTA_TIME).Dot(ContactNormal);

        float thisRestitution = Restitution;
        if (Math::Abs(ContactVelocity.x) < velocityLimit)
            thisRestitution = 0.0f;

        // Combine the bounce velocity with the removed  acceleration velocity.7
        DesiredDeltaVelocity =
            -ContactVelocity.x - thisRestitution * (ContactVelocity.x - velocityFromAcc);
    }

    void Contact::ApplyPositionChange(Vector3 linearChange[2], Vector3 angularChange[2], float penetration)
    {
        const float angularLimit = 0.25f;

        float angularMove[2];
        float angularInertia[2];

        float linearMove[2];
        float linearInertia[2];

        float totalInertia = 0;

        for (CeU32 i = 0; i < 2; i++)
        {
            auto b = BodyAt(i);

            PhysMatrix3 inverseInertiaTensor;
            inverseInertiaTensor = b->GetInverseInertiaTensorWorld();

            Vector3 angularInertiaWorld = RelativeContactPosition[i] % ContactNormal;
            angularInertiaWorld = inverseInertiaTensor.Transform(angularInertiaWorld);
            angularInertiaWorld = angularInertiaWorld % RelativeContactPosition[i];
            angularInertia[i] = angularInertiaWorld.Dot(ContactNormal);

            linearInertia[i] = _GetMassWithBodyType(b, true); // TODO: make sure this is inversed

            totalInertia += linearInertia[i] + angularInertia[i];
        }

        for (CeU32 i = 0; i < 2; i++)
        {
            auto b = BodyAt(i);

            float sign = (i == 0) ? 1 : -1;
            angularMove[i] = sign * penetration * (angularInertia[i] / totalInertia);
            linearMove[i] = sign * penetration * (linearInertia[i] / totalInertia);

            Vector3 projection = RelativeContactPosition[i];
            auto negative = -RelativeContactPosition[i];

            projection += (ContactNormal * (negative.Dot(ContactNormal)));

            float maxMagnitude = angularLimit * projection.Magnitude();

            if (angularMove[i] < -maxMagnitude)
            {
                float totalMove = angularMove[i] + linearMove[i];
                angularMove[i] = -maxMagnitude;
                linearMove[i] = totalMove - angularMove[i];
            }
            else if (angularMove[i] > maxMagnitude)
            {
                float totalMove = angularMove[i] + linearMove[i];
                angularMove[i] = maxMagnitude;
                linearMove[i] = totalMove - angularMove[i];
            }

            if (angularMove[i] == 0)
                angularChange[i].Set(0, 0, 0);
            else
            {
                Vector3 targetAngularDirection = RelativeContactPosition[i].VectorProduct(ContactNormal);
                PhysMatrix3 inverseInertiaTensor = b->GetInverseInertiaTensorWorld();
                angularChange[i] = inverseInertiaTensor.Transform(targetAngularDirection) * (angularMove[i] / angularInertia[i]);
            }

            linearChange[i] = ContactNormal * linearMove[i];

            if (b->GetType() == PhysicsBody::Rigid)
            {
                Vector3 position;
                Quaternion orientation;
                position.Set(BodyAt(i)->GetPosition());
                position += (ContactNormal * linearMove[i]);
                orientation.Set(BodyAt(i)->GetOrientation());
                orientation.AddScaledVector(angularChange[i], 1.0);
                BodyAt(i)->GetPosition().Set(position);
                BodyAt(i)->SetOrientation(orientation);
            }

            BodyAt(i)->_CalculateData();
        }
    }

    void Contact::ApplyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2])
    {
        // TODO: BODY CAST
        PhysicsBody *b1 = BodyAt(0);
        PhysicsBody *b2 = BodyAt(1);

        PhysMatrix3 inverseInertiaTensor[2];
        inverseInertiaTensor[0].From(b1->GetInverseInertiaTensorWorld());
        inverseInertiaTensor[1].From(b2->GetInverseInertiaTensorWorld());

        Vector3 impulseContact = CalculateFrictionlessImpulse(inverseInertiaTensor);
        Vector3 impulse = ContactToWorld.Transform(impulseContact);

        Vector3 impulsiveTorque = (RelativeContactPosition[0] % impulse);
        rotationChange[0] = inverseInertiaTensor[0].Transform(impulsiveTorque);
        velocityChange[0].Set(impulse * _GetMassWithBodyType(b1, true));

        _ImplementValueChangeWithBodyType(b1, velocityChange[0], rotationChange[0]);

        {
            impulsiveTorque = (impulse % RelativeContactPosition[1]);
            rotationChange[1] = inverseInertiaTensor[1].Transform(impulsiveTorque);
            velocityChange[1].Set(impulse * -_GetMassWithBodyType(b2, true));

            _ImplementValueChangeWithBodyType(b2, velocityChange[1], rotationChange[1]);
        }
    }

    Vector3 Contact::CalculateFrictionlessImpulse(PhysMatrix3 *inverseInertiaTensor)
    {
        // TODO: BODY CAST
        PhysicsBody *b1 = BodyAt(0);
        PhysicsBody *b2 = BodyAt(1);

        Vector3 impulseContact;

        Vector3 deltaVelWorld = RelativeContactPosition[0] % ContactNormal;
        deltaVelWorld = inverseInertiaTensor[0].Transform(deltaVelWorld);
        deltaVelWorld = deltaVelWorld % RelativeContactPosition[0];

        float deltaVelocity = deltaVelWorld.Dot(ContactNormal);
        deltaVelocity += _GetMassWithBodyType(b1, true);

        {
            Vector3 deltaVelWorld = RelativeContactPosition[1] % ContactNormal;
            deltaVelWorld = inverseInertiaTensor[1].Transform(deltaVelWorld);
            deltaVelWorld = deltaVelWorld % RelativeContactPosition[1];
            deltaVelocity += deltaVelWorld.Dot(ContactNormal);
            deltaVelocity += _GetMassWithBodyType(b2, true);
        }

        // Calculate the required size of the impulse
        impulseContact.x = DesiredDeltaVelocity / deltaVelocity;
        impulseContact.y = 0;
        impulseContact.z = 0;
        return impulseContact;
    }
}