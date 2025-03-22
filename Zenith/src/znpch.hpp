#pragma once

#ifdef ZN_PLATFORM_WINDOWS
#include <Windows.h>
#include <intrin.h>
#endif

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdlib>
#include <cstdarg>
#include <csignal>
#include <filesystem>
#include <fstream>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <mutex>
#include <random>
#include <set>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>
#include <filesystem>
#include <thread>

#include <Zenith/Core/Version.hpp>
#include <Zenith/Core/Assert.hpp>
#include <Zenith/Core/Base.hpp>
#include <Zenith/Core/Log.hpp>
