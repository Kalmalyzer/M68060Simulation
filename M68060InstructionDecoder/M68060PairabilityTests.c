
#include "M68060PairabilityTests.h"
#include "../Assert.h"

const char* PairabilityTestResultToString(PairabilityTestResult pairabilityTestResult)
{
	static const char* pairabilityTestResultStrings[] =
	{
		"Success",
		"Test2Failure_FirstInstructionIs_pOEPOnly",
		"Test2Failure_SecondInstructionIsNot_pOEPOrsOEP",
		"Test3Failure_SecondInstructionUsesPCRelativeAddressing",
		"Test4Failure_BothInstructionsReferenceMemory",
		"Test5Failure_SecondInstructionBaseRegisterDependsOnFirstInstructionAguResult",
		"Test5Failure_SecondInstructionBaseRegisterDependsOnFirstInstructionIeeResult",
		"Test5Failure_SecondInstructionIndexRegisterDependsOnFirstInstructionAguResult",
		"Test5Failure_SecondInstructionIndexRegisterDependsOnFirstInstructionIeeResult",
		"Test6Failure_SecondInstructionIeeARegisterDependsOnFirstInstructionAguResult",
		"Test6Failure_SecondInstructionIeeARegisterDependsOnFirstInstructionIeeResult",
		"Test6Failure_SecondInstructionIeeBRegisterDependsOnFirstInstructionAguResult",
		"Test6Failure_SecondInstructionIeeBRegisterDependsOnFirstInstructionIeeResult",
	};

	M68060_ASSERT((size_t) pairabilityTestResult < (sizeof pairabilityTestResultStrings / sizeof pairabilityTestResultStrings[0]), "Invalid pairabilityTestResult");
	
	return pairabilityTestResultStrings[(int) pairabilityTestResult];
}

PairabilityTestResult checkPairability_Test2_InstructionClassification(uOP* uOP0, uOP* uOP1)
{
	if (uOP0->pairability == Pairability_pOEP_Only)
		return PairabilityTestResult_Test2Failure_FirstInstructionIs_pOEPOnly;

	if (uOP1->pairability != Pairability_pOEP_Or_sOEP)
		return PairabilityTestResult_Test2Failure_SecondInstructionIsNot_pOEPOrsOEP;
		
	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability_Test3_AllowableAddressingModesInsOEP(uOP* uOP1)
{
	if (uOP1->aguBase == ExecutionResource_PC)
		return PairabilityTestResult_Test3Failure_SecondInstructionUsesPCRelativeAddressing;

	// No need to test for Ops with double-indirection in sOEP, beecause those will generate pOEP-only support uOPs
	
	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability_Test4_AllowableOperandDataMemoryReference(uOP* uOP0, uOP* uOP1)
{
	bool uOP0HasMemoryReference = (uOP0->memoryRead || uOP0->memoryWrite);
	bool uOP1HasMemoryReference = (uOP1->memoryRead || uOP1->memoryWrite);

	if (uOP0HasMemoryReference && uOP1HasMemoryReference)
		return PairabilityTestResult_Test4Failure_BothInstructionsReferenceMemory;

	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability_Test5_RegisterConflictsOnAguResources(uOP* uOP0, uOP* uOP1)
{
	if (isRegister(uOP1->aguBase))
	{
		if (uOP1->aguBase == uOP0->aguResult)
			return PairabilityTestResult_Test5Failure_SecondInstructionBaseRegisterDependsOnFirstInstructionAguResult;
		else if (uOP1->aguBase == uOP0->ieeResult)
			return PairabilityTestResult_Test5Failure_SecondInstructionBaseRegisterDependsOnFirstInstructionIeeResult;
	}
	if (isRegister(uOP1->aguIndex))
	{
		if (uOP1->aguIndex == uOP0->aguResult)
			return PairabilityTestResult_Test5Failure_SecondInstructionIndexRegisterDependsOnFirstInstructionAguResult;
		else if (uOP1->aguIndex == uOP0->ieeResult)
			return PairabilityTestResult_Test5Failure_SecondInstructionIndexRegisterDependsOnFirstInstructionIeeResult;
	}

	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability_Test6_RegisterConflictsOnIeeResources(uOP* uOP0, uOP* uOP1)
{
	if (isRegister(uOP1->ieeA))
	{
		if (uOP1->ieeA == uOP0->aguResult)
			return PairabilityTestResult_Test6Failure_SecondInstructionIeeARegisterDependsOnFirstInstructionAguResult;
		else if (uOP1->ieeA == uOP0->ieeResult)
			return PairabilityTestResult_Test6Failure_SecondInstructionIeeARegisterDependsOnFirstInstructionIeeResult;
	}
	if (isRegister(uOP1->ieeB))
	{
		if (uOP1->ieeB == uOP0->aguResult)
			return PairabilityTestResult_Test6Failure_SecondInstructionIeeBRegisterDependsOnFirstInstructionAguResult;
		else if (uOP1->ieeB == uOP0->ieeResult)
			return PairabilityTestResult_Test6Failure_SecondInstructionIeeBRegisterDependsOnFirstInstructionIeeResult;
	}

	// TODO: There are some exceptions with regard to MOVE.L operations, that can make instructions pair.
	// These exceptions should either be handled outside of the pairability tests - by rewriting the first/second instruction
	//  such that the pairing tests automatically succeed - or by adding extra logic to this pairability test.
	
	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability(uOP* uOP0, uOP* uOP1)
{
	PairabilityTestResult test2Result = checkPairability_Test2_InstructionClassification(uOP0, uOP1);
	PairabilityTestResult test3Result = checkPairability_Test3_AllowableAddressingModesInsOEP(uOP1);
	PairabilityTestResult test4Result = checkPairability_Test4_AllowableOperandDataMemoryReference(uOP0, uOP1);
	PairabilityTestResult test5Result = checkPairability_Test5_RegisterConflictsOnAguResources(uOP0, uOP1);
	PairabilityTestResult test6Result = checkPairability_Test6_RegisterConflictsOnIeeResources(uOP0, uOP1);

	if (test2Result != PairabilityTestResult_Success)
		return test2Result;

	if (test3Result != PairabilityTestResult_Success)
		return test3Result;

	if (test4Result != PairabilityTestResult_Success)
		return test4Result;

	if (test5Result != PairabilityTestResult_Success)
		return test5Result;

	if (test6Result != PairabilityTestResult_Success)
		return test6Result;

	return PairabilityTestResult_Success;
}
