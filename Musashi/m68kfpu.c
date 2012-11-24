#include <math.h>
#include "m68k_types.h"
#include "m68k_log.h"
#include "m68kcpu.h"

#define FPCC_N			0x08000000
#define FPCC_Z			0x04000000
#define FPCC_I			0x02000000
#define FPCC_NAN		0x01000000

#define FPES_OE			0x00002000
#define FPAE_IOP		0x00000080

#define DOUBLE_INFINITY					U64(0x7ff0000000000000)
#define DOUBLE_EXPONENT					U64(0x7ff0000000000000)
#define DOUBLE_MANTISSA					U64(0x000fffffffffffff)
#define float64_default_nan_high 0xFFFF
#define float64_default_nan_low  LIT64( 0xFFFFFFFFFFFFFFFF )

// masks for packed dwords, positive k-factor
static const uint32_t pkmask2[18] =
{
	0xffffffff, 0, 0xf0000000, 0xff000000, 0xfff00000, 0xffff0000,
	0xfffff000, 0xffffff00, 0xfffffff0, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
	0xffffffff, 0xffffffff, 0xffffffff
};

static const uint32_t pkmask3[18] =
{
	0xffffffff, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0xf0000000, 0xff000000, 0xfff00000, 0xffff0000,
	0xfffff000, 0xffffff00, 0xfffffff0, 0xffffffff,
};

static void SET_CONDITION_CODES(m68ki_cpu_core *m68k, float64 reg)
{
//  uint64_t *regi;

//  regi = (uint64_t *)&reg;

	REG_FPSR &= ~(FPCC_N|FPCC_Z|FPCC_I|FPCC_NAN);

	// sign flag
	if (reg.v.i & 0x8000)
	{
		REG_FPSR |= FPCC_N;
	}

	// zero flag
	if (((reg.v.i & 0x7fff) == 0) && ((reg.v.i << 1) == 0))
	{
		REG_FPSR |= FPCC_Z;
	}

	// infinity flag
	if (((reg.v.i & 0x7fff) == 0x7fff) && ((reg.v.i << 1) == 0))
	{
		REG_FPSR |= FPCC_I;
	}

	// NaN flag
#if defined(_WIN32)
	if (reg.v.f != reg.v.f)
#else
	if (isnan(reg.v.f))
#endif
	{
		REG_FPSR |= FPCC_NAN;
	}
}

static int TEST_CONDITION(m68ki_cpu_core *m68k, int condition)
{
	int n = (REG_FPSR & FPCC_N) != 0;
	int z = (REG_FPSR & FPCC_Z) != 0;
	int nan = (REG_FPSR & FPCC_NAN) != 0;
	int r = 0;
	switch (condition)
	{
		case 0x10:
		case 0x00:		return 0;					// False

		case 0x11:
		case 0x01:		return (z);					// Equal

		case 0x12:
		case 0x02:		return (!(nan || z || n));			// Greater Than

		case 0x13:
		case 0x03:		return (z || !(nan || n));			// Greater or Equal

		case 0x14:
		case 0x04:		return (n && !(nan || z));			// Less Than

		case 0x15:
		case 0x05:		return (z || (n && !nan));			// Less Than or Equal

		case 0x16:
		case 0x06:		return !nan && !z;

		case 0x17:
		case 0x07:		return !nan;

		case 0x18:
		case 0x08:		return nan;

		case 0x19:
		case 0x09:		return nan || z;

		case 0x1a:
		case 0x0a:		return (nan || !(n || z));			// Not Less Than or Equal

		case 0x1b:
		case 0x0b:		return (nan || z || !n);			// Not Less Than

		case 0x1c:
		case 0x0c:		return (nan || (n && !z));			// Not Greater or Equal Than

		case 0x1d:
		case 0x0d:		return (nan || z || n);				// Not Greater Than

		case 0x1e:
		case 0x0e:		return (!z);					// Not Equal

		case 0x1f:
		case 0x0f:		return 1;					// True

		default:		m68k_log(M68K_LOG_ERROR, "M68kFPU: test_condition: unhandled condition %02X\n", condition);
	}

	return r;
}

static uint8_t READ_EA_8(int ea)
{
	int mode = (ea >> 3) & 0x7;
	int reg = (ea & 0x7);

	switch (mode)
	{
		case 0:		// Dn
		{
			return (uint8_t)REG_D[reg];
		}
		case 2: 	// (An)
		{
			ea = REG_A[reg];
			return (uint8_t)m68ki_read_8(ea);
		}
		case 3:		// (An)+
		{
			ea = EA_AY_PI_8();
			return (uint8_t)m68ki_read_8(ea);
		}
		case 4:		// -(An)
		{
			ea = EA_AY_PD_8();
			return (uint8_t)m68ki_read_8(ea);
		}
		case 5:		// (d16, An)
		{
			ea = EA_AY_DI_8();
			return (uint8_t)m68ki_read_8(ea);
		}
		case 6:		// (An) + (Xn) + d8
		{
			ea = EA_AY_IX_8();
			return (uint8_t)m68ki_read_8(ea);
		}
		case 7:
		{
			switch (reg)
			{
				case 0:		// (xxx).W
				{
					ea = (uint32_t)OPER_I_16();
					return (uint8_t)m68ki_read_8(ea);
				}
				case 1:		// (xxx).L
				{
					uint32_t d1 = OPER_I_16();
					uint32_t d2 = OPER_I_16();
					ea = (d1 << 16) | d2;
					return (uint8_t)m68ki_read_8(ea);
				}
				case 2:		// (d16, PC)
				{
					ea = EA_PCDI_8();
					return (uint8_t)m68ki_read_8(ea);
				}
				case 3:		// (PC) + (Xn) + d8
				{
					ea =  EA_PCIX_8();
					return (uint8_t)m68ki_read_8(ea);
				}
				case 4:		// #<data>
				{
					return  (uint8_t)OPER_I_8();
				}
				default:	m68k_log(M68K_LOG_ERROR, "M68kFPU: READ_EA_8: unhandled mode %d, reg %d at %08X\n", mode, reg, REG_PC);
			}
			break;
		}
		default:	m68k_log(M68K_LOG_ERROR, "M68kFPU: READ_EA_8: unhandled mode %d, reg %d at %08X\n", mode, reg, REG_PC);
	}

	return 0;
}

