
#include "M68060InstructionLengthDecoder.h"
#include "../Types.h"

#include <stdio.h>
#include <stdlib.h>


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

uint16_t readLittleEndian(uint16_t* data)
{
	uint16_t value = *data;
	return ((value << 8) & 0xff00) | ((value >> 8) & 0x00ff);
}

int main(int argc, char** argv)
{
	void* buffer;
	uint size;
	uint16_t* start;
	uint16_t* end;
	uint16_t* currentInstruction;
	uint16_t instructionBufferUsage;
	uint16_t instructionBuffer[16];
	
	if (argc != 2)
	{
		printf("Usage: InstructionLengthDisassembler <AmigaOS executable file name>\n");
		return 0;
	}
	
	if (!loadFile(argv[1], &buffer, &size))
		return -1;

	if (size < 40 || *((uint32_t*) buffer) != 0xf3030000)
	{
		printf("Not a valid AmigaOS executable\n");
		return -1;
	}

	start = (uint16_t*) ((uint8_t*) buffer + 0x20);
	end = (uint16_t*) ((uint8_t*) buffer + size - 4);
	
	currentInstruction = start;
	instructionBufferUsage = 0;

	printf("Opcodes                       NumWords  Mnemonic\n");
	while (currentInstruction + instructionBufferUsage + 1 < end)
	{
		InstructionLength instructionLength;
		bool decodeSuccess;

		instructionBuffer[instructionBufferUsage] = readLittleEndian(currentInstruction + instructionBufferUsage);
		instructionBufferUsage++;
		
		decodeSuccess = decodeInstructionLengthFromInstructionWords(instructionBuffer, instructionBufferUsage, &instructionLength);
		if (decodeSuccess)
		{
			uint i;
			for (i = 0; i < 6; i++)
				if (i < instructionBufferUsage)
					printf("%04x ", instructionBuffer[i]);
				else if (i < instructionLength.totalWords)
					printf(".... ");
				else
					printf("     ");
			printf("[%d]       %s\n", instructionLength.totalWords, instructionLength.mnemonic);
			
			currentInstruction += instructionLength.totalWords;
			instructionBufferUsage = 0;
		}
		else
		{
			if (instructionBufferUsage > 16)
			{
				printf("Decode failure\n");
				break;
			}

		}
	}
	
	free(buffer);
	return 0;
}