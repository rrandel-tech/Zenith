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
set(SDL_DIALOG       OFF)
set(SDL_TRAY         OFF)
set(SDL_GPU          OFF)
set(SDL_RENDER       OFF)
set(SDL_OPENGL       OFF)
set(SDL_OPENGLES     OFF)
set(SDL_KMSDRM       OFF)
set(SDL_HAPTIC       OFF)

set(SDL_VULKAN       ON)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/SDL3 SYSTEM)