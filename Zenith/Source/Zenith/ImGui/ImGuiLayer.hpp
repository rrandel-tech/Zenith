#pragma once

#include "znpch.hpp"
#include "Zenith/Core/Layer.hpp"

namespace Zenith {

	class ImGuiLayer : public Layer
	{
	public:
		virtual void Begin() = 0;
		virtual void End() = 0;

		virtual bool IsInputEnabled() = 0;
		virtual void SetInputEnabled(bool enabled) = 0;

		static ImGuiLayer* Create();
	};

}
