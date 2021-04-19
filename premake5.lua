gitVersioningCommand = "git describe --tags --dirty --always"
gitCurrentBranchCommand = "git symbolic-ref -q --short HEAD"

-- Quote the given string input as a C string
function cstrquote(value)
	if value == nil then
		return "\"\""
	end
	result = value:gsub("\\", "\\\\")
	result = result:gsub("\"", "\\\"")
	result = result:gsub("\n", "\\n")
	result = result:gsub("\t", "\\t")
	result = result:gsub("\r", "\\r")
	result = result:gsub("\a", "\\a")
	result = result:gsub("\b", "\\b")
	result = "\"" .. result .. "\""
	return result
end

-- Converts tags in "vX.X.X" format and given revision number Y to an array of numbers {X,X,X,Y}.
-- In the case where the format does not work fall back to padding with zeroes and just ending with the revision number.
-- partscount can be either 3 or 4.
function vertonumarr(value, vernumber, partscount)
	vernum = {}
	for num in string.gmatch(value or "", "%d+") do
		if #vernum < 3 then
			table.insert(vernum, tonumber(num))
		end
	end
	while #vernum < 3 do
		table.insert(vernum, 0)
	end
	if #vernum < partscount then
		table.insert(vernum, tonumber(vernumber))
	end
	return vernum
end

dependencies = {
	basePath = "./deps"
}

function dependencies.load()
	dir = path.join(dependencies.basePath, "premake/*.lua")
	deps = os.matchfiles(dir)

	for i, dep in pairs(deps) do
		dep = dep:gsub(".lua", "")
		require(dep)
	end
end

function dependencies.imports()
	for i, proj in pairs(dependencies) do
		if type(i) == 'number' then
			proj.import()
		end
	end
end

function dependencies.projects()
	for i, proj in pairs(dependencies) do
		if type(i) == 'number' then
			proj.project()
		end
	end
end

dependencies.load()

workspace "h2-mod"
	location "./build"
	objdir "%{wks.location}/obj/%{cfg.buildcfg}"
	targetdir "%{wks.location}/bin/%{cfg.buildcfg}"
	targetname "%{prj.name}"

	language "C++"

	architecture "x64"
	platforms "x64"

	buildoptions "/std:c++latest"
	systemversion "latest"

	flags
	{
		"NoIncrementalLink",
		"MultiProcessorCompile",
	}

	configurations { "Debug", "Release", }

	symbols "On"
	
	configuration "Release"
		optimize "Full"
		defines { "NDEBUG" }
	configuration{}

	configuration "Debug"
		optimize "Debug"
		defines { "DEBUG", "_DEBUG" }
	configuration {}

	startproject "h2-mod"

    project "h2-mod"
        kind "SharedLib"
        language "C++"

        pchheader "stdinc.hpp"
		pchsource "src/stdinc.cpp"

        includedirs { "src" }
        
        files { "src/**.h", "src/**.hpp", "src/**.cpp" }
		
		dependencies.imports()
	
	group "Dependencies"
	dependencies.projects()