#pragma once

#include "Event.hpp"

#include <format>

#include "Core/KeyCodes.hpp"

namespace Zenith {

	class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float x, float y)
            : m_MouseX(x), m_MouseY(y) {}

        float GetX() const { return m_MouseX; }
        float GetY() const { return m_MouseY; }

        std::string ToString() const override
        {
            return std::format("MouseMovedEvent: {}, {}", m_MouseX, m_MouseY);
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float m_MouseX, m_MouseY;
    };

    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(float xOffset, float yOffset)
            : m_XOffset(xOffset), m_YOffset(yOffset) {}

        float GetXOffset() const { return m_XOffset; }
        float GetYOffset() const { return m_YOffset; }

        std::string ToString() const override
        {
            return std::format("MouseScrolledEvent: {}, {}", m_XOffset, m_YOffset);
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    private:
        float m_XOffset, m_YOffset;
    };

    class MouseButtonEvent : public Event
    {
    public:
        MouseButton GetMouseButton() const { return m_Button; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    protected:
        MouseButtonEvent(MouseButton button)
            : m_Button(button) {}

        MouseButton m_Button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(MouseButton button)
            : MouseButtonEvent(button) {}

        std::string ToString() const override
        {
            return std::format("MouseButtonPressedEvent: {}", static_cast<int>(m_Button));
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(MouseButton button)
            : MouseButtonEvent(button) {}

        std::string ToString() const override
        {
            return std::format("MouseButtonReleasedEvent: {}", static_cast<int>(m_Button));
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };

    class MouseButtonDownEvent : public MouseButtonEvent
    {
    public:
        MouseButtonDownEvent(MouseButton button)
            : MouseButtonEvent(button) {}

        std::string ToString() const override
        {
            return std::format("MouseButtonDownEvent: {}", static_cast<int>(m_Button));
        }

        EVENT_CLASS_TYPE(MouseButtonDown)
    };
}