
#include "m68kcpu.h"
#include "m68k_log.h"

#include <stdarg.h>

static unsigned char s_musashiMemory[1024 * 1024];

/* Read from anywhere */
unsigned int  m68k_read_memory_8(unsigned int address)
{
	return s_musashiMemory[address];
}

unsigned int  m68k_read_memory_16(unsigned int address)
{
	return (s_musashiMemory[address] << 8) | s_musashiMemory[address + 1];
}

unsigned int  m68k_read_memory_32(unsigned int address)
{
	return (s_musashiMemory[address] << 24) | (s_musashiMemory[address + 1] << 16) | (s_musashiMemory[address + 2] << 8) | s_musashiMemory[address + 3];
}


/* Memory access for the disassembler */
unsigned int m68k_read_disassembler_8  (unsigned int address)
{
	return s_musashiMemory[address];
}

unsigned int m68k_read_disassembler_16 (unsigned int address)
{
	return (s_musashiMemory[address] << 8) | s_musashiMemory[address + 1];
}

unsigned int m68k_read_disassembler_32 (unsigned int address)
{
	return (s_musashiMemory[address] << 24) | (s_musashiMemory[address + 1] << 16) | (s_musashiMemory[address + 2] << 8) | s_musashiMemory[address + 3];
}


/* Write to anywhere */
void m68k_write_memory_8(unsigned int address, unsigned int value)
{
	s_musashiMemory[address] = value & 0xff;
}

void m68k_write_memory_16(unsigned int address, unsigned int value)
{
	s_musashiMemory[address + 0] = (value >> 8) & 0xff;
	s_musashiMemory[address + 1] = value & 0xff;
}

void m68k_write_memory_32(unsigned int address, unsigned int value)
{
	s_musashiMemory[address + 0] = (value >> 24) & 0xff;
	s_musashiMemory[address + 1] = (value >> 16) & 0xff;
	s_musashiMemory[address + 2] = (value >> 8) & 0xff;
	s_musashiMemory[address + 3] = value & 0xff;
}

extern void m68k_enter_subroutine(unsigned int pc, unsigned int targetPc)
{
}

extern void m68k_leave_subroutine()
{
}

void m68k_log(int logLevel, const char* format, ...)
{
	int minLogLevel = M68K_LOG_DEBUG;

    va_list ap;

    if (logLevel < minLogLevel)
        return;

    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
}
