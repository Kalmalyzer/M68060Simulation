
#include "M68060IeeAlu.h"
#include "../Assert.h"

typedef struct
{
	const char* description;
	IeeOperation ieeOperation;
	OperationSize operationSize;
	Flags flags;
	uint32_t ieeAValue;
	uint32_t ieeBValue;
	uint32_t expectedIeeResult;
	FlagsModifier expectedFlagsModifier;

} IeeAluTest;

IeeAluTest tests[] =
{
	{ "add.b 0x12,0x123456fc", IeeOperation_Add, OperationSize_Byte, 0, 0x00000012, 0x123456fc, 0x1234560e, { 0, Flags_Extend_Mask | Flags_Carry_Mask, }, },
	{ "add.b 0xfe,0x123456fc", IeeOperation_Add, OperationSize_Byte, 0, 0xfffffffe, 0x123456fc, 0x123456fa, { 0, Flags_Extend_Mask | Flags_Negative_Mask | Flags_Carry_Mask, }, },
	{ "add.w 0x40fe,0x1234c6fc", IeeOperation_Add, OperationSize_Word, 0, 0x000040fe, 0x1234c6fc, 0x123407fa, { 0, Flags_Extend_Mask | Flags_Carry_Mask, }, },
	{ "add.w 0xfffe,0x123456fc", IeeOperation_Add, OperationSize_Word, 0, 0xfffffffe, 0x123456fc, 0x123456fa, { 0, Flags_Extend_Mask | Flags_Carry_Mask, }, },
	{ "add.l 0x10000001,0x123456fc", IeeOperation_Add, OperationSize_Long, 0, 0x10000001, 0x123456fc, 0x223456fd, { 0, 0, }, },
	{ "adda.w 0x40fe,0x1234c6fc", IeeOperation_AddA, OperationSize_Word, 0, 0x000040fe, 0x1234c6fc, 0x123507fa, { Flags_All_Mask, 0, }, },
	{ "adda.w 0x8000,0x123456fc", IeeOperation_AddA, OperationSize_Word, 0, 0x00008000, 0x123456fc, 0x1233d6fc, { Flags_All_Mask, 0, }, },
	{ "adda.w 0xfffe,0x123456fc", IeeOperation_AddA, OperationSize_Word, 0, 0xfffffffe, 0x123456fc, 0x123456fa, { Flags_All_Mask, 0, }, },
	{ "adda.l 0x10000001,0x123456fc", IeeOperation_AddA, OperationSize_Long, 0, 0x10000001, 0x123456fc, 0x223456fd, { Flags_All_Mask, 0, }, },
	{ "addx.b 0x12,0x123456fc ()", IeeOperation_AddX, OperationSize_Byte, 0, 0x00000012, 0x123456fc, 0x1234560e, { 0, Flags_Extend_Mask | Flags_Carry_Mask, }, },
	{ "addx.b 0x12,0x123456fc (X)", IeeOperation_AddX, OperationSize_Byte, Flags_Extend_Mask, 0x00000012, 0x123456fc, 0x1234560f, { 0, Flags_Extend_Mask | Flags_Carry_Mask, }, },
	{ "addx.b 0,0x12345600 (Z)", IeeOperation_AddX, OperationSize_Byte, Flags_Zero_Mask, 0x00000000, 0x12345600, 0x12345600, { 0, Flags_Zero_Mask, }, },
	{ "addx.b 0,0x12345600 (XZ)", IeeOperation_AddX, OperationSize_Byte, Flags_Extend_Mask | Flags_Zero_Mask, 0x00000000, 0x12345600, 0x12345601, { 0, 0, }, },
	{ "sub.b 0x12,0x123456fc", IeeOperation_Sub, OperationSize_Byte, 0, 0x00000012, 0x123456fc, 0x123456ea, { 0, Flags_Negative_Mask, }, },
	{ "sub.b 0x7f,0x123456fc", IeeOperation_Sub, OperationSize_Byte, 0, 0x0000007f, 0x123456fc, 0x1234567d, { 0, Flags_Overflow_Mask, }, },
	{ "sub.b 0xff,0x123456fc", IeeOperation_Sub, OperationSize_Byte, 0, 0x000000ff, 0x123456fc, 0x123456fd, { 0, Flags_Extend_Mask | Flags_Negative_Mask | Flags_Carry_Mask, }, },
	{ "sub.l 0x10000001,0x023456fc", IeeOperation_Sub, OperationSize_Long, 0, 0x10000001, 0x023456fc, 0xf23456fb, { 0, Flags_Extend_Mask | Flags_Negative_Mask | Flags_Carry_Mask, }, },
	{ "suba.w 0x8000,0x123456fc", IeeOperation_SubA, OperationSize_Word, 0, 0x00008000, 0x123456fc, 0x1234d6fc, { Flags_All_Mask, 0, }, },
	{ "suba.l 0x10000001,0x123456fc", IeeOperation_SubA, OperationSize_Long, 0, 0x10000001, 0x123456fc, 0x023456fb, { Flags_All_Mask, 0, }, },
	{ "subx.w 0x1234,0x12345678 (X)", IeeOperation_SubX, OperationSize_Word, Flags_Extend_Mask, 0x00001234, 0x12345678, 0x12344445, { 0, 0, }, },
	{ "cmp.b 0x7f,0xff", IeeOperation_Cmp, OperationSize_Byte, 0, 0x0000007f, 0x000000ff, 0x00000000, { Flags_Extend_Mask, Flags_Negative_Mask, }, },
	{ "cmp.b 0x20,0x40", IeeOperation_Cmp, OperationSize_Byte, 0, 0x00000020, 0x00000040, 0x00000000, { Flags_Extend_Mask, 0, }, },
	{ "cmp.w 0x0040,0x0020", IeeOperation_Cmp, OperationSize_Word, 0, 0x00000040, 0x00000020, 0x00000000, { Flags_Extend_Mask, Flags_Negative_Mask | Flags_Carry_Mask, }, },
	{ "cmpa.w 0x7fff,0x000f0001", IeeOperation_CmpA, OperationSize_Word, 0, 0x00007fff, 0x000f0001, 0x00000000, { Flags_Extend_Mask, 0, }, },
	{ "cmpa.w 0x7fff,0x00000001", IeeOperation_CmpA, OperationSize_Word, 0, 0x00007fff, 0x00000001, 0x00000000, { Flags_Extend_Mask, Flags_Negative_Mask | Flags_Carry_Mask, }, },
	{ "cmpa.w 0x8000,0x00000001", IeeOperation_CmpA, OperationSize_Word, 0, 0x00008000, 0x00000001, 0x00000000, { Flags_Extend_Mask, Flags_Carry_Mask, }, },
	{ "neg.w 0xff00", IeeOperation_Neg, OperationSize_Word, 0, 0x00000000, 0x0000ff00, 0x00000100, { 0, Flags_Extend_Mask | Flags_Carry_Mask, }, },
	{ "negx.w 0xff00 ()", IeeOperation_NegX, OperationSize_Word, 0, 0x00000000, 0x0000ff00, 0x00000100, { 0, Flags_Extend_Mask | Flags_Carry_Mask, }, },
	{ "negx.w 0xff00 (X)", IeeOperation_NegX, OperationSize_Word, Flags_Extend_Mask, 0x00000000, 0x0000ff00, 0x00000101, { 0, Flags_Extend_Mask | Flags_Carry_Mask, }, },
	{ "tst.w 0x0000ff00", IeeOperation_Tst, OperationSize_Word, 0, 0x00000000, 0x0000ff00, 0x00000000, { Flags_Extend_Mask, Flags_Negative_Mask, }, },
	{ "tst.w 0xffff0000", IeeOperation_Tst, OperationSize_Word, 0, 0x00000000, 0xffff0000, 0x00000000, { Flags_Extend_Mask, Flags_Zero_Mask, }, },
	{ "asl.w 0,0x12348000", IeeOperation_Asl, OperationSize_Word, 0, 0x00000000, 0x12348000, 0x12348000, { Flags_Extend_Mask, Flags_Negative_Mask, }, },
	{ "asl.w 0,0xffff1234", IeeOperation_Asl, OperationSize_Word, 0, 0x00000000, 0xffff1234, 0xffff1234, { Flags_Extend_Mask, 0, }, },
	{ "asl.w 64,0xffff1234", IeeOperation_Asl, OperationSize_Word, 0, 0x00000040, 0xffff1234, 0xffff1234, { Flags_Extend_Mask, 0, }, },
	{ "asl.w 1,0xffff1234", IeeOperation_Asl, OperationSize_Word, 0, 0x00000001, 0xffff1234, 0xffff2468, { 0, 0, }, },
	{ "asl.w 4,0x1234", IeeOperation_Asl, OperationSize_Word, 0, 0x00000004, 0x00001234, 0x00002340, { 0, Flags_Extend_Mask | Flags_Overflow_Mask | Flags_Carry_Mask, }, },
	{ "asl.w 5,0x1234", IeeOperation_Asl, OperationSize_Word, 0, 0x00000005, 0x00001234, 0x00004680, { 0, Flags_Overflow_Mask, }, },
	{ "asl.w 15,0x1234", IeeOperation_Asl, OperationSize_Word, 0, 0x0000000f, 0x00001234, 0x00000000, { 0, Flags_Overflow_Mask | Flags_Zero_Mask, }, },
	{ "asl.w 16,0x1234", IeeOperation_Asl, OperationSize_Word, 0, 0x00000010, 0x00001234, 0x00000000, { 0, Flags_Overflow_Mask | Flags_Zero_Mask, }, },
	{ "asl.w 32,0x1234", IeeOperation_Asl, OperationSize_Word, 0, 0x00000020, 0x00001234, 0x00000000, { 0, Flags_Overflow_Mask | Flags_Zero_Mask, }, },
	{ "asl.w 33,0x1234", IeeOperation_Asl, OperationSize_Word, 0, 0x00000021, 0x00001234, 0x00000000, { 0, Flags_Overflow_Mask | Flags_Zero_Mask, }, },
	{ "asl.w 63,0x1234", IeeOperation_Asl, OperationSize_Word, 0, 0x0000003f, 0x00001234, 0x00000000, { 0, Flags_Overflow_Mask | Flags_Zero_Mask, }, },
	{ "asr.l 4,0x12345678", IeeOperation_Asr, OperationSize_Long, 0, 0x00000004, 0x12345678, 0x01234567, { 0, Flags_Extend_Mask | Flags_Carry_Mask, }, },
	{ "asr.l 4,0x87654321", IeeOperation_Asr, OperationSize_Long, 0, 0x00000004, 0x87654321, 0xf8765432, { 0, Flags_Negative_Mask, }, },
	{ "lsl.w 4,0x1234", IeeOperation_Lsl, OperationSize_Word, 0, 0x00000004, 0x00001234, 0x00002340, { 0, Flags_Extend_Mask | Flags_Carry_Mask, }, },
	{ "lsr.w 4,0x8765", IeeOperation_Lsr, OperationSize_Word, 0, 0x00000004, 0x00008765, 0x00000876, { 0, 0, }, },
	{ "lsr.l 4,0x87654321", IeeOperation_Lsr, OperationSize_Long, 0, 0x00000004, 0x87654321, 0x08765432, { 0, 0, }, },
	{ "rol.b 5,0x12", IeeOperation_Rol, OperationSize_Byte, 0, 0x00000005, 0x00000012, 0x00000042, { Flags_Extend_Mask, 0, }, },
	{ "ror.l 1,87654321", IeeOperation_Ror, OperationSize_Long, 0, 0x00000001, 0x87654321, 0xc3b2a190, { Flags_Extend_Mask, Flags_Negative_Mask | Flags_Carry_Mask, }, },
	{ "roxl.w 2,0x12", IeeOperation_Roxl, OperationSize_Word, Flags_Extend_Mask, 0x00000002, 0x00000012, 0x0000004a, { 0, 0, }, },
	{ "roxl.w 2,0x4000", IeeOperation_Roxl, OperationSize_Word, 0, 0x00000002, 0x0000004000, 0x00000000, { 0, Flags_Extend_Mask | Flags_Zero_Mask | Flags_Carry_Mask, }, },
	{ "roxl.w 0,0x1234", IeeOperation_Roxl, OperationSize_Word, Flags_Extend_Mask, 0x00000000, 0x0000001234, 0x00001234, { Flags_Extend_Mask, Flags_Carry_Mask, }, },
	{ "roxr.l 1,87654321", IeeOperation_Roxr, OperationSize_Long, 0, 0x00000001, 0x87654321, 0x43b2a190, { 0, Flags_Extend_Mask | Flags_Carry_Mask, }, },
	{ "roxr.l 1,87654321", IeeOperation_Roxr, OperationSize_Long, Flags_Extend_Mask, 0x00000001, 0x87654321, 0xc3b2a190, { 0, Flags_Extend_Mask | Flags_Negative_Mask | Flags_Carry_Mask, }, },
	{ "and.b 0x0f,0x12345678", IeeOperation_And, OperationSize_Byte, 0, 0x0000000f, 0x12345678, 0x12345608, { Flags_Extend_Mask, 0, }, },
	{ "or.w 0xff0f,0x12345678", IeeOperation_Or, OperationSize_Word, 0, 0x0000ff0f, 0x12345678, 0x1234ff7f, { Flags_Extend_Mask, Flags_Negative_Mask, }, },
	{ "eor.w 0xff0f,0x12345678", IeeOperation_Eor, OperationSize_Word, 0, 0x0000ff0f, 0x12345678, 0x1234a977, { Flags_Extend_Mask, Flags_Negative_Mask, }, },
	{ "not.w 0x1234ff0f", IeeOperation_Not, OperationSize_Word, 0, 0x00000000, 0x1234ff0f, 0x123400f0, { Flags_Extend_Mask, 0, }, },
};

