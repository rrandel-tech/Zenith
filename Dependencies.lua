include "./vendor/premake_customization/ordered_pairs.lua"

-- Utility function for converting the first character to uppercase
function firstToUpper(str)
  return (str:gsub("^%l", string.upper))
end

Dependencies = {
  GLFW = {
    -- No need to specify LibDir for GLFW since it's automatically handled by premake
    LibName = "GLFW",
    IncludeDir = "%{wks.location}/Zenith/vendor/GLFW/include",
  },
  Glad = {
    LibName = "Glad",
    IncludeDir = "%{wks.location}/Zenith/vendor/Glad/include",
  },
  ImGui = {
    LibName = "ImGui",
    IncludeDir = "%{wks.location}/Zenith/vendor/imgui",
  },
  GLM = {
		IncludeDir = "%{wks.location}/Zenith/vendor/glm",
	},
  Assimp = {
		IncludeDir = "%{wks.location}/Zenith/vendor/assimp/include",
		Windows = { LibName = "assimp-vc143-mt", DebugLibName = "assimp-vc143-mtd", LibDir = "%{wks.location}/Zenith/vendor/assimp/bin/windows/%{cfg.buildcfg}/" },
		Linux = { LibName = "assimp", LibDir = "%{wks.location}/Zenith/vendor/assimp/bin/linux/" },
		Configurations = "Debug,Release"
	},
  STB = {
		IncludeDir = "%{wks.location}/Zenith/vendor/stb/include",
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