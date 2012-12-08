#include "M68060InstructionLengthDecoder.h"
#include "../Assert.h"

#include "M68060EA.h"
#include "M68060OpWord.h"

//Define this if you want decoding of some illegal EAs to not outright reject the instruction.
// but to ignore any base/outer displacements instead; this is what Musashi seems to do.
//#define MUSASHI_COMPATIBILITY_MODE


typedef enum
{
	EAEncoding_None,
	EAEncoding_DefaultEALocation,
	EAEncoding_MoveDestinationEALocation,
	EAEncoding_Immediate,
	EAEncoding_D16,
	EAEncoding_RelativeBranch,

} EAEncoding;

typedef enum
{
	SizeEncoding_None,
	SizeEncoding_Byte,
	SizeEncoding_Word,
	SizeEncoding_Long,
	SizeEncoding_DefaultOpModeEncoding,
	SizeEncoding_DefaultOpSizeEncoding,
	SizeEncoding_RelativeBranchEncoding,

} SizeEncoding;

typedef enum
{
	OpWordClass_NoExtraWords,
	OpWordClass_Move_B,
	OpWordClass_Move_W,
	OpWordClass_Move_L,
	OpWordClass_EncodedSize_Ea_Dn,
	OpWordClass_EncodedSize_Ea_Rn,
	OpWordClass_LongMulDiv,
	OpWordClass_Bitfield_ReadEa,
	OpWordClass_Control,
	OpWordClass_Byte_SrcEa,
	OpWordClass_Word_SrcEa,
	OpWordClass_Long_SrcEa,
	OpWordClass_BitImmInstruction_Read,
	OpWordClass_BitImmInstruction_ReadWrite,
	OpWordClass_EncodedSize_Imm_Ea_Read,
	OpWordClass_EncodedSize_Imm_Ea_ReadWrite,
	OpWordClass_EncodedSize_Rn_Ea_1,
	OpWordClass_EncodedSize_Rn_Ea_2,
	OpWordClass_EncodedSize_DestEa_Read,
	OpWordClass_EncodedSize_DestEa_ReadWrite,
	OpWordClass_DestEa_Alterable,
	OpWordClass_Byte_DestEa_Data,
	OpWordClass_DestEa_DataAlterable,
	OpWordClass_DestEa_MemoryAlterable,
	OpWordClass_EncodedSize,
	OpWordClass_ImmediateByte,
	OpWordClass_ImmediateWord,
	OpWordClass_ImmediateLong,
	OpWordClass_RelativeBranch,
	OpWordClass_D16An,
	OpWordClass_1SpecialWord,
	OpWordClass_2SpecialWords,
	OpWordClass_Movem_RegistersToMemory,
	OpWordClass_Movem_MemoryToRegisters,
	OpWordClass_Bitfield_ReadWriteEa,
	
} OpWordClass;

typedef struct
{
	uint16_t mask;
	uint16_t match;

	const char* description;
	
	OpWordClass class;
	
} OpWordLengthInfo;

typedef struct
{
	uint numSpecialOperandSpecifierWords;
	SizeEncoding sizeEncoding;
	EAEncoding sourceEAEncoding;
	EAModeMask sourceEAModeMask;
	EAEncoding destinationEAEncoding;
	EAModeMask destinationEAModeMask;
} OpWordClassInfo;

