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
    static void SetClearColor(float r, float g, float b, float a);

    static void ClearMagenta();

    void Init();

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
      auto storageBuffer = s_Instance->m_CommandQueue.Allocate(renderCmd, sizeof(func));
      new (storageBuffer) FuncT(std::forward<FuncT>(func));
    }

    void WaitAndRender();

    inline static Renderer& Get() { return *s_Instance; }
  private:
    static Renderer* s_Instance;

    RenderCommandQueue m_CommandQueue;
  };

}

#define ZN_RENDER_PASTE2(a, b) a ## b
#define ZN_RENDER_PASTE(a, b) ZN_RENDER_PASTE2(a, b)
#define ZN_RENDER_UNIQUE(x) ZN_RENDER_PASTE(x, __LINE__)

#define ZN_RENDER(code) \
    struct ZN_RENDER_UNIQUE(ZNRenderCommand) \
    {\
        static void Execute(void*)\
        {\
            code\
        }\
    };\
  {\
    auto mem = RenderCommandQueue::Submit(sizeof(ZN_RENDER_UNIQUE(ZNRenderCommand)), ZN_RENDER_UNIQUE(ZNRenderCommand)::Execute);\
    new (mem) ZN_RENDER_UNIQUE(ZNRenderCommand)();\
  }\

#define ZN_RENDER_1(arg0, code) \
  do {\
    struct ZN_RENDER_UNIQUE(ZNRenderCommand) \
    {\
    ZN_RENDER_UNIQUE(ZNRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0) \
    : arg0(arg0) {}\
    \
        static void Execute(void* self)\
        {\
      auto& arg0 = ((ZN_RENDER_UNIQUE(ZNRenderCommand)*)self)->arg0;\
            code\
        }\
    \
    typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
    };\
  {\
    auto mem = ::Zenith::Renderer::Submit(ZN_RENDER_UNIQUE(ZNRenderCommand)::Execute, sizeof(ZN_RENDER_UNIQUE(ZNRenderCommand)));\
    new (mem) ZN_RENDER_UNIQUE(ZNRenderCommand)(arg0);\
  } } while(0)

#define ZN_RENDER_2(arg0, arg1, code) \
    struct ZN_RENDER_UNIQUE(ZNRenderCommand) \
    {\
    ZN_RENDER_UNIQUE(ZNRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
                      typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1) \
    : arg0(arg0), arg1(arg1) {}\
    \
        static void Execute(void* self)\
        {\
      auto& arg0 = ((ZN_RENDER_UNIQUE(ZNRenderCommand)*)self)->arg0;\
      auto& arg1 = ((ZN_RENDER_UNIQUE(ZNRenderCommand)*)self)->arg1;\
            code\
        }\
    \
    typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
    typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
    };\
  {\
    auto mem = ::Zenith::Renderer::Submit(ZN_RENDER_UNIQUE(ZNRenderCommand)::Execute, sizeof(ZN_RENDER_UNIQUE(ZNRenderCommand)));\
    new (mem) ZN_RENDER_UNIQUE(ZNRenderCommand)(arg0, arg1);\
  }\

#define ZN_RENDER_3(arg0, arg1, arg2, code) \
    struct ZN_RENDER_UNIQUE(ZNRenderCommand) \
    {\
    ZN_RENDER_UNIQUE(ZNRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
                      typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
                      typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2) \
    : arg0(arg0), arg1(arg1), arg2(arg2) {}\
    \
        static void Execute(void* self)\
        {\
      auto& arg0 = ((ZN_RENDER_UNIQUE(ZNRenderCommand)*)self)->arg0;\
      auto& arg1 = ((ZN_RENDER_UNIQUE(ZNRenderCommand)*)self)->arg1;\
      auto& arg2 = ((ZN_RENDER_UNIQUE(ZNRenderCommand)*)self)->arg2;\
            code\
        }\
    \
    typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
    typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
    typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
    };\
  {\
    auto mem = ::Zenith::Renderer::Submit(ZN_RENDER_UNIQUE(ZNRenderCommand)::Execute, sizeof(ZN_RENDER_UNIQUE(ZNRenderCommand)));\
    new (mem) ZN_RENDER_UNIQUE(ZNRenderCommand)(arg0, arg1, arg2);\
  }\

#define ZN_RENDER_4(arg0, arg1, arg2, arg3, code) \
    struct ZN_RENDER_UNIQUE(ZNRenderCommand) \
    {\
    ZN_RENDER_UNIQUE(ZNRenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
                      typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
                      typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2,\
                      typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3)\
    : arg0(arg0), arg1(arg1), arg2(arg2), arg3(arg3) {}\
    \
        static void Execute(void* self)\
        {\
      auto& arg0 = ((ZN_RENDER_UNIQUE(ZNRenderCommand)*)self)->arg0;\
      auto& arg1 = ((ZN_RENDER_UNIQUE(ZNRenderCommand)*)self)->arg1;\
      auto& arg2 = ((ZN_RENDER_UNIQUE(ZNRenderCommand)*)self)->arg2;\
      auto& arg3 = ((ZN_RENDER_UNIQUE(ZNRenderCommand)*)self)->arg3;\
            code\
        }\
    \
    typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
    typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
    typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
    typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3;\
    };\
  {\
    auto mem = Renderer::Submit(ZN_RENDER_UNIQUE(ZNRenderCommand)::Execute, sizeof(ZN_RENDER_UNIQUE(ZNRenderCommand)));\
    new (mem) ZN_RENDER_UNIQUE(ZNRenderCommand)(arg0, arg1, arg2, arg3);\
  }