static uint16_t READ_EA_16(m68ki_cpu_core *m68k, int ea)
{
	int mode = (ea >> 3) & 0x7;
	int reg = (ea & 0x7);

	switch (mode)
	{
		case 0:		// Dn
		{
			return (uint16_t)(REG_D[reg]);
		}
		case 2:		// (An)
		{
			ea = REG_A[reg];
			return (uint16_t)m68ki_read_16(ea);
		}
		case 3:		// (An)+
		{
			ea = EA_AY_PI_16();
			return (uint16_t)m68ki_read_16(ea);
		}
		case 4:		// -(An)
		{
			ea = EA_AY_PD_16();
			return (uint16_t)m68ki_read_16(ea);
		}
		case 5:		// (d16, An)
		{
			ea = EA_AY_DI_16();
			return (uint16_t)m68ki_read_16(ea);
		}
		case 6:		// (An) + (Xn) + d8
		{
			ea = EA_AY_IX_16();
			return (uint16_t)m68ki_read_16(ea);
		}
		case 7:
		{
			switch (reg)
			{
				case 0:		// (xxx).W
				{
					ea = (uint32_t)OPER_I_16();
					return (uint16_t)m68ki_read_16(ea);
				}
				case 1:		// (xxx).L
				{
					uint32_t d1 = OPER_I_16();
					uint32_t d2 = OPER_I_16();
					ea = (d1 << 16) | d2;
					return (uint16_t)m68ki_read_16(ea);
				}
				case 2:		// (d16, PC)
				{
					ea = EA_PCDI_16();
					return (uint16_t)m68ki_read_16(ea);
				}
				case 3:		// (PC) + (Xn) + d8
				{
					ea =  EA_PCIX_16();
					return (uint16_t)m68ki_read_16(ea);
				}
				case 4:		// #<data>
				{
					return (uint16_t)OPER_I_16();
				}

				default:	m68k_log(M68K_LOG_ERROR, "M68kFPU: READ_EA_16: unhandled mode %d, reg %d at %08X\n", mode, reg, REG_PC);
			}
			break;
		}
		default:	m68k_log(M68K_LOG_ERROR, "M68kFPU: READ_EA_16: unhandled mode %d, reg %d at %08X\n", mode, reg, REG_PC);
	}

	return 0;
}

static uint32_t READ_EA_32(int ea)
{
	int mode = (ea >> 3) & 0x7;
	int reg = (ea & 0x7);

	switch (mode)
	{
		case 0:		// Dn
		{
			return REG_D[reg];
		}
		case 2:		// (An)
		{
			ea = REG_A[reg];
			return m68ki_read_32(ea);
		}
		case 3:		// (An)+
		{
			ea = EA_AY_PI_32();
			return m68ki_read_32(ea);
		}
		case 4:		// -(An)
		{
			ea = EA_AY_PD_32();
			return m68ki_read_32(ea);
		}
		case 5:		// (d16, An)
		{
			ea = EA_AY_DI_32();
			return m68ki_read_32(ea);
		}
		case 6:		// (An) + (Xn) + d8
		{
			ea = EA_AY_IX_32();
			return m68ki_read_32(ea);
		}
		case 7:
		{
			switch (reg)
			{
				case 0:		// (xxx).W
				{
					ea = (uint32_t)OPER_I_16();
					return m68ki_read_32(ea);
				}
				case 1:		// (xxx).L
				{
					uint32_t d1 = OPER_I_16();
					uint32_t d2 = OPER_I_16();
					ea = (d1 << 16) | d2;
					return m68ki_read_32(ea);
				}
				case 2:		// (d16, PC)
				{
					ea = EA_PCDI_32();
					return m68ki_read_32(ea);
				}
				case 3:		// (PC) + (Xn) + d8
				{
					ea =  EA_PCIX_32();
					return m68ki_read_32(ea);
				}
				case 4:		// #<data>
				{
					return  OPER_I_32();
				}
				default:	m68k_log(M68K_LOG_ERROR, "M68kFPU: READ_EA_32: unhandled mode %d, reg %d at %08X\n", mode, reg, REG_PC);
			}
			break;
		}
		default:	m68k_log(M68K_LOG_ERROR, "M68kFPU: READ_EA_32: unhandled mode %d, reg %d at %08X\n", mode, reg, REG_PC);
	}
	return 0;
}

