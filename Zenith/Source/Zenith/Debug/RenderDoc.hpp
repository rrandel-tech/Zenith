#pragma once

#include "Zenith/Core/Log.hpp"
#include "Zenith/Core/Assert.hpp"

#include <RenderDoc/renderdoc_app.h>

#ifdef ZN_PLATFORM_WINDOWS
	#include <Windows.h>
#else
	#include <dlfcn.h>
#endif

#include <string>
#include <memory>
#include <filesystem>
#include <cstdint>

#ifdef ZN_DEBUG

namespace Zenith {

	class RenderDoc
	{
	public:
		RenderDoc() : m_API(nullptr), m_IsInitialized(false) {}
		~RenderDoc() = default;

		bool Initialize()
		{
			if (m_IsInitialized) return true;

			pRENDERDOC_GetAPI RENDERDOC_GetAPI = nullptr;

#ifdef ZN_PLATFORM_WINDOWS
			HMODULE renderDocDLL = GetModuleHandleA("renderdoc.dll");
			if (!renderDocDLL)
				renderDocDLL = LoadLibraryA("C:/_tools/RenderDoc/renderdoc.dll");

			if (!renderDocDLL)
			{
				ZN_CORE_ERROR("RenderDoc: Failed to load DLL.");
				return false;
			}

			RENDERDOC_GetAPI = reinterpret_cast<pRENDERDOC_GetAPI>(
				GetProcAddress(renderDocDLL, "RENDERDOC_GetAPI"));
#else
			void* renderDocSO = dlopen("librenderdoc.so", RTLD_NOW | RTLD_NOLOAD);
			if (!renderDocSO)
				renderDocSO = dlopen("/usr/lib/librenderdoc.so", RTLD_NOW);

			if (!renderDocSO)
			{
				ZN_CORE_ERROR("RenderDoc: Failed to load shared library.");
				return false;
			}

			RENDERDOC_GetAPI = reinterpret_cast<pRENDERDOC_GetAPI>(
				dlsym(renderDocSO, "RENDERDOC_GetAPI"));
#endif

			if (!RENDERDOC_GetAPI || RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_0, reinterpret_cast<void**>(&m_API)) != 1 || !m_API)
			{
				ZN_CORE_ERROR("RenderDoc: Failed to get API.");
				return false;
			}

			m_IsInitialized = true;
			ZN_CORE_INFO("RenderDoc initialized successfully.");
			return true;
		}

		void StartFrameCapture(void* windowHandle = nullptr)
		{
			if (IsAvailable())
				m_API->StartFrameCapture(windowHandle, windowHandle);
		}

		bool EndFrameCapture(void* windowHandle = nullptr)
		{
			return IsAvailable() && m_API->EndFrameCapture(windowHandle, windowHandle) == 1;
		}

		bool IsAvailable() const
		{
			return m_IsInitialized && m_API;
		}

		void SetCaptureOption(RENDERDOC_CaptureOption option, uint32_t value)
		{
			if (IsAvailable())
				m_API->SetCaptureOptionU32(option, value);
		}

		void SetCaptureKeys(const int* keys, int count)
		{
			if (IsAvailable())
				m_API->SetCaptureKeys(reinterpret_cast<RENDERDOC_InputButton*>(const_cast<int*>(keys)), count);
		}

		void SetCaptureName(const std::string& name)
		{
			if (IsAvailable())
			{
				std::string fullPath = "Captures/Zenith_" + name + "_";
				m_API->SetCaptureFilePathTemplate(fullPath.c_str());
			}
		}

		void TriggerCapture()
		{
			if (IsAvailable())
				m_API->TriggerCapture();
		}

		bool LaunchReplayUI()
		{
			return IsAvailable() && m_API->LaunchReplayUI(1, nullptr) == 1;
		}

	private:
		RENDERDOC_API_1_1_0* m_API;
		bool m_IsInitialized;
	};

}

#else

namespace Zenith {
	class RenderDoc
	{
	public:
		bool Initialize() { return false; }
		void SetCapturePath(const std::string&) {}
		void SetCaptureKeys(const int*, int) {}
		void TriggerCapture() {}
		bool IsAvailable() const { return false; }
	};
}

#endif
