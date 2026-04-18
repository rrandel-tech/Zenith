set(CORE_SOURCES
        Application.cpp
        Base.cpp
        Layer.cpp
        LayerStack.cpp
        Log.cpp
        Window.cpp
)

set(CORE_HEADERS
        Application.hpp
        Assert.hpp
        Base.hpp
        Layer.hpp
        LayerStack.hpp
        Log.hpp
        Version.hpp
        Window.hpp
)

target_sources(Zenith
        PRIVATE
        ${CORE_SOURCES}
        ${CORE_HEADERS}
)