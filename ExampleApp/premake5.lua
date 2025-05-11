project "ExampleApp"
	location "ExampleApp"
	kind "SharedLib"
	language "C#"

	targetdir ("%{wks.location}/Zenith-Editor/Resources/Scripts")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.cs", 
	}

	links
	{
		"Zenith-ScriptCore"
	}