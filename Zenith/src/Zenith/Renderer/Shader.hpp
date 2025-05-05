#pragma once

#include "Zenith/Core/Base.hpp"
#include "Zenith/Renderer/Renderer.hpp"

#include <string>

namespace Zenith
{
	struct ShaderUniform
	{};

	struct ShaderUniformCollection
	{};

	class Shader
	{
	public:
		virtual void Bind() = 0;


		// Represents a complete shader program stored in a single file.
		// Note: currently for simplicity this is simply a string filepath, however
		//			in the future this will be an asset object + metadata
		static Shader* Create(const std::string& filepath);
	};

}