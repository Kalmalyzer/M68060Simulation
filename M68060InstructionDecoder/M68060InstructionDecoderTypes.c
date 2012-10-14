
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
		"AbsoluteMemoryAddress",
		"PC",
		"Unknown",
	};

	M68060_ASSERT((size_t) executionResource < (sizeof executionResourceStrings / sizeof executionResourceStrings[0]), "Invalid executionResource");
	
	return executionResourceStrings[(int) executionResource];
}

const char* PairabilityToString(Pairability pairability)
{
	static const char* pairabilityStrings[] =
	{
		"pOEP_Or_sOEP",
		"pOEP_But_Allows_sOEP",
		"pOEP_Only",
		"pOEP_Until_Last",
	};

	M68060_ASSERT((size_t) pairability < (sizeof pairabilityStrings / sizeof pairabilityStrings[0]), "Invalid pairability");
	
	return pairabilityStrings[(int) pairability];
}

const char* OpModeToString(OpMode opMode)
{
	static const char* opModeStrings[] =
	{
		"None",
		"EaToRegister_Data_Byte",
		"EaToRegister_Data_Word",
		"EaToRegister_Data_Long",
		"EaToRegister_Address_WordWithSignExtension",
		"RegisterToEa_Data_Byte",
		"RegisterToEa_Data_Word",
		"RegisterToEa_Data_Long",
		"EaToRegister_Address_Long",
	};

	M68060_ASSERT((size_t) opMode < (sizeof opModeStrings / sizeof opModeStrings[0]), "Invalid opMode");
	
	return opModeStrings[(int) opMode];
}

const char* AguOperationToString(AguOperation aguOperation)
{
	static const char* aguOperationStrings[] =
	{
		"None",
		"PostIncrement",
		"PreDecrement",
		"OffsetBaseIndexScale",
	};

	M68060_ASSERT((size_t) aguOperation < (sizeof aguOperationStrings / sizeof aguOperationStrings[0]), "Invalid aguOperation");

	return aguOperationStrings[(int) aguOperation];
}

const char* AguOffsetToString(AguOffset aguOffset)
{
	static const char* aguOffsetStrings[] =
	{
		"None",
		"D8",
		"D16",
		"D32",
	};
		
	M68060_ASSERT((size_t) aguOffset < (sizeof aguOffsetStrings / sizeof aguOffsetStrings[0]), "Invalid aguOffset");

	return aguOffsetStrings[(int) aguOffset];
}

const char* IeeImmediateToString(IeeImmediate ieeImmediate)
{
	static const char* ieeImmediateStrings[] =
	{
		"None",
		"D8",
		"D16",
		"D32",
	};
		
	M68060_ASSERT((size_t) ieeImmediate < (sizeof ieeImmediateStrings / sizeof ieeImmediateStrings[0]), "Invalid ieeImmediate");

	return ieeImmediateStrings[(int) ieeImmediate];
}