const char* flagsToString(uint flags)
{
	const char* flagStrings[32] = {
		"(none)", "C", "V", "VC", "Z", "ZC", "ZV", "ZVC", "N", "NC", "NV", "NVC", "NZ", "NZC", "NZV", "NZVC",
		"X", "XC", "XV", "XVC", "XZ", "XZC", "XZV", "XZVC", "XN", "XNC", "XNV", "XNVC", "XNZ", "XNZC", "XNZV", "XNZVC",
	};

	M68060_ASSERT(flags < (sizeof flagStrings / sizeof flagStrings[0]), "Unsupported flags value");
	
	return flagStrings[flags];
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
		FlagsModifier flagsModifier;
		bool correctIeeResult = true;
		bool correctFlagsModifierAnd = true;
		bool correctFlagsModifierOr = true;
		
		evaluateIeeAluOperation(test->ieeOperation, test->operationSize, test->flags, test->ieeAValue, test->ieeBValue, &ieeResult, &flagsModifier);

		if (ieeResult != test->expectedIeeResult)
			correctIeeResult = false;
		if (flagsModifier.andFlags != test->expectedFlagsModifier.andFlags)
			correctFlagsModifierAnd = false;
		if (flagsModifier.orFlags != test->expectedFlagsModifier.orFlags)
			correctFlagsModifierOr = false;

		success = correctIeeResult && correctFlagsModifierAnd && correctFlagsModifierOr;
			
		if (success && printSuccess)
			printf("success: Invoking %s yields ieeResult %08X, flagsModifer AND %s, OR %s\n", test->description, ieeResult, flagsToString(flagsModifier.andFlags), flagsToString(flagsModifier.orFlags));
		if (!success && printFailure)
		{
			printf("failure: Invoking %s yields ieeResult %08X", test->description, ieeResult);
			if (!correctIeeResult)
				printf(" (should be %08X)", test->expectedIeeResult);
			printf(", flagsModifier AND %s", flagsToString(flagsModifier.andFlags));
			if (!correctFlagsModifierAnd)
				printf(" (should be %s)", flagsToString(test->expectedFlagsModifier.andFlags));
			printf(", OR %s", flagsToString(flagsModifier.orFlags));
			if (!correctFlagsModifierOr)
				printf(" (should be %s)", flagsToString(test->expectedFlagsModifier.orFlags));
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

	printf("Testing M68060 IEE-ALU\n");
	printf("\n");

	runTestSuite(tests, sizeof tests / sizeof tests[0], printSuccess, printFailure, &numSuccessfulTests, &numTotalTests);

	printf("\n");
	printf("%u out of %u tests succeeded.\n", numSuccessfulTests, numTotalTests);

	return 0;
}