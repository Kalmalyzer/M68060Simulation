local common = {
	Env = {
		CPPPATH = "Code",
		ASMINCPATH = "Code",
	},
	Defines = {
		{ "NDEBUG"; Config = "*-*-release" },
	},

}

local amiga = {
	Inherit = common,
	Env = {
		CPPDEFS = { "AMIGA" },
		ASMOPTS = {
			"-m68060",
            "-ignore-mult-inc",
            "-spaces",
		},
		PROGOPTS = {
			"-nostdlib",
		},
	},
}

local amiga_osx = {
	Inherit = amiga,
	ReplaceEnv = {
		["CC"] = "$(VBCC_ROOT)$(SEP)bin$(SEP)Darwin$(SEP)vc$(HOSTPROGSUFFIX)",
		["LIB"] = "$(VBCC_ROOT)$(SEP)bin$(SEP)Darwin$(SEP)vlink$(HOSTPROGSUFFIX)",
		["LD"] = "$(VBCC_ROOT)$(SEP)bin$(SEP)Darwin$(SEP)vc$(HOSTPROGSUFFIX)",
		["ASM"] = "$(VBCC_ROOT)$(SEP)bin$(SEP)Darwin$(SEP)vasmm68k_mot$(HOSTPROGSUFFIX)",
	},
}

local amiga_win32 = {
	Inherit = amiga,
	ReplaceEnv = {
		["CC"] = "$(VBCC_ROOT)$(SEP)bin$(SEP)win32$(SEP)vc$(HOSTPROGSUFFIX)",
		["LIB"] = "$(VBCC_ROOT)$(SEP)bin$(SEP)win32$(SEP)vlink$(HOSTPROGSUFFIX)",
		["LD"] = "$(VBCC_ROOT)$(SEP)bin$(SEP)win32$(SEP)vc$(HOSTPROGSUFFIX)",
		["ASM"] = "$(VBCC_ROOT)$(SEP)bin$(SEP)win32$(SEP)vasmm68k_mot$(HOSTPROGSUFFIX)",
	},
}

Build {
	Units = "units.lua",
	Configs = {
		Config { Name = "amiga-vbccmac", Inherit = amiga_osx, Tools = { "vbcc" }, Virtual = true, },
		Config { Name = "amiga-vbccwin", Inherit = amiga_win32, DefaultOnHost = { "windows" }, Tools = { "vbcc" }, },
		Config { 
			Name = "amiga-vbccosx",
			Inherit = common,
			SubConfigs = {
				host = "macosx-clang",
				target = "amiga-vbccmac",
			},

			DefaultSubConfig = "target",
			DefaultOnHost = { "macosx" }, 
		},
	},
}
