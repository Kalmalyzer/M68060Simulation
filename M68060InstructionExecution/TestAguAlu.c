
#include "M68060AguAlu.h"

typedef struct
{
	const char* description;
	AguOperation aguOperation;
	OperationSize operationSize;
	uint32_t baseValue;
	uint32_t indexValue;
	uint indexShift;
	AguIndexSize indexSize;
	uint32_t displacementValue;
	AguDisplacementSize displacementSize;
	uint32_t expectedAguResult;
	uint32_t expectedMemoryOperandReference;

} AguAluTest;

AguAluTest tests[] =
{
	{ "xxx.w (an)+", AguOperation_PostIncrement, OperationSize_Word, 0x12345678, 0, 0, AguIndexSize_None, 0, AguDisplacementSize_None, 0x1234567A, 0x12345678, },
	{ "xxx.l -(an)", AguOperation_PreDecrement, OperationSize_Long, 0x12345678, 0, 0, AguIndexSize_None, 0, AguDisplacementSize_None, 0x12345674, 0x12345674, },
	{ "xxx.b -(an)", AguOperation_PreDecrement, OperationSize_Byte, 0x12345678, 0, 0, AguIndexSize_None, 0, AguDisplacementSize_None, 0x12345677, 0x12345677, },
	{ "xxx.l d16(an)", AguOperation_OffsetBaseIndexScale, OperationSize_Long, 0x12345678, 0, 0, AguIndexSize_None, 0xf00f, AguDisplacementSize_S16, 0x12344687, 0x12344687, },
	{ "xxx.l d8(an,dn.w*1)", AguOperation_OffsetBaseIndexScale, OperationSize_Long, 0x12345678, 0xe010, 0, AguIndexSize_Word, 0x12, AguDisplacementSize_S8, 0x1234369A, 0x1234369A, },
	{ "xxx.l d8(an,dn.w*2)", AguOperation_OffsetBaseIndexScale, OperationSize_Long, 0x12345678, 0xe010, 1, AguIndexSize_Word, 0x12, AguDisplacementSize_S8, 0x123416AA, 0x123416AA, },
	{ "xxx.l d8(an,dn.l*4)", AguOperation_OffsetBaseIndexScale, OperationSize_Long, 0x12345678, 0x1234e010, 2, AguIndexSize_Long, 0x12, AguDisplacementSize_S8, 0x5B07D6CA, 0x5B07D6CA, },
	{ "xxx.l d8(an,dn.w*8)", AguOperation_OffsetBaseIndexScale, OperationSize_Long, 0x12345678, 0xe010, 3, AguIndexSize_Word, 0xf2, AguDisplacementSize_S8, 0x123356EA, 0x123356EA, },
	{ "xxx.w (sp)+", AguOperation_PostIncrementSP, OperationSize_Word, 0x12345678, 0, 0, AguIndexSize_None, 0, AguDisplacementSize_None, 0x1234567A, 0x12345678, },
	{ "xxx.l -(sp)", AguOperation_PreDecrementSP, OperationSize_Long, 0x12345678, 0, 0, AguIndexSize_None, 0, AguDisplacementSize_None, 0x12345674, 0x12345674, },
	{ "xxx.b (sp)+", AguOperation_PostIncrementSP, OperationSize_Byte, 0x12345678, 0, 0, AguIndexSize_None, 0, AguDisplacementSize_None, 0x1234567A, 0x12345678, },
	{ "xxx.b -(sp)", AguOperation_PreDecrementSP, OperationSize_Byte, 0x12345678, 0, 0, AguIndexSize_None, 0, AguDisplacementSize_None, 0x12345676, 0x12345676, },
};

void runTestSuite(const AguAluTest* tests, uint numTests, bool printSuccess, bool printFailure, uint* accumulatedSuccessfulTests, uint* accumulatedTotalTests)
{
	uint numSuccessfulTests = 0;
	uint numTotalTests = 0;
	uint testId;
	
	for (testId = 0; testId < numTests; ++testId)
	{
		bool success;
		const AguAluTest* test = &tests[testId];
		bool correctAguResult = true;
		bool correctMemoryOperandReference = true;
		
		uint32_t aguResult;
		uint32_t memoryOperandReference;

		evaluateAguAluOperation(test->aguOperation, test->operationSize, test->baseValue, test->indexValue, test->indexShift, test->indexSize,
			test->displacementValue, test->displacementSize, &aguResult, &memoryOperandReference);

		if (aguResult != test->expectedAguResult)
			correctAguResult = false;
		if (memoryOperandReference != test->expectedMemoryOperandReference)
			correctMemoryOperandReference = false;

		success = correctAguResult && correctMemoryOperandReference;
			
		if (success && printSuccess)
			printf("success: Invoking %s yields aguResult %08X and memoryOperandReference %08X\n", test->description, aguResult, memoryOperandReference);
		if (!success && printFailure)
		{
			printf("failure: Invoking %s yields aguResult %08X", test->description, aguResult);
			if (!correctAguResult)
				printf(" (should be %08X)", test->expectedAguResult);

			printf(", memoryOperandReference %08X", memoryOperandReference);
			if (!correctMemoryOperandReference)
				printf(" (should be %08X)", test->expectedMemoryOperandReference);

			printf("\n");
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

	printf("Testing M68060 AGU-ALU\n");
	printf("\n");

	runTestSuite(tests, sizeof tests / sizeof tests[0], printSuccess, printFailure, &numSuccessfulTests, &numTotalTests);

	printf("\n");
	printf("%u out of %u tests succeeded.\n", numSuccessfulTests, numTotalTests);

	return 0;
}