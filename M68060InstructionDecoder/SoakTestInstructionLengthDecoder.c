
#include "../Types.h"
#include "M68060InstructionLengthDecoder.h"

#include <stdio.h>
	
int main(void)
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
			return -1;
		}
		
		if (previousDescription != instructionLength.description)
		{
			printf("%04x: %s\n", opWord, instructionLength.description);
			previousDescription = instructionLength.description;
		}
	}

	return 0;
}