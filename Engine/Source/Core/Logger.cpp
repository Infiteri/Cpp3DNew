#include "Logger.h"
#include "Platform/Platform.h"

#include <unordered_map>
#include <iostream>
#include <cstdarg>

namespace Core
{
    static Logger::Settings LogSettings;
    struct
    {
        std::unordered_map<Logger::Level, std::string> LevelString;
        std::unordered_map<Logger::Level, Platform::PlatformLogColor> LogColors;
        std::unordered_map<std::string, Logger::Category> Categories;
        std::vector<Logger::LogInfo> Logs;
    } State;

    void Logger::Init(const Settings &_Settings)
    {
        LogSettings = _Settings;

        State.LevelString[Info] = "Info";
        State.LevelString[Warn] = "Warn";
        State.LevelString[Error] = "Error";
        State.LevelString[Debug] = "Debug";
        State.LevelString[Trace] = "Trace";

        State.LogColors[Info] = Platform::White;
        State.LogColors[Warn] = Platform::Yellow;
        State.LogColors[Error] = Platform::Red;
        State.LogColors[Debug] = Platform::Blue;
        State.LogColors[Trace] = Platform::Gray;

        CE_DEFINE_LOG_CATEGORY(CE_USER_CATEGORY_NAME, CE_USER_CATEGORY_NAME);
        CE_DEFINE_LOG_CATEGORY(CE_CORE_CATEGORY_NAME, CE_CORE_CATEGORY_NAME);
    }

    void Logger::Shutdown()
    {
    }

    void Logger::RegisterCategory(const std::string &Name, const std::string &Prefix)
    {
        State.Categories[Name].Name = Name;
        State.Categories[Name].Prefix = Prefix;
    }

    void Logger::Log(const std::string &_CategoryName, Level level, const char *_Message, ...)
    {
        Category *category = &State.Categories[_CategoryName];
        if (!category)
        {
            RegisterCategory(_CategoryName, _CategoryName);
            category = &State.Categories[_CategoryName];
        }

        if (!(level == Error))
        {
            for (auto S : LogSettings.IgnoredLevels)
            {
                if (S == category->Name)
                    return;
            }
        }

        std::string logDescription = State.LevelString[level];

        char OutMessage[CE_LOGGER_MAX_STRING_LENGTH];
        Platform::MemSet(OutMessage, 0, CE_LOGGER_MAX_STRING_LENGTH);

        // Perform string pending
        __builtin_va_list arg_ptr;
        va_start(arg_ptr, _Message);
        vsnprintf(OutMessage, CE_LOGGER_MAX_STRING_LENGTH, _Message, arg_ptr);
        va_end(arg_ptr);

        char OutMessageWithLevels[CE_LOGGER_MAX_STRING_LENGTH];
        Platform::MemSet(OutMessageWithLevels, 0, CE_LOGGER_MAX_STRING_LENGTH);

        if (LogSettings.DisplayLevelString)
            snprintf(OutMessageWithLevels, CE_LOGGER_MAX_STRING_LENGTH, "[%s %s]: %s\n", category->Prefix.c_str(), logDescription.c_str(), OutMessage);
        else
            snprintf(OutMessageWithLevels, CE_LOGGER_MAX_STRING_LENGTH, "[%s]: %s\n", category->Prefix.c_str(), OutMessage);

        Platform::SetConsoleColor(State.LogColors[level]);
        Platform::LogMessage(OutMessageWithLevels);

        LogInfo inf;
        inf.Level = level;
        inf.Message = OutMessageWithLevels;
        inf.Pending = category->Prefix;
        State.Logs.push_back(inf);
    }

    std::vector<Logger::LogInfo> Logger::GetLogInfos()
    {
        return State.Logs;
    }

    void Logger::ClearLogInfos()
    {
        State.Logs.clear();
    }
}