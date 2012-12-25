
#include "DummyMemorySubSystem.h"
#include "../Assert.h"

#include <stdlib.h>

enum { MemorySize = 1024*1024 };

static uint8_t s_memory[MemorySize];

static uint32_t readMemory8(uint32_t address)
{
	uint32_t value;
	M68060_ASSERT(address < MemorySize, "Read outside of available memory region");
	value = s_memory[address];
	return value;
}

static uint32_t readMemory16(uint32_t address)
{
	uint32_t value;
	M68060_ASSERT((address < MemorySize) && (address + sizeof(uint16_t) - 1 < MemorySize), "Read outside of available memory region");
	value = (s_memory[address] << 8) + s_memory[address + 1];
	return value;
}

static uint32_t readMemory32(uint32_t address)
{
	uint32_t value;
	M68060_ASSERT((address < MemorySize) && (address + sizeof(uint32_t) - 1 < MemorySize), "Read outside of available memory region");
	value = (s_memory[address] << 24) + (s_memory[address + 1] << 16) + (s_memory[address + 2] << 8) + s_memory[address + 3];
	return value;
}

static void writeMemory8(uint32_t address, uint32_t value)
{
	M68060_ASSERT(address < MemorySize, "Write outside of available memory region");
	s_memory[address] = value & 0xff;
}

static void writeMemory16(uint32_t address, uint32_t value)
{
	M68060_ASSERT((address < MemorySize) && (address + sizeof(uint16_t) - 1 < MemorySize), "Write outside of available memory region");
	s_memory[address + 0] = (value >> 8) & 0xff;
	s_memory[address + 1] = value & 0xff;
}

static void writeMemory32(uint32_t address, uint32_t value)
{
	M68060_ASSERT((address < MemorySize) && (address + sizeof(uint32_t) - 1 < MemorySize), "Write outside of available memory region");
	s_memory[address + 0] = (value >> 24) & 0xff;
	s_memory[address + 1] = (value >> 16) & 0xff;
	s_memory[address + 2] = (value >> 8) & 0xff;
	s_memory[address + 3] = value & 0xff;
}

void clearMemory(void)
{
	memset(s_memory, 0, sizeof s_memory);
};

uint32_t readMemory(uint32_t address, OperationSize operationSize)
{
	switch (operationSize)
	{
		case OperationSize_Byte:
			return readMemory8(address);
		case OperationSize_Word:
			return readMemory16(address);
		case OperationSize_Long:
			return readMemory32(address);
		default:
			M68060_ERROR("OperationSize not implemented");
			return 0;
	}
}

void writeMemory(uint32_t address, OperationSize operationSize, uint32_t value)
{
	switch (operationSize)
	{
		case OperationSize_Byte:
			writeMemory8(address, value);
			break;
		case OperationSize_Word:
			writeMemory16(address, value);
			break;
		case OperationSize_Long:
			writeMemory32(address, value);
			break;
		default:
			M68060_ERROR("OperationSize not implemented");
	}
}
