
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
	ExecutionResource aguBase;
	ExecutionResource aguIndex;
	ExecutionResource aguResult;
	ExecutionResource ieeA;
	ExecutionResource ieeB;
	ExecutionResource ieeResult;
	bool memoryRead;
	bool memoryWrite;
	OpMode opMode;
	uint aguIndexShift;
	AguIndexSize aguIndexSize;
	AguDisplacementSize aguDisplacementSize;
	AguOperation aguOperation;
	IeeImmediate ieeImmediate;

	uint16_t extensionWords[2];
	
	Pairability pairability;

} uOP;

#endif
