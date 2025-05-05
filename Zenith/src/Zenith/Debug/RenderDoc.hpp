#pragma once

#include <string>
#include <memory>
#include <filesystem>
#include <cstdint>

#include <Windows.h>

#include "RenderDoc/renderdoc_app.h"
#include "Zenith/Core/Log.hpp"

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

			HMODULE renderDocDLL = GetModuleHandleA("renderdoc.dll");
			if (!renderDocDLL)
				renderDocDLL = LoadLibraryA("C:/_tools/RenderDoc/renderdoc.dll");

			if (!renderDocDLL) {
				ZN_CORE_ERROR("RenderDoc: Failed to load DLL.");
				return false;
			}

			RENDERDOC_GetAPI = reinterpret_cast<pRENDERDOC_GetAPI>(
				GetProcAddress(renderDocDLL, "RENDERDOC_GetAPI"));

			if (!RENDERDOC_GetAPI || RENDERDOC_GetAPI(eRENDERDOC_API_Version_1_1_0, reinterpret_cast<void**>(&m_API)) != 1 || !m_API) {
				ZN_CORE_ERROR("RenderDoc: Failed to get API.");
				return false;
			}

			m_IsInitialized = true;
			ZN_CORE_INFO("RenderDoc initialized successfully.");
			return true;
		}

		void StartFrameCapture(void* windowHandle = nullptr) {
			if (!IsAvailable()) {
				ZN_CORE_WARN("RenderDoc: Not initialized. Cannot start capture.");
				return;
			}
			m_API->StartFrameCapture(windowHandle, windowHandle);
		}

		bool EndFrameCapture(void* windowHandle = nullptr) {
			if (!IsAvailable()) {
				ZN_CORE_WARN("RenderDoc: Not initialized. Cannot end capture.");
				return false;
			}
			return m_API->EndFrameCapture(windowHandle, windowHandle) == 1;
		}

		bool IsAvailable() const {
			return m_IsInitialized && m_API;
		}

		void SetCaptureOption(RENDERDOC_CaptureOption option, uint32_t value) {
			if (IsAvailable()) {
				m_API->SetCaptureOptionU32(option, value);
			}
		}

		void SetCaptureKeys(const int* keys, int count) {
			if (IsAvailable()) {
				m_API->SetCaptureKeys(reinterpret_cast<RENDERDOC_InputButton*>(const_cast<int*>(keys)), count);
			}
		}

		void SetCapturePath(const std::string& path) {
			if (IsAvailable()) {
				m_API->SetCaptureFilePathTemplate(path.c_str());
			}
		}

		void TriggerCapture() {
			if (IsAvailable()) {
				m_API->TriggerCapture();
			}
		}

		bool LaunchReplayUI() {
			if (!IsAvailable()) return false;
			return m_API->LaunchReplayUI(1, nullptr) == 1;
		}

	private:
		RenderDoc() : m_API(nullptr), m_IsInitialized(false) {}
		~RenderDoc() = default;

		RENDERDOC_API_1_1_0* m_API;
		bool m_IsInitialized;
	};

}