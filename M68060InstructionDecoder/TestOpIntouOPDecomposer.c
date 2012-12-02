
#include "M68060DecomposeOpIntouOPs.h"
#include "../Types.h"

typedef struct 
{
	uint numInstructionWords;
	uint16_t instructionWords[16];
	const char* description;
} TestInstruction;

TestInstruction instructionStream[] =
{
	{ 1, { 0xd000, }, "ADD.B D0, D0", },
/*
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
*/
};

int main(void)
{
	uint i;

	for (i = 0; i < (sizeof instructionStream / sizeof instructionStream[0]); ++i)
	{
		TestInstruction* testInstruction = &instructionStream[i];
		uOP uOPs[16];
		uint numuOPs;
		bool success = decomposeOpIntouOPs(testInstruction->instructionWords, testInstruction->numInstructionWords, uOPs, &numuOPs);

		if (success)
			printf("success: decoding %s yields %d uOPs\n", testInstruction->description, numuOPs);
		else
			printf("failure: unable to decode decoding %s\n", testInstruction->description);
	}
	
	return 0;
}