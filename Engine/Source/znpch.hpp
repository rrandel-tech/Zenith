#pragma once

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

#include <Core/Version.hpp>
#include <Core/Base.hpp>
#include <Core/Assert.hpp>
#include <Events/Event.hpp>
#include <Core/Log.hpp>

#ifdef ZN_PLATFORM_WINDOWS
#include <Windows.h>
#endif