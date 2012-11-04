/*
	This program is intended as a test of the instruction length decoder.
	It loads in an AmigaOS executable from disk, and then plows through it, word by word:
	Each iteration it will attempt to decode the length of the next instruction based on the instruction words seen so far.
	If length decoding succeeds, the instruction words used will be printed, along with the total instruction length and description.
	 Then the entire instruction is skipped.
	If length decoding doesn't succeed, the program will try again with one more instruction word.
	If length decoding doesn't succeed with 16 known instruction words, there's a fatal bug in the decoder and the program will thus give up.

 */

#include "M68060InstructionLengthDecoder.h"
#include "../Types.h"

#include <stdio.h>
#include <stdlib.h>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Given a filename, allocate a buffer of the appropriate size and load in the file's contents.
// Returns true if everything was successful, and false otherwise.

bool loadFile(const char* fileName, void** buf_, uint* size_)
{
	FILE* f = fopen(fileName, "rb");
	uint size;
	void* buf;
	if (!f)
	{
		printf("Cannot open %s\n", fileName);
		return false;
	}
	
	if (fseek(f, 0, SEEK_END))
	{
		printf("Error while seeking in %s\n", fileName);
		fclose(f);
		return false;
	}

	size = ftell(f);
	
	if (fseek(f, 0, SEEK_SET))
	{
		printf("Error while seeking in %s\n", fileName);
		fclose(f);
		return false;
	}

	if (!(buf = malloc(size)))
	{
		printf("Unable to allocate %u bytes\n", size);
		fclose(f);
		return false;
	}
	
	if (fread(buf, 1, size, f) != size)
	{
		printf("Error while reading from %s\n", fileName);
		fclose(f);
		free(buf);
		return false;
	}
	
	fclose(f);
	
	*size_ = size;
	*buf_ = buf;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Read big-endian uint16_t value from pointer, and return uint16_t value in host endianness

uint16_t readBigEndianUInt16(uint16_t* data)
{
	union
	{
		uint16_t data16;
		uint8_t data8[2];
	} conversion;
	conversion.data16 = *data;
	return ((conversion.data8[0] << 8) & 0xff00) | (conversion.data8[1] & 0x00ff);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Read big-endian uint32_t value from pointer, and return uint32_t value in host endianness

uint32_t readBigEndianUInt32(uint32_t* data)
{
	union
	{
		uint32_t data32;
		uint8_t data8[4];
	} conversion;
	conversion.data32 = *data;
	return ((conversion.data8[0] << 24) & 0xff000000)
		| ((conversion.data8[1] << 16) & 0x00ff0000)
		| ((conversion.data8[2] << 8) & 0x0000ff00)
		| (conversion.data8[3] & 0x000000ff);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
	enum { MaxWordsInAnInstruction = 16 };

	void* buffer;
	uint size;
	uint16_t* start;
	uint16_t* end;
	uint16_t* currentInstruction;
	uint16_t instructionBufferUsage;
	uint16_t instructionBuffer[MaxWordsInAnInstruction];
	
	if (argc != 2)
	{
		printf("Usage: InstructionLengthDisassembler <AmigaOS executable file name>\n");
		return 0;
	}

	// Fetch AmigaOS executable from disk
	
	if (!loadFile(argv[1], &buffer, &size))
		return -1;

	// Sanity check the executable; it should have at least some of the standard AmigaOS hunk structure
		
	if (size < 40 || readBigEndianUInt32((uint32_t*) buffer) != 0x000003f3)
	{
		printf("Not a valid AmigaOS executable\n");
		return -1;
	}

	start = (uint16_t*) ((uint8_t*) buffer + 0x20);
	end = (uint16_t*) ((uint8_t*) buffer + size - 4);
	
	currentInstruction = start;
	instructionBufferUsage = 0;

	printf("Opcodes                       NumWords  Description\n");

	while (currentInstruction + instructionBufferUsage + 1 < end)
	{
		InstructionLength instructionLength;
		bool decodeSuccess;

		// Add instruction to the currently-live instruction buffer
		
		instructionBuffer[instructionBufferUsage] = readBigEndianUInt16(currentInstruction + instructionBufferUsage);
		instructionBufferUsage++;

		// Attempt length decode on contents in instruction buffer
		
		decodeSuccess = decodeInstructionLengthFromInstructionWords(instructionBuffer, instructionBufferUsage, &instructionLength);
		if (decodeSuccess)
		{
			// Length decode succeeded; print contents in instruction buffer, and '....' for words not yet fetched
			
			uint i;
			for (i = 0; i < 6; i++)
				if (i < instructionBufferUsage)
					printf("%04x ", instructionBuffer[i]);
				else if (i < instructionLength.totalWords)
					printf(".... ");
				else
					printf("     ");
			printf("[%d]       %s\n", instructionLength.totalWords, instructionLength.description);
			
			currentInstruction += instructionLength.totalWords;	// Skip until beginning of next instruction
			instructionBufferUsage = 0; // Empty instruction buffer
		}
		else
		{
			// If decoding failed... is this due to a length decoder bug, and the decoder is unable to decode this altogether?

			if (instructionBufferUsage >= MaxWordsInAnInstruction)
			{
				printf("Length decoder failure; it hasn't succeeded in decoding the next instruction length with %u words lookahead. Aborting.\n", instructionBufferUsage);
				break;
			}

		}
	}
	
	free(buffer);
	return 0;
}