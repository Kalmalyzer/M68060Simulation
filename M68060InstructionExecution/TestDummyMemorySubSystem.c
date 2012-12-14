
#include "DummyMemorySubSystem.h"
#include "../Assert.h"

typedef struct
{
	uint32_t address;
	OperationSize operationSize;
	uint32_t expectedValue;

} MemoryReadTest;

MemoryReadTest tests[] =
{
	{ 0, OperationSize_Byte, 0x00000012, },
	{ 1, OperationSize_Byte, 0x00000034, },
	{ 2, OperationSize_Byte, 0x00000056, },
	{ 3, OperationSize_Byte, 0x00000078, },
	{ 0, OperationSize_Word, 0x00001234, },
	{ 1, OperationSize_Word, 0x00003456, },
	{ 2, OperationSize_Word, 0x00005678, },
	{ 0, OperationSize_Long, 0x12345678, },
	{ 4, OperationSize_Byte, 0x00000089, },
	{ 5, OperationSize_Byte, 0x000000ab, },
	{ 6, OperationSize_Byte, 0x000000fe, },
	{ 7, OperationSize_Byte, 0x000000dc, },
	{ 8, OperationSize_Byte, 0x000000ba, },
	{ 9, OperationSize_Byte, 0x00000098, },
	{ 5, OperationSize_Long, 0xabfedcba, },
};

void runTestSuite(const MemoryReadTest* tests, uint numTests, bool printSuccess, bool printFailure, uint* accumulatedSuccessfulTests, uint* accumulatedTotalTests)
{
	uint numSuccessfulTests = 0;
	uint numTotalTests = 0;
	uint testId;
	
	for (testId = 0; testId < numTests; ++testId)
	{
		bool success;
		const MemoryReadTest* test = &tests[testId];
		
		uint32_t value = readMemory(test->address, test->operationSize);
		
		success = (value == test->expectedValue);
			
		if (success && printSuccess)
			printf("success: Reading %s from %08X yields %08X\n", OperationSizeToString(test->operationSize), test->address, value);
		if (!success && printFailure)
			printf("failure: Reading %s from %08X yields %08X (should be %08X)\n", OperationSizeToString(test->operationSize), test->address, value, test->expectedValue);

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

	printf("Testing M68060 dummy memory interface\n");
	printf("\n");

	writeMemory(0, OperationSize_Word, 0x1234);
	writeMemory(2, OperationSize_Byte, 0x56);
	writeMemory(3, OperationSize_Byte, 0x78);
	writeMemory(4, OperationSize_Long, 0x89abcdef);
	writeMemory(6, OperationSize_Long, 0xfedcba98);

	runTestSuite(tests, sizeof tests / sizeof tests[0], printSuccess, printFailure, &numSuccessfulTests, &numTotalTests);

	printf("\n");
	printf("%u out of %u tests succeeded.\n", numSuccessfulTests, numTotalTests);

	return 0;
}