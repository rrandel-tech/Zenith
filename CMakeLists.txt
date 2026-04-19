cmake_minimum_required(VERSION 3.25...4.0 FATAL_ERROR)

# ==== Configuration Types ====
if (CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR) # Only do this when we're at the top level, see: https://gitlab.kitware.com/cmake/cmake/-/issues/24181
	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
		set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Dist")
	elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
		set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Dist")
	endif()
endif()

project(Zenith  LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS_DIST           "${CMAKE_CXX_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_DIST    "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
set(CMAKE_SHARED_LINKER_FLAGS_DIST "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")

if (MSVC)
	add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/MP>)
endif()

# ==== CMake Module Path ====
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/BuildSystem/CMake")

# ==== Modern CMake Policies ====
if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.31")
	cmake_policy(SET CMP0171 NEW)
	cmake_policy(SET CMP0163 NEW)
endif()

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.30")
	cmake_policy(SET CMP0163 NEW)
endif()

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.20")
	cmake_policy(SET CMP0118 NEW)
endif()

cmake_policy(SET CMP0069 NEW)
cmake_policy(SET CMP0077 NEW)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

# ==== Build Config ====
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(BUILD_SHARED_LIBS OFF)

# ==== Interprocedural Optimization (Per-target approach) ====
include(CheckIPOSupported)
check_ipo_supported(RESULT IPO_SUPPORTED)

if(IPO_SUPPORTED)
	message(STATUS "Interprocedural Optimization (LTO): Available for per-target use")
else()
	message(WARNING "Interprocedural Optimization (LTO): Not supported by compiler")
endif()

# ==== CMake Utilities ====
include(CmakeUtils/ZenithOptimization)

add_compile_options(
		$<$<CXX_COMPILER_ID:MSVC>:/Gy>
		$<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>
)

# ==== Add Subsystems ====
add_subdirectory(ThirdParty)
add_subdirectory(Engine)
add_subdirectory(Editor)