
#include "M68060SimpleOEP.h"
#include "../M68060InstructionDecoder/M68060DecodeOpIntoUOps.h"

typedef struct
{
	const char* description;
	uint totalInstructionWords;
	const uint16_t instructionWords[16];
} M68kOp;

M68kOp instructionStream[] =
{
	{ "ADDI.B #$12,D2", 2, { 0x0602, 0x0012, }, },
	{ "ADD.W D2,$2.W", 2, { 0xd578, 0x0002, }, },
	{ "ADDI.W #$C,D3", 2, { 0x0643, 0x000c, }, },
	{ "ADDI.W #$A,$2.L", 4, { 0x0679, 0x000a, 0x0000, 0x0002, }, },
	{ "ADDA.W #$2,A1", 2, { 0xd2fc, 0x0002, }, },
	{ "ADD.W (A1),D3", 1, { 0xd651, }, }, 
};

void printUOp(const UOp* UOp)
{
	printf("    UOp: %s\n", UOp->mnemonic);
	printf("      ExtensionWords: %04x,%04x\n", UOp->extensionWords[0], UOp->extensionWords[1]);
	printf("      Agu: Base %s, Index %s, IndexShift %d, IndexSize %s, DisplacementSize %s, Operation %s, Result %s\n",
		ExecutionResourceToString(UOp->aguBase), ExecutionResourceToString(UOp->aguIndex),
		UOp->aguIndexShift, AguIndexSizeToString(UOp->aguIndexSize), AguDisplacementSizeToString(UOp->aguDisplacementSize),
		AguOperationToString(UOp->aguOperation), ExecutionResourceToString(UOp->aguResult));
	printf("      MemoryRead: %s\n", UOp->memoryRead ? "yes" : "no");
	printf("      Iee: A %s, B %s, OperationSize %s, Operation %s, Result %s\n",
		ExecutionResourceToString(UOp->ieeA), ExecutionResourceToString(UOp->ieeB),
		OperationSizeToString(UOp->ieeOperationSize), IeeOperationToString(UOp->ieeOperation), ExecutionResourceToString(UOp->ieeResult));
	printf("      MemoryWrite: %s\n", UOp->memoryWrite ? "yes" : "no");
}

void printRegisters(void)
{
	ExecutionResource reg;
	for (reg = ExecutionResource_D0; reg <= ExecutionResource_A7; ++reg)
		printf("  %s: %08X\n", ExecutionResourceToString(reg), readIntegerRegister(reg));
}

int main(void)
{
	uint instructionPos;

	for (instructionPos = 0; instructionPos < (sizeof instructionStream / sizeof instructionStream[0]); ++instructionPos)
	{
		const M68kOp* op = &instructionStream[instructionPos];
		UOp UOps[16];
		uint numUOps;
		uint UOpId;

		printf("Decoding %s\n", op->description);
		decomposeOpIntoUOps(op->instructionWords, op->totalInstructionWords, UOps, &numUOps);
		
		printf("Total %u UOps\n", numUOps);
		
		for (UOpId = 0; UOpId < numUOps; ++UOpId)
		{
			const UOp* UOp = &UOps[UOpId];
			printf("Executing %s\n", UOp->mnemonic);
			printUOp(UOp);
			executeUOp(UOp);
			printRegisters();
		}
	}

	
	return 0;
}