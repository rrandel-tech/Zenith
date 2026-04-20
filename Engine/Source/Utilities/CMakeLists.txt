set(UTILITIES_SOURCES
        CommandLineParser.cpp
        FileSystem.cpp
        StringUtils.cpp
)

set(UTILITIES_HEADERS
        CommandLineParser.hpp
        FileSystem.hpp
        StringUtils.hpp
)

target_sources(Zenith
        PRIVATE
        ${UTILITIES_SOURCES}
        ${UTILITIES_HEADERS}
)