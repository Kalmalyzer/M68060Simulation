
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
		"PC",
		"uOpByte0",
		"uOpWord0",
		"uOpWord1",
		"uOpLong",
		"AguTemp",
		"IeeTemp",
		"Imm3Bit",
	};

	M68060_ASSERT((size_t) executionResource < (sizeof executionResourceStrings / sizeof executionResourceStrings[0]), "Invalid executionResource");
	
	return executionResourceStrings[(int) executionResource];
}

bool isRegister(ExecutionResource executionResource)
{
	return (uint) executionResource >= (uint) ExecutionResource_D0
		&& (uint) executionResource <= (uint) ExecutionResource_A7;
}

const char* PairabilityToString(Pairability pairability)
{
	static const char* pairabilityStrings[] =
	{
		"pOEP_Or_sOEP",
		"pOEP_But_Allows_sOEP",
		"pOEP_Only",
	};

	M68060_ASSERT((size_t) pairability < (sizeof pairabilityStrings / sizeof pairabilityStrings[0]), "Invalid pairability");
	
	return pairabilityStrings[(int) pairability];
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

const char* AguIndexSizeToString(AguIndexSize aguIndexSize)
{
	static const char* aguIndexSizeStrings[] =
	{
		"None",
		"Word",
		"Long",
	};

	M68060_ASSERT((size_t) aguIndexSize < (sizeof aguIndexSizeStrings / sizeof aguIndexSizeStrings[0]), "Invalid aguIndexSize");

	return aguIndexSizeStrings[(int) aguIndexSize];
}

const char* AguDisplacementSizeToString(AguDisplacementSize aguDisplacementSize)
{
	static const char* aguDisplacementSizeStrings[] =
	{
		"None",
		"S8",
		"S16",
		"S32",
	};
		
	M68060_ASSERT((size_t) aguDisplacementSize < (sizeof aguDisplacementSizeStrings / sizeof aguDisplacementSizeStrings[0]), "Invalid aguDisplacementSize");

	return aguDisplacementSizeStrings[(int) aguDisplacementSize];
}

const char* OperationSizeToString(OperationSize operationSize)
{
	static const char* operationSizeStrings[] =
	{
		"None",
		"Byte",
		"Word",
		"Long",
	};
		
	M68060_ASSERT((size_t) operationSize < (sizeof operationSizeStrings / sizeof operationSizeStrings[0]), "Invalid operationSize");

	return operationSizeStrings[(int) operationSize];
}

const char* IeeOperationToString(IeeOperation ieeOperation)
{
	static const char* ieeOperationStrings[] =
	{
		"None",
		"ForwardIeeA",
		"Add",
		"AddA",
		"AddX",
		"Cmp",
		"CmpA",
		"Sub",
		"SubA",
		"SubX",
	};

	M68060_ASSERT((size_t) ieeOperation < (sizeof ieeOperationStrings / sizeof ieeOperationStrings[0]), "Invalid ieeOperation");

	return ieeOperationStrings[(int) ieeOperation];
}

