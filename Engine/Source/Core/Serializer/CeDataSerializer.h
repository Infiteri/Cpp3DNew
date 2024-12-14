#pragma once

#include "Core/Serializer/CeSerializer.h"
#include "Core/Data/CeDataSet.h"

namespace Core
{
    class CE_API CeDataSerializer : public CeSerializer
    {
    public:
        CeDataSet *DataSet;
        CeDataSerializer(CeDataSet *set);
        ~CeDataSerializer();

        void Serialize(YAML::Emitter &out);
        bool Deserialize(YAML::Node node);
    };
}