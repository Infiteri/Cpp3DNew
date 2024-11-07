#pragma once

#include "Core/Serializer/CeSerializer.h"
#include "Scene/Scene.h"
#include "Scene/Actor.h"

namespace Core
{
    class CE_API SceneSerializer : public CeSerializer
    {
    private:
        Scene *scene;

        void _SerializeDirLight(DirectionalLight *l, YAML::Emitter &out);
        void _SerializeSky(Sky *sky, YAML::Emitter &out);

        void _DeserializeDirLight(DirectionalLight *l, YAML::Node node);
        void _DeserializeSky(Sky *sky, YAML::Node node);

    public:
        SceneSerializer(Scene *scene);
        ~SceneSerializer();

        void Serialize(const std::string &filepath);
        void Deserialize(const std::string &filepath);

        void SerializeActor(Actor *a, YAML::Emitter &out);

        inline void Set(Scene *scene) { this->scene = scene; };
    };

}