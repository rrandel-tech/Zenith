#include "znpch.hpp"
#include "Renderer.hpp"

#include <glad/glad.h>

#include "RendererAPI.hpp"

namespace Zenith {

	static RendererAPI* s_RendererAPI = nullptr;

	void RendererAPI::SetAPI(RendererAPIType api)
	{
		// TODO: make sure this is called at a valid time
		ZN_CORE_ASSERT(api == RendererAPIType::OpenGL, "OpenGL is currently the only supported Renderer API");
		s_CurrentRendererAPI = api;
	}

	constexpr static uint32_t s_RenderCommandQueueCount = 2;
	static RenderCommandQueue* s_CommandQueue[s_RenderCommandQueueCount];
	static std::atomic<uint32_t> s_RenderCommandQueueSubmissionIndex = 0;

	void Renderer::Init()
	{
		s_CommandQueue[0] = znew RenderCommandQueue();
		s_CommandQueue[1] = znew RenderCommandQueue();

		Renderer::Submit([]() { RendererAPI::Init(); });
	}

	void Renderer::Shutdown()
	{
		delete s_CommandQueue[0];
		delete s_CommandQueue[1];
	}

	void Renderer::Clear()
	{
		Renderer::Submit([]() {
			RendererAPI::Clear(0.0f, 0.0f, 0.0f, 1.0f);
		});
	}

	void Renderer::Clear(float r, float g, float b, float a)
	{
		Renderer::Submit([=]() {
			RendererAPI::Clear(r, g, b, a);
		});
	}

	void Renderer::ClearMagenta()
	{
		Clear(1, 0, 1);
	}

	void Renderer::DrawIndexed(uint32_t count)
	{
		Renderer::Submit([=]() {
			RendererAPI::DrawIndexed(count);
		});
	}

	void Renderer::WaitAndRender()
	{
		s_CommandQueue[GetRenderQueueIndex()]->Execute();
	}

	void Renderer::SwapQueues()
	{
		s_RenderCommandQueueSubmissionIndex = (s_RenderCommandQueueSubmissionIndex + 1) % s_RenderCommandQueueCount;
	}

	uint32_t Renderer::GetRenderQueueIndex()
	{
		return (s_RenderCommandQueueSubmissionIndex + 1) % s_RenderCommandQueueCount;
	}

	uint32_t Renderer::GetRenderQueueSubmissionIndex()
	{
		return s_RenderCommandQueueSubmissionIndex;
	}

	RenderCommandQueue& Renderer::GetRenderCommandQueue()
	{
		return *s_CommandQueue[s_RenderCommandQueueSubmissionIndex];
	}

}
