asmjit = {
	source = path.join(dependencies.basePath, "asmjit"),
}

function asmjit.import()
	links { "asmjit" }
	asmjit.includes()
end

function asmjit.includes()
	includedirs {
		path.join(asmjit.source, "")
	}
	
	defines {
		"ASMJIT_STATIC"
	}
end

function asmjit.project()
	project "asmjit"
		language "C++"

		asmjit.includes()

		files {
			path.join(asmjit.source, "asmjit/**.cpp"),
		}

		warnings "Off"
		kind "StaticLib"
end

table.insert(dependencies, asmjit)
