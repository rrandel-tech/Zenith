#pragma once

#include "RenderCommandQueue.hpp"
#include "RenderPass.hpp"

#include "Mesh.hpp"

namespace Zenith {

	class ShaderLibrary;

	// TODO: Maybe this should be renamed to RendererAPI? Because we want an actual renderer vs API calls...
	class Renderer
	{
	public:
		typedef void(*RenderCommandFn)(void*);

		// Commands
		static void Clear();
		static void Clear(float r, float g, float b, float a = 1.0f);
		static void SetClearColor(float r, float g, float b, float a);

		static void DrawIndexed(uint32_t count, PrimitiveType type, bool depthTest = true);

		// For OpenGL
		static void SetLineThickness(float thickness);

		static void ClearMagenta();

		static void Init();

		static const Scope<ShaderLibrary>& GetShaderLibrary();

		template<typename FuncT>
		static void Submit(FuncT&& func)
		{
			auto renderCmd = [](void* ptr) {
				auto pFunc = (FuncT*)ptr;
				(*pFunc)();

				// NOTE: Instead of destroying we could try and enforce all items to be trivally destructible
				// however some items like uniforms which contain std::strings still exist for now
				// static_assert(std::is_trivially_destructible_v<FuncT>, "FuncT must be trivially destructible");
				pFunc->~FuncT();
				};
			auto storageBuffer = GetRenderCommandQueue().Allocate(renderCmd, sizeof(func));
			new (storageBuffer) FuncT(std::forward<FuncT>(func));
		}

		/*static void* Submit(RenderCommandFn fn, unsigned int size)
		{
			return s_Instance->m_CommandQueue.Allocate(fn, size);
		}*/

		static void WaitAndRender();

		// ~Actual~ Renderer here... TODO: remove confusion later
		static void BeginRenderPass(const Ref<RenderPass>& renderPass, bool clear = true);
		static void EndRenderPass();

		static void SubmitQuad(const Ref<MaterialInstance>& material, const glm::mat4& transform = glm::mat4(1.0f));
		static void SubmitFullscreenQuad(const Ref<MaterialInstance>& material);
		static void SubmitMesh(const Ref<Mesh>& mesh, const glm::mat4& transform, const Ref<MaterialInstance>& overrideMaterial = nullptr);

		static void DrawAABB(const AABB& aabb, const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
		static void DrawAABB(const Ref<Mesh>& mesh, const glm::mat4& transform, const glm::vec4& color = glm::vec4(1.0f));
	private:
		static RenderCommandQueue& GetRenderCommandQueue();
	};

}
