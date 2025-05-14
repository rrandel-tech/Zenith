#pragma once

#include <string>
#include <memory>
#include <filesystem>
#include <cstdint>

#ifdef ZN_PLATFORM_WINDOWS
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#include "RenderDoc/renderdoc_app.h"
#include "Zenith/Core/Log.hpp"
#include "Zenith/Core/Assert.hpp"

namespace Zenith {

	class RenderDoc {
	public:
		static RenderDoc& Get() {
			static RenderDoc instance;
			return instance;
		}

		bool Initialize() {
			if (m_IsInitialized) return true;

			pRENDERDOC_GetAPI RENDERDOC_GetAPI = nullptr;

#ifdef ZN_PLATFORM_WINDOWS
			HMODULE renderDocDLL = GetModuleHandleA("renderdoc.dll");
			if (!renderDocDLL)
				renderDocDLL = LoadLibraryA("C:/_tools/RenderDoc/renderdoc.dll");

			ZN_ASSERT(renderDocDLL && "RenderDoc: Failed to load DLL");
			if (!renderDocDLL) {
				ZN_CORE_ERROR("RenderDoc: Failed to load DLL.");
				return false;
			}

			RENDERDOC_GetAPI = reinterpret_cast<pRENDERDOC_GetAPI>(
				GetProcAddress(renderDocDLL, "RENDERDOC_GetAPI"));
#else
			void* renderDocSO = dlopen("librenderdoc.so", RTLD_NOW | RTLD_NOLOAD);
			if (!renderDocSO)
				renderDocSO = dlopen("/usr/lib/librenderdoc.so", RTLD_NOW);

			ZN_ASSERT(renderDocSO && "RenderDoc: Failed to load shared library");
			if (!renderDocSO) {
				ZN_CORE_ERROR("RenderDoc: Failed to load shared library.");
				return false;
			}

			RENDERDOC_GetAPI = reinterpret_cast<pRENDERDOC_GetAPI>(
				dlsym(renderDocSO, "RENDERDOC_GetAPI"));
#endif

			ZN_ASSERT(RENDERDOC_GetAPI && "RenderDoc: Failed to get RENDERDOC_GetAPI symbol");
			if (!RENDERDOC_GetAPI || RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_0, reinterpret_cast<void**>(&m_API)) != 1 || !m_API) {
				ZN_CORE_ERROR("RenderDoc: Failed to get API.");
				return false;
			}

			ZN_ASSERT(m_API && "RenderDoc: API pointer is null after RENDERDOC_GetAPI");
			m_IsInitialized = true;
			ZN_CORE_ERROR("RenderDoc initialized successfully.");
			return true;
		}

		void StartFrameCapture(void* windowHandle = nullptr) {
			ZN_ASSERT(IsAvailable() && "RenderDoc::StartFrameCapture() called before initialization");
			m_API->StartFrameCapture(windowHandle, windowHandle);
		}

		bool EndFrameCapture(void* windowHandle = nullptr) {
			ZN_ASSERT(IsAvailable() && "RenderDoc::EndFrameCapture() called before initialization");
			return m_API->EndFrameCapture(windowHandle, windowHandle) == 1;
		}

		bool IsAvailable() const {
			return m_IsInitialized && m_API;
		}

		void SetCaptureOption(RENDERDOC_CaptureOption option, uint32_t value) {
			ZN_ASSERT(IsAvailable() && "RenderDoc::SetCaptureOption() called before initialization");
			m_API->SetCaptureOptionU32(option, value);
		}

		void SetCaptureKeys(const int* keys, int count) {
			ZN_ASSERT(IsAvailable() && "RenderDoc::SetCaptureKeys() called before initialization");
			m_API->SetCaptureKeys(reinterpret_cast<RENDERDOC_InputButton*>(const_cast<int*>(keys)), count);
		}

		void SetCapturePath(const std::string& path) {
			ZN_ASSERT(IsAvailable() && "RenderDoc::SetCapturePath() called before initialization");
			m_API->SetCaptureFilePathTemplate(path.c_str());
		}

		void TriggerCapture() {
			ZN_ASSERT(IsAvailable() && "RenderDoc::TriggerCapture() called before initialization");
			m_API->TriggerCapture();
		}

		bool LaunchReplayUI() {
			ZN_ASSERT(IsAvailable() && "RenderDoc::LaunchReplayUI() called before initialization");
			return m_API->LaunchReplayUI(1, nullptr) == 1;
		}

	private:
		RenderDoc() : m_API(nullptr), m_IsInitialized(false) {}
		~RenderDoc() = default;

		RENDERDOC_API_1_1_0* m_API;
		bool m_IsInitialized;
	};

}
