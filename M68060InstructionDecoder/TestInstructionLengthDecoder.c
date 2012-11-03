
#include "M68060InstructionLengthDecoder.h"
#include "../Types.h"

typedef struct
{
	const char* instruction;
	const uint16_t instructionWords[16];
	uint numInstructionWords;
} InstructionLengthDecoderTest;

typedef struct
{
	const char* description;
	const InstructionLengthDecoderTest* tests;
	uint numTests;
} TestSuite;

static const InstructionLengthDecoderTest ea6BitTests[] =
{
	{ "ADD.B D1,D3", { 0xd601, }, 1 },
	{ "ADD.L D1,D3", { 0xd681, }, 1 },
	{ "ADD.L A1,D3", { 0xd689, }, 1 },
	{ "ADD.L (A1),D3", { 0xd691, }, 1 },
	{ "ADD.L (A1)+,D3", { 0xd699, }, 1 },
	{ "ADD.L -(A2),D3", { 0xd6a2, }, 1 },
	{ "ADD.W ($1234,A1),D3", { 0xd27c, 0x1234, }, 2 },
	{ "ADD.W $12(A1,D3.W*4),D5", { 0xda71, 0x3412, }, 2 },
	{ "ADD.B #$12,D1", { 0xd23c, 0x0012, }, 2 },
	{ "ADD.W #$1234,D1", { 0xd27c, 0x1234, }, 2 },
	{ "ADD.L #$12345678,D1", { 0xd2bc, 0x1234, 0x5678, }, 3 },
	{ "ADDA.W #$1234,A1", { 0xd3e9, 0x1234, }, 2 },
	{ "ADDA.L #$12345678,A1", { 0xd3fc, 0x1234, 0x5678, }, 3 },
	{ "ADD.B $1234.W,D1", { 0xd238, 0x1234, }, 2 },
	{ "ADD.W $1234.W,D1", { 0xd278, 0x1234, }, 2 },
	{ "ADD.L $1234.W,D1", { 0xd2b8, 0x1234, }, 2 },
	{ "ADD.B $12345678.L,D1", { 0xd239, 0x1234, 0x5678, }, 3 },
	{ "ADD.W $12345678.L,D1", { 0xd279, 0x1234, 0x5678, }, 3 },
	{ "ADD.L $12345678.L,D1", { 0xd2b9, 0x1234, 0x5678, }, 3 },
	{ "ADD.L ($1234,PC),D3", { 0xd6ba, 0x1234, }, 2 },
	{ "ADD.L $12(PC,D3.W*4),D5", { 0xdabb, 0x3412, }, 2 },
	{ "ADD.L ($1234.W,A0,D0.L),D3", { 0xd6b0, 0x0920, 0x1234, }, 3 },
	{ "ADD.L ($12345678.L,A0,ZD0),D3", { 0xd6b0, 0x0170, 0x1234, 0x5678, }, 4 },
	{ "ADD.L ($12345678.L,A0,D0.L),D3", { 0xd6b0, 0x0930, 0x1234, 0x5678, }, 4 },
	{ "ADD.L ([$12345678.L,A0],D0.L),D3", { 0xd6b0, 0x0935, 0x1234, 0x5678, }, 4 },
	{ "ADD.L ([$12345678.L,A0],D0.L,$1234.W),D3", { 0xd6b0, 0x0936, 0x1234, 0x5678, 0x1234, }, 5 },
	{ "ADD.L ([$12345678.L,A0],D0.L,$12345678.L),D3", { 0xd6b0, 0x0937, 0x1234, 0x5678, 0x1234, 0x5678, }, 6 },
	{ "ADD.L ([$1234.W,A0],D0.L,$12345678.L),D3", { 0xd6b0, 0x0927, 0x1234, 0x1234, 0x5678, }, 5 },
	{ "ADD.L ([A0,D0.L],$12345678.L),D3", { 0xd6b0, 0x0913, 0x1234, 0x5678, }, 4 },

	{ "ADD.L D3,(A1)+", { 0xd799, }, 1 },
	{ "ADD.L D3,-(A2)", { 0xd7a2, }, 1 },
	{ "ADD.W D3,($1234,A1)", { 0xd769, 0x1234, }, 2 },
	{ "ADD.W D5,$12(A1,D3.W*4)", { 0xdb71, 0x3412, }, 2 },
	{ "ADD.B D1,$1234.W", { 0xd338, 0x1234, }, 2 },
	{ "ADD.W D1,$12345678.L", { 0xd379, 0x1234, 0x5678, }, 3 },
	{ "ADD.L D3,($12345678.L,A0,D0.L)", { 0xd7b0, 0x0930, 0x1234, 0x5678, }, 4 },
	{ "ADD.L D3,([$12345678.L,A0],D0.L)", { 0xd7b0, 0x0935, 0x1234, 0x5678, }, 4 },
	{ "ADD.L D3,([$12345678.L,A0],D0.L,$1234.W)", { 0xd7b0, 0x0936, 0x1234, 0x5678, 0x1234, }, 5 },
	{ "ADD.L D3,([$12345678.L,A0],D0.L,$12345678.L)", { 0xd7b0, 0x0937, 0x1234, 0x5678, 0x1234, 0x5678, }, 6 },
	{ "ADD.L D3,([$1234.W,A0],D0.L,$12345678.L)", { 0xd7b0, 0x0927, 0x1234, 0x1234, 0x5678, }, 5 },
	{ "ADD.L D3,([A0,D0.L],$12345678.L)", { 0xd7b0, 0x0913, 0x1234, 0x5678, }, 4 },

};

