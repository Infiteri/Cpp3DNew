#pragma once

#include "Base.h"

#include <string>

namespace Core
{
    namespace StringUtils
    {
        CE_API std::string GetExtension(const std::string &name);
        CE_API std::string GetFormattedNoExt(const std::string &name);
    }
}