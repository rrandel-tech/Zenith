set(EDITOR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)

set(EDITOR_SOURCES
        ${EDITOR_SRC_DIR}/Zenith-Editor.cpp
        ${EDITOR_SRC_DIR}/EditorLayer.cpp
)

set(EDITOR_HEADERS
        ${EDITOR_SRC_DIR}/EditorLayer.hpp
)

add_executable(Zenith-Editor ${EDITOR_SOURCES} ${EDITOR_HEADERS})

target_compile_features(Zenith-Editor PRIVATE cxx_std_23)
target_include_directories(Zenith-Editor PUBLIC ${EDITOR_SRC_DIR})

if (MSVC)
    target_link_options(Zenith-Editor PRIVATE
            $<$<CONFIG:Debug>:/SUBSYSTEM:CONSOLE>
            $<$<CONFIG:Release>:/SUBSYSTEM:CONSOLE>
            $<$<CONFIG:Dist>:/SUBSYSTEM:WINDOWS>
    )
endif()

target_link_libraries(Zenith-Editor PRIVATE Zenith)

zenith_enable_lto(Zenith-Editor)