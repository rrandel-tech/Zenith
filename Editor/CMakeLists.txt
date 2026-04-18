set(EDITOR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Source)

set(EDITOR_SOURCES
        ${EDITOR_SRC_DIR}/Zenith-Editor.cpp
)

add_executable(Zenith-Editor ${EDITOR_SOURCES})

target_compile_features(Zenith-Editor PRIVATE cxx_std_23)
target_include_directories(Zenith-Editor PUBLIC ${EDITOR_SRC_DIR})

target_link_libraries(Zenith-Editor PRIVATE Zenith)

zenith_enable_lto(Zenith-Editor)