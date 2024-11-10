#include "ComponentSerializer.h"
#include "Scene/Components/Components.h"
#include "Core/Serializer/CeSerializerUtils.h"
#include "Core/Serializer/CeDataSerializer.h"
#include "Resources/Loaders/MaterialLoader.h"

#define CE_COMP_SIZE(type) count.type##Count = a->GetComponents<type##Component>().size();

#define CE_SERIALIZE_COMP_CALLBACK(type)                 \
    {                                                    \
        auto list = a->GetComponents<type##Component>(); \
        for (int i = 0; i < list.size(); i++)            \
            Serialize##type##Component(list[i], i, out); \
    }

#define CE_DESERIALIZE_COMPONENT(name, fun)                 \
    for (int i = 0; i < GetNodeCount(name, actorNode); i++) \
    fun(actorNode[std::string(name) + std::string(" ") + std::to_string(i)])

namespace Core
{
    static int GetNodeCount(const std::string &name, YAML::Node node) // Ensures node exists and then returns it as "<int>", useful for when components get added but not serialized yet
    {
        std::string fmt = name + "Count";
        if (node[fmt])
            return node[fmt].as<int>();
        else
            return 0;
    };

    ComponentSerializer::ComponentSerializer(Actor *target)
    {
        a = target;
    }

    ComponentSerializer::~ComponentSerializer()
    {
    }

    void ComponentSerializer::Serialize(YAML::Emitter &out)
    {
        // ADD WHEN NEW COMPONENTS
        SerializeComponentCount(out);

        CE_SERIALIZE_COMP_CALLBACK(Mesh);
        CE_SERIALIZE_COMP_CALLBACK(DataSet);
        CE_SERIALIZE_COMP_CALLBACK(Script);
        CE_SERIALIZE_COMP_CALLBACK(Camera);
        CE_SERIALIZE_COMP_CALLBACK(PointLight);
        CE_SERIALIZE_COMP_CALLBACK(RigidBody);
        CE_SERIALIZE_COMP_CALLBACK(Tag);
    }

    void ComponentSerializer::Deserialize(YAML::Node actorNode)
    {
        // ADD WHEN NEW COMPONENTS

        CE_DESERIALIZE_COMPONENT("MeshComponent", DeserializeMeshComponent);
        CE_DESERIALIZE_COMPONENT("DataSetComponent", DeserializeDataSetComponent);
        CE_DESERIALIZE_COMPONENT("ScriptComponent", DeserializeScriptComponent);
        CE_DESERIALIZE_COMPONENT("CameraComponent", DeserializeCameraComponent);
        CE_DESERIALIZE_COMPONENT("PointLightComponent", DeserializePointLightComponent);
        CE_DESERIALIZE_COMPONENT("RigidBodyComponent", DeserializeRigidBodyComponent);
        CE_DESERIALIZE_COMPONENT("TagComponent", DeserializeTagComponent);
    }

    void ComponentSerializer::FillComponentCountData()
    {
        // ADD WHEN NEW COMPONENTS
        CE_COMP_SIZE(Mesh);
        CE_COMP_SIZE(DataSet);
        CE_COMP_SIZE(Script);
        CE_COMP_SIZE(Camera);
        CE_COMP_SIZE(PointLight);
        CE_COMP_SIZE(RigidBody);
        CE_COMP_SIZE(Tag);
    }

    void ComponentSerializer::SerializeComponentCount(YAML::Emitter &out)
    {
        // ADD WHEN NEW COMPONENTS
        FillComponentCountData();

        CE_SERIALIZE_FIELD("MeshComponentCount", count.MeshCount);
        CE_SERIALIZE_FIELD("DataSetComponentCount", count.DataSetCount);
        CE_SERIALIZE_FIELD("ScriptComponentCount", count.ScriptCount);
        CE_SERIALIZE_FIELD("CameraComponentCount", count.CameraCount);
        CE_SERIALIZE_FIELD("PointLightComponentCount", count.PointLightCount);
        CE_SERIALIZE_FIELD("RigidBodyComponentCount", count.RigidBodyCount);
        CE_SERIALIZE_FIELD("TagComponentCount", count.TagCount);
    }

