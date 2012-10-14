
#include "M68060OpWordDecoder.h"
#include "M68060PairabilityTests.h"
#include "../Types.h"

typedef struct
{
	uint16_t opWord0;
	uint16_t opWord1;
	PairabilityTestResult expectedResult;
} PairabilityTest;

int main(void)
{
	static const PairabilityTest pairabilityTests[] =
	{
		{ 0x4e71,	// NOP					pOEP-only
		  0xd000,	// ADD.B D0,D0			pOEP|sOEP	<- instruction class mismatch
		  PairabilityTestResult_Test2Failure_FirstInstructionIs_pOEPOnly },

		{ 0xd201,	// ADD.B D1,D1			pOEP|sOEP
		  0xd000,	// ADD.B D0,D0			pOEP|sOEP
		  PairabilityTestResult_Success },

		{ 0xda73,	// ADD.W d8(A3,Xn.q),D5	pOEP-until-last		<- not a standard instruction due to Xn not being part of opword
		  0xd000,	// ADD.B D0,D0			pOEP|sOEP
		  PairabilityTestResult_Success },

		{ 0xda7a,	// ADD.W d16(PC),D5		pOEP|sOEP
		  0xd000,	// ADD.B D0,D0			pOEP|sOEP
		  PairabilityTestResult_Success },

		{ 0xd000,	// ADD.B D0,D0			pOEP|sOEP
		  0xda7a,	// ADD.W d16(PC),D5		pOEP|sOEP			<- PC relative addressing modes not allowed in sOEP
		  PairabilityTestResult_Test3Failure_SecondInstructionUsesPCRelativeAddressing },

		{ 0xd1ca,	// ADD.W D0,A2			pOEP|sOEP
		  0xd213, 	// ADD.W (A3),D1		pOEP|sOEP
		  PairabilityTestResult_Success },
		  
		{ 0xd1cb,	// ADDA.W D0,A3			pOEP|sOEP
		  0xd213, 	// ADD.W (A3),D1		pOEP|sOEP			<- Second instruction uses base register which is written by first instruction
		  PairabilityTestResult_Test5Failure_SecondInstructionBaseRegisterDependsOnFirstInstructionIeeResult },

		{ 0xd000,	// ADD.B D0,D0			pOEP|sOEP
		  0xd000,	// ADD.B D0,D0			pOEP|sOEP	<- register dependency
		  PairabilityTestResult_Test6Failure_SecondInstructionIeeARegisterDependsOnFirstInstructionIeeResult },

		{ 0xd000,	// ADD.B D0,D0			pOEP|sOEP
		  0xd001,	// ADD.B D1,D0			pOEP|sOEP	<- register dependency
		  PairabilityTestResult_Test6Failure_SecondInstructionIeeBRegisterDependsOnFirstInstructionIeeResult },

		{ 0xd018,	// ADD.B (A0)+,D0		pOEP|sOEP
		  0xd248,	// ADD.W A0,D1			pOEP|sOEP	<- register dependency
		  PairabilityTestResult_Test6Failure_SecondInstructionIeeARegisterDependsOnFirstInstructionAguResult },

		{ 0xd018,	// ADD.B (A0)+,D0		pOEP|sOEP
		  0xd3c8,	// ADDA.W D1,A0			pOEP|sOEP	<- register dependency
		  PairabilityTestResult_Test6Failure_SecondInstructionIeeBRegisterDependsOnFirstInstructionAguResult },
	};
	
	uint i;

	for (i = 0; i < (sizeof pairabilityTests / sizeof pairabilityTests[0]); ++i)
	{
		const PairabilityTest* pairabilityTest = &pairabilityTests[i];
		uint16_t opWord0 = pairabilityTest->opWord0;
		uint16_t opWord1 = pairabilityTest->opWord1;
		DecodedOpWord decodedOpWord0 = decodeOpWord(opWord0);
		DecodedOpWord decodedOpWord1 = decodeOpWord(opWord1);
		PairabilityTestResult testResult = checkPairability(&decodedOpWord0, &decodedOpWord1);

		if (testResult == pairabilityTest->expectedResult)
			printf("Testing %04x and %04x: success, result %s\n", opWord0, opWord1, PairabilityTestResultToString(testResult));
		else
			printf("Testing %04x and %04x: failure, result %s - expected %s\n", opWord0, opWord1,
				PairabilityTestResultToString(testResult),
				PairabilityTestResultToString(pairabilityTest->expectedResult));
	}
	
	return 0;
}