#pragma once

#include "Zenith/Script/ScriptEngine.hpp"
#include "Zenith/Core/KeyCodes.hpp"

#include <glm/glm.hpp>

extern "C" {
	typedef struct _MonoString MonoString;
	typedef struct _MonoArray MonoArray;
}

namespace Zenith { namespace Script {

	// Math
	float Zenith_Noise_PerlinNoise(float x, float y);

	// Input
	bool Zenith_Input_IsKeyPressed(KeyCode key);

	// Entity
	void Zenith_Entity_GetTransform(uint64_t entityID, glm::mat4* outTransform);
	void Zenith_Entity_SetTransform(uint64_t entityID, glm::mat4* inTransform);
	void Zenith_Entity_CreateComponent(uint64_t entityID, void* type);
	bool Zenith_Entity_HasComponent(uint64_t entityID, void* type);

	void* Zenith_MeshComponent_GetMesh(uint64_t entityID);
	void Zenith_MeshComponent_SetMesh(uint64_t entityID, Ref<Mesh>* inMesh);

	// Renderer
	// Texture2D
	void* Zenith_Texture2D_Constructor(uint32_t width, uint32_t height);
	void Zenith_Texture2D_Destructor(Ref<Texture2D>* _this);
	void Zenith_Texture2D_SetData(Ref<Texture2D>* _this, MonoArray* inData, int32_t count);

	// Material
	void Zenith_Material_Destructor(Ref<Material>* _this);
	void Zenith_Material_SetFloat(Ref<Material>* _this, MonoString* uniform, float value);
	void Zenith_Material_SetTexture(Ref<Material>* _this, MonoString* uniform, Ref<Texture2D>* texture);

	void Zenith_MaterialInstance_Destructor(Ref<MaterialInstance>* _this);
	void Zenith_MaterialInstance_SetFloat(Ref<MaterialInstance>* _this, MonoString* uniform, float value);
	void Zenith_MaterialInstance_SetVector3(Ref<MaterialInstance>* _this, MonoString* uniform, glm::vec3* value);
	void Zenith_MaterialInstance_SetTexture(Ref<MaterialInstance>* _this, MonoString* uniform, Ref<Texture2D>* texture);

	// Mesh
	Ref<Mesh>* Zenith_Mesh_Constructor(MonoString* filepath);
	void Zenith_Mesh_Destructor(Ref<Mesh>* _this);
	Ref<Material>* Zenith_Mesh_GetMaterial(Ref<Mesh>* inMesh);
	Ref<MaterialInstance>* Zenith_Mesh_GetMaterialByIndex(Ref<Mesh>* inMesh, int index);
	int Zenith_Mesh_GetMaterialCount(Ref<Mesh>* inMesh);

	void* Zenith_MeshFactory_CreatePlane(float width, float height);
} }