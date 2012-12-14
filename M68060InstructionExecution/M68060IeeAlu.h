
#ifndef M68060IEEALU_H
#define M68060IEEALU_H

#include "../M68060InstructionDecoder/M68060InstructionDecoderTypes.h"
#include "../Types.h"

void evaluateIeeAluOperation(IeeOperation ieeOperation, OperationSize operationSize, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult);

#endif
