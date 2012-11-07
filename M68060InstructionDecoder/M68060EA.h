
#ifndef M68060EA_H
#define M68060EA_H

typedef enum
{
	EA6BitMode_Upper3Bits_Dn,
	EA6BitMode_Upper3Bits_An,
	EA6BitMode_Upper3Bits_Mem_An,
	EA6BitMode_Upper3Bits_Mem_An_PostIncrement,
	EA6BitMode_Upper3Bits_Mem_PreDecrement_An,
	EA6BitMode_Upper3Bits_Mem_D16_An,
	EA6BitMode_Upper3Bits_Mem_BriefOrFullExtensionWord_An,
	EA6BitMode_Upper3Bits_CheckLower3Bits,

} EA6BitMode_Upper3Bits;

typedef enum
{
	EA6BitMode_Lower3Bits_Mem_Absolute_Word,
	EA6BitMode_Lower3Bits_Mem_Absolute_Long,
	EA6BitMode_Lower3Bits_Mem_D16_PC,
	EA6BitMode_Lower3Bits_Mem_BriefOrFullExtensionWord_PC,
	EA6BitMode_Lower3Bits_Immediate,

} EA6BitMode_Lower3Bits;

enum { ExtensionWord_DA_Shift = 15 };
enum { ExtensionWord_DA_Bits = 1 };
enum { ExtensionWord_DA_Mask = ((1 << ExtensionWord_DA_Bits) - 1) << ExtensionWord_DA_Shift };

enum { ExtensionWord_Register_Shift = 12 };
enum { ExtensionWord_Register_Bits = 3 };
enum { ExtensionWord_Register_Mask = ((1 << ExtensionWord_Register_Bits) - 1) << ExtensionWord_Register_Shift };

enum { ExtensionWord_WL_Shift = 11 };
enum { ExtensionWord_WL_Bits = 1 };
enum { ExtensionWord_WL_Mask = ((1 << ExtensionWord_WL_Bits) - 1) << ExtensionWord_WL_Shift };

enum { ExtensionWord_Scale_Shift = 9 };
enum { ExtensionWord_Scale_Bits = 2 };
enum { ExtensionWord_Scale_Mask = ((1 << ExtensionWord_Scale_Bits) - 1) << ExtensionWord_Scale_Shift };

enum { ExtensionWord_FullWord_Shift = 8 };
enum { ExtensionWord_FullWord_Bits = 1 };
enum { ExtensionWord_FullWord_Mask = ((1 << ExtensionWord_FullWord_Bits) - 1) << ExtensionWord_FullWord_Shift };

enum { BriefExtensionWord_Displacement_Shift = 0 };
enum { BriefExtensionWord_Displacement_Bits = 8 };
enum { BriefExtensionWord_Displacement_Mask = ((1 << BriefExtensionWord_Displacement_Bits) - 1) << BriefExtensionWord_Displacement_Shift };

enum { FullExtensionWord_BaseRegisterSuppress_Shift = 7 };
enum { FullExtensionWord_BaseRegisterSuppress_Bits = 1 };
enum { FullExtensionWord_BaseRegisterSuppress_Mask = ((1 << FullExtensionWord_BaseRegisterSuppress_Bits) - 1) << FullExtensionWord_BaseRegisterSuppress_Shift };

enum { FullExtensionWord_IndexSuppress_Shift = 6 };
enum { FullExtensionWord_IndexSuppress_Bits = 1 };
enum { FullExtensionWord_IndexSuppress_Mask = ((1 << FullExtensionWord_IndexSuppress_Bits) - 1) << FullExtensionWord_IndexSuppress_Shift };

enum { FullExtensionWord_BaseDisplacementSize_Shift = 4 };
enum { FullExtensionWord_BaseDisplacementSize_Bits = 2 };
enum { FullExtensionWord_BaseDisplacementSize_Mask = ((1 << FullExtensionWord_BaseDisplacementSize_Bits) - 1) << FullExtensionWord_BaseDisplacementSize_Shift };

enum { FullExtensionWord_OuterDisplacementSize_Shift = 0 };
enum { FullExtensionWord_OuterDisplacementSize_Bits = 2 };
enum { FullExtensionWord_OuterDisplacementSize_Mask = ((1 << FullExtensionWord_OuterDisplacementSize_Bits) - 1) << FullExtensionWord_OuterDisplacementSize_Shift };

enum { FullExtensionWord_IIS_Shift = 0 };
enum { FullExtensionWord_IIS_Bits = 3 };
enum { FullExtensionWord_IIS_Mask = ((1 << FullExtensionWord_IIS_Bits) - 1) << FullExtensionWord_IIS_Shift };

typedef enum
{
	DisplacementSize_Reserved,
	DisplacementSize_Null,
	DisplacementSize_Word,
	DisplacementSize_Long,

} DisplacementSize;

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
enum { EAModeMask_Immediate = (1U << EAMode_Immediate) };
enum { EAModeMask_MemoryReference_PC = (1U << EAMode_Mem_D16_PC) | (1U << EAMode_Mem_BriefOrFullExtensionWord_PC) };
enum { EAModeMask_RelativeBranch = (1U << EAMode_RelativeBranch) };

enum { EAModeMask_All = EAModeMask_Rn | EAModeMask_MemoryReference_Rn | EAModeMask_MemoryReference_Absolute | EAModeMask_Immediate | EAModeMask_MemoryReference_PC };
enum { EAModeMask_MemoryAlterable = EAModeMask_MemoryReference_Rn | EAModeMask_MemoryReference_Absolute };
enum { EAModeMask_DataAlterable = EAModeMask_Dn | EAModeMask_MemoryReference_Rn | EAModeMask_MemoryReference_Absolute };
enum { EAModeMask_Alterable = EAModeMask_Rn | EAModeMask_MemoryReference_Rn | EAModeMask_MemoryReference_Absolute };
enum { EAModeMask_Data = EAModeMask_Dn | EAModeMask_MemoryReference_Rn | EAModeMask_MemoryReference_Absolute | EAModeMask_Immediate | EAModeMask_MemoryReference_PC };
enum { EAModeMask_Control = EAModeMask_MemoryReference_StaticRn | EAModeMask_MemoryReference_Absolute | EAModeMask_MemoryReference_PC };
enum { EAModeMask_ControlAlterable = EAModeMask_MemoryReference_StaticRn | EAModeMask_MemoryReference_Absolute };
enum { EAModeMask_DnOrControl = EAModeMask_Data | EAModeMask_Control };

#endif
	