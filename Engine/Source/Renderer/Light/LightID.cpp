#include "LightID.h"

namespace Core
{

    struct IDs
    {
        CeU32 PointLight;
    };

    static IDs ids;
    static IDs count;

    void LightID::Begin()
    {
        ids.PointLight = 0;
    }

    CeU32 LightID::GetNewPointLight()
    {
        CeU32 id = ids.PointLight;
        ids.PointLight++;
        return id;
    }

    CeU32 LightID::GetPointLight()
    {
        return ids.PointLight;
    }

    void LightID::AddNewPointLightCount()
    {
        count.PointLight++;
    }

    CeU32 LightID::GetPointLightCount()
    {
        return count.PointLight;
    }
}