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
        // ADD WHEN NEW COMPONENTS
        struct ComponentCount
        {
            int MeshCount;
            int DataSetCount;
            int ScriptCount;
            int CameraCount;
            int PointLightCount;
            int RigidBodyCount;
            int TagCount;
        };
        ComponentCount count;

        Actor *a;

        void FillComponentCountData();
        void SerializeComponentCount(YAML::Emitter &out);

        // ADD WHEN NEW COMPONENTS

        void SerializeMeshComponent(MeshComponent *c, int index, YAML::Emitter &out);
        void DeserializeMeshComponent(YAML::Node node);

        void SerializeDataSetComponent(DataSetComponent *c, int index, YAML::Emitter &out);
        void DeserializeDataSetComponent(YAML::Node node);

        void SerializeScriptComponent(ScriptComponent *c, int index, YAML::Emitter &out);
        void DeserializeScriptComponent(YAML::Node node);

        void SerializeCameraComponent(CameraComponent *c, int index, YAML::Emitter &out);
        void DeserializeCameraComponent(YAML::Node node);

        void SerializePointLightComponent(PointLightComponent *c, int index, YAML::Emitter &out);
        void DeserializePointLightComponent(YAML::Node node);

        void SerializeRigidBodyComponent(RigidBodyComponent *c, int index, YAML::Emitter &out);
        void DeserializeRigidBodyComponent(YAML::Node node);

        void SerializeTagComponent(TagComponent *c, int index, YAML::Emitter &out);
        void DeserializeTagComponent(YAML::Node node);

    public:
        ComponentSerializer(Actor *target);
        ~ComponentSerializer();

        void Serialize(YAML::Emitter &out);
        void Deserialize(YAML::Node actorNode);
    };

}
