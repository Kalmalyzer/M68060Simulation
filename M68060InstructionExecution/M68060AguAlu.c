
#include "M68060AguAlu.h"
#include "../Assert.h"

int toScaleFactor(IndexScale indexScale)
{
	switch (indexScale)
	{
		case IndexScale_1:
			return 1;
		case IndexScale_2:
			return 2;
		case IndexScale_4:
			return 4;
		case IndexScale_8:
			return 8;
		default:
			M68060_ERROR("toScaleFactor case not implemented");
			return 0;
	}
}

uint32_t signExtend16to32(uint32_t value)
{
	return (uint32_t) (int32_t) (int16_t) value;
}

void evaluateAguAluOperation(AguOperation aguOperation, uint32_t baseValue, uint32_t indexValue, IndexScale indexScale, uint32_t offsetValue, uint32_t* aguResult, uint32_t* eaResult)
{
	*aguResult = 0;
	*eaResult = 0;
	
	switch (aguOperation)
	{
		case  AguOperation_OffsetBaseIndexScale:
			*aguResult = *eaResult = baseValue + indexValue * toScaleFactor(indexScale) + offsetValue;
			break;
	}
}
