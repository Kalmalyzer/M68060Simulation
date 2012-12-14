
#ifndef M68060INTEGERREGISTERFILE_H
#define M68060INTEGERREGISTERFILE_H

#include "../Types.h"
#include "../M68060InstructionDecoder/M68060InstructionDecoderTypes.h"

uint32_t readIntegerRegister(ExecutionResource reg);

void writeIntegerRegister(ExecutionResource reg, uint32_t value);

#endif
