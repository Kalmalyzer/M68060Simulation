
#ifndef M68060INSTRUCTIONLENGTHDECODER_H
#define M68060INSTRUCTIONLENGTHDECODER_H

#include "../Types.h"

typedef struct
{
	uint numSpecialOperandSpecifierWords;
	uint numSourceEAExtensionWords;
	uint numDestinationEAExtensionWords;

	uint totalWords;
	
	const char* mnemonic;
	
} InstructionLength;

bool decodeInstructionLengthFromInstructionWords(const uint16_t* instructionWords, uint numInstructionWordsAvailable, InstructionLength* instructionLength);

#endif
