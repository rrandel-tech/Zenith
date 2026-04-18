#pragma once

namespace Zenith {
    void InitializeCore();
    void ShutdownCore();
};

#if defined(_WIN64) || defined(_WIN32)
#define ZN_PLATFORM_WINDOWS
#elif defined(__linux__)
#define ZN_PLATFORM_LINUX
#else
#error "Unsupported platform! Zenith supports Windows and Linux."
#endif

#ifdef NDEBUG
#define ZN_RELEASE
#else
#define ZN_DEBUG
#endif

#define BIT(x) (1u << x)

//------------------------------------------------------------------------------
// Compiler Detection
//------------------------------------------------------------------------------

#if defined(__clang__)
#define ZN_COMPILER_CLANG
#elif defined(__GNUC__)
#define ZN_COMPILER_GCC
#elif defined(_MSC_VER)
#define ZN_COMPILER_MSVC
#else
#error "Unknown compiler! Zenith only supports MSVC, GCC, and Clang."
#endif