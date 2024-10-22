#include "StringUtils.h"

namespace Core
{
    namespace StringUtils
    {
        std::string GetExtension(const std::string &name)
        {
            size_t dotPosition = name.find_last_of('.');
            if (dotPosition == std::string::npos || dotPosition == 0 || dotPosition == name.size() - 1)
                return "";
            return name.substr(dotPosition + 1);
        }
    }
}