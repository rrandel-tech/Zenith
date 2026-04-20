set(MATH_SOURCES
        Noise.cpp
)

set(MATH_HEADERS
        Noise.hpp
)

target_sources(Zenith
        PRIVATE
        ${MATH_SOURCES}
        ${MATH_HEADERS}
)