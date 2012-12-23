
#include "M68060DecodeOpIntoUOps.h"
#include "M68060PairabilityTests.h"
#include "../Types.h"

typedef struct
{
	const char* description;
	uint numWords;
	uint16_t instructionWords[16];
} TestInstruction;

typedef struct
{
	TestInstruction instruction0;
	TestInstruction instruction1;
	PairabilityTestResult expectedResult;
} PairabilityTest;

int main(void)
{
	static const PairabilityTest pairabilityTests[] =
	{
		{	{ "ADD.B D1,D1",			1, { 0xd201, }, },	// pOEP|sOEP
			{ "ADD.B D0,D0", 			1, { 0xd000, }, },	// pOEP|sOEP
			PairabilityTestResult_Success },

		{	{ "ADD.W $12(A3,D0.L),D5",	2, { 0xda73, 0x0012, }, },	// pOEP|sOEP
			{ "ADD.B D0,D0", 			1, { 0xd000, }, },			// pOEP|sOEP
			PairabilityTestResult_Success },

		{	{ "ADD.W $1234(PC),D5",		2, { 0xda7a, 0x1234}, },	// pOEP|sOEP
			{ "ADD.B D0,D0", 			1, { 0xd000, }, },			// pOEP|sOEP
			PairabilityTestResult_Success },

		{	{ "ADDX.L D1,D1", 			1, { 0xd381, }, },	// pOEP-only
			{ "ADD.B D0,D0",			1, { 0xd000, }, },	// pOEP|sOEP	<- instruction class mismatch
			PairabilityTestResult_Test2Failure_FirstInstructionIs_pOEPOnly },

		{	{ "ADD.B D0,D0",			1, { 0xd000, }, },			// pOEP|sOEP
			{ "ADD.W $1234(PC),D5",		2, { 0xda7a, 0x1234, }, },	// pOEP|sOEP			<- PC relative addressing modes not allowed in sOEP
			PairabilityTestResult_Test3Failure_SecondInstructionUsesPCRelativeAddressing },

		{	{ "ADDA.W D0,A2",			1, { 0xd4c0, }, },	// pOEP|sOEP
			{ "ADD.W (A3),D1",			1, { 0xd213, }, }, 	// pOEP|sOEP
			PairabilityTestResult_Success },

		{	{ "ADD.W (A3),D1",			1, { 0xd213, }, }, 	// pOEP|sOEP
			{ "ADD.W (A3),D1",			1, { 0xd213, }, }, 	// pOEP|sOEP			<- Both instructions have a memory reference
			PairabilityTestResult_Test4Failure_BothInstructionsReferenceMemory },
			
		{	{ "ADDA.W D0,A3",			1, { 0xd6c0, }, },	// pOEP|sOEP
			{ "ADD.W (A3),D1",			1, { 0xd213, }, }, 	// pOEP|sOEP			<- Second instruction uses base register which is written by first instruction
			PairabilityTestResult_Test5Failure_SecondInstructionBaseRegisterDependsOnFirstInstructionIeeResult },

		{	{ "ADD.B D0,D0",			1, { 0xd000, }, },	// pOEP|sOEP
			{ "ADD.B D0,D0",			1, { 0xd000, }, },	// pOEP|sOEP	<- register dependency
			PairabilityTestResult_Test6Failure_SecondInstructionIeeARegisterDependsOnFirstInstructionIeeResult },

		{	{ "ADD.B D0,D0",			1, { 0xd000, }, },	// pOEP|sOEP
			{ "ADD.B D1,D0",			1, { 0xd001, }, },	// pOEP|sOEP	<- register dependency
			PairabilityTestResult_Test6Failure_SecondInstructionIeeBRegisterDependsOnFirstInstructionIeeResult },

		{	{ "ADD.B (A0)+,D0",			1, { 0xd018, }, },	// pOEP|sOEP
			{ "ADD.W A0,D1",			1, { 0xd248, }, },	// pOEP|sOEP	<- register dependency
			PairabilityTestResult_Test6Failure_SecondInstructionIeeARegisterDependsOnFirstInstructionAguResult },

		{	{ "ADD.B (A0)+,D0",			1, { 0xd018, }, },	// pOEP|sOEP
			{ "ADDA.W D1,A0",			1, { 0xd0c1, }, },	// pOEP|sOEP	<- register dependency
			PairabilityTestResult_Test6Failure_SecondInstructionIeeBRegisterDependsOnFirstInstructionAguResult },
	};
	
	uint i;

	for (i = 0; i < (sizeof pairabilityTests / sizeof pairabilityTests[0]); ++i)
	{
		bool success = true;
		const PairabilityTest* test = &pairabilityTests[i];
		uint numUOps0;
		UOp UOps0[16];
		uint numUOps1;
		UOp UOps1[16];
		
		bool decode0Success = decomposeOpIntoUOps(test->instruction0.instructionWords, test->instruction0.numWords, UOps0, &numUOps0);
		bool decode1Success = decomposeOpIntoUOps(test->instruction1.instructionWords, test->instruction1.numWords, UOps1, &numUOps1);

		bool op0HasMultipleUOps = false;
		bool op1HasMultipleUOps = false;
		
		PairabilityTestResult pairabilityTestResult;
		
		if (!decode0Success)
			success = false;
		if (!decode1Success)
			success = false;

		if (success)
		{
			op0HasMultipleUOps = (numUOps0 > 1);
			op1HasMultipleUOps = (numUOps1 > 1);
		}

		if (op0HasMultipleUOps || op1HasMultipleUOps)
			success = false;
		
		if (success)
			pairabilityTestResult = checkPairability(&UOps0[0], &UOps1[0]);
			
		if (pairabilityTestResult != test->expectedResult)
			success = false;
			
		if (success)
		{
			printf("success: Testing %s and %s yields %s\n", test->instruction0.description, test->instruction1.description,
				PairabilityTestResultToString(pairabilityTestResult));
		}
		else
		{
			printf("failure: Testing %s and %s - ", test->instruction0.description, test->instruction1.description);
			if (!decode0Success)
				printf("instruction 0 does not decode properly\n");
			else if (!decode1Success)
				printf("instruction 1 does not decode properly\n");
			else if (op0HasMultipleUOps)
				printf("instruction 0 decodes into multiple UOps and will therefore not pair\n");
			else if (op1HasMultipleUOps)
				printf("instruction 1 decodes into multiple UOps and will therefore not pair\n");
			else
				printf("result %s - expected %s\n", 
					PairabilityTestResultToString(pairabilityTestResult),
					PairabilityTestResultToString(test->expectedResult));
		}
	}
	
	return 0;
}