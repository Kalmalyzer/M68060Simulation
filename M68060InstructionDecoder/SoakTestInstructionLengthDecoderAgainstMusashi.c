
#include "../Types.h"
#include "M68060InstructionLengthDecoder.h"
#include "../Musashi/m68kcpu.h"

#include <stdio.h>

typedef struct
{
	uint16_t mask;
	uint16_t match;
} IgnoreOp;

IgnoreOp ignoreOps[] =
{
	{ 0xffff, 0x007c, }, // ORI #imm,SR
	{ 0xffc0, 0x00c0, }, // CMP2.B <ea>,D0
	{ 0xffff, 0x027c, }, // ANDI #imm,SR
	{ 0xffc0, 0x02c0, }, // CMP2.W <ea>,D0
	{ 0xffc0, 0x04c0, }, // CMP2.L <ea>,D0
	{ 0xffff, 0x083c, }, // BTST #imm,#imm
	{ 0xffff, 0x0a7c, }, // EORI #imm,SR
	{ 0xffc0, 0x0ac0, }, // CAS.B D0,D0,<ea>
	{ 0xffc0, 0x0cc0, }, // CAS.W D0,D0,<ea>
	{ 0xffc0, 0x0e00, }, // MOVES.B <ea>,Dn
	{ 0xffc0, 0x0e40, }, // MOVES.W <ea>,Dn
	{ 0xffc0, 0x0e80, }, // MOVES.L <ea>,Dn
	{ 0xffc0, 0x0ec0, }, // CAS.L D0,D0,<ea>
};

void writeInstructionBufferToMusashiMemory(const uint16_t* instructionBuffer, uint instructionBufferSize)
{
	uint word;
	for (word = 0; word < instructionBufferSize; ++word)
		m68k_write_memory_16(word * sizeof(uint16_t), instructionBuffer[word]);
}

bool soakTestOpWordDecoding(void)
{
	const char* previousDescription = 0;
	uint opWord;

	printf("Attempting to decode lengths for all opwords...\n");
	
	for (opWord = 0; opWord < 0x10000; ++opWord)
	{
		InstructionLength instructionLength;
		bool success;
		uint16_t instructionBuffer[16] = { 0 };
		uint musashiInstructionLength;
		char musashiDisassembledInstruction[256];

		instructionBuffer[0] = opWord;

		success = decodeInstructionLengthFromInstructionWords(instructionBuffer, sizeof instructionBuffer / sizeof instructionBuffer[0], &instructionLength);

		if (!success)
		{
			printf("Error: decoding failed for instruction %04x\n", opWord);
			return false;
		}
		
		writeInstructionBufferToMusashiMemory(instructionBuffer, sizeof instructionBuffer / sizeof instructionBuffer[0]);
		musashiInstructionLength = m68k_disassemble(musashiDisassembledInstruction, 0, M68K_CPU_TYPE_68040);

		if (musashiInstructionLength != instructionLength.totalWords * 2)
		{
			uint i;
			bool found = false;
			printf("Error: instruction length decoding mismatch for instruction %04x (%s) - M68060InstructionLength decoder claims %d bytes, Musashi claims %d bytes\n",
			opWord, musashiDisassembledInstruction, instructionLength.totalWords * 2, musashiInstructionLength);
			for (i = 0; i < (sizeof ignoreOps / sizeof ignoreOps[0]); ++i)
				if ((opWord & ignoreOps[i].mask) == ignoreOps[i].match)
					found = true;
					
			if (!found)
				return false;
		}
		
		if (previousDescription != instructionLength.description)
		{
			printf("%04x: %s\n", opWord, instructionLength.description);
			previousDescription = instructionLength.description;
		}
	}
	
	return true;
}

bool soakTestExtensionWordDecoding(void)
{
	const char* previousDescription = 0;
	uint extensionWord;

	printf("Attempting to decode lengths for extension words...\n");
	
	for (extensionWord = 0; extensionWord < 0x10000; ++extensionWord)
	{
		InstructionLength instructionLength;
		bool success;
		uint16_t instructionBuffer[16] = { 0xd691 };
		instructionBuffer[1] = extensionWord;

		success = decodeInstructionLengthFromInstructionWords(instructionBuffer, sizeof instructionBuffer / sizeof instructionBuffer[0], &instructionLength);

		if (!success)
		{
			printf("Error: decoding failed for instruction %04x %04x\n", instructionBuffer[0], instructionBuffer[1]);
			return false;
		}

		if (previousDescription != instructionLength.description)
		{
			printf("%04x %04x: %s\n", instructionBuffer[0], instructionBuffer[1], instructionLength.description);
			previousDescription = instructionLength.description;
		}
	}
	
	return true;
}

int main(void)
{
	if (!soakTestOpWordDecoding())
		return -1;

	if (!soakTestExtensionWordDecoding())
		return -1;

	printf("Done.\n");
		
	return 0;
}