    void ComponentSerializer::SerializeMeshComponent(MeshComponent *c, int index, YAML::Emitter &out)
    {
        auto mat = c->mesh->GetMaterial();
        auto geo = c->mesh->GetGeometry();

        out << YAML::Key << "MeshComponent " + std::to_string(index);
        out << YAML::BeginMap;

        {
            CE_SERIALIZE_FIELD("Type", mat->GetType());

            switch (mat->GetType())
            {
            case Material::Config:
            {
                MaterialLoader loader;
                loader.SerializeNonSpecific(out, mat);
            }
            break;

            case Material::File:
                CE_SERIALIZE_FIELD("FilePath", mat->GetFilePath().c_str());
                break;
            }
        }

        {
            CE_SERIALIZE_FIELD("GeometryType", geo->GetType());

            switch (geo->GetType())
            {
            case Geometry::Box:
            {
                auto g = (BoxGeometry *)geo;
                CE_SERIALIZE_FIELD("Width", g->Width);
                CE_SERIALIZE_FIELD("Height", g->Height);
                CE_SERIALIZE_FIELD("Depth", g->Depth);
            }
            break;

            case Geometry::Sphere:
            {
                auto g = (SphereGeometry *)geo;
                CE_SERIALIZE_FIELD("Radius", g->Radius);
                CE_SERIALIZE_FIELD("LongitudeSegments", g->LongitudeSegments);
                CE_SERIALIZE_FIELD("LatitudeSegments", g->LatitudeSegments);
            }
            break;
            }
        }

        out << YAML::EndMap;
    }

    void ComponentSerializer::DeserializeMeshComponent(YAML::Node mc)
    {
        auto c = a->AddComponent<MeshComponent>();

        {
            Material::MaterialTypes loadMode = (Material::MaterialTypes)mc["Type"].as<int>();

            switch (loadMode)
            {
            case Material::Default:
                c->mesh->MakeMaterialDefault();
                break;

            case Material::Config:
            {
                Material::Configuration config;
                MaterialLoader l;
                l.DeserializeNonSpecific(mc, &config);
                c->mesh->MakeMaterialUnique(config);
            }
            break;

            case Material::File:
                c->mesh->MakeMaterialFromFile(mc["FilePath"].as<std::string>());
                break;
            }
        }

        // geometry
        {
            Geometry::Types type = (Geometry::Types)mc["GeometryType"].as<int>();

            switch (type)
            {
            case Geometry::Box:
                c->mesh->SetGeometry(new BoxGeometry(mc["Width"].as<float>(), mc["Height"].as<float>(), mc["Depth"].as<float>()));
                break;

            case Geometry::Sphere:
                c->mesh->SetGeometry(new SphereGeometry(mc["Radius"].as<float>(), mc["LongitudeSegments"].as<float>(), mc["LatitudeSegments"].as<float>()));
                break;

            default:
                c->mesh->SetGeometry(new Geometry());
                break;
            }
        }
    }

    void ComponentSerializer::SerializeDataSetComponent(DataSetComponent *c, int index, YAML::Emitter &out)
    {
        out << YAML::Key << "DataSetComponent " + std::to_string(index);
        out << YAML::BeginSeq;

        CeDataSerializer ser(&c->Set);
        ser.Serialize(out);

        out << YAML::EndSeq;
    }

    void ComponentSerializer::DeserializeDataSetComponent(YAML::Node node)
    {
        auto c = a->AddComponent<DataSetComponent>();
        CeDataSerializer ser(&c->Set);
        ser.Deserialize(node);
    }

    void ComponentSerializer::SerializeScriptComponent(ScriptComponent *c, int index, YAML::Emitter &out)
    {
        out << YAML::Key << "ScriptComponent " + std::to_string(index);
        out << YAML::BeginMap;
        CE_SERIALIZE_FIELD("ClassName", c->ClassName.c_str());
        out << YAML::EndMap;
    }

    void ComponentSerializer::DeserializeScriptComponent(YAML::Node node)
    {
        auto c = a->AddComponent<ScriptComponent>();
        c->ClassName = node["ClassName"].as<std::string>();
    }

