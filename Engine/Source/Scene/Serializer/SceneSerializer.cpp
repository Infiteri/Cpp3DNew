#include "SceneSerializer.h"
#include "Core/Logger.h"
#include "Core/Serializer/CeSerializerUtils.h"
#include "Core/Serializer/CeDataSerializer.h"
#include "ComponentSerializer.h"

#include <fstream>

namespace Core
{
    void SceneSerializer::_SerializeDirLight(DirectionalLight *l, YAML::Emitter &out)
    {
        out << YAML::Key << "DirectionalLight" << YAML::Value << YAML::BeginMap;
        SerializerUtils::ColorToYAML(out, "Color", &l->Color);
        SerializerUtils::Vector3ToYAML(out, "Direction", &l->Direction);
        CE_SERIALIZE_FIELD("Intensity", l->Intensity);
        out << YAML::EndMap;
    }

    void SceneSerializer::_SerializeSky(Sky *sky, YAML::Emitter &out)
    {
        out << YAML::Key << "Sky" << YAML::Value << YAML::BeginMap;
        CE_SERIALIZE_FIELD("Mode", (int)sky->GetMode());

        switch (sky->GetMode())
        {
        case Sky::ColorMode:
            SerializerUtils::ColorToYAML(out, "Color", &sky->GetColor());
            break;

        case Sky::CubeMapMode:
            CE_SERIALIZE_FIELD("CubeMapPath", sky->GetCubemapPath().c_str());
            break;

        case Sky::ShaderMode:
        {
            CE_SERIALIZE_FIELD("ShaderPath", sky->GetShaderName().c_str());
            CeDataSerializer ser(&sky->GetShaderDataSet());
            out << YAML::Key << "ShaderData" << YAML::Value << YAML::BeginSeq;
            ser.Serialize(out);
            out << YAML::EndSeq;
        }
        break;
        }
        out << YAML::EndMap;
    }

    void SceneSerializer::_DeserializeDirLight(DirectionalLight *l, YAML::Node node)
    {
        SerializerUtils::YAMLToColor(node["Color"], &l->Color);
        SerializerUtils::YAMLToVector3(node["Direction"], &l->Direction);
        l->Intensity = node["Intensity"].as<float>();
    }

    void SceneSerializer::_DeserializeSky(Sky *sky, YAML::Node node)
    {
        Sky::Mode mode = (Sky::Mode)node["Mode"].as<int>();

        switch (mode)
        {
        case Sky::ColorMode:
        {
            Color c;
            SerializerUtils::YAMLToColor(node["Color"], &c);
            sky->SetModeToColor(c);
        }
        break;

        case Sky::CubeMapMode:
        {
            std::string cubeMapPath = node["CubeMapPath"].as<std::string>();
            sky->SetModeToCubeMap(cubeMapPath);
        }
        break;

        case Sky::ShaderMode:
        {
            std::string shaderPath = node["ShaderPath"].as<std::string>();
            sky->SetModeToShader(shaderPath);

            CeDataSerializer ser(&sky->GetShaderDataSet());
            ser.Deserialize(node["ShaderData"]);
        }
        break;
        }
    }

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

        // Serialize scene environment
        {
            SceneEnvironment *env = scene->GetEnvironment();
            if (env)
            {
                out << YAML::Key << "SceneEnvironment" << YAML::Value << YAML::BeginMap;
                _SerializeDirLight(&env->DirectionalLight, out);
                _SerializeSky(env->SkyInst, out);
                out << YAML::EndMap;
            }
        }

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

    bool SceneSerializer::Deserialize(const std::string &filepath)
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

        if (!data || !data["Scene"])
        {
            return false;
        }

        // Takes the directional light field directly
        _DeserializeDirLight(&scene->GetEnvironment()->DirectionalLight, data["SceneEnvironment"]["DirectionalLight"]);
        _DeserializeSky(scene->GetEnvironment()->SkyInst, data["SceneEnvironment"]["Sky"]);

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
                if (actor["ParentUUID"])
                {
                    if (actor["ParentUUID"].as<CeU64>() == 0)
                        scene->AddActor(a);
                    else
                    {
                        auto parent = scene->GetActorInHierarchy(actor["ParentUUID"].as<CeU64>());
                        parent->AddChild(a);
                    }
                }
                else
                {
                    scene->AddActor(a);
                }

                if (actor["UUID"])
                    a->id = actor["UUID"].as<CeU64>();
                else
                    a->id = 0;
            }
        }

        //? update scene state

        // TODO: There should be problems with the new sky design so make sure no matter what happens everything is right ( _SetSkyInstance could be moved to the Render function of the scene )
        // Note: After loading the proper sky configuration, the sky instance must be updated/
        scene->_SetSkyInstance();

        return true;
    }

    void SceneSerializer::SerializeActor(Actor *a, YAML::Emitter &out)
    {
        // serialize base actor information
        // serialize transform
        // create component serializer
        // serialize each component
        // serialize children with

        out << YAML::BeginMap;
        out << YAML::Key << "Actor";
        out << YAML::Value << "Filed";

        CE_SERIALIZE_FIELD("Name", a->GetName().c_str());
        CE_SERIALIZE_FIELD("UUID", a->GetUUID().Get());
        CE_SERIALIZE_FIELD("ParentUUID", ((a->GetParent() != nullptr) ? a->GetParent()->GetUUID().Get() : 0));

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