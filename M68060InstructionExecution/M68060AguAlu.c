
#include "M68060AguAlu.h"
#include "../Assert.h"

static uint32_t indexValueTo32Bit(uint32_t indexValue, AguIndexSize indexSize)
{
	switch (indexSize)
	{
		case AguIndexSize_None:
			return 0;
		case AguIndexSize_Word:
			return (uint32_t) (int32_t) (int16_t) indexValue;
		case AguIndexSize_Long:
			return indexValue;
		default:
			M68060_ERROR("indexSize not supported");
			return 0;
	}
}

static uint32_t displacementValueTo32Bit(uint32_t displacementValue, AguDisplacementSize displacementSize)
{
	switch (displacementSize)
	{
		case AguDisplacementSize_None:
			return 0;
		case AguDisplacementSize_S8:
			return (uint32_t) (int32_t) (int8_t) displacementValue;
		case AguDisplacementSize_S16:
			return (uint32_t) (int32_t) (int16_t) displacementValue;
		case AguDisplacementSize_S32:
			return displacementValue;
		default:
			M68060_ERROR("displacementSize not supported");
			return 0;
	}
}

static operationSizeToValue(OperationSize operationSize)
{
	switch (operationSize)
	{
		case OperationSize_None:
			return 0;
		case OperationSize_Byte:
			return 1;
		case OperationSize_Word:
			return 2;
		case OperationSize_Long:
			return 4;
		default:
			M68060_ERROR("operationSize not supported");
			return 0;
	}
}

void evaluateAguAluOperation(AguOperation aguOperation, OperationSize operationSize, uint32_t baseValue, uint32_t indexValue, uint indexShift, AguIndexSize indexSize,
	uint32_t displacementValue, AguDisplacementSize displacementSize, uint32_t* aguResult, uint32_t* memoryOperandReference)
{
	switch (aguOperation)
	{
		case  AguOperation_OffsetBaseIndexScale:
			{
				uint32_t result = baseValue + (indexValueTo32Bit(indexValue, indexSize) << indexShift) + displacementValueTo32Bit(displacementValue, displacementSize);
				*aguResult = result;
				*memoryOperandReference = result;
				break;
			}
		case  AguOperation_PostIncrement:
			{
				uint32_t incrementedAddress = baseValue + operationSizeToValue(operationSize);
				*memoryOperandReference = baseValue;
				*aguResult = incrementedAddress;
				break;
			}
		case  AguOperation_PreDecrement:
			{
				uint32_t decrementedAddress = baseValue - operationSizeToValue(operationSize);
				*memoryOperandReference = decrementedAddress;
				*aguResult = decrementedAddress;
				break;
			}
		default:
			M68060_ERROR("aguOperation not implemented");
	}
}
