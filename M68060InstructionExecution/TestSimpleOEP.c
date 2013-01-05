
#include "M68060SimpleOEP.h"

#include "DummyMemorySubSystem.h"
#include "M68060IntegerRegisterFile.h"
#include "../M68060InstructionDecoder/M68060DecodeOpIntoUOps.h"

#include <stdio.h>

enum { MaxMemoryRange = 64 };
enum { MaxIntegerRegisters = 16 };

typedef struct
{
	Flags flags;
	uint32_t rn[MaxIntegerRegisters];

} RegisterValues;

typedef uint8_t MemoryRange[MaxMemoryRange];

typedef struct
{
	const char* description;
	uint totalInstructionWords;
	const uint16_t instructionWords[16];

	const RegisterValues registersBefore;
	uint32_t memoryRangeBaseAddress;
	uint memoryRangeSize;
	const MemoryRange memoryRangeBefore;
	
	const RegisterValues expectedRegistersAfter;
	const MemoryRange expectedMemoryRangeAfter;
	
} M68kOpTest;

M68kOpTest tests[] =
{
	{	"ADDI.B #$12,D2", 2, { 0x0602, 0x0012, },
		{	0,
			{	0x00000000, 0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777,
				0xf0f0f0f0, 0xe1e1e1e1, 0xd2d2d2d2, 0xc3c3c3c3, 0xb4b4b4b4, 0xa5a5a5a5, 0x96969696, 0x87878787, }, },
		0, 0,
		{ 0, },
		{	0,
			{	0x00000000, 0x11111111, 0x22222234, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777,
				0xf0f0f0f0, 0xe1e1e1e1, 0xd2d2d2d2, 0xc3c3c3c3, 0xb4b4b4b4, 0xa5a5a5a5, 0x96969696, 0x87878787, }, },
		{ 0, },
	},

	{ "ADD.W D2,$2.W", 2, { 0xd578, 0x0002, },
		{	0,
			{	0x00000000, 0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777,
				0xf0f0f0f0, 0xe1e1e1e1, 0xd2d2d2d2, 0xc3c3c3c3, 0xb4b4b4b4, 0xa5a5a5a5, 0x96969696, 0x87878787, }, },
		0, 8,
		{ 0x12, 0x34, 0x56, 0x78, 0xab, 0xcd, 0xef, 0x99, },
		{	0,
			{	0x00000000, 0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777,
				0xf0f0f0f0, 0xe1e1e1e1, 0xd2d2d2d2, 0xc3c3c3c3, 0xb4b4b4b4, 0xa5a5a5a5, 0x96969696, 0x87878787, }, },
		{ 0x12, 0x34, 0x78, 0x9a, 0xab, 0xcd, 0xef, 0x99, },
	},

	{ "ADDI.W #$C,D3", 2, { 0x0643, 0x000c, },
		{	0,
			{	0x00000000, 0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777,
				0xf0f0f0f0, 0xe1e1e1e1, 0xd2d2d2d2, 0xc3c3c3c3, 0xb4b4b4b4, 0xa5a5a5a5, 0x96969696, 0x87878787, }, },
		0, 0,
		{ 0, },
		{	0,
			{	0x00000000, 0x11111111, 0x22222222, 0x3333333f, 0x44444444, 0x55555555, 0x66666666, 0x77777777,
				0xf0f0f0f0, 0xe1e1e1e1, 0xd2d2d2d2, 0xc3c3c3c3, 0xb4b4b4b4, 0xa5a5a5a5, 0x96969696, 0x87878787, }, },
		{ 0, },
	},

	{ "ADDI.W #$A,$2.L", 4, { 0x0679, 0x000a, 0x0000, 0x0002, },
		{	0,
			{	0x00000000, 0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777,
				0xf0f0f0f0, 0xe1e1e1e1, 0xd2d2d2d2, 0xc3c3c3c3, 0xb4b4b4b4, 0xa5a5a5a5, 0x96969696, 0x87878787, }, },
		0, 8,
		{ 0x12, 0x34, 0x56, 0x78, 0xab, 0xcd, 0xef, 0x99, },
		{	0,
			{	0x00000000, 0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777,
				0xf0f0f0f0, 0xe1e1e1e1, 0xd2d2d2d2, 0xc3c3c3c3, 0xb4b4b4b4, 0xa5a5a5a5, 0x96969696, 0x87878787, }, },
		{ 0x12, 0x34, 0x56, 0x82, 0xab, 0xcd, 0xef, 0x99, },
	},

	{ "ADDA.W #$2,A1", 2, { 0xd2fc, 0x0002, },
		{	Flags_Extend_Mask | Flags_Zero_Mask,
			{	0x00000000, 0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777,
				0xf0f0f0f0, 0xe1e1e1e1, 0xd2d2d2d2, 0xc3c3c3c3, 0xb4b4b4b4, 0xa5a5a5a5, 0x96969696, 0x87878787, }, },
		0, 0,
		{ 0, },
		{	Flags_Extend_Mask | Flags_Zero_Mask,
			{	0x00000000, 0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777,
				0xf0f0f0f0, 0xe1e1e1e3, 0xd2d2d2d2, 0xc3c3c3c3, 0xb4b4b4b4, 0xa5a5a5a5, 0x96969696, 0x87878787, }, },
		{ 0, },
	},
	
	{ "ADD.W (A1),D3", 1, { 0xd651, },
		{	0,
			{	0x00000000, 0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777,
				0xf0f0f0f0, 0x00000004, 0xd2d2d2d2, 0xc3c3c3c3, 0xb4b4b4b4, 0xa5a5a5a5, 0x96969696, 0x87878787, }, },
		0, 8,
		{ 0x12, 0x34, 0x56, 0x78, 0xab, 0xcd, 0xef, 0x99, },
		{	Flags_Negative_Mask,
			{	0x00000000, 0x11111111, 0x22222222, 0x3333df00, 0x44444444, 0x55555555, 0x66666666, 0x77777777,
				0xf0f0f0f0, 0x00000004, 0xd2d2d2d2, 0xc3c3c3c3, 0xb4b4b4b4, 0xa5a5a5a5, 0x96969696, 0x87878787, }, },
		{ 0x12, 0x34, 0x56, 0x78, 0xab, 0xcd, 0xef, 0x99, },
	},
	
	{ "ADDQ.L #2,D3", 1, { 0x5483, },
		{	Flags_Extend_Mask | Flags_Zero_Mask,
			{	0x00000000, 0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777,
				0xf0f0f0f0, 0xe1e1e1e1, 0xd2d2d2d2, 0xc3c3c3c3, 0xb4b4b4b4, 0xa5a5a5a5, 0x96969696, 0x87878787, }, },
		0, 0,
		{ 0, },
		{	0,
			{	0x00000000, 0x11111111, 0x22222222, 0x33333335, 0x44444444, 0x55555555, 0x66666666, 0x77777777,
				0xf0f0f0f0, 0xe1e1e1e1, 0xd2d2d2d2, 0xc3c3c3c3, 0xb4b4b4b4, 0xa5a5a5a5, 0x96969696, 0x87878787, }, },
		{ 0, },
	},
	
	{ "SUBQ.L #1,D3", 1, { 0x5383, },
		{	0,
			{	0x00000000, 0x11111111, 0x22222222, 0x33333333, 0x44444444, 0x55555555, 0x66666666, 0x77777777,
				0xf0f0f0f0, 0xe1e1e1e1, 0xd2d2d2d2, 0xc3c3c3c3, 0xb4b4b4b4, 0xa5a5a5a5, 0x96969696, 0x87878787, }, },
		0, 0,
		{ 0, },
		{	0,
			{	0x00000000, 0x11111111, 0x22222222, 0x33333332, 0x44444444, 0x55555555, 0x66666666, 0x77777777,
				0xf0f0f0f0, 0xe1e1e1e1, 0xd2d2d2d2, 0xc3c3c3c3, 0xb4b4b4b4, 0xa5a5a5a5, 0x96969696, 0x87878787, }, },
		{ 0, },
	},

};

