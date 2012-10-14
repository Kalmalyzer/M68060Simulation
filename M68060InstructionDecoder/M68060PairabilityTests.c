
#include "M68060PairabilityTests.h"
#include "../Assert.h"

const char* PairabilityTestResultToString(PairabilityTestResult pairabilityTestResult)
{
	static const char* pairabilityTestResultStrings[] =
	{
		"Success",
		"Test2Failure_FirstInstructionIs_pOEPOnly",
		"Test2Failure_SecondInstructionIsNot_pOEPOrsOEP",
	};

	M68060_ASSERT((size_t) pairabilityTestResult < (sizeof pairabilityTestResultStrings / sizeof pairabilityTestResultStrings[0]), "Invalid pairabilityTestResult");
	
	return pairabilityTestResultStrings[(int) pairabilityTestResult];
}

PairabilityTestResult checkPairability_Test2_InstructionClassification(DecodedOpWord* opWord0, DecodedOpWord* opWord1)
{
	if (opWord0->pairability == Pairability_pOEP_Only)
		return PairabilityTestResult_Test2Failure_FirstInstructionIs_pOEPOnly;

	if (opWord1->pairability != Pairability_pOEP_Or_sOEP)
		return PairabilityTestResult_Test2Failure_SecondInstructionIsNot_pOEPOrsOEP;
		
	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability(DecodedOpWord* opWord0, DecodedOpWord* opWord1)
{
	PairabilityTestResult test2Result = checkPairability_Test2_InstructionClassification(opWord0, opWord1);
	if (test2Result != PairabilityTestResult_Success)
		return test2Result;

	return PairabilityTestResult_Success;
}
