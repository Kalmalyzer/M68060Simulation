
#include "M68060IeeAlu.h"
#include "../Assert.h"

typedef enum
{
	Minterm_Sm_Dm_Rm = 0x01,
	Minterm_Sm_Dm_InvRm = 0x02,
	Minterm_Sm_InvDm_Rm = 0x04,
	Minterm_Sm_InvDm_InvRm = 0x08,
	Minterm_InvSm_Dm_Rm = 0x10,
	Minterm_InvSm_Dm_InvRm = 0x20,
	Minterm_InvSm_InvDm_Rm = 0x40,
	Minterm_InvSm_InvDm_InvRm = 0x80,

} Minterms;

static bool computeMintermResult(bool sm, bool dm, bool rm, Minterms minterms)
{
	bool minterm0 = sm && dm && rm;
	bool minterm1 = sm && dm && !rm;
	bool minterm2 = sm && !dm && rm;
	bool minterm3 = sm && !dm && !rm;
	bool minterm4 = !sm && dm && rm;
	bool minterm5 = !sm && dm && !rm;
	bool minterm6 = !sm && !dm && rm;
	bool minterm7 = !sm && !dm && !rm;

	bool result = (minterm0 && (minterms & Minterm_Sm_Dm_Rm))
		|| (minterm1 && (minterms & Minterm_Sm_Dm_InvRm))
		|| (minterm2 && (minterms & Minterm_Sm_InvDm_Rm))
		|| (minterm3 && (minterms & Minterm_Sm_InvDm_InvRm))
		|| (minterm4 && (minterms & Minterm_InvSm_Dm_Rm))
		|| (minterm5 && (minterms & Minterm_InvSm_Dm_InvRm))
		|| (minterm6 && (minterms & Minterm_InvSm_InvDm_Rm))
		|| (minterm7 && (minterms & Minterm_InvSm_InvDm_InvRm));
		
	return result;
}

static uint32_t mask(uint32_t a, uint32_t b, uint32_t maskField)
{
	return (a & maskField) | (b & ~maskField);
}

static void setEmptyFlagsModifier(FlagsModifier* flagsModifier)
{
	flagsModifier->andFlags = Flags_All_Mask;
	flagsModifier->orFlags = 0;
}

static void setFlagsModifierXNZVC(bool x, bool n, bool z, bool v, bool c, FlagsModifier* flagsModifier)
{
	flagsModifier->orFlags = (x ? Flags_Extend_Mask : 0) | (n ? Flags_Negative_Mask : 0) | (z ? Flags_Zero_Mask : 0)| (v ? Flags_Overflow_Mask : 0) | (c ? Flags_Carry_Mask : 0);
	flagsModifier->andFlags = 0;
}

static uint getOperationMask(OperationSize operationSize)
{
	switch (operationSize)
	{
		case OperationSize_Byte:
			return 0x000000ff;
		case OperationSize_Word:
			return 0x0000ffff;
		case OperationSize_Long:
			return 0xffffffff;
		default:
			M68060_ERROR("OperationSize not implemented");
			return 0;
	};
}

static void computeNZVC(uint32_t sourceValue, uint32_t destinationValue, uint32_t resultValue, uint32_t operationMask, Minterms overflowMinterms, Minterms carryMinterms, bool* n, bool* z, bool* v, bool* c)
{
	uint32_t operationHighestBitMask = operationMask - (operationMask >> 1);
	
	bool sm = (sourceValue & operationHighestBitMask) != 0;
	bool dm = (destinationValue & operationHighestBitMask) != 0;
	bool rm = (resultValue & operationHighestBitMask) != 0;
	
	*n = (resultValue & operationHighestBitMask);
	*z = !(resultValue & operationMask);
	*v = computeMintermResult(sm, dm, rm, overflowMinterms);
	*c = computeMintermResult(sm, dm, rm, carryMinterms);
}


static void evaluateAdd(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	uint32_t operationMask = getOperationMask(operationSize);

	uint32_t resultValue = ieeAValue + ieeBValue;

	Minterms overflowMinterms = Minterm_Sm_Dm_InvRm | Minterm_InvSm_InvDm_Rm;
	Minterms carryMinterms = (Minterm_Sm_Dm_Rm | Minterm_Sm_Dm_InvRm) | (Minterm_Sm_Dm_InvRm | Minterm_Sm_InvDm_InvRm) | (Minterm_Sm_Dm_InvRm | Minterm_InvSm_Dm_InvRm);
	bool n, z, v, c;

	computeNZVC(ieeAValue, ieeBValue, resultValue, operationMask, overflowMinterms, carryMinterms, &n, &z, &v, &c);

	setFlagsModifierXNZVC(c, n, z, v, c, flagsModifier);
	
	*ieeResult = mask(resultValue, ieeBValue, operationMask);
}

static void evaluateAddA(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult)
{
	switch (operationSize)
	{
		case OperationSize_Word:
			*ieeResult = ((uint32_t) (int32_t) (int16_t) ieeAValue) + ieeBValue;
			break;
		case OperationSize_Long:
			*ieeResult = ieeAValue + ieeBValue;
			break;
		case OperationSize_Byte:
		case OperationSize_None:
			M68060_ERROR("OperationSize not implemented");
	};
}

static void evaluateSub(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	uint32_t operationMask = getOperationMask(operationSize);

	uint32_t resultValue = -ieeAValue + ieeBValue;

	Minterms overflowMinterms = Minterm_InvSm_Dm_InvRm | Minterm_Sm_InvDm_Rm;
	Minterms carryMinterms = (Minterm_Sm_InvDm_Rm | Minterm_Sm_InvDm_InvRm) | (Minterm_Sm_Dm_Rm | Minterm_Sm_InvDm_Rm) | (Minterm_Sm_InvDm_Rm | Minterm_InvSm_InvDm_Rm);
	bool n, z, v, c;

	computeNZVC(ieeAValue, ieeBValue, resultValue, operationMask, overflowMinterms, carryMinterms, &n, &z, &v, &c);

	setFlagsModifierXNZVC(c, n, z, v, c, flagsModifier);
	
	*ieeResult = mask(resultValue, ieeBValue, operationMask);
}

static void evaluateSubA(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult)
{
	switch (operationSize)
	{
		case OperationSize_Word:
			*ieeResult = -((uint32_t) (int32_t) (int16_t) ieeAValue) + ieeBValue;
			break;
		case OperationSize_Long:
			*ieeResult = -ieeAValue + ieeBValue;
			break;
		case OperationSize_Byte:
		case OperationSize_None:
			M68060_ERROR("OperationSize not implemented");
	};
}

void evaluateIeeAluOperation(IeeOperation ieeOperation, OperationSize operationSize, Flags flags, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	setEmptyFlagsModifier(flagsModifier);
	switch (ieeOperation)
	{
		case IeeOperation_None:
			break;
		case IeeOperation_ForwardIeeA:
			*ieeResult = ieeAValue;
			break;
		case IeeOperation_Add:
			{
				evaluateAdd(operationSize, ieeAValue, ieeBValue, ieeResult, flagsModifier);
				break;
			}
		case IeeOperation_AddA:
			{
				evaluateAddA(operationSize, ieeAValue, ieeBValue, ieeResult);
				break;
			}
		case IeeOperation_Sub:
			{
				evaluateSub(operationSize, ieeAValue, ieeBValue, ieeResult, flagsModifier);
				break;
			}
		case IeeOperation_SubA:
			{
				evaluateSubA(operationSize, ieeAValue, ieeBValue, ieeResult);
				break;
			}
		default:
			M68060_ERROR("IeeOperation not implemented");
	}
}
