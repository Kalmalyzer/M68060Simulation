
#ifndef M68060UOP_H
#define M68060UOP_H

#include "../Types.h"
#include "M68060InstructionDecoderTypes.h"

typedef struct
{
	const char* mnemonic;

	uint16_t extensionWords[2];
	
	ExecutionResource aguBase;
	ExecutionResource aguIndex;
	uint aguIndexShift;
	AguIndexSize aguIndexSize;
	AguDisplacementSize aguDisplacementSize;
	AguOperation aguOperation;
	ExecutionResource aguResult;

	bool memoryRead;

	ExecutionResource ieeA;
	ExecutionResource ieeB;
	OperationSize ieeOperationSize;
	IeeOperation ieeOperation;
	ExecutionResource ieeResult;

	bool memoryWrite;

	Pairability pairability;

	uint8_t imm3Bit;
	
} UOp;

#endif
