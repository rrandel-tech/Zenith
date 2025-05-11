#include "znpch.hpp"
#include "ScriptEngineRegistry.hpp"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Zenith/Scene/Entity.hpp"
#include "ScriptWrappers.hpp"
#include <iostream>

namespace Zenith {

	std::unordered_map<MonoType*, std::function<bool(Entity&)>> s_HasComponentFuncs;
	std::unordered_map<MonoType*, std::function<void(Entity&)>> s_CreateComponentFuncs;

	extern MonoImage* s_CoreAssemblyImage;

#define Component_RegisterType(Type) \
	{\
		MonoType* type = mono_reflection_type_from_name((char*)"Zenith." #Type, s_CoreAssemblyImage);\
		if (type) {\
			uint32_t id = mono_type_get_type(type);\
			s_HasComponentFuncs[type] = [](Entity& entity) { return entity.HasComponent<Type>(); };\
			s_CreateComponentFuncs[type] = [](Entity& entity) { entity.AddComponent<Type>(); };\
		} else {\
			ZN_CORE_ERROR("No C# component class found for " #Type "!");\
		}\
	}

	static void InitComponentTypes()
	{
		Component_RegisterType(TagComponent);
		Component_RegisterType(TransformComponent);
		Component_RegisterType(MeshComponent);
		Component_RegisterType(ScriptComponent);
		Component_RegisterType(CameraComponent);
		Component_RegisterType(SpriteRendererComponent);
		Component_RegisterType(RigidBody2DComponent);
		Component_RegisterType(BoxCollider2DComponent);
	}

	void ScriptEngineRegistry::RegisterAll()
	{
		InitComponentTypes();

		mono_add_internal_call("Zenith.Noise::PerlinNoise_Native", Zenith::Script::Zenith_Noise_PerlinNoise);

		mono_add_internal_call("Zenith.Entity::GetTransform_Native", Zenith::Script::Zenith_Entity_GetTransform);
		mono_add_internal_call("Zenith.Entity::SetTransform_Native", Zenith::Script::Zenith_Entity_SetTransform);
		mono_add_internal_call("Zenith.Entity::CreateComponent_Native", Zenith::Script::Zenith_Entity_CreateComponent);
		mono_add_internal_call("Zenith.Entity::HasComponent_Native", Zenith::Script::Zenith_Entity_HasComponent);
		
		mono_add_internal_call("Zenith.MeshComponent::GetMesh_Native", Zenith::Script::Zenith_MeshComponent_GetMesh);
		mono_add_internal_call("Zenith.MeshComponent::SetMesh_Native", Zenith::Script::Zenith_MeshComponent_SetMesh);

		mono_add_internal_call("Zenith.RigidBody2DComponent::ApplyLinearImpulse_Native", Zenith::Script::Zenith_RigidBody2DComponent_ApplyLinearImpulse);

		mono_add_internal_call("Zenith.Input::IsKeyPressed_Native", Zenith::Script::Zenith_Input_IsKeyPressed);

		mono_add_internal_call("Zenith.Texture2D::Constructor_Native", Zenith::Script::Zenith_Texture2D_Constructor);
		mono_add_internal_call("Zenith.Texture2D::Destructor_Native", Zenith::Script::Zenith_Texture2D_Destructor);
		mono_add_internal_call("Zenith.Texture2D::SetData_Native", Zenith::Script::Zenith_Texture2D_SetData);

		mono_add_internal_call("Zenith.Material::Destructor_Native", Zenith::Script::Zenith_Material_Destructor);
		mono_add_internal_call("Zenith.Material::SetFloat_Native", Zenith::Script::Zenith_Material_SetFloat);
		mono_add_internal_call("Zenith.Material::SetTexture_Native", Zenith::Script::Zenith_Material_SetTexture);

		mono_add_internal_call("Zenith.MaterialInstance::Destructor_Native", Zenith::Script::Zenith_MaterialInstance_Destructor);
		mono_add_internal_call("Zenith.MaterialInstance::SetFloat_Native", Zenith::Script::Zenith_MaterialInstance_SetFloat);
		mono_add_internal_call("Zenith.MaterialInstance::SetVector3_Native", Zenith::Script::Zenith_MaterialInstance_SetVector3);
		mono_add_internal_call("Zenith.MaterialInstance::SetVector4_Native", Zenith::Script::Zenith_MaterialInstance_SetVector4);
		mono_add_internal_call("Zenith.MaterialInstance::SetTexture_Native", Zenith::Script::Zenith_MaterialInstance_SetTexture);

		mono_add_internal_call("Zenith.Mesh::Constructor_Native", Zenith::Script::Zenith_Mesh_Constructor);
		mono_add_internal_call("Zenith.Mesh::Destructor_Native", Zenith::Script::Zenith_Mesh_Destructor);
		mono_add_internal_call("Zenith.Mesh::GetMaterial_Native", Zenith::Script::Zenith_Mesh_GetMaterial);
		mono_add_internal_call("Zenith.Mesh::GetMaterialByIndex_Native", Zenith::Script::Zenith_Mesh_GetMaterialByIndex);
		mono_add_internal_call("Zenith.Mesh::GetMaterialCount_Native", Zenith::Script::Zenith_Mesh_GetMaterialCount);

		mono_add_internal_call("Zenith.MeshFactory::CreatePlane_Native", Zenith::Script::Zenith_MeshFactory_CreatePlane);

		// static bool IsKeyPressed(KeyCode key) { return s_Instance->IsKeyPressedImpl(key); }
		// 
		// static bool IsMouseButtonPressed(MouseCode button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		// static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
		// static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		// static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
	}

}