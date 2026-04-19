#include "znpch.hpp"
#include "Core/Input.hpp"
#include "Core/Application.hpp"

#include <SDL3/SDL.h>

namespace Zenith {

    void Input::Update()
    {
        int gamepadCount = 0;
        SDL_JoystickID* gamepads = SDL_GetGamepads(&gamepadCount);

        // Cleanup disconnected controllers
        for (auto it = s_Controllers.begin(); it != s_Controllers.end(); )
        {
            int id = it->first;
            bool found = false;
            for (int i = 0; i < gamepadCount; i++)
            {
                if (gamepads[i] == id) { found = true; break; }
            }
            it = found ? ++it : s_Controllers.erase(it);
        }

        for (int i = 0; i < gamepadCount; i++)
        {
            SDL_JoystickID id = gamepads[i];
            Controller& controller = s_Controllers[id];
            controller.ID = id;

            SDL_Gamepad* gamepad = SDL_GetGamepadFromID(id);
            if (!gamepad) continue;

            controller.Name = SDL_GetGamepadName(gamepad);

            for (int b = 0; b < SDL_GAMEPAD_BUTTON_COUNT; b++)
            {
                SDL_GamepadButton button = static_cast<SDL_GamepadButton>(b);
                bool down = SDL_GetGamepadButton(gamepad, button);

                if (down && !controller.ButtonDown[button])
                    controller.ButtonStates[button].State = KeyState::Pressed;
                else if (!down && controller.ButtonDown[button])
                    controller.ButtonStates[button].State = KeyState::Released;

                controller.ButtonDown[button] = down;
            }

            for (int a = 0; a < SDL_GAMEPAD_AXIS_COUNT; a++)
            {
                SDL_GamepadAxis axis = static_cast<SDL_GamepadAxis>(a);
                float value = SDL_GetGamepadAxis(gamepad, axis) / 32767.0f;
                controller.AxisStates[axis] = std::abs(value) > controller.DeadZones[axis] ? value : 0.0f;
            }
        }

        SDL_free(gamepads);
    }

    bool Input::IsKeyPressed(KeyCode key)
    {
        return s_KeyData.find(key) != s_KeyData.end() && s_KeyData[key].State == KeyState::Pressed;
    }

    bool Input::IsKeyHeld(KeyCode key)
    {
        return s_KeyData.find(key) != s_KeyData.end() && s_KeyData[key].State == KeyState::Held;
    }

    bool Input::IsKeyDown(KeyCode keycode)
    {
        const bool* keys = SDL_GetKeyboardState(nullptr);
        return keys[static_cast<SDL_Scancode>(keycode)];
    }

    bool Input::IsKeyReleased(KeyCode key)
    {
        return s_KeyData.find(key) != s_KeyData.end() && s_KeyData[key].State == KeyState::Released;
    }

    bool Input::IsKeyToggledOn(KeyCode key)
    {
        SDL_Keymod mods = SDL_GetModState();

        if (key == KeyCode::CapsLock)   return mods & SDL_KMOD_CAPS;
        if (key == KeyCode::NumLock)    return mods & SDL_KMOD_NUM;
        if (key == KeyCode::ScrollLock) return mods & SDL_KMOD_SCROLL;

        return false;
    }

    bool Input::IsMouseButtonPressed(MouseButton button)
    {
        return s_MouseData.find(button) != s_MouseData.end() && s_MouseData[button].State == KeyState::Pressed;
    }

    bool Input::IsMouseButtonHeld(MouseButton button)
    {
        return s_MouseData.find(button) != s_MouseData.end() && s_MouseData[button].State == KeyState::Held;
    }

    bool Input::IsMouseButtonDown(MouseButton button)
    {
        float x, y;
        SDL_MouseButtonFlags state = SDL_GetMouseState(&x, &y);
        return state & SDL_BUTTON_MASK(static_cast<int>(button));
    }

    bool Input::IsMouseButtonReleased(MouseButton button)
    {
        return s_MouseData.find(button) != s_MouseData.end() && s_MouseData[button].State == KeyState::Released;
    }

    float Input::GetMouseX()
    {
        auto [x, y] = GetMousePosition();
        return x;
    }

    float Input::GetMouseY()
    {
        auto [x, y] = GetMousePosition();
        return y;
    }

    std::pair<float, float> Input::GetMousePosition()
    {
        SDL_Window* window = static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow());

        if (SDL_GetWindowRelativeMouseMode(window))
        {
            float x, y;
            SDL_GetRelativeMouseState(&x, &y);
            return { x, y };
        }

