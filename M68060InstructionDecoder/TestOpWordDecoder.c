
#include "M68060OpWordDecoder.h"
#include "../Types.h"


int main(void)
{
	uint16_t instructionStream[] =
	{
		0x4e71,	// NOP
		0xd000, // ADD.B D0,D0
		0xd040, // ADD.W D0,D0
		0xd080, // ADD.L D0,D0
		0xd200, // ADD.B D0,D1
		0xd400, // ADD.B D0,D2
		0xde00, // ADD.B D0,D7
		0xd001, // ADD.B D1,D0
		0xd002, // ADD.B D2,D0
		0xd003, // ADD.B D3,D0
		0xd006, // ADD.B D6,D0
	};
	
	uint i;

	for (i = 0; i < (sizeof instructionStream / sizeof instructionStream[0]); ++i)
	{
		uint16_t opWord = instructionStream[i];
		DecodedOpWord decodedOpWord = decodeOpWord(opWord);

		printf("OpWord: %04x - mnemonic %s, opMode %s, aguBase %s, aguIndex %s, ieeA %s, ieeB %s\n", opWord, decodedOpWord.mnemonic,
			OpModeToString(decodedOpWord.opMode),
			ExecutionResourceToString(decodedOpWord.aguBase),
			ExecutionResourceToString(decodedOpWord.aguIndex),
			ExecutionResourceToString(decodedOpWord.ieeA),
			ExecutionResourceToString(decodedOpWord.ieeB));
	}
	
	return 0;
}