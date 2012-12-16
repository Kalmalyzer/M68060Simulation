
#ifndef M68060INSTRUCTIONDECODER_H
#define M68060INSTRUCTIONDECODER_H

typedef struct
{
	enum { MaxUOps = 16 };

	const char* mnemonic;
	uint numUOps;
	UOp UOps[maxUOps];
	uint length;

} DecodedInstruction;

void decodeInstruction(uint16_t* instructionStream, DecodedInstruction* decodedInstruction);

#endif
