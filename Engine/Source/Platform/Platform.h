#pragma once

#include "Base.h"

namespace Core
{
    class Platform
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

        static void SetConsoleColor(PlatformLogColor Color);
        static void LogMessage(const char *Message);

        // -- MEMORY --
        static void *MemSet(void *Block, CeU32 Value, CeU64 Size);
        static void *MemALloc(CeU64 Size);
        static void *MemCpy(void *Block, const void *Source, CeU64 size);
        static void Free(void *Memo);
        // ------------
    };

}