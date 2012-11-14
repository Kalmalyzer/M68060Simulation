
#include "../Types.h"
#include "M68060InstructionLengthDecoder.h"

#include <stdio.h>

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
		instructionBuffer[0] = opWord;

		success = decodeInstructionLengthFromInstructionWords(instructionBuffer, sizeof instructionBuffer / sizeof instructionBuffer[0], &instructionLength);

		if (!success)
		{
			printf("Error: decoding failed for instruction %04x\n", opWord);
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