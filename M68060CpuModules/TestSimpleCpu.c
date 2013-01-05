
#include "M68060SimpleCpu.h"
#include "../M68060InstructionExecution/DummyMemorySubSystem.h"

#include "../Types.h"

#include <stdio.h>
#include <stdlib.h>

bool loadFileIntoMemory(const char* name)
{
	FILE* f = fopen(name, "rb");
	uint size;
	char* buf;
	uint codeStartOffset = 0x20;
	uint i;
	if (!f)
		return false;

	fseek(f, 0, SEEK_END);
	size = ftell(f) - codeStartOffset;
	fseek(f, codeStartOffset, SEEK_SET);
	
	buf = malloc(size);
	fread(buf, 1, size, f);
	fclose(f);
	for (i = 0; i < size; ++i)
		writeMemory(i, OperationSize_Byte, buf[i]);

	free(buf);
		
	return true;
}

int main(int argc, char** argv)
{
	uint x = 0;
	resetCpu();

	if (argc < 2 || !loadFileIntoMemory(argv[1]))
		return 1;
	
	while (1)
	{
		printf("Executing one instruction\n");
		executeCpuInstruction();
		x++;
		if (x % 20 == 0)
		{
			uint i;
			for (i = 0; i < 16; ++i)
				printf(" %04x", readMemory(0x1000 + i * 2, OperationSize_Word));
			printf("\n");
		}
	}
	
	return 0;
}