
#include "M68060DecodeOpIntoUOps.h"
#include "../Musashi/m68kcpu.h"
#include "../Types.h"

void printUOp(uint id, const UOp* UOp)
{
	printf("    UOp %d: %s\n", id, UOp->description);
	printf("      OpWord & ExtensionWords: %04x,%04x,%04x\n", UOp->opWord, UOp->extensionWords[0], UOp->extensionWords[1]);
	printf("      Agu: Base %s, Index %s, IndexShift %d, IndexSize %s, DisplacementSize %s, Operation %s, Result %s\n",
		ExecutionResourceToString(UOp->aguBase), ExecutionResourceToString(UOp->aguIndex),
		UOp->aguIndexShift, AguIndexSizeToString(UOp->aguIndexSize), AguDisplacementSizeToString(UOp->aguDisplacementSize),
		AguOperationToString(UOp->aguOperation), ExecutionResourceToString(UOp->aguResult));
	printf("      MemoryRead: %s\n", UOp->memoryRead ? "yes" : "no");
	printf("      Iee: A %s, B %s, OperationSize %s, Operation %s, Result %s\n",
		ExecutionResourceToString(UOp->ieeA), ExecutionResourceToString(UOp->ieeB),
		OperationSizeToString(UOp->ieeOperationSize), IeeOperationToString(UOp->ieeOperation), ExecutionResourceToString(UOp->ieeResult));
	printf("      MemoryWrite: %s\n", UOp->memoryWrite ? "yes" : "no");
	printf("      Pairability: %s\n", PairabilityToString(UOp->pairability));
}

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
		instructionWords[word] = strtol(argv[word + 1], &end, 16);

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
		printUOp(UOpId, &UOps[UOpId]);
	
	return 0;
}
