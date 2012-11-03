
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
