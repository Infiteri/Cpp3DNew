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

        std::string GetFormattedNoExt(const std::string &name)
        {
            size_t lastSlashPos = name.find_last_of("/\\");
            std::string fileName = (lastSlashPos != std::string::npos) ? name.substr(lastSlashPos + 1) : name;
            size_t lastDotPos = fileName.find_last_of('.');
            if (lastDotPos != std::string::npos)
                fileName = fileName.substr(0, lastDotPos);

            return fileName;
        }

    }
}