
require "tundra.syntax.glob" 

--------------------------------------------------

DefRule {
	Name = "m68kmake",
	Pass = "CodeGeneration",
	Command = "$(M68KMAKE) $(<) 68k_emu_core/m68k_in.c",
	ImplicitInputs = { "$(M68KMAKE)", "68k_emu_core/m68k_in.c" },

	Blueprint = {
		TargetDir = { Required = true, Type = "string", Help = "Input filename", },
	},

	Setup = function (env, data)
		return {
			InputFiles = { data.TargetDir },
			OutputFiles = { 
				"$(OBJECTDIR)/_generated/m68kopac.c", 
				"$(OBJECTDIR)/_generated/m68kopdm.c", 
				"$(OBJECTDIR)/_generated/m68kopnz.c", 
				"$(OBJECTDIR)/_generated/m68kops.c", 
				"$(OBJECTDIR)/_generated/m68kops.h", 
			},
		}
	end,
}

Program {
	Name = "m68kmake",
	Pass = "CompileGenerator",
	Target = "$(M68KMAKE)",
	Sources = { "68k_emu_core/m68kmake.c" },
}

StaticLibrary {
	Name = "Musashi",
	Pass = "CodeGeneration", 
	Env = {
		CPPPATH = { "Musashi",
					"$(OBJECTDIR)/_generated",
		},
	},

	Sources = {
		"Musashi/m68kcpu.c", 
		"Musashi/m68kfpu.c", 
		"Musashi/m68kdasm.c", 

		m68kmake {
			TargetDir = "$(OBJECTDIR)/_generated",
		},
	},

} 

Program {
	Name = "TestMusashi",
--	Pass = "CodeGeneration", 
	Env = {
		CPPPATH = { "Musashi",
		},
	},
	Sources = {
		"Musashi/TestMusashi.c",
	},

	Depends = { "Musashi" },
}
 
--------------------------------------------------

Program {
	Name = "TestCache",
	Sources = {
		"M68060Cache/Tests.c",
		"M68060Cache/M68060Cache.c",
		"M68060Cache/M68060CacheMetrics.c"
	},
}

Program {
	Name = "TestOpWordDecoder",
	Sources = {
		"M68060InstructionDecoder/M68060OpWordDecoder.c",
		"M68060InstructionDecoder/M68060InstructionDecoderTypes.c",
		"M68060InstructionDecoder/TestOpWordDecoder.c",
	},
}

Program {
	Name = "TestPairability",
	Sources = {
		"M68060InstructionDecoder/M68060OpWordDecoder.c",
		"M68060InstructionDecoder/M68060InstructionDecoderTypes.c",
		"M68060InstructionDecoder/M68060PairabilityTests.c",
		"M68060InstructionDecoder/TestPairability.c",
	},
}

Program {
	Name = "TestAguAlu",
	Sources = {
		"M68060InstructionDecoder/M68060InstructionDecoderTypes.c",
		"M68060InstructionExecution/M68060AguAlu.c",
		"M68060InstructionExecution/TestAguAlu.c",
	},
}

Program {
	Name = "TestInstructionLengthDecoder",
	Sources = {
		"M68060InstructionDecoder/M68060InstructionLengthDecoder.c",
		"M68060InstructionDecoder/TestInstructionLengthDecoder.c",
	},
}

Program {
	Name = "SoakTestInstructionLengthDecoder",
	Sources = {
		"M68060InstructionDecoder/M68060InstructionLengthDecoder.c",
		"M68060InstructionDecoder/SoakTestInstructionLengthDecoder.c",
	},
}

Program {
	Name = "InstructionLengthDisassembler",
	Sources = {
		"M68060InstructionDecoder/M68060InstructionLengthDecoder.c",
		"M68060InstructionDecoder/InstructionLengthDisassembler.c",
	},
}

Default "TestCache"
Default "TestOpWordDecoder"
Default "TestPairability"
Default "TestAguAlu"
Default "TestInstructionLengthDecoder"
Default "SoakTestInstructionLengthDecoder"
Default "InstructionLengthDisassembler"

Default "Musashi"
Default "TestMusashi"
