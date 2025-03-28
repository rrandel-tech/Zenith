#pragma once

#include <functional>
#include <memory>

namespace Zenith {

  void InitializeCore();
  void ShutdownCore();

};

#if !defined(ZN_PLATFORM_WINDOWS) && !defined(ZN_PLATFORM_LINUX)
  #error Unknown platform.
#endif

#if defined(__GNUC__)
  #if defined(__clang__)
    #define ZN_COMPILER_CLANG
  #else
    #define ZN_COMPILER_GCC
  #endif
#elif defined(_MSC_VER)
  #define ZN_COMPILER_MSVC
#endif

#ifdef ZN_COMPILER_MSVC
  #define ZN_FORCE_INLINE __forceinline
  #define ZN_EXPLICIT_STATIC static
#elif defined(__GNUC__)
  #define ZN_FORCE_INLINE __attribute__((always_inline)) inline
  #define ZN_EXPLICIT_STATIC
#else
  #define ZN_FORCE_INLINE inline
  #define ZN_EXPLICIT_STATIC
#endif

#define BIT(x) (1 << x)

#define ZN_BIND_EVENT_FN(fn) std::bind(&##fn, this, std::placeholders::_1)

// Pointer wrappers
namespace Zenith {

  template<typename T>
  using Scope = std::unique_ptr<T>;

  template<typename T>
  using Ref = std::shared_ptr<T>;

  using byte = uint8_t;

}