    void ComponentSerializer::SerializeCameraComponent(CameraComponent *c, int index, YAML::Emitter &out)
    {
        out << YAML::Key << "CameraComponent " + std::to_string(index);
        out << YAML::BeginMap;
        CE_SERIALIZE_FIELD("FOV", c->FOV);
        CE_SERIALIZE_FIELD("Near", c->Near);
        CE_SERIALIZE_FIELD("Far", c->Far);
        CE_SERIALIZE_FIELD("IsPrimary", c->IsPrimary);
        out << YAML::EndMap;
    }

    void ComponentSerializer::DeserializeCameraComponent(YAML::Node node)
    {
        auto c = a->AddComponent<CameraComponent>();
        c->FOV = node["FOV"].as<float>();
        c->Near = node["Near"].as<float>();
        c->Far = node["Far"].as<float>();
        c->IsPrimary = node["IsPrimary"].as<bool>();
        c->UpdateCameraState();
    }

    void ComponentSerializer::SerializePointLightComponent(PointLightComponent *c, int index, YAML::Emitter &out)
    {
        out << YAML::Key << "PointLightComponent " + std::to_string(index);
        out << YAML::BeginMap;
        SerializerUtils::Vector3ToYAML(out, "Position", &c->Light->Position);
        SerializerUtils::Vector3ToYAML(out, "Specular", &c->Light->Specular);
        SerializerUtils::ColorToYAML(out, "Color", &c->Light->Color);
        CE_SERIALIZE_FIELD("Constant", c->Light->Constant);
        CE_SERIALIZE_FIELD("Linear", c->Light->Linear);
        CE_SERIALIZE_FIELD("Quadratic", c->Light->Quadratic);
        CE_SERIALIZE_FIELD("Radius", c->Light->Radius);
        CE_SERIALIZE_FIELD("Intensity", c->Light->Intensity);
        out << YAML::EndMap;
    }

    void ComponentSerializer::DeserializePointLightComponent(YAML::Node node)
    {
        auto c = a->AddComponent<PointLightComponent>();

        SerializerUtils::YAMLToVector3(node["Position"], &c->Light->Position);
        SerializerUtils::YAMLToVector3(node["Specular"], &c->Light->Specular);
        SerializerUtils::YAMLToColor(node["Color"], &c->Light->Color);
        c->Light->Constant = node["Constant"].as<float>();
        c->Light->Linear = node["Linear"].as<float>();
        c->Light->Quadratic = node["Quadratic"].as<float>();
        c->Light->Radius = node["Radius"].as<float>();
        c->Light->Intensity = node["Intensity"].as<float>();
    }

    void ComponentSerializer::SerializeRigidBodyComponent(RigidBodyComponent *c, int index, YAML::Emitter &out)
    {
        out << YAML::Key << "RigidBodyComponent " + std::to_string(index);
        out << YAML::BeginMap;
        CE_SERIALIZE_FIELD("LinearDamp", c->Config.LinearDamp);
        CE_SERIALIZE_FIELD("AngularDamp", c->Config.AngularDamp);
        CE_SERIALIZE_FIELD("Mass", c->Config.Mass);
        out << YAML::EndMap;
    }

    void ComponentSerializer::DeserializeRigidBodyComponent(YAML::Node node)
    {
        auto c = a->AddComponent<RigidBodyComponent>();

        c->Config.LinearDamp = node["LinearDamp"].as<float>();
        c->Config.AngularDamp = node["AngularDamp"].as<float>();
        c->Config.Mass = node["Mass"].as<float>();
    }

    void ComponentSerializer::SerializeTagComponent(TagComponent *c, int index, YAML::Emitter &out)
    {
        out << YAML::Key << "TagComponent " + std::to_string(index);
        out << YAML::BeginMap;
        CE_SERIALIZE_FIELD("Tag", c->Tag.c_str());
        out << YAML::EndMap;
    }

    void ComponentSerializer::DeserializeTagComponent(YAML::Node node)
    {
        auto c = a->AddComponent<TagComponent>();
        c->Tag = node["Tag"].as<std::string>();
    }
}