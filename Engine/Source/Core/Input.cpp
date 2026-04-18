#include "znpch.hpp"
#include "Input.hpp"

namespace Zenith {

    bool Input::IsKeyPressed(SDL_Scancode scancode)
    {
        const bool* state = SDL_GetKeyboardState(nullptr);
        return state[scancode];
    }

    bool Input::IsMouseButtonPressed(int button)
    {
        Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);
        Uint32 buttonFlag = SDL_BUTTON_MASK(button);
        return (mouseState & buttonFlag) != 0;
    }

    float Input::GetMouseX()
    {
        float x, y;
        SDL_GetMouseState(&x, &y);
        return x;
    }

    float Input::GetMouseY()
    {
        float x, y;
        SDL_GetMouseState(&x, &y);
        return y;
    }

    void Input::GetMousePosition(float& x, float& y)
    {
        SDL_GetMouseState(&x, &y);
    }

}