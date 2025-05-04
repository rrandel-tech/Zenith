project "Zenith-Editor"
	kind "ConsoleApp"

	targetdir ("../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../bin-int/" .. outputdir .. "/%{prj.name}")

	links { "Zenith" }

	files  { 
		"src/**.h",
		"src/**.c",
		"src/**.hpp",
		"src/**.cpp"
	}

	includedirs  {
		"src/",

		"../Zenith/src/",
		"../Zenith/vendor/"
	}

	filter "system:windows"
		systemversion "latest"
		defines { "ZN_PLATFORM_WINDOWS" }

	filter "system:linux"
		defines { "ZN_PLATFORM_LINUX", "__EMULATE_UUID" }
		links { "dw", "dl", "unwind", "pthread" }

		result, err = os.outputof("pkg-config --libs gtk+-3.0")
		linkoptions { result }

	filter "configurations:Debug or configurations:Debug-AS"
		symbols "On"
		defines { "ZN_DEBUG" }

	filter { "system:windows", "configurations:Debug-AS" }
		sanitize { "Address" }
		flags { "NoRuntimeChecks", "NoIncrementalLink" }

	filter "configurations:Release"
		optimize "On"
		vectorextensions "AVX2"
		isaextensions { "BMI", "POPCNT", "LZCNT", "F16C" }
		defines { "ZN_RELEASE", }

	filter "configurations:Dist"
		flags { "ExcludeFromBuild" }