static OpWordClassInfo opWordClassInformation[] =
{
	{ 0, SizeEncoding_None, EAEncoding_None, EAModeMask_None, EAEncoding_None, EAModeMask_None, }, // OpWordClass_NoExtraWords
	{ 0, SizeEncoding_Byte, EAEncoding_DefaultEALocation, EAModeMask_Data, EAEncoding_MoveDestinationEALocation, EAModeMask_DataAlterable, }, // OpWordClass_Move_B,
	{ 0, SizeEncoding_Word, EAEncoding_DefaultEALocation, EAModeMask_All, EAEncoding_MoveDestinationEALocation, EAModeMask_Alterable, }, // OpWordClass_Move_W,
	{ 0, SizeEncoding_Long, EAEncoding_DefaultEALocation, EAModeMask_All, EAEncoding_MoveDestinationEALocation, EAModeMask_Alterable, }, // OpWordClass_Move_L,
	{ 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_DefaultEALocation, EAModeMask_Data, EAEncoding_None, EAModeMask_None, }, // OpWordClass_EncodedSize_Ea_Dn,
	{ 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_DefaultEALocation, EAModeMask_All, EAEncoding_None, EAModeMask_None, }, // OpWordClass_EncodedSize_Ea_Rn,
	{ 1, SizeEncoding_Long, EAEncoding_DefaultEALocation, EAModeMask_Data, EAEncoding_None, EAModeMask_None, }, // OpWordClass_LongMulDiv,
	{ 1, SizeEncoding_None, EAEncoding_DefaultEALocation, EAModeMask_DnOrControl, EAEncoding_None, EAModeMask_None, }, // OpWordClass_Bitfield_ReadEa,
	{ 0, SizeEncoding_None, EAEncoding_DefaultEALocation, EAModeMask_Control, EAEncoding_None, EAModeMask_None, }, // OpWordClass_Control,
	{ 0, SizeEncoding_Byte, EAEncoding_DefaultEALocation, EAModeMask_Data, EAEncoding_None, EAModeMask_None, }, // OpWordClass_Byte_SrcEa,
	{ 0, SizeEncoding_Word, EAEncoding_DefaultEALocation, EAModeMask_Data, EAEncoding_None, EAModeMask_None, }, // OpWordClass_Word_SrcEa,
	{ 0, SizeEncoding_Long, EAEncoding_DefaultEALocation, EAModeMask_Data, EAEncoding_None, EAModeMask_None, }, // OpWordClass_Long_SrcEa,
	{ 0, SizeEncoding_Byte, EAEncoding_Immediate, EAModeMask_All, EAEncoding_DefaultEALocation, EAModeMask_Data, }, // OpWordClass_BitImmInstruction_Read,
	{ 0, SizeEncoding_Byte, EAEncoding_Immediate, EAModeMask_All, EAEncoding_DefaultEALocation, EAModeMask_DataAlterable, }, // OpWordClass_BitImmInstruction_ReadWrite,
	{ 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_Immediate, EAModeMask_All, EAEncoding_DefaultEALocation, EAModeMask_DataAlterable | EAModeMask_MemoryReference_PC, }, // OpWordClass_EncodedSize_Imm_Ea_Read,
	{ 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_Immediate, EAModeMask_All, EAEncoding_DefaultEALocation, EAModeMask_DataAlterable, }, // OpWordClass_EncodedSize_Imm_Ea_ReadWrite,
	{ 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_None, EAModeMask_None, EAEncoding_DefaultEALocation, EAModeMask_MemoryAlterable, }, // OpWordClass_EncodedSize_Rn_Ea_1,
	{ 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_None, EAModeMask_None, EAEncoding_DefaultEALocation, EAModeMask_DataAlterable, }, // OpWordClass_EncodedSize_Rn_Ea_2,
	{ 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_None, EAModeMask_None, EAEncoding_DefaultEALocation, EAModeMask_All, }, // OpWordClass_EncodedSize_DestEa_Read,
	{ 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_None, EAModeMask_None, EAEncoding_DefaultEALocation, EAModeMask_DataAlterable, }, // OpWordClass_EncodedSize_DestEa_ReadWrite,
	{ 0, SizeEncoding_None, EAEncoding_None, EAModeMask_None, EAEncoding_DefaultEALocation, EAModeMask_Alterable, }, // OpWordClass_DestEa_Alterable,
	{ 0, SizeEncoding_Byte, EAEncoding_None, EAModeMask_None, EAEncoding_DefaultEALocation, EAModeMask_Data, }, // OpWordClass_Byte_DestEa_Data,
	{ 0, SizeEncoding_None, EAEncoding_None, EAModeMask_None, EAEncoding_DefaultEALocation, EAModeMask_DataAlterable, }, // OpWordClass_DestEa_DataAlterable,
	{ 0, SizeEncoding_None, EAEncoding_None, EAModeMask_None, EAEncoding_DefaultEALocation, EAModeMask_MemoryAlterable, }, // OpWordClass_DestEa_MemoryAlterable,
	{ 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_None, EAModeMask_None, EAEncoding_None, EAModeMask_None, }, // OpWordClass_EncodedSize,
	{ 0, SizeEncoding_Byte, EAEncoding_Immediate, EAModeMask_All, EAEncoding_None, EAModeMask_None, }, // OpWordClass_ImmediateByte,
	{ 0, SizeEncoding_Word, EAEncoding_Immediate, EAModeMask_All, EAEncoding_None, EAModeMask_None, }, // OpWordClass_ImmediateWord,
	{ 0, SizeEncoding_Long, EAEncoding_Immediate, EAModeMask_All, EAEncoding_None, EAModeMask_None, }, // OpWordClass_ImmediateLong,
	{ 0, SizeEncoding_RelativeBranchEncoding, EAEncoding_RelativeBranch, EAModeMask_RelativeBranch, EAEncoding_None, EAModeMask_None, }, // OpWordClass_RelativeBranch,
	{ 0, SizeEncoding_Word, EAEncoding_D16, EAModeMask_All, EAEncoding_None, EAModeMask_None, }, // OpWordClass_D16An
	{ 1, SizeEncoding_None, EAEncoding_None, EAModeMask_None, EAEncoding_None, EAModeMask_None, }, // OpWordClass_1SpecialWord
	{ 2, SizeEncoding_None, EAEncoding_None, EAModeMask_None, EAEncoding_None, EAModeMask_None, }, // OpWordClass_2SpecialWords
	{ 1, SizeEncoding_None, EAEncoding_None, EAModeMask_None, EAEncoding_DefaultEALocation, EAModeMask_ControlAlterable | (1U << EAMode_Mem_PreDecrement_An), }, // OpWordClass_Movem_RegistersToMemory
	{ 1, SizeEncoding_None, EAEncoding_DefaultEALocation, EAModeMask_Control | (1U << EAMode_Mem_An_PostIncrement), EAEncoding_None, EAModeMask_None, }, // OpWordClass_Movem_MemoryToRegisters
	{ 1, SizeEncoding_None, EAEncoding_None, EAModeMask_None, EAEncoding_DefaultEALocation, (1U << EAMode_Dn) | EAModeMask_ControlAlterable, }, // OpWordClass_Bitfield_ReadWriteEa
};

