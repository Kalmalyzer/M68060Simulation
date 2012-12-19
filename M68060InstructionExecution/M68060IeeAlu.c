
#include "M68060IeeAlu.h"
#include "../Assert.h"

static uint32_t mask(uint32_t a, uint32_t b, uint32_t maskField)
{
	return (a & maskField) | (b & ~maskField);
}

static void evaluateAdd(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult)
{
	switch (operationSize)
	{
		case OperationSize_Byte:
			*ieeResult = mask(ieeAValue + ieeBValue, ieeBValue, 0x000000ff);
			break;
		case OperationSize_Word:
			*ieeResult = mask(ieeAValue + ieeBValue, ieeBValue, 0x0000ffff);
			break;
		case OperationSize_Long:
			*ieeResult = ieeAValue + ieeBValue;
			break;
		case OperationSize_None:
			M68060_ERROR("OperationSize not implemented");
	};
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

static void evaluateSub(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult)
{
	evaluateAdd(operationSize, (uint32_t) -ieeAValue, ieeBValue, ieeResult);
}

static void evaluateSubA(OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult)
{
	evaluateAddA(operationSize, (uint32_t) -ieeAValue, ieeBValue, ieeResult);
}

void evaluateIeeAluOperation(IeeOperation ieeOperation, OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult)
{
	switch (ieeOperation)
	{
		case IeeOperation_None:
			break;
		case IeeOperation_ForwardIeeA:
			*ieeResult = ieeAValue;
			break;
		case IeeOperation_Add:
			{
				evaluateAdd(operationSize, ieeAValue, ieeBValue, ieeResult);
				break;
			}
		case IeeOperation_AddA:
			{
				evaluateAddA(operationSize, ieeAValue, ieeBValue, ieeResult);
				break;
			}
		case IeeOperation_Sub:
			{
				evaluateSub(operationSize, ieeAValue, ieeBValue, ieeResult);
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
