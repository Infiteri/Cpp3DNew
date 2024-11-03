#pragma once

/// @brief Version (MAJOR.MINOR.SUBTLE - DD/MM/YYYY)
#define CE_VERSION "0.1.8 - 03.11.2024"

/// @brief Value for the unknown OS
#define CE_UNKNOWN_PLATFORM_VALUE 0

/// @brief Value for the Windows OS
#define CE_WIN32_PLATFORM_VALUE 1

/// @brief Represents the current build platform (check CE_PLATFORM == CE_PlatformName_PLATFORM_VALUE)
/// @example if CE_PLATFORM == CE_WIN32_PLATFORM_VALUE
#define CE_PLATFORM CE_UNKNOWN_PLATFORM_VALUE

#if defined(__WIN32)
#undef CE_PLATFORM
#define CE_PLATFORM CE_WIN32_PLATFORM_VALUE
#endif

#if CE_PLATFORM == CE_WIN32_PLATFORM_VALUE
#ifdef CE_BUILD_DLL
// The dll export
#define CE_API __declspec(dllexport)
#else
// The dll import
#define CE_API __declspec(dllimport)
#endif
#endif

typedef unsigned char CeU8;
typedef unsigned short CeU16;
typedef unsigned int CeU32;
typedef unsigned long long CeU64;

typedef signed char CeI8;
typedef signed short CeI16;
typedef signed int CeI32;
typedef signed long long CeI64;

#define CE_BUILD_RELEASE 0

#ifdef _MSVC_VER_
#undef CE_FUNCTION_SIGNATURE
#define CE_FUNCTION_SIGNATURE __FUNCSIG__
#else
#undef CE_FUNCTION_SIGNATURE
#define CE_FUNCTION_SIGNATURE __PRETTY_FUNCTION__
#endif

#define CE_DEFINE_SCRIPT_EXPORT(className) \
    extern "C" __declspec(dllexport) ActorScript *className##Create() { return new className(); }
