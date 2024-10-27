#pragma once

#include "Base.h"

#include <string>
#include <unordered_map>
#include <vector>

namespace Core
{
    // TODO: maybe no CE_API, maybe platformAPI that does the same thing expect without having full access to platform code (sucks but i get it :))
    class CE_API Platform
    {
    public:
        Platform() {}
        ~Platform() {}

        struct DynamicLibraryFunction
        {
            std::string Name;
            void *PFN;
        };

        struct DynamicLibrary
        {
            std::string Name;
            bool Valid = false;
            void *Internal;
            std::unordered_map<std::string, DynamicLibraryFunction *> Functions;
        };

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

        // --------------- LIBRARY ------------------------
        static DynamicLibrary CreateLibrary(const std::string &name);
        static void DestroyLibrary(DynamicLibrary *lib);
        static bool LoadFunction(DynamicLibrary *lib, const std::string &name);

        template <typename T>
        static T GetFunction(DynamicLibrary *lib, const std::string &name)
        {
            if (!lib->Valid)
                return NULL;

            if (lib->Functions.find(name) == lib->Functions.end())
                LoadFunction(lib, name);

            return (T)lib->Functions[name]->PFN;
        };
        // ------------------------------------------------
    };

}