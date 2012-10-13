
Program {
	Name = "TestCache",
	Sources = {
		"M68060Cache/Tests.c",
		"M68060Cache/M68060Cache.c",
		"M68060Cache/M68060CacheMetrics.c"
	},
}

Program {
	Name = "TestInstructionDecoder",
	Sources = {
		"M68060InstructionDecoder/M68060OpWordDecoder.c",
		"M68060InstructionDecoder/Tests.c",
	},
}

Default "TestCache"
Default "TestInstructionDecoder"

