
#ifndef M68060UOP_H
#define M68060UOP_H

#include "../Types.h"
#include "M68060InstructionDecoderTypes.h"

typedef struct
{
	const char* description;

	uint16_t opWord;
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
	
} UOp;

bool areUOpsEquivalent(const UOp* a, const UOp* b);
void printUOp(const UOp* UOp);

#endif
