#pragma once

#include <SDL3/SDL.h>

namespace Zenith {

    class Input
    {
    public:
        Input() = delete;

        static bool IsKeyPressed(SDL_Scancode scancode);
        static bool IsMouseButtonPressed(int button);

        static float GetMouseX();
        static float GetMouseY();
        static void GetMousePosition(float& x, float& y);
    };

}