#pragma once

#ifdef ZN_PLATFORM_WINDOWS
#include <Windows.h>
#include <intrin.h>
#endif

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstdint>
#include <cstdarg>
#include <csignal>
#include <climits>

#include <algorithm>
#include <array>
#include <bit>
#include <chrono>
#include <compare>
#include <concepts>
#include <initializer_list>
#include <limits>
#include <memory>
#include <new>
#include <optional>
#include <random>
#include <type_traits>
#include <utility>
#include <variant>

#include <deque>
#include <map>
#include <set>
#include <stack>
#include <queue>
#include <span>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <thread>

#include <functional>

#include <Zenith/Core/Version.hpp>
#include <Zenith/Core/Base.hpp>
#include <Zenith/Core/Assert.hpp>
#include <Zenith/Core/Events/Event.hpp>
#include <Zenith/Core/Log.hpp>

// Jolt (Safety because this file has to be included before all other Jolt headers, at all times)
#ifdef ZN_DEBUG

#define JPH_ENABLE_ASSERTS
#endif
#include <Jolt/Jolt.h>
