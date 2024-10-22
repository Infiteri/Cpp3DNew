#pragma once

#include "Base.h"
#include <string>
#include <vector>

namespace Core
{
    // TODO: maybe no CE_API, maybe platformAPI that does the same thing expect without having full access to platform code (sucks but i get it :))
    class CE_API Platform
    {
    public:
        Platform() {}
        ~Platform() {}

        enum PlatformLogColor
        {
            White,
            Red,
            DarkRed,
            Blue,
            Gray,
            Green,
            Yellow,
        };

        struct DirectoryEntry
        {
            std::string Name;
            bool IsFolder = false;
        };

        static void SetConsoleColor(PlatformLogColor Color);
        static void LogMessage(const char *Message);

        // -- MEMORY --
        static void *MemSet(void *Block, CeU32 Value, CeU64 Size);
        static void *MemALloc(CeU64 Size);
        static void *MemCpy(void *Block, const void *Source, CeU64 size);
        static void Free(void *Memo);
        // ------------

        // --------------- FILE DIALOGS ------------------
        static std::string OpenFileDialog(const char *filter);
        static std::string SaveFileDialog(const char *filter);
        // ------------------------------------------------

        // --------------- DIRECTORY ENTRY ----------------
        static std::vector<std::string> GetFilePathsInDirectory(const std::string &directoryPath);
        static std::vector<std::string> GetFolderPathsInDirectory(const std::string &directoryPath);
        static std::vector<DirectoryEntry> GetDirectoryEntries(const std::string &directoryPath);
        // ------------------------------------------------
    };

}