
#include "M68060IntegerRegisterFile.h"
#include "../Assert.h"

typedef struct
{
	ExecutionResource reg;
	uint32_t expectedValue;

} IntegerRegisterFileTest;

IntegerRegisterFileTest tests[] =
{
	{ ExecutionResource_D0, 0x12345678, },
	{ ExecutionResource_D1, 0, },
	{ ExecutionResource_D2, 0, },
	{ ExecutionResource_D3, 0, },
	{ ExecutionResource_D4, 0, },
	{ ExecutionResource_D5, 0x87654321, },
	{ ExecutionResource_D6, 0, },
	{ ExecutionResource_D7, 0, },
	{ ExecutionResource_A0, 0, },
	{ ExecutionResource_A1, 0, },
	{ ExecutionResource_A2, 0, },
	{ ExecutionResource_A3, 0, },
	{ ExecutionResource_A4, 0, },
	{ ExecutionResource_A5, 0, },
	{ ExecutionResource_A6, 0, },
	{ ExecutionResource_A7, 0xfedcba98, },
};

void runTestSuite(const IntegerRegisterFileTest* tests, uint numTests, bool printSuccess, bool printFailure, uint* accumulatedSuccessfulTests, uint* accumulatedTotalTests)
{
	uint numSuccessfulTests = 0;
	uint numTotalTests = 0;
	uint testId;
	
	for (testId = 0; testId < numTests; ++testId)
	{
		bool success;
		const IntegerRegisterFileTest* test = &tests[testId];
		
		uint32_t value = readIntegerRegister(test->reg);
		
		success = (value == test->expectedValue);
			
		if (success && printSuccess)
			printf("success: Reading %s yields %08X\n", ExecutionResourceToString(test->reg), value);
		if (!success && printFailure)
			printf("failure: Reading %s yields %08X (should be %08X)\n", ExecutionResourceToString(test->reg), value, test->expectedValue);

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

	writeIntegerRegister(ExecutionResource_D0, 0x12345678);
	writeIntegerRegister(ExecutionResource_D5, 0x87654321);
	writeIntegerRegister(ExecutionResource_A7, 0xfedcba98);

	runTestSuite(tests, sizeof tests / sizeof tests[0], printSuccess, printFailure, &numSuccessfulTests, &numTotalTests);

	printf("\n");
	printf("%u out of %u tests succeeded.\n", numSuccessfulTests, numTotalTests);

	return 0;
}