        float x, y;
        SDL_GetMouseState(&x, &y);
        return { x, y };
    }

    void Input::SetCursorMode(CursorMode mode)
    {
        SDL_Window* window = static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow());

        switch (mode)
        {
            case CursorMode::Normal:
                SDL_SetWindowRelativeMouseMode(window, false);
                SDL_ShowCursor();
                break;
            case CursorMode::Hidden:
                SDL_SetWindowRelativeMouseMode(window, false);
                SDL_HideCursor();
                break;
            case CursorMode::Locked:
                SDL_SetWindowRelativeMouseMode(window, true);
                break;
        }
    }

    CursorMode Input::GetCursorMode()
    {
        SDL_Window* window = static_cast<SDL_Window*>(Application::Get().GetWindow().GetNativeWindow());

        if (SDL_GetWindowRelativeMouseMode(window)) return CursorMode::Locked;
        if (SDL_CursorVisible())                    return CursorMode::Normal;

        return CursorMode::Hidden;
    }

    bool Input::IsControllerPresent(int id)
    {
        return s_Controllers.find(id) != s_Controllers.end();
    }

    std::vector<int> Input::GetConnectedControllerIDs()
    {
        std::vector<int> ids;
        ids.reserve(s_Controllers.size());
        for (auto& [id, controller] : s_Controllers)
            ids.emplace_back(id);
        return ids;
    }

    const Controller* Input::GetController(int id)
    {
        if (!IsControllerPresent(id)) return nullptr;
        return &s_Controllers.at(id);
    }

    std::string_view Input::GetControllerName(int id)
    {
        if (!IsControllerPresent(id)) return {};
        return s_Controllers.at(id).Name;
    }

    bool Input::IsControllerButtonPressed(int controllerID, SDL_GamepadButton button)
    {
        if (!IsControllerPresent(controllerID)) return false;
        auto& controller = s_Controllers.at(controllerID);
        return controller.ButtonStates.find(button) != controller.ButtonStates.end()
            && controller.ButtonStates[button].State == KeyState::Pressed;
    }

    bool Input::IsControllerButtonHeld(int controllerID, SDL_GamepadButton button)
    {
        if (!IsControllerPresent(controllerID)) return false;
        auto& controller = s_Controllers.at(controllerID);
        return controller.ButtonStates.find(button) != controller.ButtonStates.end()
            && controller.ButtonStates[button].State == KeyState::Held;
    }

    bool Input::IsControllerButtonDown(int controllerID, SDL_GamepadButton button)
    {
        if (!IsControllerPresent(controllerID)) return false;
        const Controller& controller = s_Controllers.at(controllerID);
        if (controller.ButtonDown.find(button) == controller.ButtonDown.end()) return false;
        return controller.ButtonDown.at(button);
    }

    bool Input::IsControllerButtonReleased(int controllerID, SDL_GamepadButton button)
    {
        if (!IsControllerPresent(controllerID)) return true;
        auto& controller = s_Controllers.at(controllerID);
        return controller.ButtonStates.find(button) != controller.ButtonStates.end()
            && controller.ButtonStates[button].State == KeyState::Released;
    }

    float Input::GetControllerAxis(int controllerID, SDL_GamepadAxis axis)
    {
        if (!IsControllerPresent(controllerID)) return 0.0f;
        const Controller& controller = s_Controllers.at(controllerID);
        if (controller.AxisStates.find(axis) == controller.AxisStates.end()) return 0.0f;
        return controller.AxisStates.at(axis);
    }

    float Input::GetControllerDeadzone(int controllerID, SDL_GamepadAxis axis)
    {
        if (!IsControllerPresent(controllerID)) return 0.0f;
        return s_Controllers.at(controllerID).DeadZones.at(axis);
    }

    void Input::SetControllerDeadzone(int controllerID, SDL_GamepadAxis axis, float deadzone)
    {
        if (!IsControllerPresent(controllerID)) return;
        s_Controllers.at(controllerID).DeadZones[axis] = deadzone;
    }

    void Input::TransitionPressedKeys()
    {
        for (const auto& [key, keyData] : s_KeyData)
        {
            if (keyData.State == KeyState::Pressed)
                UpdateKeyState(key, KeyState::Held);
        }
    }

    void Input::TransitionPressedButtons()
    {
        for (const auto& [button, buttonData] : s_MouseData)
        {
            if (buttonData.State == KeyState::Pressed)
                UpdateButtonState(button, KeyState::Held);
        }

        for (const auto& [id, controller] : s_Controllers)
        {
            for (const auto& [button, buttonStates] : controller.ButtonStates)
            {
                if (buttonStates.State == KeyState::Pressed)
                    UpdateControllerButtonState(id, button, KeyState::Held);
            }
        }
    }

    void Input::UpdateKeyState(KeyCode key, KeyState newState)
    {
        auto& keyData = s_KeyData[key];
        keyData.Key = key;
        keyData.OldState = keyData.State;
        keyData.State = newState;
    }

    void Input::UpdateKeyMods(KeyMods mods)
    {
        s_Mods = mods;
    }

    void Input::UpdateButtonState(MouseButton button, KeyState newState)
    {
        auto& mouseData = s_MouseData[button];
        mouseData.Button = button;
        mouseData.OldState = mouseData.State;
        mouseData.State = newState;
    }

    void Input::UpdateControllerButtonState(int controllerID, SDL_GamepadButton button, KeyState newState)
    {
        auto& data = s_Controllers.at(controllerID).ButtonStates.at(button);
        data.Button = button;
        data.OldState = data.State;
        data.State = newState;
    }

    void Input::ClearReleasedKeys()
    {
        for (const auto& [key, keyData] : s_KeyData)
        {
            if (keyData.State == KeyState::Released)
                UpdateKeyState(key, KeyState::None);
        }

        for (const auto& [button, buttonData] : s_MouseData)
        {
            if (buttonData.State == KeyState::Released)
                UpdateButtonState(button, KeyState::None);
        }

        for (const auto& [id, controller] : s_Controllers)
        {
            for (const auto& [button, buttonStates] : controller.ButtonStates)
            {
                if (buttonStates.State == KeyState::Released)
                    UpdateControllerButtonState(id, button, KeyState::None);
            }
        }
    }
}