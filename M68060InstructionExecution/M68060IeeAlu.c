
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

static uint32_t extendValueTo32Bits(OperationSize operationSize, uint32_t value)
{
	switch (operationSize)
	{
		case OperationSize_None:
			M68060_ERROR("OperationSize not supported");
		case OperationSize_Byte:
			return (uint32_t) (int32_t) (int8_t) value;
		case OperationSize_Word:
			return (uint32_t) (int32_t) (int16_t) value;
			break;
		case OperationSize_Long:
			return value;
		default:
			M68060_ERROR("OperationSize not implemented");
			return 0;
	};
}

static void setEmptyFlagsModifier(FlagsModifier* flagsModifier)
{
	flagsModifier->andFlags = Flags_All_Mask;
	flagsModifier->orFlags = 0;
}

static void setFlagsModifierNZVC(bool n, bool z, bool v, bool c, FlagsModifier* flagsModifier)
{
	flagsModifier->orFlags = (n ? Flags_Negative_Mask : 0) | (z ? Flags_Zero_Mask : 0)| (v ? Flags_Overflow_Mask : 0) | (c ? Flags_Carry_Mask : 0);
	flagsModifier->andFlags = Flags_Extend_Mask;
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
	uint32_t ieeAValue32Bits = extendValueTo32Bits(operationSize, ieeAValue);
	*ieeResult = ieeAValue32Bits + ieeBValue;
}

static void evaluateAddX(OperationSize operationSize, Flags flags, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	uint32_t operationMask = getOperationMask(operationSize);

	uint32_t resultValue = ieeAValue + ieeBValue + ((flags & Flags_Extend_Mask) ? 1 : 0);

	Minterms overflowMinterms = Minterm_Sm_Dm_InvRm | Minterm_InvSm_InvDm_Rm;
	Minterms carryMinterms = (Minterm_Sm_Dm_Rm | Minterm_Sm_Dm_InvRm) | (Minterm_Sm_Dm_InvRm | Minterm_Sm_InvDm_InvRm) | (Minterm_Sm_Dm_InvRm | Minterm_InvSm_Dm_InvRm);
	bool n, z, v, c;

	computeNZVC(ieeAValue, ieeBValue, resultValue, operationMask, overflowMinterms, carryMinterms, &n, &z, &v, &c);

	setFlagsModifierXNZVC(c, n, z && (flags & Flags_Zero_Mask), v, c, flagsModifier);
	
	*ieeResult = mask(resultValue, ieeBValue, operationMask);
}

static void evaluateCmpCommon(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, FlagsModifier* flagsModifier)
{
	uint32_t operationMask = getOperationMask(operationSize);

	uint32_t resultValue = -ieeAValue + ieeBValue;

	Minterms overflowMinterms = Minterm_InvSm_Dm_InvRm | Minterm_Sm_InvDm_Rm;
	Minterms carryMinterms = (Minterm_Sm_InvDm_Rm | Minterm_Sm_InvDm_InvRm) | (Minterm_Sm_Dm_Rm | Minterm_Sm_InvDm_Rm) | (Minterm_Sm_InvDm_Rm | Minterm_InvSm_InvDm_Rm);
	bool n, z, v, c;

	computeNZVC(ieeAValue, ieeBValue, resultValue, operationMask, overflowMinterms, carryMinterms, &n, &z, &v, &c);

	setFlagsModifierNZVC(n, z, v, c, flagsModifier);
}

static void evaluateCmp(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, FlagsModifier* flagsModifier)
{
	evaluateCmpCommon(operationSize, ieeAValue, ieeBValue, flagsModifier);
}

static void evaluateCmpA(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, FlagsModifier* flagsModifier)
{
	evaluateCmpCommon(OperationSize_Long, extendValueTo32Bits(operationSize, ieeAValue), ieeBValue, flagsModifier);
}

