
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

PairabilityTestResult checkPairability_Test2_InstructionClassification(DecodedOpWord* opWord0, DecodedOpWord* opWord1)
{
	if (opWord0->pairability == Pairability_pOEP_Only)
		return PairabilityTestResult_Test2Failure_FirstInstructionIs_pOEPOnly;

	if (opWord1->pairability != Pairability_pOEP_Or_sOEP)
		return PairabilityTestResult_Test2Failure_SecondInstructionIsNot_pOEPOrsOEP;
		
	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability_Test3_AllowableAddressingModesInsOEP(DecodedOpWord* opWord1)
{
	if (opWord1->aguBase == ExecutionResource_PC)
		return PairabilityTestResult_Test3Failure_SecondInstructionUsesPCRelativeAddressing;

	// TODO: return failure if using {(bd, An, Xi*SF)} addressing mode
	
	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability_Test4_AllowableOperandDataMemoryReference(DecodedOpWord* opWord0, DecodedOpWord* opWord1)
{
	if (opWord0->hasMemoryReference && opWord1->hasMemoryReference)
		return PairabilityTestResult_Test4Failure_BothInstructionsReferenceMemory;

	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability_Test5_RegisterConflictsOnAguResources(DecodedOpWord* opWord0, DecodedOpWord* opWord1)
{
	if (isRegister(opWord1->aguBase))
	{
		if (opWord1->aguBase == opWord0->aguResult)
			return PairabilityTestResult_Test5Failure_SecondInstructionBaseRegisterDependsOnFirstInstructionAguResult;
		else if (opWord1->aguBase == opWord0->ieeResult)
			return PairabilityTestResult_Test5Failure_SecondInstructionBaseRegisterDependsOnFirstInstructionIeeResult;
	}
	if (isRegister(opWord1->aguIndex))
	{
		if (opWord1->aguIndex == opWord0->aguResult)
			return PairabilityTestResult_Test5Failure_SecondInstructionIndexRegisterDependsOnFirstInstructionAguResult;
		else if (opWord1->aguIndex == opWord0->ieeResult)
			return PairabilityTestResult_Test5Failure_SecondInstructionIndexRegisterDependsOnFirstInstructionIeeResult;
	}

	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability_Test6_RegisterConflictsOnIeeResources(DecodedOpWord* opWord0, DecodedOpWord* opWord1)
{
	if (isRegister(opWord1->ieeA))
	{
		if (opWord1->ieeA == opWord0->aguResult)
			return PairabilityTestResult_Test6Failure_SecondInstructionIeeARegisterDependsOnFirstInstructionAguResult;
		else if (opWord1->ieeA == opWord0->ieeResult)
			return PairabilityTestResult_Test6Failure_SecondInstructionIeeARegisterDependsOnFirstInstructionIeeResult;
	}
	if (isRegister(opWord1->ieeB))
	{
		if (opWord1->ieeB == opWord0->aguResult)
			return PairabilityTestResult_Test6Failure_SecondInstructionIeeBRegisterDependsOnFirstInstructionAguResult;
		else if (opWord1->ieeB == opWord0->ieeResult)
			return PairabilityTestResult_Test6Failure_SecondInstructionIeeBRegisterDependsOnFirstInstructionIeeResult;
	}

	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability(DecodedOpWord* opWord0, DecodedOpWord* opWord1)
{
	PairabilityTestResult test2Result = checkPairability_Test2_InstructionClassification(opWord0, opWord1);
	PairabilityTestResult test3Result = checkPairability_Test3_AllowableAddressingModesInsOEP(opWord1);
	PairabilityTestResult test4Result = checkPairability_Test4_AllowableOperandDataMemoryReference(opWord0, opWord1);
	PairabilityTestResult test5Result = checkPairability_Test5_RegisterConflictsOnAguResources(opWord0, opWord1);
	PairabilityTestResult test6Result = checkPairability_Test6_RegisterConflictsOnIeeResources(opWord0, opWord1);

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
