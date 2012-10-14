
#ifndef M68060AGUALU_H
#define M68060AGUALU_H

#include "../M68060InstructionDecoder/M68060InstructionDecoderTypes.h"
#include "../Types.h"

typedef enum
{
	IndexScale_None,
	IndexScale_1,
	IndexScale_2,
	IndexScale_4,
	IndexScale_8,
} IndexScale;

void evaluateAguAluOperation(AguOperation aguOperation, uint32_t baseValue, uint32_t indexValue, IndexScale indexScale, uint32_t offsetValue, uint32_t* aguResult, uint32_t* eaResult);

#endif
