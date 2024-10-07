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

    public:
        SceneSerializer(Scene *scene);
        ~SceneSerializer();

        void Serialize(const std::string &filepath);
        void Deserialize(const std::string &filepath);

        void SerializeActor(Actor *a, YAML::Emitter& out);

        inline void Set(Scene *scene) { this->scene = scene; };
    };

}