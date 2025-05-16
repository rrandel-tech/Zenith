include "./vendor/premake_customization/ordered_pairs.lua"

-- Utility function for converting the first character to uppercase
function firstToUpper(str)
	return (str:gsub("^%l", string.upper))
end

VULKAN_SDK = os.getenv("VULKAN_SDK")

Dependencies = {
	-- Platform-Specific System Libraries
	WS2 = {
		Windows = { LibName = "ws2_32" },
	},
	Dbghelp = {
		Windows = { LibName = "Dbghelp" },
	},

	-- Graphics & Rendering
	Vulkan = {
		Windows = {
			LibName = "vulkan-1",
			IncludeDir = "%{VULKAN_SDK}/Include/",
			LibDir = "%{VULKAN_SDK}/Lib/",
		},
		Linux = {
			LibName = "vulkan",
			IncludeDir = "%{VULKAN_SDK}/include/",
			LibDir = "%{VULKAN_SDK}/lib/",
		},
	},

	GLFW = {
		Windows = {
			LibName = "GLFW",
			IncludeDir = "%{wks.location}/Zenith/vendor/GLFW/include"
		},
		Linux = {
			LibName = "glfw"
		},
	},

	Glad = {
		LibName = "Glad",
		IncludeDir = "%{wks.location}/Zenith/vendor/Glad/include"
	},

	ImGui = {
		LibName = "ImGui",
		IncludeDir = "%{wks.location}/Zenith/vendor/imgui",
	},

	-- Math & ECS
	GLM = {
		IncludeDir = "%{wks.location}/Zenith/vendor/glm",
	},

	EnTT = {
		IncludeDir = "%{wks.location}/Zenith/vendor/entt/include",
	},

	-- Tools & Utility Libs
	FastNoise = {
		IncludeDir = "%{wks.location}/Zenith/vendor/FastNoise",
	},

	JoltPhysics = {
		LibName = "JoltPhysics",
		IncludeDir = "%{wks.location}/Zenith/vendor/JoltPhysics/JoltPhysics",
	},

	YAML_CPP = {
		IncludeDir = "%{wks.location}/Zenith/vendor/yaml-cpp/include",
	},

	JSON = {
		IncludeDir = "%{wks.location}/Zenith/vendor/json/include"
	},

	Tracy = {
		LibName = "Tracy",
		IncludeDir = "%{wks.location}/Zenith/vendor/tracy/tracy/public",
	},

	STB = {
		IncludeDir = "%{wks.location}/Zenith/vendor/stb/include",
	},

	BackwardCPP = {
		IncludeDir = "%{wks.location}/Zenith/vendor/backward-cpp"
	},

	NFDExtended = {
		LibName = "NFD-Extended",
		IncludeDir = "%{wks.location}/Zenith/vendor/NFD-Extended/NFD-Extended/src/include"
	},

	-- Asset Loading
	Assimp = {
		IncludeDir = "%{wks.location}/Zenith/vendor/assimp/include",
		Windows = {
			LibName = "assimp-vc143-mt",
			DebugLibName = "assimp-vc143-mtd",
			LibDir = "%{wks.location}/Zenith/vendor/assimp/bin/windows/%{cfg.buildcfg}/",
		},
		Linux = {
			LibName = "assimp",
			LibDir = "%{wks.location}/Zenith/vendor/assimp/bin/linux/",
		},
		Configurations = "Debug,Release"
	},

	-- SPIR-V Cross Compilation
	SPIRVCrossCore = {
		LibName = "spirv-cross-core",
		Windows = {
			DebugLibName = "spirv-cross-cored",
		},
		Configurations = "Debug,Release"
	},

	SPIRVCrossGLSL = {
		LibName = "spirv-cross-glsl",
		Windows = {
			DebugLibName = "spirv-cross-glsld",
		},
		Configurations = "Debug,Release"
	},

	SPIRVTools = {
		LibName = "SPIRV-Tools",
		Windows = {
			DebugLibName = "SPIRV-Toolsd",
		},
		Configurations = "Debug,Release"
	},
}

function LinkDependency(table, is_debug, target)

	-- Setup library directory
	if table.LibDir ~= nil then
		libdirs { table.LibDir }
	end

	-- Try linking
	local libraryName = nil
	if table.LibName ~= nil then
		libraryName = table.LibName
	end

	if table.DebugLibName ~= nil and is_debug and target == "Windows" then
		libraryName = table.DebugLibName
	end

	if libraryName ~= nil then
		links { libraryName }
		return true
	end

	return false
end

function AddDependencyIncludes(table)
	if table.IncludeDir ~= nil then
		externalincludedirs { table.IncludeDir }
	end
end

function ProcessDependencies(config_name)
	local target = firstToUpper(os.target())

	for key, libraryData in orderedPairs(Dependencies) do

		-- Always match config_name if no Configurations list is specified
		local matchesConfiguration = true

		if config_name ~= nil and libraryData.Configurations ~= nil then
			matchesConfiguration = string.find(libraryData.Configurations, config_name)
		end

		local isDebug = config_name == "Debug"

		if matchesConfiguration then
			local continueLink = true

			-- Process Platform Scope
			if libraryData[target] ~= nil then
				continueLink = not LinkDependency(libraryData[target], isDebug, target)
				AddDependencyIncludes(libraryData[target])
			end

			-- Process Global Scope
			if continueLink then
				LinkDependency(libraryData, isDebug, target)
			end

			AddDependencyIncludes(libraryData)
		end

	end
end

function IncludeDependencies(config_name)
	local target = firstToUpper(os.target())

	for key, libraryData in orderedPairs(Dependencies) do

		-- Always match config_name if no Configurations list is specified
		local matchesConfiguration = true

		if config_name ~= nil and libraryData.Configurations ~= nil then
			matchesConfiguration = string.find(libraryData.Configurations, config_name)
		end

		if matchesConfiguration then
			-- Process Global Scope
			AddDependencyIncludes(libraryData)

			-- Process Platform Scope
			if libraryData[target] ~= nil then
				AddDependencyIncludes(libraryData[target])
			end
		end

	end
end
