set(CORE_SOURCES
        Application.cpp
        Base.cpp
        Log.cpp
)

set(CORE_HEADERS
        Application.hpp
        Assert.hpp
        Base.hpp
        Log.hpp
        Version.hpp
)

target_sources(Zenith
        PRIVATE
        ${CORE_SOURCES}
        ${CORE_HEADERS}
)