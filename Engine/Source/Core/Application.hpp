#pragma once

#include "Core/Base.hpp"

#include <string>
#include <memory>

namespace Zenith {

    struct ApplicationSpecification
    {
        std::string name = "Zenith";
        uint32_t windowWidth = 1600, windowHeight = 900;
    };

    class Application
    {
    public:
        explicit Application(const ApplicationSpecification& specification);
        virtual ~Application();

        void Run();
        void Close();

        virtual void OnInit() {}

        void OnShutdown();

        const ApplicationSpecification& GetSpecification() const { return m_specification; }

        static const char* GetConfigurationName();
        static const char* GetPlatformName();
    private:
        ApplicationSpecification m_specification;

        bool m_isRunning = true;
        bool m_isMinimized = false;
    };

    // Implemented by CLIENT
    Application* CreateApplication(int argc, char** argv);
}