static OpWordLengthInfo opWordLengthInformation[] =
{

	{ 0xf0ff, 0x50fc, "TRAPcc", OpWordClass_NoExtraWords, }, // Shadows Scc <ea>
	{ 0xf0ff, 0x50fa, "TRAPcc.W #imm", OpWordClass_ImmediateWord, }, // Shadows Scc <ea>
	{ 0xf0ff, 0x50fb, "TRAPcc.L #imm", OpWordClass_ImmediateLong, }, // Shadows Scc <ea>
	{ 0xf0f8, 0x50c8, "DBcc <relative address>", OpWordClass_ImmediateWord, }, // Shadows Scc <ea>
	{ 0xf0c0, 0x50c0, "Scc <ea>", OpWordClass_DestEa_DataAlterable, }, // Shadows ADDQ/SUBQ

	{ 0xf1f8, 0xc100, "ABCD Dx,Dy", OpWordClass_NoExtraWords, },
	{ 0xf1f8, 0xc108, "ABCD -(Ax),-(Ay)", OpWordClass_NoExtraWords, },
	{ 0xf138, 0xd100, "ADDX Dx,Dy", OpWordClass_NoExtraWords, }, // Shadows ADD Dn,<ea>
	{ 0xf138, 0xd108, "ADDX -(Ax),-(Ay)", OpWordClass_NoExtraWords, }, // Shadows ADD Dn,<ea>
	{ 0xf1c0, 0xd0c0, "ADDA.W <ea>,An", OpWordClass_EncodedSize_Ea_Rn, }, // Shadows ADD <ea>,Dn
	{ 0xf1c0, 0xd1c0, "ADDA.L <ea>,An", OpWordClass_EncodedSize_Ea_Rn, }, // Shadows ADD Dn,<ea>
	{ 0xf100, 0xd000, "ADD <ea>,Dn", OpWordClass_EncodedSize_Ea_Rn, },
	{ 0xf100, 0xd100, "ADD Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea_1, },
	{ 0xff00, 0x0600, "ADDI #imm,<ea>", OpWordClass_EncodedSize_Imm_Ea_ReadWrite, },
	{ 0xf100, 0x5000, "ADDQ #imm,<ea>", OpWordClass_DestEa_Alterable, },
	{ 0xf1f8, 0xc140, "EXG Dm,Dn", OpWordClass_NoExtraWords, }, // Shadows AND
	{ 0xf1f8, 0xc148, "EXG Am,An", OpWordClass_NoExtraWords, }, // Shadows AND
	{ 0xf1f8, 0xc188, "EXG Dm,An", OpWordClass_NoExtraWords, }, // Shadows AND
	{ 0xf1c0, 0xc1c0, "MULS.W <ea>,Dn", OpWordClass_Word_SrcEa, }, // Shadows AND
	{ 0xf1c0, 0xc0c0, "MULU.W <ea>,Dn", OpWordClass_Word_SrcEa, }, // Shadows AND
	{ 0xf100, 0xc000, "AND <ea>,Dn", OpWordClass_EncodedSize_Ea_Dn, },
	{ 0xf100, 0xc100, "AND Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea_1, },

	{ 0xffff, 0x023c, "ANDI #imm,CCR", OpWordClass_ImmediateByte, }, // Shadows ANDI #imm,<ea>
	{ 0xff00, 0x0200, "ANDI #imm,<ea>", OpWordClass_EncodedSize_Imm_Ea_ReadWrite, },

	{ 0xffc0, 0xeac0, "BFCHG <ea>{Do:Dw}", OpWordClass_Bitfield_ReadWriteEa, }, // Shadows ASL/ASR #imm/Dm,Dn
	{ 0xffc0, 0xeec0, "BFSET <ea>{Do:Dw}", OpWordClass_Bitfield_ReadWriteEa, }, // Shadows ASL/ASR #imm/Dm,Dn

	{ 0xfec0, 0xe0c0, "ASL/ASR <ea>", OpWordClass_DestEa_MemoryAlterable, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn
	{ 0xfec0, 0xe2c0, "LSL/LSR <ea>", OpWordClass_DestEa_MemoryAlterable, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn
	{ 0xfec0, 0xe4c0, "ROXL/ROXR <ea>", OpWordClass_DestEa_MemoryAlterable, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn
	{ 0xfec0, 0xe6c0, "ROL/ROR <ea>", OpWordClass_DestEa_MemoryAlterable, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn

	{ 0xff00, 0x6100, "BSR <relative address>", OpWordClass_RelativeBranch, }, // Shadows Bcc
	{ 0xf000, 0x6000, "Bcc <relative address>", OpWordClass_RelativeBranch, },
	{ 0xf138, 0x0108, "MOVEP Dx <-> d16(An)", OpWordClass_D16An, }, // Shadows BCLR/BCHG Dn,<ea>
	{ 0xf1c0, 0x0140, "BCHG Dn,<ea>", OpWordClass_DestEa_DataAlterable, },
	{ 0xffc0, 0x0840, "BCHG #imm,<ea>", OpWordClass_BitImmInstruction_ReadWrite, },
	{ 0xf1c0, 0x0180, "BCLR Dn,<ea>", OpWordClass_DestEa_DataAlterable, },
	{ 0xffc0, 0x0880, "BCLR #imm,<ea>", OpWordClass_BitImmInstruction_ReadWrite, },
	{ 0xfff8, 0x4848, "BKPT #imm", OpWordClass_NoExtraWords, }, // Shadows PEA
	{ 0xf1c0, 0x01c0, "BSET Dn,<ea>", OpWordClass_DestEa_DataAlterable, },
	{ 0xffc0, 0x08c0, "BSET #imm,<ea>", OpWordClass_BitImmInstruction_ReadWrite, },
	{ 0xf1c0, 0x0100, "BTST Dn,<ea>", OpWordClass_Byte_DestEa_Data, },
	{ 0xffc0, 0x0800, "BTST #imm,<ea>", OpWordClass_BitImmInstruction_Read, },
	{ 0xffc0, 0xecc0, "BFCLR <ea>{Do:Dw}", OpWordClass_Bitfield_ReadWriteEa, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn
	{ 0xffc0, 0xebc0, "BFEXTS <ea>{Do:Dw},Dn", OpWordClass_Bitfield_ReadEa, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn
	{ 0xffc0, 0xe9c0, "BFEXTU <ea>{Do:Dw},Dn", OpWordClass_Bitfield_ReadEa, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn
	{ 0xffc0, 0xedc0, "BFFFO <ea>{Do:Dw},Dn", OpWordClass_Bitfield_ReadEa, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn
	{ 0xffc0, 0xefc0, "BFINS Dn,<ea>{Do:Dw}", OpWordClass_Bitfield_ReadWriteEa, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn
	{ 0xffc0, 0xe8c0, "BFTST <ea>{Do:Dw}", OpWordClass_Bitfield_ReadEa, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn
	{ 0xf1c0, 0x4180, "CHK.W <ea>,Dn", OpWordClass_Word_SrcEa, },
	{ 0xf1c0, 0x4100, "CHK.L <ea>,Dn", OpWordClass_Long_SrcEa, },

	{ 0xf018, 0xe000, "ASL/ASR #imm/Dm,Dn", OpWordClass_EncodedSize, },
	{ 0xf018, 0xe008, "LSL/LSR #imm/Dm,Dn", OpWordClass_EncodedSize, },
	{ 0xf018, 0xe010, "ROXL/ROXR #imm/Dm,Dn", OpWordClass_EncodedSize, },
	{ 0xf018, 0xe018, "ROL/ROR #imm/Dm,Dn", OpWordClass_EncodedSize, },
	
	{ 0xffc0, 0x42c0, "MOVE CCR,<ea>", OpWordClass_DestEa_DataAlterable, }, // Shadows CLR

	{ 0xff00, 0x4200, "CLR <ea>", OpWordClass_EncodedSize_DestEa_ReadWrite, },
	{ 0xf138, 0xb108, "CMPM (Ax)+,(Ay)+", OpWordClass_NoExtraWords, }, // Shadows EOR
	{ 0xf1c0, 0xb100, "EOR.B Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea_2, }, // Shadows CMP
	{ 0xf1c0, 0xb140, "EOR.W Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea_2, }, // Shadows CMP
	{ 0xf1c0, 0xb180, "EOR.L Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea_2, }, // Shadows CMP
	{ 0xf000, 0xb000, "CMP <ea>,Rn", OpWordClass_EncodedSize_Ea_Rn, },
	{ 0xff00, 0x0c00, "CMPI #imm,<ea>", OpWordClass_EncodedSize_Imm_Ea_Read, },
	{ 0xf1c0, 0x81c0, "DIVS.W <ea>,Dn", OpWordClass_Word_SrcEa, },
	{ 0xffc0, 0x4c40, "DIVS/DIVU.L <ea>,Dr:Dq (can be 32bit or 64bit division)", OpWordClass_LongMulDiv, },
	{ 0xf1c0, 0x80c0, "DIVU.W <ea>,Dn", OpWordClass_Word_SrcEa, },
	{ 0xffff, 0x0a3c, "EORI #imm,CCR", OpWordClass_ImmediateByte, }, // Shadows EORI #imm,<ea>
	{ 0xff00, 0x0a00, "EORI #imm,<ea>", OpWordClass_EncodedSize_Imm_Ea_ReadWrite, },
	{ 0xfff8, 0x4880, "EXT.W Dn", OpWordClass_NoExtraWords, },
	{ 0xfff8, 0x48c0, "EXT.L Dn", OpWordClass_NoExtraWords, },
	{ 0xfff8, 0x49c0, "EXTB.L Dn", OpWordClass_NoExtraWords, },
	{ 0xffff, 0x4afc, "ILLEGAL", OpWordClass_NoExtraWords, },
	{ 0xffc0, 0x4ec0, "JMP <ea>", OpWordClass_Control, },
	{ 0xffc0, 0x4e80, "JSR <ea>", OpWordClass_Control, },
	{ 0xf1c0, 0x41c0, "LEA <ea>,An", OpWordClass_Control, },
	{ 0xfff8, 0x4e50, "LINK.W An,#imm", OpWordClass_ImmediateWord, },
	{ 0xfff8, 0x4808, "LINK.L An,#imm", OpWordClass_ImmediateLong, },

	{ 0xf000, 0x1000, "MOVE.B <ea>,<ea>", OpWordClass_Move_B, },
	{ 0xf000, 0x2000, "MOVE.L <ea>,<ea>", OpWordClass_Move_L, }, /* Combined MOVE.L & MOVEA.L */
	{ 0xf000, 0x3000, "MOVE.W <ea>,<ea>", OpWordClass_Move_W, }, /* Combined MOVE.W & MOVEA.W */

	{ 0xffc0, 0x44c0, "MOVE <ea>,CCR", OpWordClass_Byte_SrcEa, },

	{ 0xffc0, 0x40c0, "MOVE SR,<ea>", OpWordClass_DestEa_DataAlterable, },

	{ 0xfff8, 0xf620, "MOVE16 (Ax)+,(Ay)+", OpWordClass_1SpecialWord, },
	{ 0xffe0, 0xf600, "MOVE16 (An <-> xxx.L)", OpWordClass_2SpecialWords, },

	{ 0xff80, 0x4880, "MOVEM reglist,<ea>", OpWordClass_Movem_RegistersToMemory, },
	{ 0xff80, 0x4c80, "MOVEM <ea>,reglist", OpWordClass_Movem_MemoryToRegisters, },

	{ 0xf100, 0x7000, "MOVEQ #imm,Dn", OpWordClass_NoExtraWords, },

	{ 0xffc0, 0x4c00, "MULS/MULU.L <ea>,Dm:Dn (can be 32bit or 64bit multiply)", OpWordClass_LongMulDiv, },

	{ 0xffc0, 0x4800, "NBCD <ea>", OpWordClass_DestEa_DataAlterable, },
	{ 0xff00, 0x4400, "NEG <ea>", OpWordClass_EncodedSize_DestEa_ReadWrite, },
	{ 0xff00, 0x4000, "NEGX <ea>", OpWordClass_EncodedSize_DestEa_ReadWrite, },

	{ 0xffff, 0x4e71, "NOP", OpWordClass_NoExtraWords, },

	{ 0xff00, 0x4600, "NOT <ea>", OpWordClass_EncodedSize_DestEa_ReadWrite, },

	{ 0xf1f8, 0x8140, "PACK Dm,Dn,#imm", OpWordClass_ImmediateWord, }, // Shadows OR
	{ 0xf1f8, 0x8148, "PACK -(Am),-(An),#imm", OpWordClass_ImmediateWord, }, // Shadows OR
	{ 0xf1f8, 0x8180, "UNPK Dm,Dn,#imm", OpWordClass_ImmediateWord, }, // Shadows OR
	{ 0xf1f8, 0x8188, "UNPK -(Am),-(An),#imm", OpWordClass_ImmediateWord, }, // Shadows OR

	{ 0xf1f8, 0x8100, "SBCD Dx,Dy", OpWordClass_NoExtraWords, }, // Shadows OR
	{ 0xf1f8, 0x8108, "SBCD -(Ax),-(Ay)", OpWordClass_NoExtraWords, }, // Shadows OR

	{ 0xf100, 0x8000, "OR <ea>,Dn", OpWordClass_EncodedSize_Ea_Dn, },
	{ 0xf100, 0x8100, "OR Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea_1, },

	{ 0xffff, 0x003c, "ORI #imm,CCR", OpWordClass_ImmediateByte, }, // Shadows ANDI #imm,<ea>
	{ 0xff00, 0x0000, "ORI #imm,<ea>", OpWordClass_EncodedSize_Imm_Ea_ReadWrite, },

	{ 0xfff8, 0x4840, "SWAP Dn", OpWordClass_NoExtraWords, }, // Shadows PEA

	{ 0xffc0, 0x4840, "PEA <ea>", OpWordClass_Control, },


	{ 0xffff, 0x4e74, "RTD #imm", OpWordClass_ImmediateWord, },
	{ 0xffff, 0x4e77, "RTR", OpWordClass_NoExtraWords, },
	{ 0xffff, 0x4e75, "RTS", OpWordClass_NoExtraWords, },

	{ 0xf138, 0x9100, "SUBX Dx,Dy", OpWordClass_NoExtraWords, }, // Shadows SUB Dn,<e>
	{ 0xf138, 0x9108, "SUBX -(Ax),-(Ay)", OpWordClass_NoExtraWords, }, // Shadows SUB Dn,<e>
	{ 0xf1c0, 0x90c0, "SUBA.W <ea>,An", OpWordClass_EncodedSize_Ea_Rn, }, // Shadows SUB <ea>,Dn
	{ 0xf1c0, 0x91c0, "SUBA.L <ea>,An", OpWordClass_EncodedSize_Ea_Rn, }, // Shadows SUB Dn,<ea>
	{ 0xf100, 0x9000, "SUB <ea>,Dn", OpWordClass_EncodedSize_Ea_Rn, },
	{ 0xf100, 0x9100, "SUB Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea_1, },
	{ 0xff00, 0x0400, "SUBI #imm,<ea>", OpWordClass_EncodedSize_Imm_Ea_ReadWrite, },
	{ 0xf100, 0x5100, "SUBQ #imm,<ea>", OpWordClass_DestEa_Alterable, },

	{ 0xffc0, 0x4ac0, "TAS <ea>", OpWordClass_DestEa_DataAlterable, },

	{ 0xfff8, 0x4e40, "TRAP #imm", OpWordClass_NoExtraWords, },
	{ 0xffff, 0x4e76, "TRAPV", OpWordClass_NoExtraWords, },

	{ 0xff00, 0x4a00, "TST <ea>", OpWordClass_EncodedSize_DestEa_Read, },

	{ 0xfff8, 0x4e58, "UNLK An", OpWordClass_NoExtraWords, },

	{ 0, 0, "Unknown instruction", },
};

static bool decodeBriefOrFullExtensionWordLength(uint16_t firstExtensionWord, uint* numExtensionWords_)
{
	uint numExtensionWords = 1;
		
	if (firstExtensionWord & ExtensionWord_FullWord_Mask)
	{
		DisplacementSize baseDisplacementSize = (firstExtensionWord & FullExtensionWord_BaseDisplacementSize_Mask) >> FullExtensionWord_BaseDisplacementSize_Shift;
		IS_IIS is_iis = (firstExtensionWord & FullExtensionWord_IS_IIS_Mask);

		switch (baseDisplacementSize)
		{
			case DisplacementSize_Null:
				break;
			case DisplacementSize_Word:
				numExtensionWords++;
				break;
			case DisplacementSize_Long:
				numExtensionWords += 2;
				break;
			case DisplacementSize_Reserved:
#ifdef MUSASHI_COMPATIBILITY_MODE
				break;
#else
				return false;
#endif
			default:
				M68060_ERROR("Unsupported BaseDisplacementSize");
		}

		switch (is_iis)
		{
			case IS_IIS_PreIndexed_NoMemoryIndirect:
			case IS_IIS_NonIndexed_NoMemoryIndirect:
			case IS_IIS_PreIndexed_Indirect_NullOuterDisplacement:
			case IS_IIS_PostIndexed_Indirect_NullOuterDisplacement:
			case IS_IIS_NonIndexed_Indirect_NullOuterDisplacement:
				break;
			case IS_IIS_PreIndexed_Indirect_WordOuterDisplacement:
			case IS_IIS_PostIndexed_Indirect_WordOuterDisplacement:
			case IS_IIS_NonIndexed_Indirect_WordOuterDisplacement:
				numExtensionWords++;
				break;
			case IS_IIS_PreIndexed_Indirect_LongOuterDisplacement:
			case IS_IIS_PostIndexed_Indirect_LongOuterDisplacement:
			case IS_IIS_NonIndexed_Indirect_LongOuterDisplacement:
				numExtensionWords += 2;
				break;
			default:
#ifdef MUSASHI_COMPATIBILITY_MODE
				break;
#else
				return false;
#endif
		}
	}
	
	*numExtensionWords_ = numExtensionWords;

	return true;
}

static bool decodeEA6BitMode(EA6BitMode_Upper3Bits eaUpper3Bits, EA6BitMode_Lower3Bits eaLower3Bits, EAMode* eaMode)
{
	switch (eaUpper3Bits)
	{
		case EA6BitMode_Upper3Bits_Dn:
			*eaMode = EAMode_Dn;
			return true;
		case EA6BitMode_Upper3Bits_An:
			*eaMode = EAMode_An;
			return true;
		case EA6BitMode_Upper3Bits_Mem_An:
			*eaMode = EAMode_Mem_An;
			return true;
		case EA6BitMode_Upper3Bits_Mem_An_PostIncrement:
			*eaMode = EAMode_Mem_An_PostIncrement;
			return true;
		case EA6BitMode_Upper3Bits_Mem_PreDecrement_An:
			*eaMode = EAMode_Mem_PreDecrement_An;
			return true;
		case EA6BitMode_Upper3Bits_Mem_D16_An:
			*eaMode = EAMode_Mem_D16_An;
			return true;
		case EA6BitMode_Upper3Bits_Mem_BriefOrFullExtensionWord_An:
			*eaMode = EAMode_Mem_BriefOrFullExtensionWord_An;
			return true;
		case EA6BitMode_Upper3Bits_CheckLower3Bits:
			switch (eaLower3Bits)
			{
				case EA6BitMode_Lower3Bits_Mem_Absolute_Word:
					*eaMode = EAMode_Mem_Absolute_Word;
					return true;
				case EA6BitMode_Lower3Bits_Mem_Absolute_Long:
					*eaMode = EAMode_Mem_Absolute_Long;
					return true;
				case EA6BitMode_Lower3Bits_Mem_D16_PC:
					*eaMode = EAMode_Mem_D16_PC;
					return true;
				case EA6BitMode_Lower3Bits_Mem_BriefOrFullExtensionWord_PC:
					*eaMode = EAMode_Mem_BriefOrFullExtensionWord_PC;
					return true;
				case EA6BitMode_Lower3Bits_Immediate:
					*eaMode = EAMode_Immediate;
					return true;
				default:
					return false;
			}
			break;
		default:
			M68060_ERROR("Invalid 6-bit EA");
			return false;
	}
}

typedef enum
{
	DecodeOperandLengthResult_InsufficientData,
	DecodeOperandLengthResult_InvalidInstruction,
	DecodeOperandLengthResult_ValidInstruction,

} DecodeOperandLengthResult;

static DecodeOperandLengthResult decodeOperandLength(EAMode eaMode, bool firstExtensionWordAvailable, uint16_t firstExtensionWord, OperationSize operationSize, uint* numExtensionWords)
{
	switch (eaMode)
	{
		case EAMode_None:
		case EAMode_Dn:
		case EAMode_An:
		case EAMode_Mem_An:
		case EAMode_Mem_An_PostIncrement:
		case EAMode_Mem_PreDecrement_An:
			*numExtensionWords = 0;
			break;
		case EAMode_Mem_D16_An:
			*numExtensionWords = 1;
			break;
		case EAMode_Mem_BriefOrFullExtensionWord_An:
			if (!firstExtensionWordAvailable)
				return DecodeOperandLengthResult_InsufficientData;
			if (!decodeBriefOrFullExtensionWordLength(firstExtensionWord, numExtensionWords))
				return DecodeOperandLengthResult_InvalidInstruction;
			break;
		case EAMode_Mem_Absolute_Word:
			*numExtensionWords = 1;
			break;
		case EAMode_Mem_Absolute_Long:
			*numExtensionWords = 2;
			break;
		case EAMode_Mem_D16_PC:
			*numExtensionWords = 1;
			break;
		case EAMode_Mem_BriefOrFullExtensionWord_PC:
			if (!firstExtensionWordAvailable)
				return DecodeOperandLengthResult_InsufficientData;
			if (!decodeBriefOrFullExtensionWordLength(firstExtensionWord, numExtensionWords))
				return DecodeOperandLengthResult_InvalidInstruction;
			break;
		case EAMode_Immediate:
			switch (operationSize)
			{
				case OperationSize_Byte:
				case OperationSize_Word:
					*numExtensionWords = 1;
					break;
				case OperationSize_Long:
					*numExtensionWords = 2;
					break;
				default:
					M68060_ERROR("Invalid operation size");
			}
			break;
		case EAMode_RelativeBranch:
			switch (operationSize)
			{
				case OperationSize_Byte:
					*numExtensionWords = 0;
					break;
				case OperationSize_Word:
					*numExtensionWords = 1;
					break;
				case OperationSize_Long:
					*numExtensionWords = 2;
					break;
				default:
					M68060_ERROR("Invalid operation size");
			}
			break;
		default:
			M68060_ERROR("Invalid EAMode");
	}
	
	return DecodeOperandLengthResult_ValidInstruction;
}

static bool decodeOperand(uint16_t opWord, EAEncoding eaEncoding, EAMode* eaMode)
{
	switch (eaEncoding)
	{
		case EAEncoding_None:
			*eaMode = EAMode_None;
			return true;
		case EAEncoding_DefaultEALocation:
		{
			uint ea6BitMode = opWord & 0x3f;
			EA6BitMode_Upper3Bits eaUpper3Bits = (ea6BitMode >> 3) & 7;
			EA6BitMode_Lower3Bits eaLower3Bits = ea6BitMode & 7;
			return decodeEA6BitMode(eaUpper3Bits, eaLower3Bits, eaMode);
		}
		case EAEncoding_MoveDestinationEALocation:
		{
			uint ea6BitMode = (opWord >> 6) & 0x3f;
			EA6BitMode_Upper3Bits eaUpper3Bits = ea6BitMode & 7;
			EA6BitMode_Lower3Bits eaLower3Bits = (ea6BitMode >> 3) & 7;
			return decodeEA6BitMode(eaUpper3Bits, eaLower3Bits, eaMode);
		}
		case EAEncoding_Immediate:
			*eaMode = EAMode_Immediate;
			return true;
		case EAEncoding_D16:
			*eaMode = EAMode_Mem_D16_An;
			return true;
		case EAEncoding_RelativeBranch:
			*eaMode = EAMode_RelativeBranch;
			return true;
		default:
			M68060_ERROR("Unsupported EAEncoding");
			return false;
	}
}

static bool decodeOperationSize(uint16_t opWord, SizeEncoding sizeEncoding, OperationSize* operationSize)
{
	switch (sizeEncoding)
	{
		case SizeEncoding_None:
			*operationSize = OperationSize_Reserved;
			return true;
		case SizeEncoding_Byte:
			*operationSize = OperationSize_Byte;
			return true;
		case SizeEncoding_Word:
			*operationSize = OperationSize_Word;
			return true;
		case SizeEncoding_Long:
			*operationSize = OperationSize_Long;
			return true;
		case SizeEncoding_DefaultOpModeEncoding:
			{
				OpMode opMode = (opWord & OpWord_OpMode_Mask) >> OpWord_OpMode_Shift;
				switch (opMode)
				{
					case OpMode_MemoryToDataRegister_Byte:
						*operationSize = OperationSize_Byte;
						return true;
					case OpMode_MemoryToDataRegister_Word:
						*operationSize = OperationSize_Word;
						return true;
					case OpMode_MemoryToDataRegister_Long:
						*operationSize = OperationSize_Long;
						return true;
					case OpMode_MemoryToAddressRegister_Word:
						*operationSize = OperationSize_Word;
						return true;
					case OpMode_DataRegisterToMemory_Byte:
						*operationSize = OperationSize_Byte;
						return true;
					case OpMode_DataRegisterToMemory_Word:
						*operationSize = OperationSize_Word;
						return true;
					case OpMode_DataRegisterToMemory_Long:
						*operationSize = OperationSize_Long;
						return true;
					case OpMode_MemoryToAddressRegister_Long:
						*operationSize = OperationSize_Long;
						return true;
					default:
						M68060_ERROR("Unsupported OpMode");
						return false;
				}
			}
		case SizeEncoding_DefaultOpSizeEncoding:
			{
				OpSize opSize = (opWord & OpWord_OpSize_Mask) >> OpWord_OpSize_Shift;
				switch (opSize)
				{
					case OpSize_Byte:
						*operationSize = OperationSize_Byte;
						return true;
					case OpSize_Word:
						*operationSize = OperationSize_Word;
						return true;
					case OpSize_Long:
						*operationSize = OperationSize_Long;
						return true;
					default:
						return false;
				}
			}
		case SizeEncoding_RelativeBranchEncoding:
			{
				uint displacement8Bit = (opWord & 0xff);
				if (displacement8Bit == 0x00)
					*operationSize = OperationSize_Word;
				else if (displacement8Bit == 0xff)
					*operationSize = OperationSize_Long;
				else
					*operationSize = OperationSize_Byte;
				return true;
			}
		default:
			M68060_ERROR("Unsupported SizeEncoding");
			return false;
	}
}

static bool isValidEAMode(EAMode eaMode, EAModeMask eaModeMask)
{
	return (eaMode == EAMode_None || ((1U << eaMode) & eaModeMask));
}

bool decodeInstructionLengthFromInstructionWords(const uint16_t* instructionWords, uint numInstructionWordsAvailable, InstructionLength* outInstructionLength)
{
	const OpWordLengthInfo* opWordLengthInfo = opWordLengthInformation;
	const OpWordClassInfo* opWordClassInfo;
	uint16_t opWord;
	uint operandOffset;
	InstructionLength instructionLength = { 0 };
	bool validInstruction = true;

	M68060_ASSERT(numInstructionWordsAvailable > 0, "Must supply at least one instruction word");

	opWord = instructionWords[0];

	while ((opWord & opWordLengthInfo->mask) != opWordLengthInfo->match)
	{
		opWordLengthInfo++;
	}

	if (opWordLengthInfo->mask == 0 && opWordLengthInfo->match == 0)
	{
		validInstruction = false;
	}
	else
	{
		bool validOperationSize;
		OperationSize operationSize;
		opWordClassInfo = &opWordClassInformation[opWordLengthInfo->class];

		instructionLength.numSpecialOperandSpecifierWords = opWordClassInfo->numSpecialOperandSpecifierWords;
		instructionLength.description = opWordLengthInfo->description;

		validOperationSize = decodeOperationSize(opWord, opWordClassInfo->sizeEncoding, &operationSize);

		if (!validOperationSize)
			validInstruction = false;
		
		operandOffset = 1 + instructionLength.numSpecialOperandSpecifierWords;

		if (validInstruction)
		{
			EAMode eaMode;
			bool validEaMode = decodeOperand(opWord, opWordClassInfo->sourceEAEncoding, &eaMode);
			bool firstExtensionWordAvailable = (operandOffset < numInstructionWordsAvailable);
			uint16_t firstExtensionWord = (firstExtensionWordAvailable ? instructionWords[operandOffset] : 0);

			if (validEaMode && isValidEAMode(eaMode, opWordClassInfo->sourceEAModeMask))
			{
				DecodeOperandLengthResult decodeOperandLengthResult = decodeOperandLength(eaMode, firstExtensionWordAvailable, firstExtensionWord, operationSize, &instructionLength.numSourceEAExtensionWords);

				if (decodeOperandLengthResult == DecodeOperandLengthResult_InsufficientData)
					return false;
				else if (decodeOperandLengthResult == DecodeOperandLengthResult_InvalidInstruction)
					validInstruction = false;
			}
			else
				validInstruction = false;
		}

		if (validInstruction)
		{
		
			operandOffset += instructionLength.numSourceEAExtensionWords;

			{
				EAMode eaMode;
				bool validEaMode = decodeOperand(opWord, opWordClassInfo->destinationEAEncoding, &eaMode);
				bool firstExtensionWordAvailable = (operandOffset < numInstructionWordsAvailable);
				uint16_t firstExtensionWord = (firstExtensionWordAvailable ? instructionWords[operandOffset] : 0);

				if (validEaMode && isValidEAMode(eaMode, opWordClassInfo->destinationEAModeMask))
				{
					DecodeOperandLengthResult decodeOperandLengthResult = decodeOperandLength(eaMode, firstExtensionWordAvailable, firstExtensionWord, operationSize, &instructionLength.numDestinationEAExtensionWords);

					if (decodeOperandLengthResult == DecodeOperandLengthResult_InsufficientData)
						return false;
					else if (decodeOperandLengthResult == DecodeOperandLengthResult_InvalidInstruction)
						validInstruction = false;
				}
				else
					validInstruction = false;
			}
		}
	}

	
	if (!validInstruction)
	{
		InstructionLength invalidInstructionLength = { 0, 0, 0, 0, "invalid instruction" } ;
		
		instructionLength = invalidInstructionLength;
	}

	instructionLength.totalWords = 1 + instructionLength.numSpecialOperandSpecifierWords
		 + instructionLength.numSourceEAExtensionWords + instructionLength.numDestinationEAExtensionWords;
		
	*outInstructionLength = instructionLength;
	
	return true;
}