static uint64_t READ_EA_64(m68ki_cpu_core *m68k, int ea)
{
	int mode = (ea >> 3) & 0x7;
	int reg = (ea & 0x7);
	uint32_t h1, h2;

	switch (mode)
	{
		case 2:		// (An)
		{
			ea = REG_A[reg];
			h1 = m68ki_read_32(ea+0);
			h2 = m68ki_read_32(ea+4);
			return  (uint64_t)(h1) << 32 | (uint64_t)(h2);
		}
		case 3:		// (An)+
		{
			ea = REG_A[reg];
			REG_A[reg] += 8;
			h1 = m68ki_read_32(ea+0);
			h2 = m68ki_read_32(ea+4);
			return  (uint64_t)(h1) << 32 | (uint64_t)(h2);
		}
		case 4:		// -(An)
		{
			ea = REG_A[reg]-8;
			REG_A[reg] -= 8;
			h1 = m68ki_read_32(ea+0);
			h2 = m68ki_read_32(ea+4);
			return  (uint64_t)(h1) << 32 | (uint64_t)(h2);
		}
		case 5:		// (d16, An)
		{
			ea = EA_AY_DI_32();
			h1 = m68ki_read_32(ea+0);
			h2 = m68ki_read_32(ea+4);
			return  (uint64_t)(h1) << 32 | (uint64_t)(h2);
		}
		case 6:		// (An) + (Xn) + d8
		{
			ea = EA_AY_IX_32();
			h1 = m68ki_read_32(ea+0);
			h2 = m68ki_read_32(ea+4);
			return  (uint64_t)(h1) << 32 | (uint64_t)(h2);
		}
		case 7:
		{
			switch (reg)
			{
				case 1:		// (xxx).L
				{
					uint32_t d1 = OPER_I_16();
					uint32_t d2 = OPER_I_16();
					ea = (d1 << 16) | d2;
					return (uint64_t)(m68ki_read_32(ea)) << 32 | (uint64_t)(m68ki_read_32(ea+4));
				}
				case 3:		// (PC) + (Xn) + d8
				{
					ea =  EA_PCIX_32();
					h1 = m68ki_read_32(ea+0);
					h2 = m68ki_read_32(ea+4);
					return  (uint64_t)(h1) << 32 | (uint64_t)(h2);
				}
				case 4:		// #<data>
				{
					h1 = OPER_I_32();
					h2 = OPER_I_32();
					return  (uint64_t)(h1) << 32 | (uint64_t)(h2);
				}
				case 2:		// (d16, PC)
				{
					ea = EA_PCDI_32();
					h1 = m68ki_read_32(ea+0);
					h2 = m68ki_read_32(ea+4);
					return  (uint64_t)(h1) << 32 | (uint64_t)(h2);
				}
				default:	m68k_log(M68K_LOG_ERROR, "M68kFPU: READ_EA_64: unhandled mode %d, reg %d at %08X\n", mode, reg, REG_PC);
			}
			break;
		}
		default:	m68k_log(M68K_LOG_ERROR, "M68kFPU: READ_EA_64: unhandled mode %d, reg %d at %08X\n", mode, reg, REG_PC);
	}

	return 0;
}


static void WRITE_EA_8(m68ki_cpu_core *m68k, int ea, uint8_t data)
{
	int mode = (ea >> 3) & 0x7;
	int reg = (ea & 0x7);

	switch (mode)
	{
		case 0:		// Dn
		{
			REG_D[reg] = data;
			break;
		}
		case 2:		// (An)
		{
			ea = REG_A[reg];
			m68ki_write_8(ea, data);
			break;
		}
		case 3:		// (An)+
		{
			ea = EA_AY_PI_8();
			m68ki_write_8(ea, data);
			break;
		}
		case 4:		// -(An)
		{
			ea = EA_AY_PD_8();
			m68ki_write_8(ea, data);
			break;
		}
		case 5:		// (d16, An)
		{
			ea = EA_AY_DI_8();
			m68ki_write_8(ea, data);
			break;
		}
		case 6:		// (An) + (Xn) + d8
		{
			ea = EA_AY_IX_8();
			m68ki_write_8(ea, data);
			break;
		}
		case 7:
		{
			switch (reg)
			{
				case 1:		// (xxx).B
				{
					uint32_t d1 = OPER_I_16();
					uint32_t d2 = OPER_I_16();
					ea = (d1 << 16) | d2;
					m68ki_write_8(ea, data);
					break;
				}
				case 2:		// (d16, PC)
				{
					ea = EA_PCDI_16();
					m68ki_write_8(ea, data);
					break;
				}
				default:	m68k_log(M68K_LOG_ERROR, "M68kFPU: WRITE_EA_8: unhandled mode %d, reg %d at %08X\n", mode, reg, REG_PC);
			}
			break;
		}
		default:	m68k_log(M68K_LOG_ERROR, "M68kFPU: WRITE_EA_8: unhandled mode %d, reg %d, data %08X at %08X\n", mode, reg, data, REG_PC);
	}
}

static void WRITE_EA_16(m68ki_cpu_core *m68k, int ea, uint16_t data)
{
	int mode = (ea >> 3) & 0x7;
	int reg = (ea & 0x7);

	switch (mode)
	{
		case 0:		// Dn
		{
			REG_D[reg] = data;
			break;
		}
		case 2:		// (An)
		{
			ea = REG_A[reg];
			m68ki_write_16(ea, data);
			break;
		}
		case 3:		// (An)+
		{
			ea = EA_AY_PI_16();
			m68ki_write_16(ea, data);
			break;
		}
		case 4:		// -(An)
		{
			ea = EA_AY_PD_16();
			m68ki_write_16(ea, data);
			break;
		}
		case 5:		// (d16, An)
		{
			ea = EA_AY_DI_16();
			m68ki_write_16(ea, data);
			break;
		}
		case 6:		// (An) + (Xn) + d8
		{
			ea = EA_AY_IX_16();
			m68ki_write_16(ea, data);
			break;
		}
		case 7:
		{
			switch (reg)
			{
				case 1:		// (xxx).W
				{
					uint32_t d1 = OPER_I_16();
					uint32_t d2 = OPER_I_16();
					ea = (d1 << 16) | d2;
					m68ki_write_16(ea, data);
					break;
				}
				case 2:		// (d16, PC)
				{
					ea = EA_PCDI_16();
					m68ki_write_16(ea, data);
					break;
				}
				default:	m68k_log(M68K_LOG_ERROR, "M68kFPU: WRITE_EA_16: unhandled mode %d, reg %d at %08X\n", mode, reg, REG_PC);
			}
			break;
		}
		default:	m68k_log(M68K_LOG_ERROR, "M68kFPU: WRITE_EA_16: unhandled mode %d, reg %d, data %08X at %08X\n", mode, reg, data, REG_PC);
	}
}

