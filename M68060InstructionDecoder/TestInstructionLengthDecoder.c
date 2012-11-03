
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
	{ "BSR.S *+$12", { 0x6112, }, 1 },
	{ "BSR.W *+$1234", { 0x6100, 0x1234, }, 2 },
	{ "BSR.L *+$12345678", { 0x61ff, 0x1234, 0x5678, }, 3 },
	{ "DBRA *+$12", { 0x51c9, 0x1234, }, 2 },
	{ "DBEQ *+$12", { 0x57ca, 0x1234, }, 2 },
};

static const InstructionLengthDecoderTest singleBitInstructionTests[] =
{
	{ "BTST D1,$12345678.L", { 0x0339, 0x1234, 0x5678, }, 3 },
	{ "BCLR #3,$12345678.L", { 0x08b9, 0x0003, 0x1234, 0x5678, }, 4 },
	{ "BCHG D1,D3", { 0x0343, }, 1 },
	{ "BSET #31,D2", { 0x08c2, 0x001f, }, 2 },
};

static const InstructionLengthDecoderTest bitFieldInstructionTests[] =
{
	{ "BFCHG $1234(A1,D1.l){D2:D3}", { 0xeaf1, 0x08a3, 0x1920, 0x1234, }, 4 },
	{ "BFCHG D1{D2:D3}", { 0xeac1, 0x08a3, }, 2 },
	{ "BFCLR $1234(A1,D1.l){D2:D3}", { 0xecf1, 0x08a3, 0x1920, 0x1234, }, 4 },
	{ "BFEXTS ($12345678.L,A1,D1.l){D2:D3},D4", { 0xebf1, 0x48a3, 0x1930, 0x1234, 0x5678, }, 5 },
	{ "BFEXTU ($1234.w,A1){D2:D3},D4", { 0xe9e9, 0x48a3, 0x1234, }, 3 },
	{ "BFFFO (A1){D2:D3},D4", { 0xedd1, 0x48a3, }, 2 },
	{ "BFINS D1,($1234.w,A1){D2:D3}", { 0xefe9, 0x18a3, 0x1234, }, 3 },
	{ "BFSET D1{D2:D3}", { 0xeec1, 0x08a3, }, 2 },
	{ "BFTST $1234(A1,D1.l){D2:D3}", { 0xe8f1, 0x08a3, 0x1920, 0x1234, }, 4 },
};

static const InstructionLengthDecoderTest integerArithmeticTests[] =
{
	{ "ABCD D2,D3", { 0xc702, }, 1 },
	{ "ABCD -(A1),-(A3)", { 0xc709, }, 1 },
	{ "ADDQ.L #5,([A2])", { 0x5ab2, 0x0151, }, 2 },
	{ "ADDX.W D2,D3", { 0xd742, }, 1 },
	{ "ADDX.L -(A1),-(A3)", { 0xd789, }, 1 },
	{ "CMP.L (A3),D1", { 0xb293, }, 1 },
	{ "CMPI.L #$12345678,D1", { 0x0c81, 0x1234, 0x5678, }, 3 },
	{ "CMPM.B (A5)+,(A4)+", { 0xb90d, }, 1 },
	{ "CMPM.L (A3)+,(A2)+", { 0xb58b, }, 1, },

	{ "DIVS.W #$1234,D1", { 0x83fc, 0x1234, }, 2 },
	{ "DIVS.W $1234(A1),D1", { 0x83e9, 0x1234, }, 2 },
	{ "DIVU.W $12(A1,D2.L),D3", { 0x86f1, 0x2812, }, 2 },
	{ "DIVS.L $1234(A1),D0", { 0x4c69, 0x0800, 0x1234 }, 3 },
	{ "DIVS.L $1234(A1),D0:D2", { 0x4c69, 0x2c00, 0x1234 }, 3 },
	{ "DIVUL.L $1234(A1),D0:D2", { 0x4c69, 0x2000, 0x1234 }, 3 },
};

static const InstructionLengthDecoderTest shiftRotateTests[] =
{
	{ "ASL.W D0,D1", { 0xe161, }, 1 },
	{ "ASL.L #3,D2", { 0xe782, }, 1 },
	{ "ASR.B #1,D1", { 0xe201, }, 1 },
	{ "ASR.W $1234(A5)", { 0xe0ed, 0x1234, }, 2 },
};

static const InstructionLengthDecoderTest integerLogicTests[] =
{
	{ "AND.W $1234(A1),D3", { 0xc669, 0x1234, }, 2 },
	{ "AND.L D3,$1234(A1)", { 0xc7a9, 0x1234, }, 2 },
	{ "ANDI.L #$12345678,($12345678,D2)", { 0x02b0, 0x1234, 0x5678, 0x21b0, 0x1234, 0x5678, }, 6 },

	{ "EOR.L D1,(A3)", { 0xb393, }, 1 },
	{ "EORI.L #$12345678,$1234(A3)", { 0x0aab, 0x1234, 0x5678, 0x1234, }, 4 },
};

static const InstructionLengthDecoderTest miscellaneousTests[] =
{
	{ "BKPT #4", { 0x484c, }, 1 },
	{ "CHK.W $1234(A1),D1", { 0x43a9, 0x1234, }, 2 },
	{ "CHK.L #$12345678,D1", { 0x433c, 0x1234, 0x5678, }, 3 },
	{ "CLR.W $1234(A1)", { 0x4269, 0x1234, }, 2 },

	{ "EXG D2,D3", { 0xc543, }, 1 },
	{ "EXG A4,A5", { 0xc94d, }, 1 },
	{ "EXG D3,A7", { 0xc78f, }, 1 },
	{ "ANDI #$12,CCR", { 0x023c, 0x0012, }, 2 },
};

TestSuite testSuites[] =
{
	{ "6-bit EA decoding tests", ea6BitTests, (sizeof ea6BitTests / sizeof ea6BitTests[0]) },
	{ "Immediate source operand tests", immediateTests, (sizeof immediateTests / sizeof immediateTests[0]) },
	{ "Relative branch tests", relativeBranchTests, (sizeof relativeBranchTests / sizeof relativeBranchTests[0]) },
	{ "Single-bit instruction tests", singleBitInstructionTests, (sizeof singleBitInstructionTests / sizeof singleBitInstructionTests[0]) },
	{ "Bit field instruction tests", bitFieldInstructionTests, (sizeof bitFieldInstructionTests / sizeof bitFieldInstructionTests[0]) },
	{ "Integer arithmetic tests", integerArithmeticTests, (sizeof integerArithmeticTests / sizeof integerArithmeticTests[0]) },
	{ "Shift/rotate tests", shiftRotateTests, (sizeof shiftRotateTests / sizeof shiftRotateTests[0]) },
	{ "Integer logic tests", integerLogicTests, (sizeof integerLogicTests / sizeof integerLogicTests[0]) },
	{ "Miscellaneous tests", miscellaneousTests, (sizeof miscellaneousTests / sizeof miscellaneousTests[0]) },
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
			printf("success: Decoding %s yields %u words (%s)\n", test->instruction, test->numInstructionWords, instructionLength.mnemonic);
		if (!success && printFailure)
			printf("failure: Decoding %s yields %u words but should be %u words (%s)\n", test->instruction, instructionLength.totalWords, test->numInstructionWords, instructionLength.mnemonic ? instructionLength.mnemonic : "no mnemonic given");

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