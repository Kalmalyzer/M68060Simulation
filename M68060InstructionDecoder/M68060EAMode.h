
#ifndef M68060EAMODE_H
#define M68060EAMODE_H

typedef enum
{
	EAMode_None,
	EAMode_Dn,
	EAMode_An,
	EAMode_Mem_An,
	EAMode_Mem_An_PostIncrement,
	EAMode_Mem_PreDecrement_An,
	EAMode_Mem_D16_An,
	EAMode_Mem_BriefOrFullExtensionWord_An,
	EAMode_Mem_Absolute_Word,
	EAMode_Mem_Absolute_Long,
	EAMode_Mem_D16_PC,
	EAMode_Mem_BriefOrFullExtensionWord_PC,
	EAMode_Immediate,
	EAMode_Immediate_Byte,
	EAMode_RelativeBranch,
} EAMode;

typedef uint32_t EAModeMask;

enum { EAModeMask_None = 0 };
enum { EAModeMask_Dn = (1U << EAMode_Dn) };
enum { EAModeMask_An = (1U << EAMode_An) };
enum { EAModeMask_Rn = EAModeMask_Dn | EAModeMask_An };
enum { EAModeMask_MemoryReference_Rn = (1U << EAMode_Mem_An) | (1U << EAMode_Mem_An_PostIncrement) | (1U << EAMode_Mem_PreDecrement_An) | (1U << EAMode_Mem_D16_An) | (1U << EAMode_Mem_BriefOrFullExtensionWord_An) };
enum { EAModeMask_MemoryReference_StaticRn = (1U << EAMode_Mem_An) | (1U << EAMode_Mem_D16_An) | (1U << EAMode_Mem_BriefOrFullExtensionWord_An) };
enum { EAModeMask_MemoryReference_Absolute = (1U << EAMode_Mem_Absolute_Word) | (1U << EAMode_Mem_Absolute_Long) };
enum { EAModeMask_Immediate = (1U << EAMode_Immediate) | (1U << EAMode_Immediate_Byte) };
enum { EAModeMask_MemoryReference_PC = (1U << EAMode_Mem_D16_PC) | (1U << EAMode_Mem_BriefOrFullExtensionWord_PC) };
enum { EAModeMask_RelativeBranch = (1U << EAMode_RelativeBranch) };

enum { EAModeMask_All = EAModeMask_Rn | EAModeMask_MemoryReference_Rn | EAModeMask_MemoryReference_Absolute | EAModeMask_Immediate | EAModeMask_MemoryReference_PC };
enum { EAModeMask_MemoryAlterable = EAModeMask_MemoryReference_Rn | EAModeMask_MemoryReference_Absolute };
enum { EAModeMask_DataAlterable = EAModeMask_Dn | EAModeMask_MemoryReference_Rn | EAModeMask_MemoryReference_Absolute };
enum { EAModeMask_Alterable = EAModeMask_Rn | EAModeMask_MemoryReference_Rn | EAModeMask_MemoryReference_Absolute };
enum { EAModeMask_Data = EAModeMask_Dn | EAModeMask_MemoryReference_Rn | EAModeMask_MemoryReference_Absolute | EAModeMask_Immediate | EAModeMask_MemoryReference_PC };
enum { EAModeMask_Control = EAModeMask_MemoryReference_StaticRn | EAModeMask_MemoryReference_Absolute | EAModeMask_MemoryReference_PC };
enum { EAModeMask_ControlAlterable = EAModeMask_MemoryReference_StaticRn | EAModeMask_MemoryReference_Absolute };
enum { EAModeMask_DnOrControl = EAModeMask_Dn | EAModeMask_Control };

#endif
