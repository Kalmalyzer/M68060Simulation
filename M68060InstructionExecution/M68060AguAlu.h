
#ifndef M68060AGUALU_H
#define M68060AGUALU_H

#include "../M68060InstructionDecoder/M68060InstructionDecoderTypes.h"
#include "../Types.h"

void evaluateAguAluOperation(AguOperation aguOperation, OperationSize operationSize, uint32_t baseValue, uint32_t indexValue, uint indexShift, AguIndexSize indexSize,
	uint32_t displacementValue, AguDisplacementSize displacementSize, uint32_t* aguResult, uint32_t* memoryOperand);

#endif
