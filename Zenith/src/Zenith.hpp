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

#include "Zenith/Core/Events/Event.hpp"
#include "Zenith/Core/Events/ApplicationEvent.hpp"
#include "Zenith/Core/Events/KeyEvent.hpp"
#include "Zenith/Core/Events/MouseEvent.hpp"

#include "imgui.h"

// === Zenith Renderer API ===========================
#include "Zenith/Renderer/Renderer.hpp"
#include "Zenith/Renderer/Framebuffer.hpp"
#include "Zenith/Renderer/Buffer.hpp"
#include "Zenith/Renderer/Texture.hpp"
#include "Zenith/Renderer/Shader.hpp"
#include "Zenith/Renderer/Mesh.hpp"
#include "Zenith/Renderer/Camera.hpp"
#include "Zenith/Renderer/Material.hpp"
// ===================================================