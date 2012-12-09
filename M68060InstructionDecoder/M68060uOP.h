
#ifndef M68060UOP_H
#define M68060UOP_H

#include "../Types.h"
#include "M68060InstructionDecoderTypes.h"

typedef enum
{
	IeeOperation_None,
	IeeOperation_Add,
} IeeOperation;

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
	IeeImmediate ieeImmediate; // Obsolete
	OperationSize ieeOperationSize;
	ExecutionResource ieeResult;

	bool memoryWrite;

	OpMode opMode;
	
	Pairability pairability;

} uOP;

#endif
