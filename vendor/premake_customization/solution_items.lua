require("vstudio")

premake.api.register {
	name = "solution_items",
	scope = "workspace",
	kind = "list:string",
}

premake.override(premake.vstudio.sln2005, "projects", function(base, wks)
	if wks.solution_items and #wks.solution_items > 0 then
		local solutionFolderGUID = "{2150E333-8FDC-42A3-9474-1A3956D46DE8}" -- GUID for "Solution Folder" type
		local projectUUID = "{" .. os.uuid("Solution Items:" .. wks.name) .. "}"

		premake.push(string.format('Project("%s") = "Solution Items", "Solution Items", "%s"', solutionFolderGUID, projectUUID))
		premake.push("ProjectSection(SolutionItems) = preProject")

		for _, path in ipairs(wks.solution_items) do
			premake.w('%s = %s', path, path)
		end

		premake.pop("EndProjectSection")
		premake.pop("EndProject")
	end

	base(wks)
end)
