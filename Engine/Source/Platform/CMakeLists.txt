if (WIN32)
    set(PLATFORM_SOURCES
            Windows/WindowsFileSystem.cpp
    )

elseif (UNIX AND NOT APPLE)
    set(PLATFORM_SOURCES
    )

else()
    message(FATAL_ERROR "[Zenith] Unsupported platform.")
endif()

target_sources(Zenith
        PRIVATE
        ${PLATFORM_SOURCES}
        ${PLATFORM_HEADERS}
)