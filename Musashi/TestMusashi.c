
#include "../68k_emu_core/m68kcpu.h"
#include "../68k_emu_core/m68k_log.h"

#include <stdarg.h>
#include <stdlib.h>

static char mem[4096] =
{
	0x70, 0x01, // moveq #1,d0
	0x4e, 0x71, // nop
};

/* Read from anywhere */
unsigned int  m68k_read_memory_8(unsigned int address)
{
	return mem[address];
}

unsigned int  m68k_read_memory_16(unsigned int address)
{
	return (mem[address] << 8) | mem[address + 1];
}

unsigned int  m68k_read_memory_32(unsigned int address)
{
	return (mem[address] << 24) | (mem[address + 1] << 16) | (mem[address + 2] << 8) | mem[address + 3];
}


/* Memory access for the disassembler */
unsigned int m68k_read_disassembler_8  (unsigned int address)
{
	return mem[address];
}

unsigned int m68k_read_disassembler_16 (unsigned int address)
{
	return (mem[address] << 8) | mem[address + 1];
}

unsigned int m68k_read_disassembler_32 (unsigned int address)
{
	return (mem[address] << 24) | (mem[address + 1] << 16) | (mem[address + 2] << 8) | mem[address + 3];
}


/* Write to anywhere */
void m68k_write_memory_8(unsigned int address, unsigned int value)
{
	mem[address] = value & 0xff;
}

void m68k_write_memory_16(unsigned int address, unsigned int value)
{
	mem[address + 0] = value & 0xff;
	mem[address + 1] = (value >> 8) & 0xff;
}

void m68k_write_memory_32(unsigned int address, unsigned int value)
{
	mem[address + 0] = value & 0xff;
	mem[address + 1] = (value >> 8) & 0xff;
	mem[address + 2] = (value >> 16) & 0xff;
	mem[address + 3] = (value >> 24) & 0xff;
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

int main(void)
{
	int contextSize;
	void* context;
	m68k_set_cpu_type(M68K_CPU_TYPE_68020);

	m68k_pulse_reset();

	contextSize = m68k_context_size();
	context = malloc(contextSize);

	m68k_get_context(context);
	printf("D0: %08x\n", m68k_get_reg(context, M68K_REG_D0));
	
	m68k_execute_single_instruction();

	m68k_get_context(context);
	printf("D0: %08x\n", m68k_get_reg(context, M68K_REG_D0));

	return 0;
}