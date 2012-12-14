
#ifndef DUMMYMEMORYSUBSYSTEM_H
#define DUMMYMEMORYSUBSYSTEM_H

#include "../Types.h"
#include "../M68060InstructionDecoder/M68060InstructionDecoderTypes.h"

uint32_t readMemory(uint32_t address, OperationSize operationSize);
void writeMemory(uint32_t address, OperationSize operationSize, uint32_t value);

#endif
