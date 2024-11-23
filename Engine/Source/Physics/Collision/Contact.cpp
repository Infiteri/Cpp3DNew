#include "Contact.h"
#include "Core/Logger.h"
#include "Physics/Body/RigidBody.h"
#include "Scene/Actor.h"

#include "Math/Math.h"
#include "Physics/PhysCore.h"

namespace Core
{
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
        // TODO:
        RigidBody *b = body->As<RigidBody>();

        // Work out the velocity of the contact point
        Vector3 velocity = b->GetTorque() % RelativeContactPosition[index];
        velocity += b->GetVelocity();

        Vector3 contactVelocity = ContactToWorld.TransformTranspose(velocity);

        // Velocity due to forces without reactions
        Vector3 accVelocity = ContactToWorld.TransformTranspose(b->GetLastFrameAcceleration() * CE_PHYSICS_DELTA_TIME);
        accVelocity.x = 0;

        contactVelocity += accVelocity;
        return contactVelocity;
    }

    void Contact::CalculateDesiredDeltaVelocity()
    {
        const static float velocityLimit = 0.20;
        float velocityFromAcc = 0;

        auto lastAAccel = One->As<RigidBody>()->GetLastFrameAcceleration();
        auto lastBAccel = Two->As<RigidBody>()->GetLastFrameAcceleration();

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
            // TODO: BODY TYPE
            auto b = BodyAt(i)->As<RigidBody>();

            PhysMatrix3 inverseInertiaTensor;
            inverseInertiaTensor = b->GetInverseInertiaTensorWorld();

            Vector3 angularInertiaWorld = RelativeContactPosition[i] % ContactNormal;
            angularInertiaWorld = inverseInertiaTensor.Transform(angularInertiaWorld);
            angularInertiaWorld = angularInertiaWorld % RelativeContactPosition[i];
            angularInertia[i] = angularInertiaWorld.Dot(ContactNormal);

            linearInertia[i] = b->GetInverseMass();

            totalInertia += linearInertia[i] + angularInertia[i];
        }

        for (CeU32 i = 0; i < 2; i++)
        {
            // TODO: BODY TYPE
            auto b = BodyAt(i)->As<RigidBody>();

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

            Vector3 pos;
            pos.Set(BodyAt(i)->As<RigidBody>()->GetPosition());
            pos += (ContactNormal * linearMove[i]);
            BodyAt(i)->As<RigidBody>()->GetPosition().Set(pos);

            // TODO: Quaternions and rotations
            Quaternion q;
            q.Set(BodyAt(i)->As<RigidBody>()->GetOrientation());
            q.AddScaledVector(angularChange[i], 1.0);
            BodyAt(i)->As<RigidBody>()->SetOrientation(q);
            BodyAt(i)->As<RigidBody>()->_CalculateData();
        }
    }

    void Contact::ApplyVelocityChange(Vector3 velocityChange[2], Vector3 rotationChange[2])
    {
        RigidBody *b1 = BodyAt(0)->As<RigidBody>();
        RigidBody *b2 = BodyAt(1)->As<RigidBody>();

        PhysMatrix3 inverseInertiaTensor[2];
        inverseInertiaTensor[0].From(b1->GetInverseInertiaTensorWorld());
        inverseInertiaTensor[1].From(b2->GetInverseInertiaTensorWorld());

        Vector3 impulseContact = CalculateFrictionlessImpulse(inverseInertiaTensor);
        Vector3 impulse = ContactToWorld.Transform(impulseContact);

        Vector3 impulsiveTorque = (RelativeContactPosition[0] % impulse);
        rotationChange[0] = inverseInertiaTensor[0].Transform(impulsiveTorque);
        velocityChange[0].Set(impulse * b1->GetInverseMass());

        b1->_AddVelocity(velocityChange[0]);
        b1->_AddRotation(rotationChange[0]);

        {
            impulsiveTorque = (impulse % RelativeContactPosition[1]);
            rotationChange[1] = inverseInertiaTensor[1].Transform(impulsiveTorque);
            velocityChange[1].Set(impulse * -b2->GetInverseMass());

            b2->_AddVelocity(velocityChange[1]);
            b2->_AddRotation(rotationChange[1]);
        }
    }

    Vector3 Contact::CalculateFrictionlessImpulse(PhysMatrix3 *inverseInertiaTensor)
    {
        RigidBody *b1 = BodyAt(0)->As<RigidBody>();
        RigidBody *b2 = BodyAt(1)->As<RigidBody>();

        Vector3 impulseContact;

        Vector3 deltaVelWorld = RelativeContactPosition[0] % ContactNormal;
        deltaVelWorld = inverseInertiaTensor[0].Transform(deltaVelWorld);
        deltaVelWorld = deltaVelWorld % RelativeContactPosition[0];

        float deltaVelocity = deltaVelWorld.Dot(ContactNormal);
        deltaVelocity += b1->GetInverseMass();

        {
            Vector3 deltaVelWorld = RelativeContactPosition[1] % ContactNormal;
            deltaVelWorld = inverseInertiaTensor[1].Transform(deltaVelWorld);
            deltaVelWorld = deltaVelWorld % RelativeContactPosition[1];
            deltaVelocity += deltaVelWorld.Dot(ContactNormal);
            deltaVelocity += b2->GetInverseMass();
        }

        // Calculate the required size of the impulse
        impulseContact.x = DesiredDeltaVelocity / deltaVelocity;
        impulseContact.y = 0;
        impulseContact.z = 0;
        return impulseContact;
    }
}