#include "StaticBody.h"
#include "Scene/Actor.h"

namespace Core
{
    StaticBody::StaticBody()
    {
        owner = nullptr;
        type = Static;
        collider = nullptr;
    }

    StaticBody::~StaticBody()
    {
    }

    void StaticBody::UseConfiguration(StaticBodyConfiguration *config)
    {
        this->owner = config->Owner;
        state.From(config);
    }

    void StaticBody::Update()
    {
    }

    void StaticBodyConfiguration::From(StaticBodyConfiguration *c)
    {
        Owner = c->Owner;
        Mass = c->Mass;
    }

    void StaticBody::_CalculateData()
    {
        orientation.Normalize();

        _CalculateTransformMatrix(transformMatrix, owner->GetTransform()->Position, orientation);
        _TransformInertiaTensor(inverseInertiaTensorWorld, orientation, inverseInertiaTensor, transformMatrix);

        if (!collider)
            return;
        collider->TransformMatrix = &transformMatrix;
        collider->Owner = this;
        inverseInertiaTensor = _ComposeInertiaMatrix(collider, 1);
        inverseInertiaTensor.Invert();
    }
}