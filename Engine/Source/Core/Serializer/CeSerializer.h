#pragma once

#include "Base.h"
#include <string>

#define CE_SERIALIZE_FIELD(name, value) out << YAML::Key << name << YAML::Value << value
#define CE_SERIALIZE_FIELD_TO(name, value, to) to << YAML::Key << name << YAML::Value << value

namespace YAML
{
    class Emitter;
}

namespace Core
{

    class CE_API CeSerializer
    {
    public:
        CeSerializer() {};
        ~CeSerializer() {};

        virtual void Serialize(YAML::Emitter &out) {};
        virtual void Serialize(const std::string &filepath, YAML::Emitter &out) {};
        virtual void Serialize(const std::string &filepath) {};
        virtual void Deserialize(const std::string &filepath) {};
    };

}