static void WRITE_EA_32(m68ki_cpu_core *m68k, int ea, uint32_t data)
{
	int mode = (ea >> 3) & 0x7;
	int reg = (ea & 0x7);

	switch (mode)
	{
		case 0:		// Dn
		{
			REG_D[reg] = data;
			break;
		}
		case 1:		// An
		{
			REG_A[reg] = data;
			break;
		}
		case 2:		// (An)
		{
			ea = REG_A[reg];
			m68ki_write_32(ea, data);
			break;
		}
		case 3:		// (An)+
		{
			ea = EA_AY_PI_32();
			m68ki_write_32(ea, data);
			break;
		}
		case 4:		// -(An)
		{
			ea = EA_AY_PD_32();
			m68ki_write_32(ea, data);
			break;
		}
		case 5:		// (d16, An)
		{
			ea = EA_AY_DI_32();
			m68ki_write_32(ea, data);
			break;
		}
		case 6:		// (An) + (Xn) + d8
		{
			ea = EA_AY_IX_32();
			m68ki_write_32(ea, data);
			break;
		}
		case 7:
		{
			switch (reg)
			{
				case 1:		// (xxx).L
				{
					uint32_t d1 = OPER_I_16();
					uint32_t d2 = OPER_I_16();
					ea = (d1 << 16) | d2;
					m68ki_write_32(ea, data);
					break;
				}
				case 2:		// (d16, PC)
				{
					ea = EA_PCDI_32();
					m68ki_write_32(ea, data);
					break;
				}
				default:	m68k_log(M68K_LOG_ERROR, "M68kFPU: WRITE_EA_32: unhandled mode %d, reg %d at %08X\n", mode, reg, REG_PC);
			}
			break;
		}
		default:	m68k_log(M68K_LOG_ERROR, "M68kFPU: WRITE_EA_32: unhandled mode %d, reg %d, data %08X at %08X\n", mode, reg, data, REG_PC);
	}
}

static void WRITE_EA_64(m68ki_cpu_core *m68k, int ea, uint64_t data)
{
	int mode = (ea >> 3) & 0x7;
	int reg = (ea & 0x7);

	switch (mode)
	{
		case 2:		// (An)
		{
			ea = REG_A[reg];
			m68ki_write_32(ea, (uint32_t)(data >> 32));
			m68ki_write_32(ea+4, (uint32_t)(data));
			break;
		}
		case 3:		// (An)+
		{
			ea = REG_A[reg];
			REG_A[reg] += 8;
			m68ki_write_32(ea+0, (uint32_t)(data >> 32));
			m68ki_write_32(ea+4, (uint32_t)(data));
			break;
		}
		case 4:		// -(An)
		{
			REG_A[reg] -= 8;
			ea = REG_A[reg];
			m68ki_write_32(ea+0, (uint32_t)(data >> 32));
			m68ki_write_32(ea+4, (uint32_t)(data));
			break;
		}
		case 5:		// (d16, An)
		{
			ea = EA_AY_DI_32();
			m68ki_write_32(ea+0, (uint32_t)(data >> 32));
			m68ki_write_32(ea+4, (uint32_t)(data));
			break;
		}
		case 6:		// (An) + (Xn) + d8
		{
			ea = EA_AY_IX_32();
			m68ki_write_32(ea+0, (uint32_t)(data >> 32));
			m68ki_write_32(ea+4, (uint32_t)(data));
			break;
		}
		case 7:
		{
			switch (reg)
			{
				case 1:		// (xxx).L
				{
					uint32_t d1 = OPER_I_16();
					uint32_t d2 = OPER_I_16();
					ea = (d1 << 16) | d2;
					m68ki_write_32(ea+0, (uint32_t)(data >> 32));
					m68ki_write_32(ea+4, (uint32_t)(data));
					break;
				}
				case 2:		// (d16, PC)
				{
					ea = EA_PCDI_32();
					m68ki_write_32(ea+0, (uint32_t)(data >> 32));
					m68ki_write_32(ea+4, (uint32_t)(data));
					break;
				}
				default:	m68k_log(M68K_LOG_ERROR, "M68kFPU: WRITE_EA_64: unhandled mode %d, reg %d at %08X\n", mode, reg, REG_PC);
			}
			break;
		}
		default:	m68k_log(M68K_LOG_ERROR, "M68kFPU: WRITE_EA_64: unhandled mode %d, reg %d, data %08X%08X at %08X\n", mode, reg, (uint32_t)(data >> 32), (uint32_t)(data), REG_PC);
	}
}

