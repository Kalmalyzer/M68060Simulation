
local common = {
	Env = {
		M68KMAKE = "$(OBJECTDIR)$(SEP)m68kmake$(PROGSUFFIX)",
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
			Inherit = common,
			Tools = { "msvc", "mono" },
		},
	},
}
