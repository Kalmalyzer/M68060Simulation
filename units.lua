
require "tundra.syntax.glob" 

---------------------------------------------------------------------------------------
-- Build rules for Musashi; both as a static library, and a stand-alone test program --
---------------------------------------------------------------------------------------

DefRule {
	Name = "m68kmake",
	Pass = "CodeGeneration",
	Command = "$(M68KMAKE) $(<) Musashi/m68k_in.c",
	ImplicitInputs = { "$(M68KMAKE)", "Musashi/m68k_in.c" },

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
	Sources = { "Musashi/m68kmake.c" },
}

StaticLibrary {
	Name = "Musashi",
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
 
---------------------------------------------------
-- Cache line allocation-simulation test program --
---------------------------------------------------

Program {
	Name = "TestCache",
	Sources = {
		"M68060Cache/Tests.c",
		"M68060Cache/M68060Cache.c",
		"M68060Cache/M68060CacheMetrics.c"
	},
}

------------------------------------------------
-- Instruction length decoder's test programs --
------------------------------------------------

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
	Name = "SoakTestInstructionLengthDecoderAgainstMusashi",
	Sources = {
		"M68060InstructionDecoder/M68060InstructionLengthDecoder.c",
		"M68060InstructionDecoder/SoakTestInstructionLengthDecoderAgainstMusashi.c",
		"Musashi/StandardMusashiCallbacks.c",
	},
	
	Depends = { "Musashi", },
}

Program {
	Name = "InstructionLengthDisassembler",
	Sources = {
		"M68060InstructionDecoder/M68060InstructionLengthDecoder.c",
		"M68060InstructionDecoder/InstructionLengthDisassembler.c",
	},
}

---------------------------------------------------------------------------------------------------
-- Build rules for test program that checks instruction pairability based on the leading OpWords --
-- This is a flawed approach, and will not work for all instructions;                            --
-- therefore, this program will likely get rewritten to handle uOPs gracefully in the future     --
---------------------------------------------------------------------------------------------------

Program {
	Name = "TestPairability",
	Sources = {
		"M68060InstructionDecoder/M68060OpWordDecoder.c",
		"M68060InstructionDecoder/M68060InstructionDecoderTypes.c",
		"M68060InstructionDecoder/M68060PairabilityTests.c",
		"M68060InstructionDecoder/TestPairability.c",
	},
}

-----------------------------------------------------------------------------
-- Build rules for test program for a proper instruction -> uOP(s) decoder --
-----------------------------------------------------------------------------

Program {
	Name = "TestOpIntouOPDecomposer",
	Sources = {
		"M68060InstructionDecoder/M68060DecomposeOpIntouOPs.c",
		"M68060InstructionDecoder/M68060InstructionDecoderTypes.c",
		"M68060InstructionDecoder/M68060InstructionLengthDecoder.c",
		"M68060InstructionDecoder/TestOpIntouOPDecomposer.c",
	},
}

-------------------------------------------------
-- Minimal AGU/ALU implementation test program --
-------------------------------------------------

Program {
	Name = "TestAguAlu",
	Sources = {
		"M68060InstructionDecoder/M68060InstructionDecoderTypes.c",
		"M68060InstructionExecution/M68060AguAlu.c",
		"M68060InstructionExecution/TestAguAlu.c",
	},
}

-------------------------------------------------
-- Minimal IEE/ALU implementation test program --
-------------------------------------------------

Program {
	Name = "TestIeeAlu",
	Sources = {
		"M68060InstructionDecoder/M68060InstructionDecoderTypes.c",
		"M68060InstructionExecution/M68060IeeAlu.c",
		"M68060InstructionExecution/TestIeeAlu.c",
	},
}

-------------------------------------------------
-- Minimal Integer Register File implementation test program --
-------------------------------------------------

Program {
	Name = "TestIntegerRegisterFile",
	Sources = {
		"M68060InstructionDecoder/M68060InstructionDecoderTypes.c",
		"M68060InstructionExecution/M68060IntegerRegisterFile.c",
		"M68060InstructionExecution/TestIntegerRegisterFile.c",
	},
}

-------------------------------------------
-- Minimal memory subsystem test program --
-------------------------------------------

Program {
	Name = "TestDummyMemorySubSystem",
	Sources = {
		"M68060InstructionDecoder/M68060InstructionDecoderTypes.c",
		"M68060InstructionExecution/DummyMemorySubSystem.c",
		"M68060InstructionExecution/TestDummyMemorySubSystem.c",
	},
}

Default "Musashi"
Default "TestMusashi"

Default "TestCache"

Default "TestInstructionLengthDecoder"
Default "SoakTestInstructionLengthDecoder"
Default "SoakTestInstructionLengthDecoderAgainstMusashi"
Default "InstructionLengthDisassembler"

--Default "TestPairability"

Default "TestOpIntouOPDecomposer"

Default "TestAguAlu"
Default "TestIeeAlu"
Default "TestIntegerRegisterFile"
Default "TestDummyMemorySubSystem"
