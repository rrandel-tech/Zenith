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
    }

    void EditorLayer::OnEvent(Event& e)
    {
    }

}