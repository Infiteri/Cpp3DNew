#pragma once

#include "Base.h"
#include "Core/Serializer/CeSerializer.h"
#include "Scene/Actor.h"

namespace YAML
{
    class Node;
}

namespace Core
{
    class ComponentSerializer : public CeSerializer
    {
    private:
        struct ComponentCount
        {
            int MeshCount;
        };
        ComponentCount count;

        Actor *a;

        void FillComponentCountData();
        void SerializeComponentCount(YAML::Emitter &out);

        void SerializeMeshComponent(MeshComponent *c, int index, YAML::Emitter &out);
        void DeserializeMeshComponent(YAML::Node node);

    public:
        ComponentSerializer(Actor *target);
        ~ComponentSerializer();

        void Serialize(YAML::Emitter &out);
        void Deserialize(YAML::Node actorNode);
    };

}
