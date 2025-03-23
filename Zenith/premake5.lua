project "Zenith"
  kind "StaticLib"

  targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
  objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

  pchheader "znpch.hpp"
  pchsource "src/znpch.cpp"

  files {
    "src/**.h",
    "src/**.c",
    "src/**.hpp",
    "src/**.cpp"
  }

  includedirs { "src/", "vendor/" }

  IncludeDependencies()

  defines { "GLM_FORCE_DEPTH_ZERO_TO_ONE" }

  filter "system:windows"
    systemversion "latest"
    defines { "ZN_PLATFORM_WINDOWS" }

  filter "system:linux"
    defines { "ZN_PLATFORM_LINUX", "__EMULATE_UUID" }
    links { "dw", "dl", "unwind", "pthread" }

  filter "configurations:Debug or configurations:Debug-AS"
    symbols "On"
    defines { "ZN_DEBUG", "_DEBUG" }

  filter { "system:windows", "configurations:Debug-AS" }	
    sanitize { "Address" }
    flags { "NoRuntimeChecks", "NoIncrementalLink" }

  filter "configurations:Release"
    optimize "On"
    vectorextensions "AVX2"
    isaextensions { "BMI", "POPCNT", "LZCNT", "F16C" }
    defines { "ZN_RELEASE", "NDEBUG" }

  filter "configurations:Dist"
    optimize "On"
    symbols "Off"
    vectorextensions "AVX2"
    isaextensions { "BMI", "POPCNT", "LZCNT", "F16C" }
    defines { "ZN_DIST" }