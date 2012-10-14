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
        LIBS = { "amiga" },
		ASMOPTS = {
			"-m68060",
            "-ignore-mult-inc",
            "-spaces",
		},
		CCOPTS = {
			"-O2",
			"-cpu=68060",
			"-fpu=68060",
			"-c99",
			"-warn=-1",
			"-dontwarn=170", -- dead assignment to x eliminated
			"-dontwarn=163", -- #pragma used
			"-dontwarn=307", -- member <foo> does not have natural alignment
			"-dontwarn=65", -- in function "foo": var <bar> was never used
			"-dontwarn=166", -- cast to narrow type may cause loss of precision
			"-dontwarn=167", -- pointer cast may cause alignment problems
			"-dontwarn=81", -- only 0 should be cast to pointer 
			"-dontwarn=306", -- padding bytes behind member <foo>
			"-dontwarn=72", -- character constant contains multiple chars
			"-dontwarn=168", -- no declaration of global variable <foo> before definition
			"-dontwarn=79", -- offset equals size of object (va_start generates this)
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
