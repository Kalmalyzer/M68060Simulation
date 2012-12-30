
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
		"M68060InstructionDecoder/M68060OpWordDecodeInformation.c",
		"M68060InstructionDecoder/TestInstructionLengthDecoder.c",
	},
}

Program {
	Name = "SoakTestInstructionLengthDecoder",
	Sources = {
		"M68060InstructionDecoder/M68060InstructionLengthDecoder.c",
		"M68060InstructionDecoder/M68060OpWordDecodeInformation.c",
		"M68060InstructionDecoder/SoakTestInstructionLengthDecoder.c",
	},
}

Program {
	Name = "SoakTestInstructionLengthDecoderAgainstMusashi",
	Sources = {
		"M68060InstructionDecoder/M68060InstructionLengthDecoder.c",
		"M68060InstructionDecoder/M68060OpWordDecodeInformation.c",
		"M68060InstructionDecoder/SoakTestInstructionLengthDecoderAgainstMusashi.c",
		"Musashi/StandardMusashiCallbacks.c",
	},
	
	Depends = { "Musashi", },
}

Program {
	Name = "InstructionLengthDisassembler",
	Sources = {
		"M68060InstructionDecoder/M68060InstructionLengthDecoder.c",
		"M68060InstructionDecoder/M68060OpWordDecodeInformation.c",
		"M68060InstructionDecoder/InstructionLengthDisassembler.c",
	},
}

----------------------------------------------------
-- Test program for instruction pairability check --
----------------------------------------------------

Program {
	Name = "TestPairability",
	Sources = {
		"M68060InstructionDecoder/M68060DecodeOpIntoUOps.c",
		"M68060InstructionDecoder/M68060InstructionLengthDecoder.c",
		"M68060InstructionDecoder/M68060InstructionDecoderTypes.c",
		"M68060InstructionDecoder/M68060OpWordDecodeInformation.c",
		"M68060InstructionDecoder/M68060PairabilityTests.c",
		"M68060InstructionDecoder/TestPairability.c",
	},
}

----------------------------------------------------
-- Test program for instruction -> UOp(s) decoder --
----------------------------------------------------

Program {
	Name = "TestOpIntoUOpDecoder",
	Sources = {
		"M68060InstructionDecoder/M68060DecodeOpIntoUOps.c",
		"M68060InstructionDecoder/M68060InstructionDecoderTypes.c",
		"M68060InstructionDecoder/M68060InstructionLengthDecoder.c",
		"M68060InstructionDecoder/M68060OpWordDecodeInformation.c",
		"M68060InstructionDecoder/M68060UOp.c",
		"M68060InstructionDecoder/TestOpIntoUOpDecoder.c",
	},
}

------------------------------------------------------
-- Commandline utility for opcode -> UOp(s) decoder --
------------------------------------------------------

Program {
	Name = "CommandLineOpIntoUOpDecoder",
	Sources = {
		"M68060InstructionDecoder/M68060DecodeOpIntoUOps.c",
		"M68060InstructionDecoder/M68060InstructionDecoderTypes.c",
		"M68060InstructionDecoder/M68060InstructionLengthDecoder.c",
		"M68060InstructionDecoder/M68060OpWordDecodeInformation.c",
		"M68060InstructionDecoder/M68060UOp.c",
		"M68060InstructionDecoder/CommandLineOpIntoUOpDecoder.c",
		"Musashi/StandardMusashiCallbacks.c",
	},
	
	Depends = { "Musashi", },
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
		"M68060ISA/M68060ConditionCode.c",
		"M68060ISA/M68060Flags.c",
	},
}

---------------------------------------------------------------
-- Minimal Integer Register File implementation test program --
---------------------------------------------------------------

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

-----------------------------
-- Simple OEP test program --
-----------------------------

Program {
	Name = "TestSimpleOEP",
	Sources = {
		"M68060InstructionDecoder/M68060DecodeOpIntoUOps.c",
		"M68060InstructionDecoder/M68060InstructionDecoderTypes.c",
		"M68060InstructionDecoder/M68060InstructionLengthDecoder.c",
		"M68060InstructionDecoder/M68060OpWordDecodeInformation.c",
		"M68060InstructionExecution/DummyMemorySubSystem.c",
		"M68060InstructionExecution/M68060AguAlu.c",
		"M68060InstructionExecution/M68060FlagsModifier.c",
		"M68060InstructionExecution/M68060IeeAlu.c",
		"M68060InstructionExecution/M68060IntegerRegisterFile.c",
		"M68060InstructionExecution/M68060SimpleOEP.c",
		"M68060InstructionExecution/TestSimpleOEP.c",
		"M68060ISA/M68060ConditionCode.c",
		"M68060ISA/M68060Flags.c",
	},
}

--------------------------------------------------------------------------------------------------------

Default "Musashi"
Default "TestMusashi"

Default "TestCache"

Default "TestInstructionLengthDecoder"
Default "SoakTestInstructionLengthDecoder"
Default "SoakTestInstructionLengthDecoderAgainstMusashi"
Default "InstructionLengthDisassembler"

Default "TestPairability"

Default "TestOpIntoUOpDecoder"
Default "CommandLineOpIntoUOpDecoder"

Default "TestAguAlu"
Default "TestIeeAlu"
Default "TestIntegerRegisterFile"
Default "TestDummyMemorySubSystem"

Default "TestSimpleOEP"
