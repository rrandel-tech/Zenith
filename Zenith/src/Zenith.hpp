//==============================================================================
// Zenith Engine - Client Application Header
// 
// This header is meant to be included **only** in client applications using the Zenith Engine.
// 
// **DO NOT** include this file in the engine's internal codebase.
//
// Including this file provides access to core engine functionality.
//
//==============================================================================

#pragma once

#include "Zenith/Core/Application.hpp"
#include "Zenith/Core/Log.hpp"
#include "Zenith/Core/TimeStep.hpp"
#include "Zenith/Core/Input.hpp"
#include "Zenith/Core/Timer.hpp"

#include "Zenith/Core/Events/Event.hpp"
#include "Zenith/Core/Events/ApplicationEvent.hpp"
#include "Zenith/Core/Events/KeyEvent.hpp"
#include "Zenith/Core/Events/MouseEvent.hpp"

#include "Zenith/Core/Math/AABB.hpp"
#include "Zenith/Core/Math/Ray.hpp"

#include <imgui/imgui.h>

#include "Zenith/Renderer/Renderer.hpp"
#include "Zenith/Renderer/SceneRenderer.hpp"
#include "Zenith/Renderer/RenderPass.hpp"
#include "Zenith/Renderer/Framebuffer.hpp"
#include "Zenith/Renderer/Buffer.hpp"
#include "Zenith/Renderer/VertexArray.hpp"
#include "Zenith/Renderer/Texture.hpp"
#include "Zenith/Renderer/Shader.hpp"
#include "Zenith/Renderer/Mesh.hpp"
#include "Zenith/Renderer/Camera.hpp"
#include "Zenith/Renderer/Material.hpp"

#include "Zenith/Scene/Entity.hpp"
#include "Zenith/Scene/Scene.hpp"
#include "Zenith/Scene/Components.hpp"