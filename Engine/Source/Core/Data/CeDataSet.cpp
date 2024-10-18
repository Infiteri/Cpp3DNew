#include "CeDataSet.h"

namespace Core
{
    CeDataSet::CeDataSet()
    {
    }

    CeDataSet::~CeDataSet()
    {
        for (auto i : set)
        {
            i->ClearDataBasedOnCurrentType();
        }

        set.clear();
    }

    void CeDataSet::Add(void *Data, CeData::DataType dataType, const std::string &name)
    {
        CeData *d = new CeData(Data, dataType, name);
        set.push_back(d);
    }

    void CeDataSet::Add(CeData *data)
    {
        set.push_back(data);
    }

    CeData *CeDataSet::Get(const std::string &name)
    {
        for (auto i : set)
        {
            if (i->GetName() == name)
                return i;
        }

        return nullptr;
    }

    void CeDataSet::Remove(const std::string &name)
    {
        auto it = set.begin();
        while (it != set.end())
        {
            if ((*it)->GetName() == name)
            {
                (*it)->ClearDataBasedOnCurrentType();
                delete *it;
                it = set.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}