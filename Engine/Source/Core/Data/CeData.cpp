#include "CeData.h"

#define CE_DATA_CLEAR(type, cast) \
    case type:                    \
    {                             \
        cast *d = (cast *)data;   \
        delete d;                 \
        break;                    \
    }

namespace Core
{
    CeData::CeData()
    {
        name = "Data";
        type = DataNone;
        data = nullptr;
        shouldClear = true;
    }

    CeData::CeData(CeData *other)
    {
        From(other);
    }

    CeData::CeData(void *_Data, DataType dataType, const std::string &_name)
    {
        data = _Data;
        type = dataType;
        name = _name;
    }

    CeData::~CeData()
    {
        ClearDataBasedOnCurrentType();
    }

    void CeData::ClearDataBasedOnCurrentType()
    {
        if (!shouldClear)
            return;

        shouldClear = false;
        switch (type)
        {
            CE_DATA_CLEAR(DataVec2, Vector2);
            CE_DATA_CLEAR(DataVec3, Vector2);
            CE_DATA_CLEAR(DataColor, Color);
            CE_DATA_CLEAR(DataFloat, FloatContainer);

        case DataNone:
        default:
            shouldClear = true;
            break;
        }

        data = nullptr;
    }

    void CeData::SetupDefaultValuesBaseOnCurrentType()
    {
        shouldClear = true;
        switch (type)
        {
        case DataVec2:
            data = new Vector2(0, 0);
            break;

        case DataVec3:
            data = new Vector3(0, 0, 0);
            break;

        case DataColor:
            data = new Color(255, 255, 255, 255);
            break;

        case DataFloat:
            data = new FloatContainer(0.0f);
            break;

        case DataNone:
        default:
            break;
        }
    }

    void CeData::From(CeData *s)
    {
        name = s->name;
        type = s->type;

        // ? Clear old data.
        if (shouldClear)
            ClearDataBasedOnCurrentType();

        //? Copy the new data.
        data = nullptr;
        switch (type)
        {
        case DataVec2:
        {
            Vector2 *other = (Vector2 *)s->data;
            data = new Vector2(other->x, other->y);
            break;
        }

        case DataVec3:
        {
            Vector3 *other = (Vector3 *)s->data;
            data = new Vector3(other->x, other->y, other->z);
            break;
        }

        case DataColor:
        {
            Color *other = (Color *)s->data;
            data = new Color(other->r, other->g, other->b, other->a);
            break;
        }

        case DataFloat:
        {
            FloatContainer *other = (FloatContainer *)s->data;
            data = new FloatContainer(other->Value);
            break;
        }

        case DataNone:
        default:
            break;
        }
    }
}