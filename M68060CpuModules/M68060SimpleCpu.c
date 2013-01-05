
#include "M68060SimpleCpu.h"

#include "../Types.h"
#include "../Assert.h"
#include "../M68060InstructionDecoder/M68060DecodeOpIntoUOps.h"
#include "../M68060InstructionDecoder/M68060InstructionLengthDecoder.h"
#include "../M68060InstructionExecution/DummyMemorySubSystem.h"
#include "../M68060InstructionExecution/M68060SimpleOEP.h"
#include "../M68060InstructionExecution/M68060IntegerRegisterFile.h"

uint32_t pc = 0;

static void decodeInstructionFromStream(uint32_t pc, uint* totalWords, UOp* uOps, uint* numUOps)
{
	uint16_t opWords[16];
	uint i;
	for (i = 0; i < 16; ++i)
	{
		InstructionLength instructionLength;

		opWords[i] = readMemory(pc + i * 2, OperationSize_Word);
		if (decodeInstructionLengthFromInstructionWords(opWords, i + 1, &instructionLength)
			&& instructionLength.totalWords <= i + 1)
		{
			decomposeOpIntoUOps(opWords, i + 1, uOps, numUOps);
			*totalWords = instructionLength.totalWords;
			return;
		}
	}
	
	M68060_ERROR("Unable to decode next instruction");
}

static void executeInstruction(uint32_t pc, uint totalWords, UOp* uOps, uint numUOps)
{
	uint uOpId;
	uint32_t nextPc = pc + totalWords * 2;
	
	for (uOpId = 0; uOpId < numUOps; ++uOpId)
	{
		UOp* uOp = &uOps[uOpId];
		printf("Executing uOp %s at %08x\n", uOp->description, pc);
		executeUOp(uOp, pc, nextPc);
	}
}

void resetCpu(void)
{
	clearMemory();
	clearIntegerRegisters();
	resetSimpleOEP();
	
	pc = 0;
}

void executeCpuInstruction(void)
{
	UOp uOps[16];
	uint numUOps;
	uint totalWords;
	decodeInstructionFromStream(pc, &totalWords, uOps, &numUOps);
	executeInstruction(pc, totalWords, uOps, numUOps);
	pc += totalWords * 2;
}