static void printRegisters(const RegisterValues* registers)
{
	printf("  Dn: %08X %08X %08X %08X %08X %08X %08X %08X\n", registers->rn[0], registers->rn[1], registers->rn[2], registers->rn[3],
		registers->rn[4], registers->rn[5], registers->rn[6], registers->rn[7]);
	printf("  An: %08X %08X %08X %08X %08X %08X %08X %08X\n", registers->rn[8], registers->rn[9], registers->rn[10], registers->rn[11],
		registers->rn[12], registers->rn[13], registers->rn[14], registers->rn[15]);
	printf("  Flags: %s\n", flagsToString(registers->flags));
}

static void printMemory(const uint8_t* memory, uint size)
{
	uint offset;
	for (offset = 0; offset < size; ++offset)
	{
		if (!offset)
			printf(" ");
		else if (!(offset % 16))
			printf("\n ");

		printf(" %02X", memory[offset]);
	}
	
	printf("\n");
}

void runTestSuite(const M68kOpTest* tests, uint numTests, bool printSuccess, bool printFailure, uint* accumulatedSuccessfulTests, uint* accumulatedTotalTests)
{
	uint numSuccessfulTests = 0;
	uint numTotalTests = 0;
	uint testId;
	
	for (testId = 0; testId < numTests; ++testId)
	{
		bool success = true;
		const M68kOpTest* test = &tests[testId];

		UOp UOps[16];
		uint numUOps;
		bool decodeSuccess = decomposeOpIntoUOps(test->instructionWords, test->totalInstructionWords, UOps, &numUOps);

		RegisterValues registersAfter;
		uint8_t memoryAfter[MaxMemoryRange];

		bool correctRegisters = true;
		bool correctFlags = true;
		bool correctMemoryContents = true;
		
		if (decodeSuccess)
		{
			uint UOpId;
			uint memoryOffset;
			uint registerId;

			// Reset simulation
			clearMemory();
			resetSimpleOEP();

			// Initialize simulation
			for (memoryOffset = 0; memoryOffset < test->memoryRangeSize; ++memoryOffset)
				writeMemory(test->memoryRangeBaseAddress + memoryOffset, OperationSize_Byte, test->memoryRangeBefore[memoryOffset]);
			
			for (registerId = 0; registerId < MaxIntegerRegisters; ++registerId)
				writeIntegerRegister(ExecutionResource_D0 + registerId, test->registersBefore.rn[registerId]);

			writeFlags(test->registersBefore.flags);

			// Run UOps
			for (UOpId = 0; UOpId < numUOps; ++UOpId)
				executeUOp(&UOps[UOpId], 0, 0);

			// Capture simulation state
			for (registerId = 0; registerId < MaxIntegerRegisters; ++registerId)
				registersAfter.rn[registerId] = readIntegerRegister(ExecutionResource_D0 + registerId);
				
			registersAfter.flags = readFlags();
			
			for (memoryOffset = 0; memoryOffset < test->memoryRangeSize; ++memoryOffset)
				memoryAfter[memoryOffset] = readMemory(test->memoryRangeBaseAddress + memoryOffset, OperationSize_Byte);
			
			// Compare simulation state against expected results
			
			for (registerId = 0; registerId < MaxIntegerRegisters; ++registerId)
				if (registersAfter.rn[registerId] != test->expectedRegistersAfter.rn[registerId])
					correctRegisters = false;
			
			if (registersAfter.flags != test->expectedRegistersAfter.flags)
				correctFlags = false;

			for (memoryOffset = 0; memoryOffset < test->memoryRangeSize; ++memoryOffset)
				if (memoryAfter[memoryOffset] != test->expectedMemoryRangeAfter[memoryOffset])
					correctMemoryContents = false;
		}
		
		success = decodeSuccess && correctRegisters && correctFlags && correctMemoryContents;
			
		if (success && printSuccess)
			printf("success: Invoking %s yields expected results\n", test->description);
		if (!success && printFailure)
		{
			if (!decodeSuccess)
			{
				printf("failure: Unable to decode %s\n", test->description);
			}
			else
			{
				printf("failure: Invoking %s yields incorrect results\n", test->description);
				printf("Registers before:\n");
				printRegisters(&test->registersBefore);
				
				if (test->memoryRangeSize)
				{
					printf("Memory before:\n");
					printMemory(test->memoryRangeBefore, test->memoryRangeSize);
				}

				printf("Expected registers after:\n");
				printRegisters(&test->expectedRegistersAfter);
				
				if (test->memoryRangeSize)
				{
					printf("Expected memory after:\n");
					printMemory(test->expectedMemoryRangeAfter, test->memoryRangeSize);
				}

				if (!correctRegisters || !correctFlags)
				{
					printf("Actual registers after:\n");
					printRegisters(&registersAfter);
				}
				
				if (test->memoryRangeSize && !correctMemoryContents)
				{
					printf("Actual memory after:\n");
					printMemory(memoryAfter, test->memoryRangeSize);
				}
			}
		}

		if (success)
			numSuccessfulTests++;
		numTotalTests++;
	}
	
	*accumulatedSuccessfulTests += numSuccessfulTests;
	*accumulatedTotalTests += numTotalTests;
}

int main(void)
{
	bool printSuccess = true;
	bool printFailure = true;
	
	uint numSuccessfulTests = 0;
	uint numTotalTests = 0;

	printf("Testing M68060 SimpleOEP\n");
	printf("\n");

	runTestSuite(tests, sizeof tests / sizeof tests[0], printSuccess, printFailure, &numSuccessfulTests, &numTotalTests);

	printf("\n");
	printf("%u out of %u tests succeeded.\n", numSuccessfulTests, numTotalTests);

	return 0;
}