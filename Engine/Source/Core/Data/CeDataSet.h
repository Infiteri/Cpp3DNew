#pragma once

#include "Base.h"
#include "CeData.h"

#include <vector>

namespace Core
{
    class CE_API CeDataSet
    {
    private:
        std::vector<CeData *> set;

    public:
        CeDataSet();
        ~CeDataSet();

        inline std::vector<CeData *> GetSet() { return set; };

        void Add(void *Data, CeData::DataType dataType, const std::string &name);
        void Add(CeData *data);
        CeData *Get(const std::string &name);
        void Remove(const std::string &name);
    };

}