
#include "M68060AguAlu.h"

int main(void)
{
	uint32_t aguBase = 0x12345678;
	uint32_t aguIndex = 0x87654321;
	IndexScale indexScale = IndexScale_2;
	uint32_t aguOffset = 0x44325123;
	uint32_t aguResult;
	uint32_t eaResult;
	uint32_t expectedResult = aguBase + aguIndex * 2 + aguOffset;
	evaluateAguAluOperation(AguOperation_OffsetBaseIndexScale, aguBase, aguIndex, indexScale, aguOffset, &aguResult, &eaResult);

	if (expectedResult == aguResult)
		printf("Test succeeded, result: %08x\n", aguResult);
	else
		printf("Test failed, result: %08x - expected %08x\n", aguResult, expectedResult);

	return 0;
}