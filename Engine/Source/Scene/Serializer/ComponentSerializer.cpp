#include "ComponentSerializer.h"
#include "Scene/Components/Components.h"
#include "Core/Serializer/CeSerializerUtils.h"

#define CE_COMP_SIZE(type) count.type##Count = a->GetComponents<type##Component>().size();

#define CE_SERIALIZE_COMP_CALLBACK(type)                 \
    {                                                    \
        auto list = a->GetComponents<type##Component>(); \
        for (int i = 0; i < list.size(); i++)            \
            Serialize##type##Component(list[i], i, out); \
    }

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
        SerializeComponentCount(out);
        CE_SERIALIZE_COMP_CALLBACK(Mesh);
    }

    void ComponentSerializer::Deserialize(YAML::Node actorNode)
    {
        for (int i = 0; i < GetNodeCount("MeshComponent", actorNode); i++)
            DeserializeMeshComponent(actorNode["MeshComponent " + std::to_string(i)]);
    }

    void ComponentSerializer::FillComponentCountData()
    {
        CE_COMP_SIZE(Mesh);
    }

    void ComponentSerializer::SerializeComponentCount(YAML::Emitter &out)
    {
        FillComponentCountData();
        CE_SERIALIZE_FIELD("MeshComponentCount", count.MeshCount);
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
                Color c = mat->GetState().Color;
                CE_SERIALIZE_FIELD("Name", mat->GetState().Name);
                SerializerUtils::ColorToYAML(out, "Color", &c);
                CE_SERIALIZE_FIELD("TextureName", mat->GetColorTexture()->GetImagePath().c_str());
                CE_SERIALIZE_FIELD("TextureMin", (int)mat->GetColorTexture()->GetConfig().MinFilter);
                CE_SERIALIZE_FIELD("TextureMag", (int)mat->GetColorTexture()->GetConfig().MagFilter);
                break;

                // TODO: Fill
                // case Material::File:
                //     break;
            }
        }

        {
            CE_SERIALIZE_FIELD("GeometryType", geo->GetType());

            switch (geo->GetType())
            {
            case Geometry::Box:
                auto g = (BoxGeometry *)geo;
                CE_SERIALIZE_FIELD("Width", g->Width);
                CE_SERIALIZE_FIELD("Height", g->Height);
                CE_SERIALIZE_FIELD("Depth", g->Depth);
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
                Material::Configuration config;
                config.Name = mc["Name"].as<std::string>();
                SerializerUtils::YAMLToColor(mc["Color"], &config.Color);
                // TODO: Texture
                config.TextureConfig.FilePath = mc["TextureName"].as<std::string>();
                config.TextureConfig.MinFilter = (TextureFilter)mc["TextureMin"].as<int>();
                config.TextureConfig.MagFilter = (TextureFilter)mc["TextureMag"].as<int>();
                c->mesh->MakeMaterialUnique(config);
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

            default:
                c->mesh->SetGeometry(new Geometry());
                break;
            }
        }
    }
}