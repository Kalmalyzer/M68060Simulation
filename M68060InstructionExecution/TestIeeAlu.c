
#include "M68060AguAlu.h"

typedef struct
{
	const char* description;
	IeeOperation ieeOperation;
	OperationSize operationSize;
	uint32_t ieeAValue;
	uint32_t ieeBValue;
	uint32_t expectedIeeResult;

} IeeAluTest;

IeeAluTest tests[] =
{
	{ "add.b 0x12,0x123456fc", IeeOperation_Add, OperationSize_Byte, 0x00000012, 0x123456fc, 0x1234560e, },
	{ "add.b 0xfe,0x123456fc", IeeOperation_Add, OperationSize_Byte, 0xfffffffe, 0x123456fc, 0x123456fa, },
	{ "add.w 0x40fe,0x1234c6fc", IeeOperation_Add, OperationSize_Word, 0x000040fe, 0x1234c6fc, 0x123407fa, },
	{ "add.w 0xfffe,0x123456fc", IeeOperation_Add, OperationSize_Word, 0xfffffffe, 0x123456fc, 0x123456fa, },
	{ "add.l 0x10000001,0x123456fc", IeeOperation_Add, OperationSize_Long, 0x10000001, 0x123456fc, 0x223456fd, },
	{ "adda.w 0x40fe,0x1234c6fc", IeeOperation_AddA, OperationSize_Word, 0x000040fe, 0x1234c6fc, 0x123507fa, },
	{ "adda.w 0xfffe,0x123456fc", IeeOperation_AddA, OperationSize_Word, 0xfffffffe, 0x123456fc, 0x123456fa, },
	{ "adda.l 0x10000001,0x123456fc", IeeOperation_AddA, OperationSize_Long, 0x10000001, 0x123456fc, 0x223456fd, },
};

void runTestSuite(const IeeAluTest* tests, uint numTests, bool printSuccess, bool printFailure, uint* accumulatedSuccessfulTests, uint* accumulatedTotalTests)
{
	uint numSuccessfulTests = 0;
	uint numTotalTests = 0;
	uint testId;
	
	for (testId = 0; testId < numTests; ++testId)
	{
		bool success = true;
		const IeeAluTest* test = &tests[testId];
		
		uint32_t ieeResult;

		evaluateIeeAluOperation(test->ieeOperation, test->operationSize, test->ieeAValue, test->ieeBValue, &ieeResult);

		if (ieeResult != test->expectedIeeResult)
			success = false;
			
		if (success && printSuccess)
			printf("success: Invoking %s yields ieeResult %08X\n", test->description, ieeResult);
		if (!success && printFailure)
			printf("failure: Invoking %s yields aguResult %08X (should be %08X)\n", test->description, ieeResult, test->expectedIeeResult);

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

	printf("Testing M68060 IEE-ALU\n");
	printf("\n");

	runTestSuite(tests, sizeof tests / sizeof tests[0], printSuccess, printFailure, &numSuccessfulTests, &numTotalTests);

	printf("\n");
	printf("%u out of %u tests succeeded.\n", numSuccessfulTests, numTotalTests);

	return 0;
}