static void fpgen_rm_reg(m68ki_cpu_core *m68k, uint16_t w2)
{
	int ea = m68k->ir & 0x3f;
	int rm = (w2 >> 14) & 0x1;
	int src = (w2 >> 10) & 0x7;
	int dst = (w2 >>  7) & 0x7;
	int opmode = w2 & 0x7f;
	float64 source;

	// fmovecr #$f, fp0 f200 5c0f

	if (rm)
	{
		switch (src)
		{
			case 0:		// Long-Word Integer
			{
				source.v.f = (double)READ_EA_32(ea);
				break;
			}
			case 1:		// Single-precision Real
			{
				union 
				{
					uint32_t d;
					float f;
				} data;

				data.d = READ_EA_32(ea);
				source.v.f = data.f; 
				break;
			}
			case 2:		// Extended-precision Real
			{
				m68k_log(M68K_LOG_ERROR, "fmove_rm_reg: Extended-precision Real is not supported!\n");
				break;
			}
			case 3:		// Packed-decimal Real
			{
				m68k_log(M68K_LOG_ERROR, "fmove_rm_reg: Packed-decimal Real not supported!\n");
				break;
			}
			case 4:		// Word Integer
			{
				source.v.f = (double)READ_EA_16(m68k, ea);
				break;
			}
			case 5:		// Double-precision Real
			{
				source.v.i = READ_EA_64(m68k, ea);
				break;
			}
			case 6:		// Byte Integer
			{
				source.v.f = (double)READ_EA_8(ea);
				break;
			}
			case 7:		// FMOVECR load from constant ROM
			{
				m68k_log(M68K_LOG_ERROR, "fmove_rm_reg: Is un-implemented as its emulated on 060. Don't use!\n"); 
				return;
			}
			default:	m68k_log(M68K_LOG_ERROR, "fmove_rm_reg: invalid source specifier %x at %08X\n", src, REG_PC-4);
		}
	}
	else
	{
		source = REG_FP[src];
	}

	switch (opmode)
	{
		case 0x00:		// FMOVE
		{
			REG_FP[dst] = source;
			SET_CONDITION_CODES(m68k, REG_FP[dst]);
			break;
		}
		case 0x01:		// FINT
		{
			REG_FP[dst].v.i = (int)source.v.f; 
			break;
		}
		case 0x03:		// FINTRZ
		{
			REG_FP[dst].v.f = (double)(int)(floor(source.v.f + 0.5));
			break;
		}
		case 0x04:		// FSQRT
		{
			REG_FP[dst].v.f = sqrt(source.v.f);
			SET_CONDITION_CODES(m68k, REG_FP[dst]);
			break;
		}
		case 0x06:		// FLOGNP1
		{
			REG_FP[dst].v.f = log(source.v.f + 1.0);
			SET_CONDITION_CODES(m68k, REG_FP[dst]);
			break;
		}
		case 0x0e:      // FSIN
		{
			REG_FP[dst].v.f = sin(source.v.f);
			SET_CONDITION_CODES(m68k, REG_FP[dst]);
			break;
		}
		case 0x0f:		// FTAN
		{
			REG_FP[dst].v.f = tan(source.v.f);
			SET_CONDITION_CODES(m68k, REG_FP[dst]);
			break;
		}
		case 0x14:		// FLOGN
		{
			REG_FP[dst].v.f = log(source.v.f);
			SET_CONDITION_CODES(m68k, REG_FP[dst]);
			break;
		}
		case 0x15:		// FLOG10
		{
			REG_FP[dst].v.f = log10(source.v.f);
			SET_CONDITION_CODES(m68k, REG_FP[dst]);
			break;
		}
		case 0x16:		// FLOG2
		{
			REG_FP[dst].v.f = log(source.v.f) / log(2.0);
			SET_CONDITION_CODES(m68k, REG_FP[dst]);
			break;
		}
		case 0x18:		// FABS
		{
			REG_FP[dst].v.f = fabs(source.v.f);
			SET_CONDITION_CODES(m68k, REG_FP[dst]);
			break;
		}
		case 0x1a:		// FNEG
		{
			REG_FP[dst].v.f = -source.v.f;
			SET_CONDITION_CODES(m68k, REG_FP[dst]);
			break;
		}
		case 0x1d:      // FCOS
		{
			REG_FP[dst].v.f = cos(source.v.f);
			SET_CONDITION_CODES(m68k, REG_FP[dst]);
			break;
		}
		case 0x1e:		// FGETEXP
		{
			REG_FP[dst].v.f = exp(source.v.f);
			SET_CONDITION_CODES(m68k, REG_FP[dst]);
			break;
		}
		case 0x20:		// FDIV
		{
			REG_FP[dst].v.f = REG_FP[dst].v.f / source.v.f;
			break;
		}
		case 0x22:		// FADD
		{
			REG_FP[dst].v.f += source.v.f;
			SET_CONDITION_CODES(m68k, REG_FP[dst]);
			break;
		}
		case 0x23:		// FMUL
		{
			REG_FP[dst].v.f *= source.v.f;
			SET_CONDITION_CODES(m68k, REG_FP[dst]);
			break;
		}
		case 0x24:		// FSGLDIV
		{
			REG_FP[dst].v.f = (float)REG_FP[dst].v.f / (float)source.v.f;
			break;
		}
		case 0x25:		// FREM
		{
			m68k_log(M68K_LOG_ERROR, "frem isn't implemented\n!");
			REG_FP[dst].v.f = 0.0; 
			break;
		}
		case 0x27:		// FSGLMUL
		{
			REG_FP[dst].v.f = (float)REG_FP[dst].v.f * (float)source.v.f;
			SET_CONDITION_CODES(m68k, REG_FP[dst]);
			break;
		}
		case 0x28:		// FSUB
		{
			REG_FP[dst].v.f -= source.v.f;
			SET_CONDITION_CODES(m68k, REG_FP[dst]);
			break;
		}
		case 0x38:		// FCMP
		{
			float64 res;
			res.v.f = REG_FP[dst].v.f - source.v.f;
			SET_CONDITION_CODES(m68k, res);
			break;
		}
		case 0x3a:		// FTST
		{
			SET_CONDITION_CODES(m68k, source);
			break;
		}

		default:	m68k_log(M68K_LOG_ERROR, "fpgen_rm_reg: unimplemented opmode %02X at %08X\n", opmode, REG_PPC);
	}
}

