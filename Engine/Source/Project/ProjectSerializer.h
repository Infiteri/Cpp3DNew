#pragma once

#include "Base.h"
#include "Project.h"
#include "Core/Serializer/CeSerializer.h"

namespace Core
{
    class ProjectSerializer : public CeSerializer
    {
    public:
        Project *Target;
        ProjectSerializer(Project *target) { Target = target; };
        ~ProjectSerializer() {};

        void Serialize(const std::string &name);
        bool Deserialize(const std::string &name);
    };
}
