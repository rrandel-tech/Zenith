set(ZENITH_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)

# ==== Root-level infrastructure files ====
set(ZENITH_ROOT_SOURCES
        ${ZENITH_SRC_DIR}/znpch.cpp
)

set(VANTA_ROOT_HEADERS
        ${VANTA_SRC_DIR}/Zenith.hpp
        ${VANTA_SRC_DIR}/EntryPoint.hpp
)

# ==== Third-party implementation files ====
set(ZENITH_THIRDPARTY_IMPL_SOURCES
        ${ZENITH_SRC_DIR}/ThirdParty/stb_impl.cpp

)

add_library(Zenith STATIC)

target_sources(Zenith PRIVATE ${ZENITH_ROOT_SOURCES} ${ZENITH_ROOT_HEADERS} ${ZENITH_THIRDPARTY_IMPL_SOURCES})

add_subdirectory(${ZENITH_SRC_DIR}/Core)
add_subdirectory(${ZENITH_SRC_DIR}/Events)

target_precompile_headers(Zenith PRIVATE ${ZENITH_SRC_DIR}/znpch.hpp)

target_compile_features(Zenith PUBLIC cxx_std_23)
target_include_directories(Zenith PUBLIC ${ZENITH_SRC_DIR})

target_link_libraries(Zenith
        PUBLIC

        # ==== Core Dependencies ====
        spdlog
        SDL3::SDL3-static
        glm::glm-header-only
        magic_enum::magic_enum
        stb::stb
)

zenith_enable_lto(Zenith)