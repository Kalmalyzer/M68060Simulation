
#include "M68060SimpleOEP.h"
#include "../M68060InstructionDecoder/M68060DecomposeOpIntouOPs.h"

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

void printuOP(const uOP* uOP)
{
	printf("    uOP: %s\n", uOP->mnemonic);
	printf("      ExtensionWords: %04x,%04x\n", uOP->extensionWords[0], uOP->extensionWords[1]);
	printf("      Agu: Base %s, Index %s, IndexShift %d, IndexSize %s, DisplacementSize %s, Operation %s, Result %s\n",
		ExecutionResourceToString(uOP->aguBase), ExecutionResourceToString(uOP->aguIndex),
		uOP->aguIndexShift, AguIndexSizeToString(uOP->aguIndexSize), AguDisplacementSizeToString(uOP->aguDisplacementSize),
		AguOperationToString(uOP->aguOperation), ExecutionResourceToString(uOP->aguResult));
	printf("      MemoryRead: %s\n", uOP->memoryRead ? "yes" : "no");
	printf("      Iee: A %s, B %s, OperationSize %s, Operation %s, Result %s\n",
		ExecutionResourceToString(uOP->ieeA), ExecutionResourceToString(uOP->ieeB),
		OperationSizeToString(uOP->ieeOperationSize), IeeOperationToString(uOP->ieeOperation), ExecutionResourceToString(uOP->ieeResult));
	printf("      MemoryWrite: %s\n", uOP->memoryWrite ? "yes" : "no");
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
		uOP uOPs[16];
		uint numuOPs;
		uint uOPId;

		printf("Decoding %s\n", op->description);
		decomposeOpIntouOPs(op->instructionWords, op->totalInstructionWords, uOPs, &numuOPs);
		
		printf("Total %u uOPs\n", numuOPs);
		
		for (uOPId = 0; uOPId < numuOPs; ++uOPId)
		{
			const uOP* uOP = &uOPs[uOPId];
			printf("Executing %s\n", uOP->mnemonic);
			printuOP(uOP);
			executeuOP(uOP);
			printRegisters();
		}
	}

	
	return 0;
}