static void fmove_reg_mem(m68ki_cpu_core *m68k, uint16_t w2)
{
	int ea = m68k->ir & 0x3f;
	int src = (w2 >>  7) & 0x7;
	int dst = (w2 >> 10) & 0x7;
	//int k = (w2 & 0x7f);

	switch (dst)
	{
		case 0:		// Long-Word Integer
		{
			int32_t d = (int32_t)REG_FP[src].v.f;
			WRITE_EA_32(m68k, ea, d);
			break;
		}
		case 1:		// Single-precision Real
		{
			union
			{
				uint32_t d;
				float f;
			} data;

			data.f = (float)REG_FP[src].v.f;
			WRITE_EA_32(m68k, ea, data.d);
			break;
		}
		case 2:		// Extended-precision Real
		{
			m68k_log(M68K_LOG_ERROR, "Write of Extended-precision Real is unsupported\n"); 
			break;
		}
		case 3:		// Packed-decimal Real with Static K-factor
		{
			m68k_log(M68K_LOG_ERROR, "Write of Packed-decimal is unsupported\n"); 
			break;
		}
		case 4:		// Word Integer
		{
			int32_t value;
			union
			{
				int32_t d;
				float f;
			} data;

			data.f = (float)REG_FP[src].v.f;
			value = data.d; 

			if (value > 0x7fff || value < -0x8000 )
			{
				REG_FPSR |= FPES_OE | FPAE_IOP;
			}
			WRITE_EA_16(m68k, ea, (int16_t)value);
			break;
		}
		case 5:		// Double-precision Real
		{
			WRITE_EA_64(m68k, ea, REG_FP[src].v.i);
			break;
		}
		case 6:		// Byte Integer
		{
			int32_t value;
			union
			{
				int32_t d;
				float f;
			} data;

			data.f = (float)(REG_FP[src].v.f);
			value = data.d; 

			if (value > 127 || value < -128)
			{
				REG_FPSR |= FPES_OE | FPAE_IOP;
			}
			WRITE_EA_8(m68k, ea, (int8_t) value);
			break;
		}
		case 7:		// Packed-decimal Real with Dynamic K-factor
		{
			m68k_log(M68K_LOG_ERROR, "Write of Packed-decimal is unsupported\n"); 
			break;
		}
	}
}

static void fmove_fpcr(m68ki_cpu_core *m68k, uint16_t w2)
{
	int ea = m68k->ir & 0x3f;
	int dir = (w2 >> 13) & 0x1;
	int regsel = (w2 >> 10) & 0x7;
	int mode = (ea >> 3) & 0x7;

	if ((mode == 5) || (mode == 6))
	{
		uint32_t address = 0xffffffff;	// force a bus error if this doesn't get assigned

		if (mode == 5)
		{
			address = EA_AY_DI_32();
		}
		else if (mode == 6)
		{
			address = EA_AY_IX_32();
		}

		if (dir)	// From system control reg to <ea>
		{
			if (regsel & 4) { m68ki_write_32(address, REG_FPCR); address += 4; }
			if (regsel & 2) { m68ki_write_32(address, REG_FPSR); address += 4; }
			if (regsel & 1) { m68ki_write_32(address, REG_FPIAR); address += 4; }
		}
		else		// From <ea> to system control reg
		{
			if (regsel & 4) { REG_FPCR = m68ki_read_32(address); address += 4; }
			if (regsel & 2) { REG_FPSR= m68ki_read_32(address); address += 4; }
			if (regsel & 1) { REG_FPIAR = m68ki_read_32(address); address += 4; }
		}
	}
	else
	{
		if (dir)	// From system control reg to <ea>
		{
			if (regsel & 4) WRITE_EA_32(m68k, ea, REG_FPCR);
			if (regsel & 2) WRITE_EA_32(m68k, ea, REG_FPSR);
			if (regsel & 1) WRITE_EA_32(m68k, ea, REG_FPIAR);
		}
		else		// From <ea> to system control reg
		{
			if (regsel & 4) REG_FPCR = READ_EA_32(ea);
			if (regsel & 2) REG_FPSR = READ_EA_32(ea);
			if (regsel & 1) REG_FPIAR = READ_EA_32(ea);
		}
	}

#if 0
	// FIXME: (2011-12-18 ost)
	// rounding_mode and rounding_precision of softfloat.c should be set according to current fpcr
	// but:  with this code on Apollo the following programs in /systest/fptest will fail:
	// 1. Single Precision Whetstone will return wrong results never the less
	// 2. Vector Test will fault with 00040004: reference to illegal address

	if ((regsel & 4) && dir == 0)
	{
		int rnd = (REG_FPCR() >> 4) & 3;
		int prec = (REG_FPCR() >> 6) & 3;

		logerror("m68k_fpsp:fmove_fpcr fpcr=%04x prec=%d rnd=%d\n", REG_FPCR(), prec, rnd);

#ifdef float64
		switch (prec)
		{
		case 0: // Extend (X)
			float64_rounding_precision = 80;
			break;
		case 1: // Single (S)
			float64_rounding_precision = 32;
			break;
		case 2: // Double (D)
			float64_rounding_precision = 64;
			break;
		case 3: // Undefined
			float64_rounding_precision = 80;
			break;
		}
#endif

		switch (rnd)
		{
		case 0: // To Nearest (RN)
			float_rounding_mode = float_round_nearest_even;
			break;
		case 1: // To Zero (RZ)
			float_rounding_mode = float_round_to_zero;
			break;
		case 2: // To Minus Infinitiy (RM)
			float_rounding_mode = float_round_down;
			break;
		case 3: // To Plus Infinitiy (RP)
			float_rounding_mode = float_round_up;
			break;
		}
	}
#endif

}

