
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

PairabilityTestResult checkPairability_Test2_InstructionClassification(UOp* UOp0, UOp* UOp1)
{
	if (UOp0->pairability == Pairability_pOEP_Only)
		return PairabilityTestResult_Test2Failure_FirstInstructionIs_pOEPOnly;

	if (UOp1->pairability != Pairability_pOEP_Or_sOEP)
		return PairabilityTestResult_Test2Failure_SecondInstructionIsNot_pOEPOrsOEP;
		
	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability_Test3_AllowableAddressingModesInsOEP(UOp* UOp1)
{
	if (UOp1->aguBase == ExecutionResource_PC)
		return PairabilityTestResult_Test3Failure_SecondInstructionUsesPCRelativeAddressing;

	// No need to test for Ops with double-indirection in sOEP, beecause those will generate pOEP-only support UOps
	
	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability_Test4_AllowableOperandDataMemoryReference(UOp* UOp0, UOp* UOp1)
{
	bool UOp0HasMemoryReference = (UOp0->memoryRead || UOp0->memoryWrite);
	bool UOp1HasMemoryReference = (UOp1->memoryRead || UOp1->memoryWrite);

	if (UOp0HasMemoryReference && UOp1HasMemoryReference)
		return PairabilityTestResult_Test4Failure_BothInstructionsReferenceMemory;

	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability_Test5_RegisterConflictsOnAguResources(UOp* UOp0, UOp* UOp1)
{
	if (isRegister(UOp1->aguBase))
	{
		if (UOp1->aguBase == UOp0->aguResult)
			return PairabilityTestResult_Test5Failure_SecondInstructionBaseRegisterDependsOnFirstInstructionAguResult;
		else if (UOp1->aguBase == UOp0->ieeResult)
			return PairabilityTestResult_Test5Failure_SecondInstructionBaseRegisterDependsOnFirstInstructionIeeResult;
	}
	if (isRegister(UOp1->aguIndex))
	{
		if (UOp1->aguIndex == UOp0->aguResult)
			return PairabilityTestResult_Test5Failure_SecondInstructionIndexRegisterDependsOnFirstInstructionAguResult;
		else if (UOp1->aguIndex == UOp0->ieeResult)
			return PairabilityTestResult_Test5Failure_SecondInstructionIndexRegisterDependsOnFirstInstructionIeeResult;
	}

	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability_Test6_RegisterConflictsOnIeeResources(UOp* UOp0, UOp* UOp1)
{
	if (isRegister(UOp1->ieeA))
	{
		if (UOp1->ieeA == UOp0->aguResult)
			return PairabilityTestResult_Test6Failure_SecondInstructionIeeARegisterDependsOnFirstInstructionAguResult;
		else if (UOp1->ieeA == UOp0->ieeResult)
			return PairabilityTestResult_Test6Failure_SecondInstructionIeeARegisterDependsOnFirstInstructionIeeResult;
	}
	if (isRegister(UOp1->ieeB))
	{
		if (UOp1->ieeB == UOp0->aguResult)
			return PairabilityTestResult_Test6Failure_SecondInstructionIeeBRegisterDependsOnFirstInstructionAguResult;
		else if (UOp1->ieeB == UOp0->ieeResult)
			return PairabilityTestResult_Test6Failure_SecondInstructionIeeBRegisterDependsOnFirstInstructionIeeResult;
	}

	// TODO: There are some exceptions with regard to MOVE.L operations, that can make instructions pair.
	// These exceptions should either be handled outside of the pairability tests - by rewriting the first/second instruction
	//  such that the pairing tests automatically succeed - or by adding extra logic to this pairability test.
	
	return PairabilityTestResult_Success;
}

PairabilityTestResult checkPairability(UOp* UOp0, UOp* UOp1)
{
	PairabilityTestResult test2Result = checkPairability_Test2_InstructionClassification(UOp0, UOp1);
	PairabilityTestResult test3Result = checkPairability_Test3_AllowableAddressingModesInsOEP(UOp1);
	PairabilityTestResult test4Result = checkPairability_Test4_AllowableOperandDataMemoryReference(UOp0, UOp1);
	PairabilityTestResult test5Result = checkPairability_Test5_RegisterConflictsOnAguResources(UOp0, UOp1);
	PairabilityTestResult test6Result = checkPairability_Test6_RegisterConflictsOnIeeResources(UOp0, UOp1);

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
