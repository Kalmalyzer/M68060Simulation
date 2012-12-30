
#ifndef M68060IEEALU_H
#define M68060IEEALU_H

#include "../M68060InstructionDecoder/M68060InstructionDecoderTypes.h"
#include "M68060FlagsModifier.h"

void evaluateIeeAluOperation(IeeOperation ieeOperation, OperationSize operationSize, Flags flags, uint32_t ieeAValue, uint32_t ieeBValue, uint32_t* ieeResult, FlagsModifier* flagsModifier);

#endif
