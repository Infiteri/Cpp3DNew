#include "CeDataSerializer.h"
#include "CeSerializerUtils.h"

#include <yaml-cpp/yaml.h>

namespace Core
{
    CeDataSerializer::CeDataSerializer(CeDataSet *set)
    {
        DataSet = set;
    }

    CeDataSerializer::~CeDataSerializer()
    {
    }

    void CeDataSerializer::Serialize(YAML::Emitter &out)
    {
        for (auto d : DataSet->GetSet())
        {
            out << YAML::BeginMap;
            out << YAML::Key << "Name" << YAML::Value << d->GetName();
            out << YAML::Key << "Type" << YAML::Value << (int)d->GetType();

            switch (d->GetType())
            {
            case CeData::DataVec2:
            {
                Vector2 *v = (Vector2 *)d->GetData();
                SerializerUtils::Vector2ToYAML(out, "Value", v);
                break;
            }

            case CeData::DataVec3:
            {
                Vector3 *v = (Vector3 *)d->GetData();
                SerializerUtils::Vector3ToYAML(out, "Value", v);
                break;
            }

            case CeData::DataColor:
            {
                Color *v = (Color *)d->GetData();
                SerializerUtils::ColorToYAML(out, "Value", v);

                break;
            }

            case CeData::DataFloat:
            {
                CeData::FloatContainer *v = (CeData::FloatContainer *)d->GetData();
                out << YAML::Key << "Value" << v->Value;
                break;
            }
            }

            out << YAML::EndMap;
        }
    }

    bool CeDataSerializer::Deserialize(YAML::Node node)
    {
        for (auto nodeData : node)
        {
            std::string name = nodeData["Name"].as<std::string>();
            CeData::DataType type = (CeData::DataType)(nodeData["Type"].as<int>());
            auto v = nodeData["Value"];

            switch (type)
            {
            case CeData::DataVec2:
            {
                Vector2 *data = new Vector2();
                SerializerUtils::YAMLToVector2(v, data);
                DataSet->Add(data, CeData::DataVec2, name);
            }
            break;

            case CeData::DataVec3:
            {
                Vector3 *data = new Vector3();
                SerializerUtils::YAMLToVector3(v, data);
                DataSet->Add(data, CeData::DataVec3, name);
            }
            break;

            case CeData::DataFloat:
            {
                CeData::FloatContainer *data = new CeData::FloatContainer(v.as<float>());
                DataSet->Add(data, CeData::DataFloat, name);
            }
            break;

            case CeData::DataColor:
            {
                Color *data = new Color();
                SerializerUtils::YAMLToColor(v, data);
                DataSet->Add(data, CeData::DataColor, name);
            }
            break;
            }
        }

        return true;
    }
}