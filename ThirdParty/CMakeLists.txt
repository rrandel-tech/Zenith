# ==== spdlog ====
set(SPDLOG_USE_STD_FORMAT ON)
set(SPDLOG_ENABLE_PCH ON)
set(SPDLOG_MSVC_UTF8 ON)
set(SPDLOG_BUILD_EXAMPLE OFF)
set(SPDLOG_BUILD_TESTS OFF)
set(SPDLOG_BUILD_BENCH OFF)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/spdlog SYSTEM)

# ==== SDL ====
set(SDL_SHARED       OFF)
set(SDL_STATIC       ON)
set(SDL_TEST_LIBRARY OFF)
set(SDL_TESTS        OFF)
set(SDL_EXAMPLES     OFF)
set(SDL_INSTALL      OFF)
set(SDL_INSTALL_DOCS OFF)

set(SDL_AUDIO        OFF)
set(SDL_CAMERA       OFF)
set(SDL_SENSOR       OFF)
set(SDL_POWER        OFF)
set(SDL_DIALOG       ON)
set(SDL_TRAY         OFF)
set(SDL_GPU          OFF)
set(SDL_RENDER       OFF)
set(SDL_OPENGL       OFF)
set(SDL_OPENGLES     OFF)
set(SDL_KMSDRM       OFF)
set(SDL_HAPTIC       OFF)

set(SDL_VULKAN       ON)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/SDL3 SYSTEM)

target_compile_definitions(SDL3-static INTERFACE SDL_MAIN_HANDLED)

# ==== GLM (Header-only library) ====
set(GLM_BUILD_LIBRARY OFF)
set(GLM_BUILD_TESTS   OFF)
set(GLM_BUILD_INSTALL OFF)
set(GLM_ENABLE_CXX_20 ON)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/glm SYSTEM)

target_compile_definitions(glm-header-only INTERFACE GLM_FORCE_DEPTH_ZERO_TO_ONE)

# ==== magic_enum (Header-only) ====
set(MAGIC_ENUM_OPT_BUILD_EXAMPLES OFF)
set(MAGIC_ENUM_OPT_BUILD_TESTS    OFF)
set(MAGIC_ENUM_OPT_INSTALL        OFF)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/magic_enum SYSTEM)

# ==== stb (Single header libraries) ====
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${CMAKE_CURRENT_LIST_DIR}/stb)
add_library(stb::stb ALIAS stb)

# =========================
# Vulkan
# =========================
find_package(Vulkan REQUIRED)

# ==== VMA ====
add_library(vma INTERFACE)
add_library(vma::vma ALIAS vma)
target_include_directories(vma INTERFACE
        "$ENV{VULKAN_SDK}/Include/vma"
        "$ENV{VULKAN_SDK}/Include"
)
target_compile_definitions(vma INTERFACE
        VMA_STATIC_VULKAN_FUNCTIONS=0
        VMA_DYNAMIC_VULKAN_FUNCTIONS=1
)

# ==== volk ====
add_library(volk INTERFACE)
add_library(volk::volk ALIAS volk)
target_include_directories(volk INTERFACE
        "$ENV{VULKAN_SDK}/Include/Volk"
        "$ENV{VULKAN_SDK}/Include"
)
target_compile_definitions(volk INTERFACE
        $<$<PLATFORM_ID:Windows>:VK_USE_PLATFORM_WIN32_KHR>
        $<$<PLATFORM_ID:Linux>:VK_USE_PLATFORM_XLIB_KHR>
)

# ==== Slang ====
add_library(slang INTERFACE)
add_library(slang::slang ALIAS slang)
target_include_directories(slang INTERFACE "$ENV{VULKAN_SDK}/Include/slang")
find_library(SLANG_LIB NAMES slang slang.lib
        HINTS "$ENV{VULKAN_SDK}/Lib" REQUIRED)
target_link_libraries(slang INTERFACE ${SLANG_LIB}
)

# ==== FastNoise Lite (Single header library) ====
add_library(fastnoise_lite INTERFACE)
target_include_directories(fastnoise_lite INTERFACE ${CMAKE_CURRENT_LIST_DIR}/FastNoiseLite)
add_library(fastnoise_lite::fastnoise_lite ALIAS fastnoise_lite)

# ==== choc (Tracktion utilities) ====
add_library(choc INTERFACE)

target_include_directories(choc INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/choc
)

add_library(choc::choc ALIAS choc)