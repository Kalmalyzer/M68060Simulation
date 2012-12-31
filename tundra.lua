
local common = {
	Env = {
		M68KMAKE = "$(OBJECTDIR)$(SEP)m68kmake$(PROGSUFFIX)",
	},
}
 
local win32 = {
	Inherit = common,
	Env = {
 		--GENERATE_PDB = "1",
		CCOPTS = {
			"/D_CRT_SECURE_NO_WARNINGS", -- Disable warning about using "fopen" instead of "fopen_s", etc
			"/W3",	-- Warning level 3
			"/WX",	-- Treat all warnings as errors
			"/wd4127",	-- Disable warning C4127: conditional expression is constant
			"/wd4702",	-- Disable warning C4702: unreachable code
			{ "/Od"; Config = "*-*-debug" },
			{ "/O2"; Config = "*-*-release" },
		},
	},
}


Build {
	Units = "units.lua",

	Passes = {
		CompileGenerator = { Name="Compile generator", BuildOrder = 1 },
		CodeGeneration = { Name="Generate sources", BuildOrder = 2 },
	},

 	Configs = {
		{
			Name = "macosx-gcc",
			DefaultOnHost = "macosx",
			Inherit = common,
			Tools = { "gcc", "mono" },
		},
		{
			Name = "win32-msvc",
			DefaultOnHost = "windows",
			Inherit = win32,
			Tools = { "msvc", "mono" },
		},
	},
}
