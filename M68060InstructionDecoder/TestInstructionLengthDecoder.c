
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

static const InstructionLengthDecoderTest absoluteJumpTests[] =
{
	{ "JMP $1234(A3)", { 0x4eeb, 0x1234, }, 2 },
	{ "JSR ([$12345678.L,PC])", { 0x4ebb, 0x0171, 0x1234, 0x5678, }, 4 },
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

static const InstructionLengthDecoderTest bcdArithmeticTests[] =
{
	{ "ABCD D2,D3", { 0xc702, }, 1 },
	{ "ABCD -(A1),-(A3)", { 0xc709, }, 1 },

	{ "SBCD D2,D3", { 0x8702, }, 1 },
	{ "SBCD -(A1),-(A3)", { 0x8709, }, 1 },

	{ "PACK D1,D2,#$1234", { 0x8541, 0x1234, }, 2 },
	{ "PACK -(A2),-(A1),#$1234", { 0x834a, 0x1234, }, 2 },

	{ "UNPK D2,D3,#$1234", { 0x8782, 0x1234, }, 2 },
	{ "UNPK -(A7),-(A6),#$1234", { 0x8d8f, 0x1234, }, 2 },
};

static const InstructionLengthDecoderTest integerArithmeticTests[] =
{
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

	{ "MULS.W #$1234,D1", { 0xc3fc, 0x1234, }, 2 },
	{ "MULS.W $1234(A1),D1", { 0xc3e9, 0x1234, }, 2 },
	{ "MULU.W $12(A1,D2.L),D3", { 0xc6f1, 0x2812, }, 2 },
	{ "MULS.L $1234(A1),D0", { 0x4c29, 0x0800, 0x1234 }, 3 },
	{ "MULS.L $1234(A1),D0:D2", { 0x4c29, 0x2c00, 0x1234 }, 3 },

	{ "NBCD (A3)", { 0x4813, }, 1 },
	{ "NEG.W D7", { 0x4447, }, 1 },
	{ "NEGX.L D5", { 0x4085, }, 1 },

	{ "SUB.B D1,D2", { 0x9401, }, 1 },
	{ "SUBA.W D4,A5", { 0x9ac4, }, 1 },
	{ "SUB.L D2,-(A6)", { 0x95a6, }, 1 },
	{ "SUBI.W #$1234,D5", { 0x0445, 0x1234, }, 2 },
	{ "SUBQ.L #4,D0", { 0x5980, }, 1 },
	{ "SUBQ.B #3,([A5])", { 0x5735, 0x0151, }, 2 },

	{ "SUBX.W D1,D1", { 0x9341, }, 1 },
	{ "SUBX.B -(A4),-(A3)", { 0x970c, }, 1 },
};

static const InstructionLengthDecoderTest shiftRotateTests[] =
{
	{ "ASL.W D0,D1", { 0xe161, }, 1 },
	{ "ASL.L #3,D2", { 0xe782, }, 1 },
	{ "ASR.B #1,D1", { 0xe201, }, 1 },
	{ "ASR.W $1234(A5)", { 0xe0ed, 0x1234, }, 2 },

	{ "LSL.L D2,D3", { 0xe5ab, }, 1 },
	{ "LSR.W #2,D7", { 0xe44f, }, 1 },
	{ "LSL.W ([A4])", { 0xe3f4, 0x0151, }, 2 },

	{ "ROL.W #2,D3", { 0xe55b, }, 1 },
	{ "ROR.L D2,D7", { 0xe4bf, }, 1 },
	{ "ROL (A2)", { 0xe7d2, }, 1 },

	{ "ROXL.W #1,D4", { 0xe354, }, 1 },
	{ "ROXL ([$12345678,A5])", { 0xe5f5, 0x0171, 0x1234, 0x5678, }, 4 },
	{ "ROXR.L D2,D2", { 0xe4b2, }, 1 },
};

static const InstructionLengthDecoderTest integerLogicTests[] =
{
	{ "AND.W $1234(A1),D3", { 0xc669, 0x1234, }, 2 },
	{ "AND.L D3,$1234(A1)", { 0xc7a9, 0x1234, }, 2 },
	{ "ANDI.L #$12345678,($12345678,D2)", { 0x02b0, 0x1234, 0x5678, 0x21b0, 0x1234, 0x5678, }, 6 },

	{ "ANDI #$12,CCR", { 0x023c, 0x0012, }, 2 },

	{ "EOR.L D1,(A3)", { 0xb393, }, 1 },
	{ "EORI.L #$12345678,$1234(A3)", { 0x0aab, 0x1234, 0x5678, 0x1234, }, 4 },

	{ "NOT.W -(A5)", { 0x4665, }, 1 },

	{ "OR.L $1234(A3),D2", { 0x84ab, 0x1234, }, 2 },
	{ "OR.B d2,([A4])", { 0x8534, 0x0151, }, 2 },
	{ "ORI.W #$1234,d5", { 0x0045, 0x1234, }, 2 },

	{ "ORI #$12,CCR", { 0x003c, 0x0012, }, 2 },

};

static const InstructionLengthDecoderTest moveTests[] =
{
	{ "EXG D2,D3", { 0xc543, }, 1 },
	{ "EXG A4,A5", { 0xc94d, }, 1 },
	{ "EXG D3,A7", { 0xc78f, }, 1 },

	{ "MOVE.W D3,D4", { 0x3803, }, 1 },
	{ "MOVE.B #$12,D2", { 0x143c, 0x0012, }, 2 },
	{ "MOVE.L #$12345678,(A6)", { 0x2cbc, 0x1234, 0x5678, }, 3 },
	{ "MOVE.W (A7),D5", { 0x3a17, }, 1 },
	{ "MOVE.L #$12345678,([$12345678.L,A1])", { 0x23bc, 0x1234, 0x5678, 0x0171, 0x1234, 0x5678, }, 6 },
	{ "MOVE.L ([$12345678.L,A1]),([$12345678.L,A2])", { 0x25b1, 0x0171, 0x1234, 0x5678, 0x0171, 0x1234, 0x5678, }, 7 },
	{ "MOVE.L ([$12345678.L,PC]),([D0],$12345678.L)", { 0x21bb, 0x0171, 0x1234, 0x5678, 0x0193, 0x1234, 0x5678, }, 7 },
	{ "MOVE.W D2,A2", { 0x3442, }, 1 },
	{ "MOVE.L A2,D3", { 0x260a, }, 1 },

	{ "MOVE CCR,$1234(A3)", { 0x42eb, 0x1234, }, 2 },
	{ "MOVE $1234(A2),CCR", { 0x44ea, 0x1234, }, 2 },

	{ "MOVE SR,$1234(A3)", { 0x40eb, 0x1234, }, 2 },
	
	{ "MOVEQ #$12,D2", { 0x7412, }, 1 },

	{ "MOVE16 (A2)+,(A3)+", { 0xf622, 0xb000, }, 2 },
	{ "MOVE16 $12345678.L,(A1)", { 0xf619, 0x1234, 0x5678, }, 3 },
	{ "MOVE16 (A2)+,$12345678.L", { 0xf602, 0x1234, 0x5678, }, 3 },
	
	{ "MOVEM.L D2-D7,-(A7)", { 0x48e7, 0x3f00, }, 2 },
	{ "MOVEM.W (A3),D0/D2", { 0x4c93, 0x0005, }, 2 },

	{ "MOVEP.W D2,$1234(A1)", { 0x0589, 0x1234, }, 2 },
	{ "MOVEP.L $1234(A2),D3", { 0x074a, 0x1234, }, 2 },
};

static const InstructionLengthDecoderTest miscellaneousTests[] =
{
	{ "BKPT #4", { 0x484c, }, 1 },
	{ "CHK.W $1234(A1),D1", { 0x43a9, 0x1234, }, 2 },
	{ "CHK.L #$12345678,D1", { 0x433c, 0x1234, 0x5678, }, 3 },
	{ "CLR.W $1234(A1)", { 0x4269, 0x1234, }, 2 },

	{ "EXT.W D2", { 0x4882, }, 1 },
	{ "EXT.L D3", { 0x48c3, }, 1 },
	{ "EXTB.L D4", { 0x49c4, }, 1 },
	{ "ILLEGAL", { 0x4afc, }, 1 },
	
	{ "LEA ([A3,D2.L*4],$1234.W),A2", { 0x45f3, 0x2d12, 0x1234, }, 3 },
	
	{ "LINK.W A3,#$1234", { 0x4e53, 0x1234, }, 2 },
	{ "LINK.L A3,#$12345678", { 0x480c, 0x1234, 0x5678, }, 3 },

	{ "NOP", { 0x4e71, }, 1 },

	{ "PEA $12(A3,D1.W)", { 0x4873, 0x1012, }, 2 },
	
	{ "RTS", { 0x4e75, }, 1 },

	{ "ST D7", { 0x50c7, }, 1 },
	{ "SNE (A2)", { 0x56d2, }, 1 },
	{ "SEQ $1234(A3)", { 0x57eb, 0x1234, }, 2 },
	
};

TestSuite testSuites[] =
{
	{ "6-bit EA decoding tests", ea6BitTests, (sizeof ea6BitTests / sizeof ea6BitTests[0]) },
	{ "Immediate source operand tests", immediateTests, (sizeof immediateTests / sizeof immediateTests[0]) },
	{ "Relative branch tests", relativeBranchTests, (sizeof relativeBranchTests / sizeof relativeBranchTests[0]) },
	{ "Absolute jump tests", absoluteJumpTests, (sizeof absoluteJumpTests / sizeof absoluteJumpTests[0]) },
	{ "Single-bit instruction tests", singleBitInstructionTests, (sizeof singleBitInstructionTests / sizeof singleBitInstructionTests[0]) },
	{ "Bit field instruction tests", bitFieldInstructionTests, (sizeof bitFieldInstructionTests / sizeof bitFieldInstructionTests[0]) },
	{ "BCD arithmetic tests", bcdArithmeticTests, (sizeof bcdArithmeticTests / sizeof bcdArithmeticTests[0]) },
	{ "Integer arithmetic tests", integerArithmeticTests, (sizeof integerArithmeticTests / sizeof integerArithmeticTests[0]) },
	{ "Shift/rotate tests", shiftRotateTests, (sizeof shiftRotateTests / sizeof shiftRotateTests[0]) },
	{ "Integer logic tests", integerLogicTests, (sizeof integerLogicTests / sizeof integerLogicTests[0]) },
	{ "Move/Exchange tests", moveTests, (sizeof moveTests / sizeof moveTests[0]) },
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