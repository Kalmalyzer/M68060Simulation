
#include "M68060IntegerRegisterFile.h"
#include "../Assert.h"

#include <string.h>

static uint32_t s_registerFile[16];

void clearIntegerRegisters(void)
{
	memset(s_registerFile, 0, sizeof s_registerFile);
}

uint32_t readIntegerRegister(ExecutionResource reg)
{
	uint id = reg - ExecutionResource_D0;
	M68060_ASSERT(reg >= ExecutionResource_D0 && reg <= ExecutionResource_A7, "ExecutionResource must be one of the D0 .. A7 registers");

	return s_registerFile[id];
}

void writeIntegerRegister(ExecutionResource reg, uint32_t value)
{
	uint id = reg - ExecutionResource_D0;
	M68060_ASSERT(reg >= ExecutionResource_D0 && reg <= ExecutionResource_A7, "ExecutionResource must be one of the D0 .. A7 registers");

	s_registerFile[id] = value;
}

