
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

Default "TestCache"
Default "TestOpWordDecoder"
Default "TestPairability"

