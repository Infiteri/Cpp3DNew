#pragma once

#include "Base.h"
#include <string>
#include "Math/Math.h"
#include "Math/Vectors.h"
#include "Renderer/Color.h"

namespace Core
{
    /// @brief Individual Data component, used in "CeDataSet".
    class CE_API CeData
    {
    public:
        enum DataType
        {
            DataNone = 0,
            DataVec2,
            DataVec3,
            DataColor,
            DataFloat,
        };

        struct FloatContainer
        {
            float Value;

            FloatContainer() {};
            FloatContainer(float v) { Value = v; };
        };

    private:
        std::string name;
        DataType type;
        void *data;
        bool shouldClear = false;

    public:
        CeData();
        CeData(CeData *other);
        CeData(void *_Data, DataType dataType, const std::string &_name);
        ~CeData();

        inline std::string GetName() { return name; };
        inline DataType GetType() { return type; };
        inline void SetName(const std::string &n) { name = n; }
        inline void SetType(DataType t) { type = t; }
        inline void *GetData() { return data; };

        void ClearDataBasedOnCurrentType();

        void SetupDefaultValuesBaseOnCurrentType();

        void From(CeData *s);

        template <typename T>
        T *As()
        {
            return (T *)data;
        }
    };
}