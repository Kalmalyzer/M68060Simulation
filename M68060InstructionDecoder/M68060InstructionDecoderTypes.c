
#include "M68060InstructionDecoderTypes.h"
#include "../Assert.h"

#include <stddef.h>

const char* ExecutionResourceToString(ExecutionResource executionResource)
{
	static const char* executionResourceStrings[] =
	{
		"None",
		"D0",
		"D1",
		"D2",
		"D3",
		"D4",
		"D5",
		"D6",
		"D7",
		"A0",
		"A1",
		"A2",
		"A3",
		"A4",
		"A5",
		"A6",
		"A7",
		"AguResult",
		"MemoryOperand",
		"ImmediateOperand",
		"PC",
	};

	M68060_ASSERT((size_t) executionResource < (sizeof executionResourceStrings / sizeof executionResourceStrings[0]), "Invalid executionResource");
	
	return executionResourceStrings[(int) executionResource];
}
