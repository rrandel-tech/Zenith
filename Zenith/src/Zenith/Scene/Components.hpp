#pragma once

#include <glm/glm.hpp>

#include "Zenith/Renderer/Texture.hpp"
#include "Zenith/Renderer/Mesh.hpp"
#include "Zenith/Renderer/Camera.hpp"

namespace Zenith {

	struct TagComponent
	{
		std::string Tag;

		operator std::string& () { return Tag; }
		operator const std::string& () const { return Tag; }
	};

	struct TransformComponent
	{
		glm::mat4 Transform;

		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct MeshComponent
	{
		Ref<Zenith::Mesh> Mesh;

		operator Ref<Zenith::Mesh>() { return Mesh; }
	};

	struct CameraComponent
	{
		//OrthographicCamera Camera;
		Zenith::Camera Camera;
		bool Primary = true;

		operator Zenith::Camera& () { return Camera; }
		operator const Zenith::Camera& () const { return Camera; }
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Ref<Texture2D> Texture;
		float TilingFactor = 1.0f;
	};


}