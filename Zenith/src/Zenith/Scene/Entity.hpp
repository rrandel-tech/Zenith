#pragma once

#include <glm/glm.hpp>

#include "Zenith/Renderer/Mesh.hpp"

namespace Zenith {

	class Entity
	{
	public:
		~Entity();

		// TODO: Move to Component
		void SetMesh(const Ref<Mesh>& mesh) { m_Mesh = mesh; }
		Ref<Mesh> GetMesh() { return m_Mesh; }

		void SetMaterial(const Ref<MaterialInstance>& material) { m_Material = material; }
		Ref<MaterialInstance> GetMaterial() { return m_Material; }

		const glm::mat4& GetTransform() const { return m_Transform; }
		glm::mat4& Transform() { return m_Transform; }

		const std::string& GetName() const { return m_Name; }
	private:
		Entity(const std::string& name);
	private:
		std::string m_Name;
		glm::mat4 m_Transform;

		// TODO: Temp
		Ref<Mesh> m_Mesh;
		Ref<MaterialInstance> m_Material;

		friend class Scene;
	};

}