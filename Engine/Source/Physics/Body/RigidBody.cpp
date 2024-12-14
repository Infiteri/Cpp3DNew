#include "RigidBody.h"
#include "Core/Logger.h"
#include "Physics/PhysCore.h"
#include "Scene/Actor.h"
#include "Math/Math.h"

namespace Core
{
    void RigidBody::_ClearForces()
    {
        forceAccum.Set(0, 0, 0);
        torqueAccum.Set(0, 0, 0);
    }

    void RigidBody::_CalculateData()
    {
        orientation.Normalize();

        _CalculateTransformMatrix(transformMatrix, owner->GetTransform()->Position, orientation);
        _TransformInertiaTensor(inverseInertiaTensorWorld, orientation, inverseInertiaTensor, transformMatrix);

        if (!collider)
            return;
        collider->TransformMatrix = &transformMatrix;
        collider->Owner = this;
        // TODO: dont do on every frame
        inverseInertiaTensor = _ComposeInertiaMatrix(collider, config.Mass);
        inverseInertiaTensor.Invert();
    }

    void RigidBody::AddVelocity(const Vector3 &v)
    {
        velocity += v;
    }

    void RigidBody::AddRotation(const Vector3 &v)
    {
        // TODO: this internal function call should only take the vector in radians, make sure all function calls of this do that
        torque += v;
    }

    RigidBody::RigidBody()
    {
        owner = nullptr;
        config = RigidBodyConfiguration();
        collider = nullptr;
        type = Rigid;
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

        auto transform = owner->GetTransform();

        lastFrameAcceleration = {0, 0, 0};
        lastFrameAcceleration += forceAccum / config.Mass;
        lastFrameAcceleration.y -= 9.81f;

        // Modify with accelerations
        velocity += lastFrameAcceleration;
        velocity *= Math::Pow(config.LinearDamp, 0.05);

        Vector3 angularAcceleration = inverseInertiaTensorWorld.Transform(torqueAccum);
        torque += angularAcceleration;
        torque *= Math::Pow(config.AngularDamp, 0.05);

        orientation.AddScaledVector(torque, CE_PHYSICS_DELTA_TIME);

        // transform happens here
        {
            transform->Position += velocity * CE_PHYSICS_DELTA_TIME;
            transform->Rotation += torque * CE_RAD_TO_DEG * CE_PHYSICS_DELTA_TIME;
        }

        _CalculateData();
        _ClearForces();
    }

    void RigidBody::AddForce(const Vector3 &force)
    {
        forceAccum += force;
    }

    void RigidBody::AddTorque(const Vector3 &force)
    {
        torqueAccum += force * CE_DEG_TO_RAD;
    }

    void RigidBody::AddForceAtPoint(const Vector3 &force, const Vector3 &point)
    {
        Vector3 pt = point;
        pt -= owner->GetTransform()->Position;

        forceAccum += force;
        torqueAccum += pt % force;
    }

    void RigidBodyConfiguration::From(RigidBodyConfiguration *c)
    {
        Owner = c->Owner;
        LinearDamp = c->LinearDamp;
        AngularDamp = c->AngularDamp;
        Mass = c->Mass;
    }
}