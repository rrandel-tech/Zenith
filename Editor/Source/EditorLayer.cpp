#include "EditorLayer.hpp"

namespace Zenith {

    EditorLayer::EditorLayer()
    {
    }

    EditorLayer::~EditorLayer()
    {
    }

    void EditorLayer::OnAttach()
    {
        ZN_INFO("Hello from editor!");
    }

    void EditorLayer::OnDetach()
    {
    }

    void EditorLayer::OnUpdate(Timestep ts)
    {
        if (Input::IsKeyPressed(SDL_SCANCODE_W))
            ZN_INFO("W key pressed from app!");
    }

    void EditorLayer::OnEvent(Event& e)
    {
    }

}