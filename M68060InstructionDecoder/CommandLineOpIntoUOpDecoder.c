
#include "M68060DecodeOpIntoUOps.h"
#include "../Musashi/m68kcpu.h"
#include "../Types.h"

#include <stdlib.h>

int main(int argc, char** argv)
{
	uint16_t instructionWords[16];
	uint word;
	char* end;
	uint numWords = argc - 1;
	UOp UOps[16];
	uint numUOps;
	uint UOpId;
	char musashiDisassembledInstruction[256];

	if (argc < 2)
	{
		printf("Commandline Op->UOp decoder\n");
		printf("Given a sequence of instruction words that form a complete 68k instruction,\n");
		printf("this utility will decode it into a number of UOps.\n");
		printf("Usage: CommandLineUOpDecoder <instruction word 0> [instruction word 1] ... [instruction word N]\n");
		printf("Example: CommandLineUOpDecoder 0679 000a 0000 0002\n");
		return 0;
	}
	
	for (word = 0; word < numWords; ++word)
		instructionWords[word] = (uint16_t) strtol(argv[word + 1], &end, 16);

	if (!decomposeOpIntoUOps(instructionWords, numWords, UOps, &numUOps))
	{
		printf("Unable to decompose instruction into UOps\n");
		return 0;
	}

	for (word = 0; word < numWords; ++word)
		m68k_write_memory_16(word * sizeof(uint16_t), instructionWords[word]);

	m68k_disassemble(musashiDisassembledInstruction, 0, M68K_CPU_TYPE_68040);
	printf("\nInstruction: %s\n\n", musashiDisassembledInstruction);

	for (UOpId = 0; UOpId < numUOps; ++UOpId)
	{
		printf("UOp %d:\n", UOpId);
		printUOp(&UOps[UOpId]);
	}
	
	return 0;
}
