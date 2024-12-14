#pragma once

#include "Base.h"

namespace Core
{
    class CE_API UUID
    {
    private:
        CeU64 id;

    public:
        UUID();
        UUID(CeU64 id);
        UUID(const UUID &) = default;
        ~UUID();

        operator CeU64() { return id; };

        inline CeU64 Get() { return id; };

        bool operator==(const UUID &o) const { return id == o.id; };
    };
}