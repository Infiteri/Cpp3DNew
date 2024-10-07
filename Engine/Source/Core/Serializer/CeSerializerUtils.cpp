#include "CeSerializerUtils.h"

namespace Core
{
    void SerializerUtils::Vector3ToYAML(YAML::Node node, Vector3 *vec)
    {
        if (!node)
            return;
        vec->x = node[0].as<float>();
        vec->y = node[1].as<float>();
        vec->z = node[2].as<float>();
    }

    void SerializerUtils::YAMLToVector3(YAML::Emitter &out, const char *field, Vector3 *vec)
    {
        if (!vec)
            return;

        out << YAML::Key << field << YAML::Value;
        out << YAML::Flow;
        out << YAML::BeginSeq << vec->x << vec->y << vec->z << YAML::EndSeq;
    }

    void SerializerUtils::YAMLToColor(YAML::Node node, Color *vec)
    {
        if (!node)
            return;
        vec->r = node[0].as<float>();
        vec->g = node[1].as<float>();
        vec->b = node[2].as<float>();
        vec->a = node[3].as<float>();
    }

    void SerializerUtils::ColorToYAML(YAML::Emitter &out, const char *field, Color *vec)
    {
        if (!vec)
            return;

        out << YAML::Key << field << YAML::Value;
        out << YAML::Flow;
        out << YAML::BeginSeq << vec->r << vec->g << vec->b << vec->a << YAML::EndSeq;
    }
}