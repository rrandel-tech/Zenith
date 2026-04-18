#include "znpch.hpp"
#include "Base.hpp"

#include "Log.hpp"

namespace Zenith {

    void InitializeCore()
    {
        Log::Init();

        ZN_CORE_TRACE_TAG("Core", "Zenith Engine {}", ZN_VERSION);
        ZN_CORE_TRACE_TAG("Core", "Initializing...");
    }

    void ShutdownCore()
    {
        ZN_CORE_TRACE_TAG("Core", "Shutting down...");
        Log::Shutdown();
    }

}