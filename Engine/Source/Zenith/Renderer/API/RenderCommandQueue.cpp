#include "RenderCommandQueue.hpp"
#include "znpch.hpp"

#define ZN_RENDER_TRACE(...) ZN_CORE_TRACE(__VA_ARGS__)

namespace Zenith {

RenderCommandQueue::RenderCommandQueue() {
  m_CommandBuffer = znew uint8_t[10 * 1024 * 1024]; // 10mb buffer
  m_CommandBufferPtr = m_CommandBuffer;
  memset(m_CommandBuffer, 0, 10 * 1024 * 1024);
}

RenderCommandQueue::~RenderCommandQueue() { delete[] m_CommandBuffer; }

void *RenderCommandQueue::Allocate(RenderCommandFn fn, uint32_t size) {
  *(RenderCommandFn *)m_CommandBufferPtr = fn;
  m_CommandBufferPtr += alignof(RenderCommandFn);

  *(uint32_t *)m_CommandBufferPtr = size;
  m_CommandBufferPtr += RoundUp(sizeof(uint32_t), alignof(RenderCommandFn));

  void *memory = m_CommandBufferPtr;
  m_CommandBufferPtr += RoundUp<size_t>(size, alignof(RenderCommandFn));

  m_CommandCount++;
  return memory;
}

void RenderCommandQueue::Execute() {
  // ZN_RENDER_TRACE("RenderCommandQueue::Execute -- {0} commands, {1} bytes",
  // m_CommandCount, (m_CommandBufferPtr - m_CommandBuffer));

  byte *buffer = m_CommandBuffer;

  for (uint32_t i = 0; i < m_CommandCount; i++) {
    RenderCommandFn function = *(RenderCommandFn *)buffer;
    buffer += sizeof(RenderCommandFn);

    uint32_t size = *(uint32_t *)buffer;
    buffer += RoundUp(sizeof(uint32_t), alignof(RenderCommandFn));

    function(buffer);
    buffer += RoundUp<size_t>(size, alignof(RenderCommandFn));
  }

  m_CommandBufferPtr = m_CommandBuffer;
  m_CommandCount = 0;
}

}
