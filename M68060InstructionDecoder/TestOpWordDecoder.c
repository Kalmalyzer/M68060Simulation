
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
		0xda4b, // ADD.W A3,D5
		0xda53, // ADD.W (A3),D5
		0xda5b, // ADD.W (A3)+,D5
		0xda63, // ADD.W -(A3),D5
		0xda6b, // ADD.W (d16,A3),D5
		0xda73, // ADD.W d8(A3,Xn.q),D5
		0xda78, // ADD.W xxx.W,D5
		0xda79, // ADD.W xxx.L,D5
		0xda7a, // ADD.W (d16,PC),D5
		0xda7b, // ADD.W d8(PC,Xn.q),D5
		0xda7c, // ADD.W #imm,D5
		0xdb53, // ADD.W D5,(A3)
		0xdb5b, // ADD.W D5,(A3)+
	};
	
	uint i;

	for (i = 0; i < (sizeof instructionStream / sizeof instructionStream[0]); ++i)
	{
		uint16_t opWord = instructionStream[i];
		DecodedOpWord decodedOpWord = decodeOpWord(opWord);

		printf("OpWord: %04x - mnemonic %s, opMode %s\n"
			"\taguOperation %s, aguOffset %s, aguBase %s, aguIndex %s\n"
			"\tieeImmediate %s, ieeA %s, ieeB %s\n",
			opWord, decodedOpWord.mnemonic, OpModeToString(decodedOpWord.opMode),
			AguOperationToString(decodedOpWord.aguOperation),
			AguOffsetToString(decodedOpWord.aguOffset),
			ExecutionResourceToString(decodedOpWord.aguBase),
			ExecutionResourceToString(decodedOpWord.aguIndex),
			IeeImmediateToString(decodedOpWord.ieeImmediate),
			ExecutionResourceToString(decodedOpWord.ieeA),
			ExecutionResourceToString(decodedOpWord.ieeB));
	}
	
	return 0;
}