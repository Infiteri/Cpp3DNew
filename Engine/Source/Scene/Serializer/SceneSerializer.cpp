#include "SceneSerializer.h"
#include "Core/Logger.h"
#include "Core/Serializer/CeSerializerUtils.h"
#include "ComponentSerializer.h"

#include <fstream>

namespace Core
{
    SceneSerializer::SceneSerializer(Scene *scene)
    {
        this->scene = nullptr;
        Set(scene);
    }

    SceneSerializer::~SceneSerializer()
    {
    }

    void SceneSerializer::Serialize(const std::string &filepath)
    {
        YAML::Emitter out;
        out << YAML::BeginMap;
        out << YAML::Key << "Scene";
        out << YAML::Value << scene->GetName().c_str();

        auto actors = scene->GetActors();
        if (actors.size() > 0)
        {
            out << YAML::Key << "Actors";
            out << YAML::Value << YAML::BeginSeq;

            // Serialize each actor
            for (Actor *a : actors)
                SerializeActor(a, out);

            out << YAML::EndSeq;
            out << YAML::EndMap;
        }

        std::ofstream fout(filepath);
        fout << out.c_str();
    }

    void SceneSerializer::Deserialize(const std::string &filepath)
    {
        CE_CORE_TRACE("Deserializing scene: %s", filepath.c_str());

        // load file data
        // get data needed
        // load actors
        // create component serializer to deserialize
        // add to the right target (scene/parent)

        std::ifstream stream(filepath);
        std::stringstream strStream(filepath);
        strStream << stream.rdbuf();
        YAML::Node data = YAML::Load(strStream.str());

        if (!data["Scene"])
            return;

        auto actors = data["Actors"];
        if (actors)
        {
            for (auto actor : actors)
            {
                Actor *a = new Actor();

                //? base info
                if (actor["Name"])
                    a->SetName(actor["Name"].as<std::string>());
                else
                    a->SetName("");

                if (actor["Transform"])
                {
                    SerializerUtils::YAMLToVector3(actor["Transform"]["Position"], &a->GetTransform()->Position);
                    SerializerUtils::YAMLToVector3(actor["Transform"]["Rotation"], &a->GetTransform()->Rotation);
                    SerializerUtils::YAMLToVector3(actor["Transform"]["Scale"], &a->GetTransform()->Scale);
                }

                ComponentSerializer componentSerializer(a);
                componentSerializer.Deserialize(actor);

                //? add to the right target
                if (actor["ParentUUID"].as<CeU64>() == 0)
                {
                    scene->AddActor(a);
                }
                else
                {
                    auto parent = scene->GetActorInHierarchy(actor["ParentUUID"].as<CeU64>());
                    parent->AddChild(a);
                }

                if (actor["UUID"])
                    a->id = actor["UUID"].as<CeU64>();
                else
                    a->id = 0;
            }
        }
    }

    void SceneSerializer::SerializeActor(Actor *a, YAML::Emitter &out)
    {
        // serialize base actor information
        // serialize transform
        // create component serializer
        // serialize each component
        // serialize children with

        CE_CORE_TRACE("Serializing actor: %s", a->GetName().c_str());

        out << YAML::BeginMap;
        out << YAML::Key << "Actor";
        out << YAML::Value << "Filed";

        CE_SERIALIZE_FIELD("Name", a->GetName().c_str());
        CE_SERIALIZE_FIELD("UUID", a->GetUUID().Get());
        CE_SERIALIZE_FIELD("ParentUUID", ((a->GetParent() != nullptr) ? a->GetParent()->GetUUID().Get() : 0));

        // Serialize transform
        {
            out << YAML::Key << "Transform";
            out << YAML::Value << YAML::BeginMap;
            SerializerUtils::Vector3ToYAML(out, "Position", &a->GetTransform()->Position);
            SerializerUtils::Vector3ToYAML(out, "Rotation", &a->GetTransform()->Rotation);
            SerializerUtils::Vector3ToYAML(out, "Scale", &a->GetTransform()->Scale);
            out << YAML::Value << YAML::EndMap;
        }

        ComponentSerializer componentSerializer(a);
        componentSerializer.Serialize(out);

        out << YAML::EndMap;

        for (auto c : a->GetChildren())
        {
            CE_CORE_TRACE("Serializing actor/child: %s, %s", a->GetName().c_str(), c->GetName().c_str());
            SerializeActor(c, out);
        }
    }
}