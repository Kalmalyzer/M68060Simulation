
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

enum { FullExtensionWord_IS_IIS_Mask = FullExtensionWord_IndexSuppress_Mask | FullExtensionWord_IIS_Mask };

typedef enum
{
	IS_IIS_PreIndexed_NoMemoryIndirect = 0,
	IS_IIS_PreIndexed_Indirect_NullOuterDisplacement = 1,
	IS_IIS_PreIndexed_Indirect_WordOuterDisplacement = 2,
	IS_IIS_PreIndexed_Indirect_LongOuterDisplacement = 3,
	// Reserved = 4
	IS_IIS_PostIndexed_Indirect_NullOuterDisplacement = 5,
	IS_IIS_PostIndexed_Indirect_WordOuterDisplacement = 6,
	IS_IIS_PostIndexed_Indirect_LongOuterDisplacement = 7,
	IS_IIS_NonIndexed_NoMemoryIndirect = FullExtensionWord_IndexSuppress_Mask | 0,
	IS_IIS_NonIndexed_Indirect_NullOuterDisplacement = FullExtensionWord_IndexSuppress_Mask | 1,
	IS_IIS_NonIndexed_Indirect_WordOuterDisplacement = FullExtensionWord_IndexSuppress_Mask | 2,
	IS_IIS_NonIndexed_Indirect_LongOuterDisplacement = FullExtensionWord_IndexSuppress_Mask | 3,
	// Reserved = FullExtensionWord_IndexSuppress_Mask | 4..7

} IS_IIS;

typedef enum
{
	DisplacementSize_Reserved,
	DisplacementSize_Null,
	DisplacementSize_Word,
	DisplacementSize_Long,

} DisplacementSize;

#endif
