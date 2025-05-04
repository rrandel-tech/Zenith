workspace "Zenith"
	configurations { "Debug", "Debug-AS", "Release", "Dist" }
	startproject "Zenith-Editor"
		conformancemode "On"

	language "C++"
	cppdialect "C++20"
	staticruntime "Off"

	flags { "MultiProcessorCompile" }

	defines {
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX",
		"SPDLOG_USE_STD_FORMAT",
		"_SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING",
	}

	filter "action:vs*"
		linkoptions { "/ignore:4099" }
		disablewarnings { "4068" }

	filter "language:C++ or language:C"
		architecture "x86_64"

	filter "configurations:Debug or configurations:Debug-AS"
		optimize "Off"
		symbols "On"

	filter { "system:windows", "configurations:Debug-AS" }	
		sanitize { "Address" }
		flags { "NoRuntimeChecks", "NoIncrementalLink" }

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"

	filter "system:windows"
		buildoptions { "/utf-8", "/EHsc", "/Zc:preprocessor", "/Zc:__cplusplus" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Core"
include "Zenith"
group ""

group "Misc"
include "Sandbox"
group ""