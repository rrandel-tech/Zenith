#pragma once

#include "Event.hpp"
#include "Core/KeyCodes.hpp"

#include <format>

namespace Zenith {

    class KeyEvent : public Event
    {
    public:
        KeyCode GetKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    protected:
        KeyEvent(KeyCode keycode)
            : m_KeyCode(keycode) {}

        KeyCode m_KeyCode;
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(KeyCode keycode, int repeatCount)
            : KeyEvent(keycode), m_RepeatCount(repeatCount) {}

        int GetRepeatCount() const { return m_RepeatCount; }

        std::string ToString() const override
        {
            return std::format("KeyPressedEvent: {} ({} repeats)", static_cast<int>(m_KeyCode), m_RepeatCount);
        }

        EVENT_CLASS_TYPE(KeyPressed)
    private:
        int m_RepeatCount;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(KeyCode keycode)
            : KeyEvent(keycode) {}

        std::string ToString() const override
        {
            return std::format("KeyReleasedEvent: {}", static_cast<int>(m_KeyCode));
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class KeyTypedEvent : public KeyEvent
    {
    public:
        KeyTypedEvent(KeyCode keycode)
            : KeyEvent(keycode) {}

        std::string ToString() const override
        {
            return std::format("KeyTypedEvent: {}", static_cast<int>(m_KeyCode));
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };
}