static void fmovem(m68ki_cpu_core *m68k, uint16_t w2)
{
	int i;
	int ea = m68k->ir & 0x3f;
	int dir = (w2 >> 13) & 0x1;
	int mode = (w2 >> 11) & 0x3;
	int reglist = w2 & 0xff;

	uint32_t mem_addr = 0;
	switch (ea >> 3)
	{
		case 5:		// (d16, An)
			mem_addr= EA_AY_DI_32();
			break;
		case 6:		// (An) + (Xn) + d8
			mem_addr= EA_AY_IX_32();
			break;
	}

	if (dir)	// From FP regs to mem
	{
		switch (mode)
		{
			case 1: // Dynamic register list, postincrement or control addressing mode.
				// FIXME: not really tested, but seems to work
				reglist = REG_D[(reglist >> 4) & 7];

			case 0:		// Static register list, predecrement or control addressing mode
			{
				for (i=0; i < 8; i++)
				{
					if (reglist & (1 << i))
					{
						switch (ea >> 3)
						{
							case 5:		// (d16, An)
							case 6:		// (An) + (Xn) + d8
								WRITE_EA_64(m68k, mem_addr, REG_FP[i].v.i);
								mem_addr += 8;
								break;
							default:
								WRITE_EA_64(m68k, ea, REG_FP[i].v.i);
								break;
						}
					}
				}
				break;
			}

			case 2:		// Static register list, postdecrement or control addressing mode
			{
				for (i=0; i < 8; i++)
				{
					if (reglist & (1 << i))
					{
						switch (ea >> 3)
						{
							case 5:		// (d16, An)
							case 6:		// (An) + (Xn) + d8
								WRITE_EA_64(m68k, mem_addr, REG_FP[7 - i].v.i);
								mem_addr += 8;
								break;
							default:
								WRITE_EA_64(m68k, ea, REG_FP[7-i].v.i);
								break;
						}
					}
				}
				break;
			}

			default:	m68k_log(M68K_LOG_ERROR, "M680x0: FMOVEM: mode %d unimplemented at %08X\n", mode, REG_PC-4);
		}
	}
	else		// From mem to FP regs
	{
		switch (mode)
		{
			case 3: // Dynamic register list, predecrement addressing mode.
				// FIXME: not really tested, but seems to work
				reglist = REG_D[(reglist >> 4) & 7];

			case 2:		// Static register list, postincrement or control addressing mode
			{
				for (i=0; i < 8; i++)
				{
					if (reglist & (1 << i))
					{
						switch (ea >> 3)
						{
							case 5:		// (d16, An)
							case 6:		// (An) + (Xn) + d8
								REG_FP[7-i].v.i = READ_EA_64(m68k, mem_addr);
								mem_addr += 8;
								break;
							default:
								REG_FP[7-i].v.i = READ_EA_64(m68k, ea);
								break;
						}
					}
				}
				break;
			}

			default:	m68k_log(M68K_LOG_ERROR, "M680x0: FMOVEM: mode %d unimplemented at %08X\n", mode, REG_PC-4);
		}
	}
}

static void fscc(m68ki_cpu_core *m68k)
{
	int ea = m68k->ir & 0x3f;
	int condition = (int16_t)(OPER_I_16());

	WRITE_EA_8(m68k, ea, TEST_CONDITION(m68k, condition) ? 0xff : 0);
}

static void fbcc16(m68ki_cpu_core *m68k)
{
	int32_t offset;
	int condition = m68k->ir & 0x3f;

	offset = (int16_t)(OPER_I_16());

	// TODO: condition and jump!!!
	if (TEST_CONDITION(m68k, condition))
	{
		m68ki_trace_t0();			   /* auto-disable (see m68kcpu.h) */
		m68ki_branch_16(offset-2);
	}
}

static void fbcc32(m68ki_cpu_core *m68k)
{
	int32_t offset;
	int condition = m68k->ir & 0x3f;

	offset = OPER_I_32();

	// TODO: condition and jump!!!
	if (TEST_CONDITION(m68k, condition))
	{
		m68ki_trace_t0();			   /* auto-disable (see m68kcpu.h) */
		m68ki_branch_32(offset-4);
	}
}


void m68040_fpu_op0(m68ki_cpu_core *m68k)
{
	m68k->fpu_just_reset = 0;

	switch ((m68k->ir >> 6) & 0x3)
	{
		case 0:
		{
			uint16_t w2 = (uint16_t)OPER_I_16();
			switch ((w2 >> 13) & 0x7)
			{
				case 0x0:	// FPU ALU FP, FP
				case 0x2:	// FPU ALU ea, FP
				{
					fpgen_rm_reg(m68k, w2);
					break;
				}

				case 0x3:	// FMOVE FP, ea
				{
					fmove_reg_mem(m68k, w2);
					break;
				}

				case 0x4:	// FMOVEM ea, FPCR
				case 0x5:	// FMOVEM FPCR, ea
				{
					fmove_fpcr(m68k, w2);
					break;
				}

				case 0x6:	// FMOVEM ea, list
				case 0x7:	// FMOVEM list, ea
				{
					fmovem(m68k, w2);
					break;
				}

				default:	m68k_log(M68K_LOG_ERROR, "M68kFPU: unimplemented subop %d at %08X\n", (w2 >> 13) & 0x7, REG_PC-4);
			}
			break;
		}

		case 1:		// FBcc disp16
		{
			switch ((m68k->ir >> 3) & 0x7) {
			case 1: // FDBcc
				// TODO:
				break;
			default: // FScc (?)
				fscc(m68k);
				return;
			}
			m68k_log(M68K_LOG_ERROR, "M68kFPU: unimplemented main op %d with mode %d at %08X\n", (m68k->ir >> 6) & 0x3, (m68k->ir >> 3) & 0x7, REG_PPC);
		}

		case 2:		// FBcc disp16
		{
			fbcc16(m68k);
			break;
		}
		case 3:		// FBcc disp32
		{
			fbcc32(m68k);
			break;
		}

		default:	m68k_log(M68K_LOG_ERROR, "M68kFPU: unimplemented main op %d\n", (m68k->ir >> 6)	& 0x3);
	}
}

static int perform_fsave(m68ki_cpu_core *m68k, uint32_t addr, int inc)
{
	//if(m68k->cpu_type & CPU_TYPE_040)
	{
		if(inc)
		{
			m68ki_write_32(addr, 0x41000000);
			return 4;
		}
		else
		{
			m68ki_write_32(addr-4, 0x41000000);
			return -4;
		}
	}
	/*

	if (inc)
	{
		// 68881 IDLE, version 0x1f
		m68ki_write_32(addr, 0x1f180000);
		m68ki_write_32(addr+4, 0);
		m68ki_write_32(addr+8, 0);
		m68ki_write_32(addr+12, 0);
		m68ki_write_32(addr+16, 0);
		m68ki_write_32(addr+20, 0);
		m68ki_write_32(addr+24, 0x70000000);
		return 7*4;
	}
	else
	{
		m68ki_write_32(addr-4, 0x70000000);
		m68ki_write_32(addr-8, 0);
		m68ki_write_32(addr-12, 0);
		m68ki_write_32(addr-16, 0);
		m68ki_write_32(addr-20, 0);
		m68ki_write_32(addr-24, 0);
		m68ki_write_32(addr-28, 0x1f180000);
		return -7*4;
	}
	*/
}