static const InstructionLengthDecoderTest immediateTests[] =
{
	{ "ADDI.B #$12,D2", { 0x0602, 0x0012, }, 2 },
	{ "ADDI.W #$1234,D2", { 0x0642, 0x1234, }, 2 },
	{ "ADDI.L #$12345678,D2", { 0x0682, 0x1234, 0x5678, }, 3 },
	{ "ADDI.W #$1234,$1234(A1,D2.L)", { 0x0671, 0x1234, 0x2920, 0x1234, }, 4 },
	{ "ADDI.W #$1234,$12345678.L", { 0x0679, 0x1234, 0x1234, 0x5678, }, 4 },
	{ "ADDI.W #$1234,([$12345678,A1,D2.L])", { 0x0671, 0x1234, 0x2931, 0x1234, 0x5678, }, 5 },
};

static const InstructionLengthDecoderTest relativeBranchTests[] =
{
	{ "BRA.S *+$12", { 0x6012, }, 1 },
	{ "BNE.W *+$1234", { 0x6600, 0x1234, }, 2 },
	{ "BEQ.L *+$12345678", { 0x67ff, 0x1234, 0x5678, }, 3 },
};

TestSuite testSuites[] =
{
	{ "6-bit EA decoding tests", ea6BitTests, (sizeof ea6BitTests / sizeof ea6BitTests[0]) },
	{ "Immediate source operand tests", immediateTests, (sizeof immediateTests / sizeof immediateTests[0]) },
	{ "Relative branch tests", relativeBranchTests, (sizeof relativeBranchTests / sizeof relativeBranchTests[0]) },
};

void runTestSuite(const InstructionLengthDecoderTest* tests, uint numTests, bool printSuccess, bool printFailure, uint* accumulatedSuccessfulTests, uint* accumulatedTotalTests)
{
	uint numSuccessfulTests = 0;
	uint numTotalTests = 0;
	uint testId;
	
	for (testId = 0; testId < numTests; ++testId)
	{
		bool success = true;
		const InstructionLengthDecoderTest* test = &tests[testId];
		InstructionLength instructionLength;
		
		if (!decodeInstructionLengthFromInstructionWords(test->instructionWords, test->numInstructionWords, &instructionLength))
			success = false;
		else if (instructionLength.totalWords != test->numInstructionWords)
			success = false;
		
		if (success && printSuccess)
			printf("success: Decoding %s yields %u words\n", test->instruction, test->numInstructionWords);
		if (!success && printFailure)
			printf("failure: Decoding %s yields %u words but should be %u words\n", test->instruction, instructionLength.totalWords, test->numInstructionWords);

		if (success)
			numSuccessfulTests++;
		numTotalTests++;
	}

	
	*accumulatedSuccessfulTests += numSuccessfulTests;
	*accumulatedTotalTests += numTotalTests;
}
	
int main(void)
{
	bool printSuccess = false;
	bool printFailure = true;
	
	uint numSuccessfulTests = 0;
	uint numTotalTests = 0;

	uint suite = 0;

	printf("Testing M68060 Instruction Length Decoder\n");
	printf("\n");

	for (suite = 0; suite < (sizeof testSuites / sizeof testSuites[0]); ++suite)
	{
		printf("Running %s\n", testSuites[suite].description);
		runTestSuite(testSuites[suite].tests, testSuites[suite].numTests, printSuccess, printFailure, &numSuccessfulTests, &numTotalTests);
	}

	printf("\n");
	printf("%u out of %u tests succeeded.\n", numSuccessfulTests, numTotalTests);
	
	return 0;
}