    #pragma once

#include "Base.h"
#include <string>
#include <vector>

#define CE_LOGGER_MAX_STRING_LENGTH 30000

#define CE_USER_CATEGORY_NAME "User"
#define CE_CORE_CATEGORY_NAME "Core"

namespace Core
{
    class CE_API Logger
    {
    public:
        struct Settings
        {
            bool DisplayLevelString = true;
            std::vector<std::string> IgnoredLevels;
        };

        struct Category
        {
            std::string Name;
            std::string Prefix;
        };

        enum Level
        {
            Info,
            Warn,
            Error,
            Trace,
            Debug
        };

        Logger() {}
        ~Logger() {}

        /// @brief Starts logger with settings.
        /// @param settings The settings to use.
        static void Init(const Settings &_Settings);

        /// @brief Stops the logger,
        static void Shutdown();

        static void RegisterCategory(const std::string &Name, const std::string &Prefix);

        static void Log(const std::string &_CategoryName, Level level, const char *_Message, ...);
    };
}

#define CE_DEFINE_LOG_CATEGORY(name, pending) Core::Logger::RegisterCategory(name, pending)

#define CE_LOG(name, level, msg, ...) Core::Logger::Log(name, Core::Logger::Level::level, msg, ##__VA_ARGS__)

#define CE_ERROR(message, ...) Core::Logger::Log(CE_USER_CATEGORY_NAME, Core::Logger::Error, message, ##__VA_ARGS__)
#define CE_CORE_ERROR(message, ...) Core::Logger::Log(CE_CORE_CATEGORY_NAME, Core::Logger::Error, message, ##__VA_ARGS__)

#if CE_BUILD_RELEASE == 0
#define CE_INFO(message, ...) Core::Logger::Log(CE_USER_CATEGORY_NAME, Core::Logger::Info, message, ##__VA_ARGS__)
#define CE_CORE_INFO(message, ...) Core::Logger::Log(CE_CORE_CATEGORY_NAME, Core::Logger::Info, message, ##__VA_ARGS__)
#define CE_WARN(message, ...) Core::Logger::Log(CE_USER_CATEGORY_NAME, Core::Logger::Warn, message, ##__VA_ARGS__)
#define CE_CORE_WARN(message, ...) Core::Logger::Log(CE_CORE_CATEGORY_NAME, Core::Logger::Warn, message, ##__VA_ARGS__)
#define CE_TRACE(message, ...) Core::Logger::Log(CE_USER_CATEGORY_NAME, Core::Logger::Trace, message, ##__VA_ARGS__)
#define CE_CORE_TRACE(message, ...) Core::Logger::Log(CE_CORE_CATEGORY_NAME, Core::Logger::Trace, message, ##__VA_ARGS__)
#define CE_DEBUG(message, ...) Core::Logger::Log(CE_USER_CATEGORY_NAME, Core::Logger::Debug, message, ##__VA_ARGS__)
#define CE_CORE_DEBUG(message, ...) Core::Logger::Log(CE_CORE_CATEGORY_NAME, Core::Logger::Debug, message, ##__VA_ARGS__)
#else
#define CE_INFO(message, ...)
#define CE_CORE_INFO(message, ...)
#define CE_WARN(message, ...)
#define CE_CORE_WARN(message, ...)
#define CE_TRACE(message, ...)
#define CE_CORE_TRACE(message, ...)
#define CE_DEBUG(message, ...)
#define CE_CORE_DEBUG(message, ...)
#endif