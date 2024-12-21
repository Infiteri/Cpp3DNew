#include "FileSystem.h"
#include "Core/Logger.h"
#include <fstream>
#include <filesystem>

namespace Core
{
    std::string FileSystem::ReadFile(const std::string &name)
    {
        std::ifstream file(name, std::ios::in | std::ios::binary);
        if (!file.is_open())
        {
            CE_CORE_WARN("Unable to read file '%s'.", name.c_str());
            return std::string("");
        }

        std::string content;
        file.seekg(0, std::ios::end);
        content.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(&content[0], content.size());
        file.close();

        return content;
    }

    void FileSystem::WriteFile(const std::string &name, const std::string &content)
    {
        std::ofstream file(name, std::ios::out | std::ios::binary);
        if (!file.is_open())
        {
            CE_CORE_WARN("Unable to write to file '%s'.", name.c_str());
            return;
        }

        file.write(content.c_str(), content.size());
        file.close();
    }

    void FileSystem::CreateFile(const std::string &name)
    {
        std::ofstream file(name);
        if (!file.is_open())
        {
            CE_CORE_WARN("Unable to create file '%s'.", name.c_str());
            return;
        }

        file.close();
    }

    bool FileSystem::ExistsFile(const std::string &name)
    {
        return std::filesystem::exists(name);
    }

    void FileSystem::RemoveFile(const std::string &name)
    {
        std::filesystem::remove(name);
    }
}
