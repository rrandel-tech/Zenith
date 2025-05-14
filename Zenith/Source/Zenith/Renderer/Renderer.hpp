#pragma once

#include "RenderCommandQueue.hpp"
#include "RendererAPI.hpp"

namespace Zenith {

	class Renderer
	{
	public:
		typedef void(*RenderCommandFn)(void*);

		// Commands
		static void Clear();
		static void Clear(float r, float g, float b, float a = 1.0f);

		static void DrawIndexed(uint32_t count);

		static void ClearMagenta();

		static void Init();
		static void Shutdown();

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

		static void WaitAndRender();
		static void SwapQueues();

		static uint32_t GetRenderQueueIndex();
		static uint32_t GetRenderQueueSubmissionIndex();
	private:
		static RenderCommandQueue& GetRenderCommandQueue();
	};

}
