#pragma once

#include "RendererAPI.hpp"

namespace Zenith {

	class VertexBuffer : public RefCounted
	{
	public:
		virtual ~VertexBuffer() {}

		virtual void SetData(void* buffer, uint32_t size, uint32_t offset = 0) = 0;
		virtual void Bind() const = 0;

		virtual uint32_t GetSize() const = 0;
		virtual RendererID GetRendererID() const = 0;

		static Ref<VertexBuffer> Create(uint32_t size = 0);
	};

}
