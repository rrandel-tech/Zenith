set(EVENT_HEADERS
        ApplicationEvent.hpp
        Event.hpp
        KeyEvent.hpp
        MouseEvent.hpp
)

target_sources(Zenith
        PRIVATE
        ${EVENT_SOURCES}
        ${EVENT_HEADERS}
)