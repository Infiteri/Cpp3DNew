#pragma once

#include "Base.h"
#include "CeData.h"

#include <unordered_map>

namespace Core
{
    class CE_API CeDataSet
    {
    private:
        std::unordered_map<std::string, CeData *> set;

        bool _Exists(const std::string &name);

    public:
        CeDataSet();
        ~CeDataSet();

        inline std::unordered_map<std::string, CeData *> &GetSet() { return set; };

        void Add(void *Data, CeData::DataType dataType, const std::string &name);
        void Add(CeData *data);
        CeData *Get(const std::string &name);
        void Remove(const std::string &name);

        void Clear();
    };

}