static void evaluateSubCommon(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
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

static void evaluateSub(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	evaluateSubCommon(operationSize, ieeAValue, ieeBValue, ieeResult, flagsModifier);
}

static void evaluateSubA(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult)
{
	uint32_t ieeAValue32Bits = extendValueTo32Bits(operationSize, ieeAValue);
	*ieeResult = -ieeAValue32Bits + ieeBValue;
}

static void evaluateSubXCommon(OperationSize operationSize, Flags flags, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	uint32_t operationMask = getOperationMask(operationSize);

	uint32_t resultValue = -ieeAValue + ieeBValue + ((flags & Flags_Extend_Mask) ? 1 : 0);

	Minterms overflowMinterms = Minterm_Sm_Dm_InvRm | Minterm_InvSm_InvDm_Rm;
	Minterms carryMinterms = (Minterm_Sm_Dm_Rm | Minterm_Sm_Dm_InvRm) | (Minterm_Sm_Dm_InvRm | Minterm_Sm_InvDm_InvRm) | (Minterm_Sm_Dm_InvRm | Minterm_InvSm_Dm_InvRm);
	bool n, z, v, c;

	computeNZVC(ieeAValue, ieeBValue, resultValue, operationMask, overflowMinterms, carryMinterms, &n, &z, &v, &c);

	setFlagsModifierXNZVC(c, n, z && (flags & Flags_Zero_Mask), v, c, flagsModifier);
	
	*ieeResult = mask(resultValue, ieeBValue, operationMask);
}

static void evaluateSubX(OperationSize operationSize, Flags flags, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	evaluateSubXCommon(operationSize, flags, ieeAValue, ieeBValue, ieeResult, flagsModifier);
}

static void evaluateNeg(OperationSize operationSize, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	evaluateSubCommon(operationSize, ieeBValue, 0, ieeResult, flagsModifier);
}

static void evaluateNegX(OperationSize operationSize, Flags flags, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	evaluateSubXCommon(operationSize, flags, ieeBValue, 0, ieeResult, flagsModifier);
}

static void evaluateTst(OperationSize operationSize, uint32_t ieeBValue, FlagsModifier* flagsModifier)
{
	uint32_t operationMask = getOperationMask(operationSize);
	uint32_t operationHighestBitMask = operationMask - (operationMask >> 1);
	setFlagsModifierNZVC(ieeBValue & operationHighestBitMask, !(ieeBValue & operationMask), false, false, flagsModifier);
}

typedef enum
{
	ShiftRotateOperation_Asl,
	ShiftRotateOperation_Asr,
	ShiftRotateOperation_Lsl,
	ShiftRotateOperation_Lsr,
	ShiftRotateOperation_Rol,
	ShiftRotateOperation_Ror,
	ShiftRotateOperation_Roxl,
	ShiftRotateOperation_Roxr,

} ShiftRotateOperation;

typedef enum
{
	ShiftDirection_Left,
	ShiftDirection_Right,

} ShiftDirection;

typedef enum
{
	ShiftIn_Zero,
	ShiftIn_Lsb,
	ShiftIn_Msb,
	ShiftIn_Extend,

} ShiftIn;
		
static void evaluateShiftRotateCommon(ShiftDirection shiftDirection, ShiftIn shiftIn, OperationSize operationSize, bool extend, uint shiftRotateCount, uint32_t ieeBValue, uint32_t* ieeResult, bool* negativeResult, bool* zeroResult, bool* overflowResult, bool* extendResult)
{
	uint32_t operationMask = getOperationMask(operationSize);
	uint32_t operationHighestBitMask = operationMask - (operationMask >> 1);

	if (!shiftRotateCount)
	{
		*ieeResult = ieeBValue;
		*overflowResult = false;
		*extendResult = false;
	}
	else
	{
		uint32_t operationMask = getOperationMask(operationSize);
		uint32_t operationHighestBitMask = operationMask - (operationMask >> 1);

		uint32_t result = ieeBValue & operationMask;
		bool x = extend;
		bool overflow = false;

		while (shiftRotateCount--)
		{
			uint32_t newResult = result;
			bool newX = false;
			bool shiftInOne = false;
			
			switch (shiftIn)
			{
				case ShiftIn_Zero:
					break;
				case ShiftIn_Lsb:
					shiftInOne = (result & 1);
					break;
				case ShiftIn_Msb:
					shiftInOne = (result & operationHighestBitMask);
					break;
				case ShiftIn_Extend:
					shiftInOne = x;
					break;
			}
			
			if (shiftDirection == ShiftDirection_Left)
			{
				newResult = ((result << 1) & operationMask) | (shiftInOne ? 1 : 0);
				newX = result & operationHighestBitMask;
			}
			else
			{
				newResult = ((result >> 1) & operationMask) | (shiftInOne ? operationHighestBitMask : 0);
				newX = result & 1;
			}
				
			overflow = (overflow || ((newResult ^ result) & operationHighestBitMask));

			result = newResult;
			x = newX;
		}
		
		*ieeResult = mask(result, ieeBValue, operationMask);
		*overflowResult = overflow;
		*extendResult = x;
	}
	
	*negativeResult = *ieeResult & operationHighestBitMask;
	*zeroResult = !(*ieeResult);
}

static void evaluateAsl(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	bool negative;
	bool zero;
	bool overflow;
	bool extend;
	uint shiftCount = ieeAValue & 0x3f;

	evaluateShiftRotateCommon(ShiftDirection_Left, ShiftIn_Zero, operationSize, false, shiftCount, ieeBValue, ieeResult, &negative, &zero, &overflow, &extend);

	if (shiftCount)
		setFlagsModifierXNZVC(extend, negative, zero, overflow, extend, flagsModifier);
	else
		setFlagsModifierNZVC(negative, zero, false, false, flagsModifier);
}

static void evaluateAsr(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	bool negative;
	bool zero;
	bool overflow;
	bool extend;
	uint shiftCount = ieeAValue & 0x3f;

	evaluateShiftRotateCommon(ShiftDirection_Right, ShiftIn_Msb, operationSize, false, shiftCount, ieeBValue, ieeResult, &negative, &zero, &overflow, &extend);

	if (shiftCount)
		setFlagsModifierXNZVC(extend, negative, zero, overflow, extend, flagsModifier);
	else
		setFlagsModifierNZVC(negative, zero, false, false, flagsModifier);
}

static void evaluateLsl(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	bool negative;
	bool zero;
	bool overflow;
	bool extend;
	uint shiftCount = ieeAValue & 0x3f;

	evaluateShiftRotateCommon(ShiftDirection_Left, ShiftIn_Zero, operationSize, false, shiftCount, ieeBValue, ieeResult, &negative, &zero, &overflow, &extend);

	if (shiftCount)
		setFlagsModifierXNZVC(extend, negative, zero, false, extend, flagsModifier);
	else
		setFlagsModifierNZVC(negative, zero, false, false, flagsModifier);
}

static void evaluateLsr(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	bool negative;
	bool zero;
	bool overflow;
	bool extend;
	uint shiftCount = ieeAValue & 0x3f;

	evaluateShiftRotateCommon(ShiftDirection_Right, ShiftIn_Zero, operationSize, false, shiftCount, ieeBValue, ieeResult, &negative, &zero, &overflow, &extend);

	if (shiftCount)
		setFlagsModifierXNZVC(extend, negative, zero, false, extend, flagsModifier);
	else
		setFlagsModifierNZVC(negative, zero, false, false, flagsModifier);
}

static void evaluateRol(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	bool negative;
	bool zero;
	bool overflow;
	bool extend;
	uint shiftCount = ieeAValue & 0x3f;

	evaluateShiftRotateCommon(ShiftDirection_Left, ShiftIn_Msb, operationSize, false, shiftCount, ieeBValue, ieeResult, &negative, &zero, &overflow, &extend);
	setFlagsModifierNZVC(negative, zero, false, extend, flagsModifier);
}

static void evaluateRor(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	bool negative;
	bool zero;
	bool overflow;
	bool extend;
	uint shiftCount = ieeAValue & 0x3f;

	evaluateShiftRotateCommon(ShiftDirection_Right, ShiftIn_Lsb, operationSize, false, shiftCount, ieeBValue, ieeResult, &negative, &zero, &overflow, &extend);
	setFlagsModifierNZVC(negative, zero, false, extend, flagsModifier);
}

static void evaluateRoxl(OperationSize operationSize, Flags flags, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	bool negative;
	bool zero;
	bool overflow;
	bool extend;
	uint shiftCount = ieeAValue & 0x3f;

	evaluateShiftRotateCommon(ShiftDirection_Left, ShiftIn_Extend, operationSize, flags & Flags_Extend_Mask, shiftCount, ieeBValue, ieeResult, &negative, &zero, &overflow, &extend);

	if (shiftCount)
		setFlagsModifierXNZVC(extend, negative, zero, false, extend, flagsModifier);
	else
		setFlagsModifierNZVC(negative, zero, false, flags & Flags_Extend_Mask, flagsModifier);
}

static void evaluateRoxr(OperationSize operationSize, Flags flags, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	bool negative;
	bool zero;
	bool overflow;
	bool extend;
	uint shiftCount = ieeAValue & 0x3f;

	evaluateShiftRotateCommon(ShiftDirection_Right, ShiftIn_Extend, operationSize, flags & Flags_Extend_Mask, shiftCount, ieeBValue, ieeResult, &negative, &zero, &overflow, &extend);

	if (shiftCount)
		setFlagsModifierXNZVC(extend, negative, zero, false, extend, flagsModifier);
	else
		setFlagsModifierNZVC(negative, zero, false, flags & Flags_Extend_Mask, flagsModifier);
}

static void evaluateLogicOperationCommon(OperationSize operationSize, uint32_t nonMaskedResult, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	uint32_t operationMask = getOperationMask(operationSize);
	uint32_t operationHighestBitMask = operationMask - (operationMask >> 1);

	*ieeResult = mask(nonMaskedResult, ieeBValue, operationMask);
	setFlagsModifierNZVC(nonMaskedResult & operationHighestBitMask, !(nonMaskedResult & operationMask), false, false, flagsModifier);
}

static void evaluateAnd(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	evaluateLogicOperationCommon(operationSize, ieeAValue & ieeBValue, ieeBValue, ieeResult, flagsModifier);
}

static void evaluateOr(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	evaluateLogicOperationCommon(operationSize, ieeAValue | ieeBValue, ieeBValue, ieeResult, flagsModifier);
}

static void evaluateEor(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	evaluateLogicOperationCommon(operationSize, ieeAValue ^ ieeBValue, ieeBValue, ieeResult, flagsModifier);
}

static void evaluateNot(OperationSize operationSize, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	evaluateLogicOperationCommon(operationSize, ~ieeBValue, ieeBValue, ieeResult, flagsModifier);
}

void evaluateIeeAluOperation(IeeOperation ieeOperation, OperationSize operationSize, Flags flags, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier)
{
	setEmptyFlagsModifier(flagsModifier);
	*ieeResult = 0;

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
		case IeeOperation_AddX:
			{
				evaluateAddX(operationSize, flags, ieeAValue, ieeBValue, ieeResult, flagsModifier);
				break;
			}
		case IeeOperation_Cmp:
			{
				evaluateCmp(operationSize, ieeAValue, ieeBValue, flagsModifier);
				break;
			}
		case IeeOperation_CmpA:
			{
				evaluateCmpA(operationSize, ieeAValue, ieeBValue, flagsModifier);
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
		case IeeOperation_SubX:
			{
				evaluateSubX(operationSize, flags, ieeAValue, ieeBValue, ieeResult, flagsModifier);
				break;
			}
		case IeeOperation_Neg:
			{
				evaluateNeg(operationSize, ieeBValue, ieeResult, flagsModifier);
				break;
			}
		case IeeOperation_NegX:
			{
				evaluateNegX(operationSize, flags, ieeBValue, ieeResult, flagsModifier);
				break;
			}
		case IeeOperation_Tst:
			{
				evaluateTst(operationSize, ieeBValue, flagsModifier);
				break;
			}
		case IeeOperation_Asl:
			{
				evaluateAsl(operationSize, ieeAValue, ieeBValue, ieeResult, flagsModifier);
				break;
			}
		case IeeOperation_Asr:
			{
				evaluateAsr(operationSize, ieeAValue, ieeBValue, ieeResult, flagsModifier);
				break;
			}
		case IeeOperation_Lsl:
			{
				evaluateLsl(operationSize, ieeAValue, ieeBValue, ieeResult, flagsModifier);
				break;
			}
		case IeeOperation_Lsr:
			{
				evaluateLsr(operationSize, ieeAValue, ieeBValue, ieeResult, flagsModifier);
				break;
			}
		case IeeOperation_Rol:
			{
				evaluateRol(operationSize, ieeAValue, ieeBValue, ieeResult, flagsModifier);
				break;
			}
		case IeeOperation_Ror:
			{
				evaluateRor(operationSize, ieeAValue, ieeBValue, ieeResult, flagsModifier);
				break;
			}
		case IeeOperation_Roxl:
			{
				evaluateRoxl(operationSize, flags, ieeAValue, ieeBValue, ieeResult, flagsModifier);
				break;
			}
		case IeeOperation_Roxr:
			{
				evaluateRoxr(operationSize, flags, ieeAValue, ieeBValue, ieeResult, flagsModifier);
				break;
			}
		case IeeOperation_And:
			{
				evaluateAnd(operationSize, ieeAValue, ieeBValue, ieeResult, flagsModifier);
				break;
			}
		case IeeOperation_Or:
			{
				evaluateOr(operationSize, ieeAValue, ieeBValue, ieeResult, flagsModifier);
				break;
			}
		case IeeOperation_Eor:
			{
				evaluateEor(operationSize, ieeAValue, ieeBValue, ieeResult, flagsModifier);
				break;
			}
		case IeeOperation_Not:
			{
				evaluateNot(operationSize, ieeBValue, ieeResult, flagsModifier);
				break;
			}
		default:
			M68060_ERROR("IeeOperation not implemented");
	}
}
