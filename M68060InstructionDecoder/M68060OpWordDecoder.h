
#ifndef M68060OPWORDDECODER_H
#define M68060OPWORDDECODER_H

#include "../Types.h"
#include "M68060InstructionDecoderTypes.h"

typedef struct
{
	const char* mnemonic;
	ExecutionResource aguBase;
	ExecutionResource aguIndex;
	ExecutionResource ieeA;
	ExecutionResource ieeB;
	bool hasMemoryReference;
	OpMode opMode;
	AguOperation aguOperation;
	AguOffset aguOffset;
	IeeImmediate ieeImmediate;
	
	uint numExtensionWords;

	Pairability pairability;
	
} DecodedOpWord;

DecodedOpWord decodeOpWord(uint16_t operationWord);

#endif
