#include "CeDataSet.h"

namespace Core
{
    bool CeDataSet::_Exists(const std::string &name)
    {
        return set.find(name) != set.end();
    }

    CeDataSet::CeDataSet()
    {
    }

    CeDataSet::~CeDataSet()
    {
        for (auto i : set)
        {
            i.second->ClearDataBasedOnCurrentType();
            delete i.second;
        }

        set.clear();
    }

    void CeDataSet::Add(void *Data, CeData::DataType dataType, const std::string &name)
    {
        if (_Exists(name))
            return;

        CeData *d = new CeData(Data, dataType, name);
        set[name] = d;
    }

    void CeDataSet::Add(CeData *data)
    {
        if (_Exists(data->GetName()))
            return;

        set[data->GetName()] = data;
    }

    CeData *CeDataSet::Get(const std::string &name)
    {
        if (!_Exists(name))
            return nullptr;

        return set[name];
    }

    void CeDataSet::Remove(const std::string &name)
    {
        if (!_Exists(name))
            return;
        delete set[name];
        set.erase(name);
    }

    void CeDataSet::Clear()
    {
        for (auto it : set)
            delete it.second;

        set.clear();
    }
}