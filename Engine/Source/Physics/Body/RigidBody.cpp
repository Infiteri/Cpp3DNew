#include "RigidBody.h"
#include "Physics/PhysCore.h"
#include "Scene/Actor.h"
#include "Math/Math.h"

namespace Core
{
    static inline void _TransformInertiaTensor(PhysMatrix3 &iitWorld,
                                               const Quaternion &q,
                                               const PhysMatrix3 &iitBody,
                                               const PhysMatrix4x3 &rotMat)
    {
        float t4 = rotMat.data[0] * iitBody.data[0] +
                   rotMat.data[1] * iitBody.data[3] +
                   rotMat.data[2] * iitBody.data[6];
        float t9 = rotMat.data[0] * iitBody.data[1] +
                   rotMat.data[1] * iitBody.data[4] +
                   rotMat.data[2] * iitBody.data[7];
        float t14 = rotMat.data[0] * iitBody.data[2] +
                    rotMat.data[1] * iitBody.data[5] +
                    rotMat.data[2] * iitBody.data[8];
        float t28 = rotMat.data[4] * iitBody.data[0] +
                    rotMat.data[5] * iitBody.data[3] +
                    rotMat.data[6] * iitBody.data[6];
        float t33 = rotMat.data[4] * iitBody.data[1] +
                    rotMat.data[5] * iitBody.data[4] +
                    rotMat.data[6] * iitBody.data[7];
        float t38 = rotMat.data[4] * iitBody.data[2] +
                    rotMat.data[5] * iitBody.data[5] +
                    rotMat.data[6] * iitBody.data[8];
        float t52 = rotMat.data[8] * iitBody.data[0] +
                    rotMat.data[9] * iitBody.data[3] +
                    rotMat.data[10] * iitBody.data[6];
        float t57 = rotMat.data[8] * iitBody.data[1] +
                    rotMat.data[9] * iitBody.data[4] +
                    rotMat.data[10] * iitBody.data[7];
        float t62 = rotMat.data[8] * iitBody.data[2] +
                    rotMat.data[9] * iitBody.data[5] +
                    rotMat.data[10] * iitBody.data[8];

        iitWorld.data[0] = t4 * rotMat.data[0] +
                           t9 * rotMat.data[1] +
                           t14 * rotMat.data[2];
        iitWorld.data[1] = t4 * rotMat.data[4] +
                           t9 * rotMat.data[5] +
                           t14 * rotMat.data[6];
        iitWorld.data[2] = t4 * rotMat.data[8] +
                           t9 * rotMat.data[9] +
                           t14 * rotMat.data[10];
        iitWorld.data[3] = t28 * rotMat.data[0] +
                           t33 * rotMat.data[1] +
                           t38 * rotMat.data[2];
        iitWorld.data[4] = t28 * rotMat.data[4] +
                           t33 * rotMat.data[5] +
                           t38 * rotMat.data[6];
        iitWorld.data[5] = t28 * rotMat.data[8] +
                           t33 * rotMat.data[9] +
                           t38 * rotMat.data[10];
        iitWorld.data[6] = t52 * rotMat.data[0] +
                           t57 * rotMat.data[1] +
                           t62 * rotMat.data[2];
        iitWorld.data[7] = t52 * rotMat.data[4] +
                           t57 * rotMat.data[5] +
                           t62 * rotMat.data[6];
        iitWorld.data[8] = t52 * rotMat.data[8] +
                           t57 * rotMat.data[9] +
                           t62 * rotMat.data[10];
    }

    static inline void _CalculateTransformMatrix(PhysMatrix4x3 &transformMatrix,
                                                 const Vector3 &position,
                                                 const Quaternion &orientation)
    {
        transformMatrix.data[0] = 1 - 2 * orientation.j * orientation.j -
                                  2 * orientation.k * orientation.k;
        transformMatrix.data[1] = 2 * orientation.i * orientation.j -
                                  2 * orientation.r * orientation.k;
        transformMatrix.data[2] = 2 * orientation.i * orientation.k +
                                  2 * orientation.r * orientation.j;
        transformMatrix.data[3] = position.x;

        transformMatrix.data[4] = 2 * orientation.i * orientation.j +
                                  2 * orientation.r * orientation.k;
        transformMatrix.data[5] = 1 - 2 * orientation.i * orientation.i -
                                  2 * orientation.k * orientation.k;
        transformMatrix.data[6] = 2 * orientation.j * orientation.k -
                                  2 * orientation.r * orientation.i;
        transformMatrix.data[7] = position.y;

        transformMatrix.data[8] = 2 * orientation.i * orientation.k -
                                  2 * orientation.r * orientation.j;
        transformMatrix.data[9] = 2 * orientation.j * orientation.k +
                                  2 * orientation.r * orientation.i;
        transformMatrix.data[10] = 1 - 2 * orientation.i * orientation.i -
                                   2 * orientation.j * orientation.j;
        transformMatrix.data[11] = position.z;
    }

    void RigidBody::_ClearForces()
    {
        forceAccum.Set(0, 0, 0);
        torqueAccum.Set(0, 0, 0);
    }

    void RigidBody::_CalculateData()
    {
        // TODO: Make sure that the rotation is correct
        orientation.Set(0, 0, 0, 1);
        orientation.RotateBy(owner->GetTransform()->Rotation);

        _CalculateTransformMatrix(transformMatrix, owner->GetTransform()->Position, orientation);
        _TransformInertiaTensor(inverseInertiaTensorWorld, orientation, inverseInertiaTensor, transformMatrix);
    }

    RigidBody::RigidBody()
    {
        owner = nullptr;
        config = RigidBodyConfiguration();
    }

    RigidBody::~RigidBody()
    {
    }

    void RigidBody::UseConfiguration(RigidBodyConfiguration *config)
    {
        this->owner = config->Owner;
        this->config.From(config);
    }

    void RigidBody::Update()
    {
        if (!owner)
            return;

        _CalculateData();

        auto transform = owner->GetTransform();

        // transform happens here
        {
            transform->Position += velocity * CE_PHYSICS_DELTA_TIME;
            transform->Rotation += torque * CE_PHYSICS_DELTA_TIME;
        }

        Vector3 resultingAcceleration{0, 0, 0};
        resultingAcceleration += forceAccum / config.Mass;

        // Modify with accelerations
        velocity += resultingAcceleration;
        velocity *= Math::Pow(config.LinearDamp, CE_PHYSICS_DELTA_TIME);

        torque += torqueAccum / config.Mass;
        torque *= Math::Pow(config.LinearDamp, CE_PHYSICS_DELTA_TIME);

        _ClearForces();
    }

    void RigidBody::AddForce(const Vector3 &force)
    {
        forceAccum += force;
    }

    void RigidBody::AddForceAtPoint(const Vector3 &force, const Vector3 &point)
    {
        Vector3 pt = point;
        pt -= owner->GetTransform()->Position;

        forceAccum += force;
        torqueAccum += pt % force;
    }

    void RigidBody::AddTorque(const Vector3 &force)
    {
        torqueAccum += force * CE_DEG_TO_RAD;
    }

    void RigidBody::SetInertiaTensor(const PhysMatrix3 &matrix)
    {
        inverseInertiaTensor.SetInverse(matrix);
    }

    void RigidBodyConfiguration::From(RigidBodyConfiguration *c)
    {
        Owner = c->Owner;
        LinearDamp = c->LinearDamp;
        AngularDamp = c->AngularDamp;
        Mass = c->Mass;
    }
}