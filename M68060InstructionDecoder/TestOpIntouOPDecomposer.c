
#include "M68060DecomposeOpIntouOPs.h"
#include "../Types.h"

typedef struct 
{
	const char* description;
	uint16_t instructionWords[16];
	uint numInstructionWords;
} TestInstruction;

TestInstruction instructionStream[] =
{
	{ "ADD.B D0,D1", { 0xd200, }, 1, },
	{ "ADD.B D6,D0", { 0xd006, }, 1, },
	{ "ADD.W D0,D0", { 0xd040, }, 1, },
	{ "ADD.L D0,D0", { 0xd080, }, 1, },
	{ "ADD.B D0,D1", { 0xd200, }, 1, },
	{ "ADD.B D0,D2", { 0xd400, }, 1, },
	{ "ADD.B D0,D7", { 0xde00, }, 1, },
	{ "ADD.B D1,D0", { 0xd001, }, 1, },
	{ "ADD.B D2,D0", { 0xd002, }, 1, },
	{ "ADD.B D3,D0", { 0xd003, }, 1, },
	{ "ADD.B D6,D0", { 0xd006, }, 1, },
	{ "ADD.W A3,D5", { 0xda4b, }, 1, },
	{ "ADD.W (A3),D5", { 0xda53, }, 1, },
	{ "ADD.W (A3)+,D5", { 0xda5b, }, 1, },
	{ "ADD.W -(A3),D5", { 0xda63, }, 1, },
	{ "ADD.W ($1234,A3),D5", { 0xda6b, 0x1234, }, 2, },
	{ "ADD.W $12(A1,D3.W*4),D5", { 0xda71, 0x3412, }, 2, },
	{ "ADD.W $1234.W,D5", { 0xda78, 0x1234, }, 2, },
	{ "ADD.W $12345678.L,D5", { 0xda79, 0x1234, 0x5678, }, 3, },
	{ "ADD.W ($1234,PC),D5", { 0xda7a, 0x1234, }, 2, },
	{ "ADD.L $12(PC,D3.W*4),D5", { 0xdabb, 0x3412, }, 2, },
	{ "ADD.L ($1234.W,A0,D0.L),D3", { 0xd6b0, 0x0920, 0x1234, }, 3, },
	{ "ADD.L ($12345678.L,A0,ZD0),D3", { 0xd6b0, 0x0170, 0x1234, 0x5678, }, 4, },
	{ "ADD.L ($12345678.L,A0,D0.L),D3", { 0xd6b0, 0x0930, 0x1234, 0x5678, }, 4, },
	{ "ADD.L ([$12345678.L,A0],D0.L),D3", { 0xd6b0, 0x0935, 0x1234, 0x5678, }, 4, },
	{ "ADD.L ([$12345678.L,A0],D0.L,$1234.W),D3", { 0xd6b0, 0x0936, 0x1234, 0x5678, 0x1234, }, 5, },
	{ "ADD.L ([$12345678.L,A0],D0.L,$12345678.L),D3", { 0xd6b0, 0x0937, 0x1234, 0x5678, 0x1234, 0x5678, }, 6, },
	{ "ADD.L ([$1234.W,A0],D0.L,$12345678.L),D3", { 0xd6b0, 0x0927, 0x1234, 0x1234, 0x5678, }, 5, },
	{ "ADD.L ([A0,D0.L],$12345678.L),D3", { 0xd6b0, 0x0913, 0x1234, 0x5678, }, 4, },
	//{ "ADD.W #$1234,D5", { 0xda7c, 0x1234, }, 2, },
	//{ "ADD.W D5,(A3)", { 0xdb53, }, 1, },
	//{ "ADD.W D5,(A3)+", { 0xdb5b, }, 1, },
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
		{
			uint uOPIndex;
			printf("success: decoding %s yields %d uOPs\n", testInstruction->description, numuOPs);
			for (uOPIndex = 0; uOPIndex < numuOPs; ++uOPIndex)
			{
				uOP* uOP = &uOPs[uOPIndex];
				printf("  %s, AguBase %s, AguIndex %s, AguResult %s, ieeA %s, ieeB %s, ieeResult %s\n", uOP->mnemonic, 
					ExecutionResourceToString(uOP->aguBase), ExecutionResourceToString(uOP->aguIndex), ExecutionResourceToString(uOP->aguResult),
					ExecutionResourceToString(uOP->ieeA), ExecutionResourceToString(uOP->ieeB), ExecutionResourceToString(uOP->ieeResult));
			}
		}
		else
			printf("failure: unable to decode decoding %s\n", testInstruction->description);
	}
	
	return 0;
}