#include "StaticBody.h"
#include "Scene/Actor.h"

namespace Core
{
    StaticBody::StaticBody()
    {
        owner = nullptr;
        type = Static;

        collider = new AABBCollider();
        collider->As<AABBCollider>()->Size = {1, 1, 1};

        // TODO: Mass?
        inverseInertiaTensor = _ComposeInertiaMatrix(collider, 10);
        inverseInertiaTensor.Invert();
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
    }
}