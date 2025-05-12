#include "znpch.hpp"
#include "Camera.hpp"

namespace Zenith {

	Camera::Camera(const glm::mat4& projectionMatrix)
		: m_ProjectionMatrix(projectionMatrix)
	{
	}

}