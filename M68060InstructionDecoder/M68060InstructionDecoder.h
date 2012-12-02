
#ifndef M68060INSTRUCTIONDECODER_H
#define M68060INSTRUCTIONDECODER_H

typedef struct
{
	enum { MaxuOPs = 16 };

	const char* mnemonic;
	uint numuOPs;
	uOP uOPs[maxuOPs];
	uint length;

} DecodedInstruction;

void decodeInstruction(uint16_t* instructionStream, DecodedInstruction* decodedInstruction);

#endif
