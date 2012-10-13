
#include "M68060OpWordDecoder.h"
#include "../Types.h"


int main(void)
{
	uint16_t instructionStream[] = { 0x4e71 };
	DecodedOpWord decodedOpWord;

	decodedOpWord = decodeOpWord(instructionStream[0]);

	printf("OpWord: %04x - %s - aguBase %s, aguIndex %s, ieeA %s, ieeB %s", instructionStream[0], decodedOpWord.mnemonic,
		ExecutionResourceToString(decodedOpWord.aguBase),
		ExecutionResourceToString(decodedOpWord.aguIndex),
		ExecutionResourceToString(decodedOpWord.ieeA),
		ExecutionResourceToString(decodedOpWord.ieeB));
	
	return 0;
}