// FRESTORE on a NULL frame reboots the FPU - all registers to NaN, the 3 status regs to 0
static void do_frestore_null(m68ki_cpu_core *m68k)
{
	int i;

	REG_FPCR = 0;
	REG_FPSR = 0;
	REG_FPIAR = 0;
	for (i = 0; i < 8; i++)
	{
		REG_FP[i].v.i = (uint64_t)(0xffffffffffffffff);
	}

	// Mac IIci at 408458e6 wants an FSAVE of a just-restored NULL frame to also be NULL
	// The PRM says it's possible to generate a NULL frame, but not how/when/why.  (need the 68881/68882 manual!)
	m68k->fpu_just_reset = 1;
}

static void m68040_do_fsave(m68ki_cpu_core *m68k, uint32_t addr, int reg, int inc)
{
	if (m68k->fpu_just_reset)
	{
			m68ki_write_32(addr, 0);
	}
	else
	{
		// we normally generate an IDLE frame
		int delta = perform_fsave(m68k, addr, inc);
		if(reg != -1)
			REG_A[reg] += delta;
	}
}

static void m68040_do_frestore(m68ki_cpu_core *m68k, uint32_t addr, int reg)
{
	bool m40 = 1; //m68k->cpu_type & CPU_TYPE_040;
	uint32_t temp = m68ki_read_32(addr);

	// check for NULL frame
	if (temp & 0xff000000)
	{
		// we don't handle non-NULL frames
		m68k->fpu_just_reset = 0;

		if (reg != -1)
		{
			// how about an IDLE frame?
			if (!m40 && ((temp & 0x00ff0000) == 0x00180000))
			{
				REG_A[reg] += 7*4;
			}
			else if (m40 && ((temp & 0xffff0000) == 0x41000000))
			{
				REG_A[reg] += 4;
			} // check UNIMP
			else if ((temp & 0x00ff0000) == 0x00380000)
			{
				REG_A[reg] += 14*4;
			} // check BUSY
			else if ((temp & 0x00ff0000) == 0x00b40000)
			{
				REG_A[reg] += 45*4;
			}
		}
	}
	else
	{
		do_frestore_null(m68k);
	}
}

void m68040_fpu_op1(m68ki_cpu_core *m68k)
{
	int ea = m68k->ir & 0x3f;
	int mode = (ea >> 3) & 0x7;
	int reg = (ea & 0x7);
	uint32_t addr;

	switch ((m68k->ir >> 6) & 0x3)
	{
		case 0:		// FSAVE <ea>
		{
			switch (mode)
			{
			case 2: // (An)
				addr = REG_A[reg];
				m68040_do_fsave(m68k, addr, -1, 1);
				break;

			case 3:	// (An)+
				addr = EA_AY_PI_32();
				m68040_do_fsave(m68k, addr, reg, 1);
				break;

			case 4: // -(An)
				addr = EA_AY_PD_32();
				m68040_do_fsave(m68k, addr, reg, 0);
				break;

			case 5: // (D16, An)
				addr = EA_AY_DI_16();
				m68040_do_fsave(m68k, addr, -1, 1);
				break;

			case 7: //
				switch (reg)
				{
					case 1:		// (abs32)
					{
						addr = EA_AL_32();
						m68040_do_fsave(m68k, addr, -1, 1);
						break;
					}
					case 2:		// (d16, PC)
					{
						addr = EA_PCDI_16();
						m68040_do_fsave(m68k, addr, -1, 1);
						break;
					}
					default:
						m68k_log(M68K_LOG_ERROR, "M68kFPU: FSAVE unhandled mode %d reg %d at %x\n", mode, reg, REG_PC);
				}

				break;

			default:
				m68k_log(M68K_LOG_ERROR, "M68kFPU: FSAVE unhandled mode %d reg %d at %x\n", mode, reg, REG_PC);
			}
			break;
		}
		//break;

		case 1:		// FRESTORE <ea>
		{
			switch (mode)
			{
			case 2: // (An)
				addr = REG_A[reg];
				m68040_do_frestore(m68k, addr, -1);
				break;

			case 3:	// (An)+
				addr = EA_AY_PI_32();
				m68040_do_frestore(m68k, addr, reg);
				break;

			case 5: // (D16, An)
				addr = EA_AY_DI_16();
				m68040_do_frestore(m68k, addr, -1);
				break;

			case 7: //
				switch (reg)
				{
					case 1:		// (abs32)
					{
						addr = EA_AL_32();
						m68040_do_frestore(m68k, addr, -1);
						break;
					}
					case 2:		// (d16, PC)
					{
						addr = EA_PCDI_16();
						m68040_do_frestore(m68k, addr, -1);
						break;
					}
					default:
						m68k_log(M68K_LOG_ERROR, "M68kFPU: FRESTORE unhandled mode %d reg %d at %x\n", mode, reg, REG_PC);
				}

				break;

			default:
				m68k_log(M68K_LOG_ERROR, "M68kFPU: FRESTORE unhandled mode %d reg %d at %x\n", mode, reg, REG_PC);
			}
			break;
		}
		//break;

		default:	m68k_log(M68K_LOG_ERROR, "m68040_fpu_op1: unimplemented op %d at %08X\n", (m68k->ir >> 6) & 0x3, REG_PC-2);
	}
}



