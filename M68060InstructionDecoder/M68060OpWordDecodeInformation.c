
#include "M68060OpWordDecodeInformation.h"

static OpWordClassInfo s_opWordClassInformation[] =
{
	{ 0, SizeEncoding_None, EAEncoding_None, EAModeMask_None, EAEncoding_None, EAModeMask_None, }, // OpWordClass_NoExtraWords
	{ 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_None, EAModeMask_None, EAEncoding_None, EAModeMask_None, DecodeOperand_DefaultDnLocation, DecodeOperand_SecondaryDnLocation, DecodeIeeResult_IeeB, }, // OpWordClass_EncodedSize_SrcDn_DestDn
	{ 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_None, EAModeMask_None, EAEncoding_None, EAModeMask_None, DecodeOperand_DefaultPreDecrementAnLocation, DecodeOperand_SecondaryPreDecrementAnLocation, DecodeIeeResult_IeeB, }, // OpWordClass_EncodedSize_SrcAnPreDecrement_DestAnPreDecrement
	{ 0, SizeEncoding_Byte, EAEncoding_DefaultEALocation, EAModeMask_Data, EAEncoding_MoveDestinationEALocation, EAModeMask_DataAlterable, }, // OpWordClass_Move_B,
	{ 0, SizeEncoding_Word, EAEncoding_DefaultEALocation, EAModeMask_All, EAEncoding_MoveDestinationEALocation, EAModeMask_Alterable, }, // OpWordClass_Move_W,
	{ 0, SizeEncoding_Long, EAEncoding_DefaultEALocation, EAModeMask_All, EAEncoding_MoveDestinationEALocation, EAModeMask_Alterable, }, // OpWordClass_Move_L,
	{ 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_DefaultEALocation, EAModeMask_Data, EAEncoding_None, EAModeMask_None, DecodeOperand_DefaultEALocation, DecodeOperand_SecondaryDnLocation, DecodeIeeResult_IeeB, }, // OpWordClass_EncodedSize_SrcEaData_Dn,
	{ 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_DefaultEALocation, EAModeMask_All, EAEncoding_None, EAModeMask_None, DecodeOperand_DefaultEALocation, DecodeOperand_SecondaryDnLocation, DecodeIeeResult_None, }, // OpWordClass_EncodedSize_SrcEaAll_DnReadOnly,
	{ 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_DefaultEALocation, EAModeMask_All, EAEncoding_None, EAModeMask_None, DecodeOperand_DefaultEALocation, DecodeOperand_SecondaryDnLocation, DecodeIeeResult_IeeB, }, // OpWordClass_EncodedSize_SrcEaAll_DnReadWrite,
	{ 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_DefaultEALocation, EAModeMask_All, EAEncoding_None, EAModeMask_None, DecodeOperand_DefaultEALocation, DecodeOperand_SecondaryAnLocation, DecodeIeeResult_None, }, // OpWordClass_EncodedSize_SrcEaAll_AnReadOnly,
	{ 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_DefaultEALocation, EAModeMask_All, EAEncoding_None, EAModeMask_None, DecodeOperand_DefaultEALocation, DecodeOperand_SecondaryAnLocation, DecodeIeeResult_IeeB, }, // OpWordClass_EncodedSize_SrcEaAll_AnReadWrite,
	{ 1, SizeEncoding_Long, EAEncoding_DefaultEALocation, EAModeMask_Data, EAEncoding_None, EAModeMask_None, }, // OpWordClass_LongMulDiv,
	{ 1, SizeEncoding_None, EAEncoding_DefaultEALocation, EAModeMask_DnOrControl, EAEncoding_None, EAModeMask_None, }, // OpWordClass_Bitfield_ReadEa,
	{ 0, SizeEncoding_None, EAEncoding_DefaultEALocation, EAModeMask_Control, EAEncoding_None, EAModeMask_None, }, // OpWordClass_Control,
	{ 0, SizeEncoding_Byte, EAEncoding_DefaultEALocation, EAModeMask_Data, EAEncoding_None, EAModeMask_None, }, // OpWordClass_Byte_SrcEa,
	{ 0, SizeEncoding_Word, EAEncoding_DefaultEALocation, EAModeMask_Data, EAEncoding_None, EAModeMask_None, }, // OpWordClass_Word_SrcEa,
	{ 0, SizeEncoding_Long, EAEncoding_DefaultEALocation, EAModeMask_Data, EAEncoding_None, EAModeMask_None, }, // OpWordClass_Long_SrcEa,
	{ 0, SizeEncoding_Byte, EAEncoding_Immediate, EAModeMask_All, EAEncoding_DefaultEALocation, EAModeMask_Data, }, // OpWordClass_BitImmInstruction_Read,
	{ 0, SizeEncoding_Byte, EAEncoding_Immediate, EAModeMask_All, EAEncoding_DefaultEALocation, EAModeMask_DataAlterable, }, // OpWordClass_BitImmInstruction_ReadWrite,
	{ 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_Immediate, EAModeMask_All, EAEncoding_DefaultEALocation, EAModeMask_DataAlterable | EAModeMask_MemoryReference_PC, }, // OpWordClass_EncodedSize_Imm_Ea_Read,
	{ 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_Immediate, EAModeMask_All, EAEncoding_DefaultEALocation, EAModeMask_DataAlterable, DecodeOperand_Immediate, DecodeOperand_DefaultEALocation, DecodeIeeResult_IeeB, }, // OpWordClass_EncodedSize_Imm_Ea_ReadWrite,
	{ 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_None, EAModeMask_None, EAEncoding_DefaultEALocation, EAModeMask_MemoryAlterable, DecodeOperand_SecondaryDnLocation, DecodeOperand_DefaultEALocation, DecodeIeeResult_IeeB, }, // OpWordClass_EncodedSize_Rn_Ea_1,
	{ 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_None, EAModeMask_None, EAEncoding_DefaultEALocation, EAModeMask_DataAlterable, }, // OpWordClass_EncodedSize_Rn_Ea_2,
	{ 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_None, EAModeMask_None, EAEncoding_DefaultEALocation, EAModeMask_All, }, // OpWordClass_EncodedSize_DestEa_Read,
	{ 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_None, EAModeMask_None, EAEncoding_DefaultEALocation, EAModeMask_DataAlterable, }, // OpWordClass_EncodedSize_DestEa_ReadWrite,
	{ 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_None, EAModeMask_None, EAEncoding_DefaultEALocation, EAModeMask_Alterable, DecodeOperand_Imm3Bit, DecodeOperand_DefaultEALocation, DecodeIeeResult_IeeB, }, // OpWordClass_SrcImm3Bit_DestEa_Alterable,
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

static OpWordDecodeInfo s_opWordDecodeInformation[] =
{
	{ false, 0xf0ff, 0x50fc, "TRAPcc", OpWordClass_NoExtraWords, }, // Shadows Scc <ea>
	{ false, 0xf0ff, 0x50fa, "TRAPcc.W #imm", OpWordClass_ImmediateWord, }, // Shadows Scc <ea>
	{ false, 0xf0ff, 0x50fb, "TRAPcc.L #imm", OpWordClass_ImmediateLong, }, // Shadows Scc <ea>
	{ false, 0xf0f8, 0x50c8, "DBcc <relative address>", OpWordClass_ImmediateWord, }, // Shadows Scc <ea>
	{ false, 0xf0c0, 0x50c0, "Scc <ea>", OpWordClass_DestEa_DataAlterable, }, // Shadows ADDQ/SUBQ

	{ false, 0xf1f8, 0xc100, "ABCD Dx,Dy", OpWordClass_NoExtraWords, },
	{ false, 0xf1f8, 0xc108, "ABCD -(Ax),-(Ay)", OpWordClass_NoExtraWords, },
	{ true, 0xf138, 0xd100, "ADDX Dx,Dy", OpWordClass_EncodedSize_SrcDn_DestDn, IeeOperation_AddX, Pairability_pOEP_Only, }, // Shadows ADD Dn,<ea>
	{ true, 0xf138, 0xd108, "ADDX -(Ax),-(Ay)", OpWordClass_EncodedSize_SrcAnPreDecrement_DestAnPreDecrement, IeeOperation_AddX, Pairability_pOEP_Only, }, // Shadows ADD Dn,<ea>

	{ true, 0xf1c0, 0xd0c0, "ADDA.W <ea>,An", OpWordClass_EncodedSize_SrcEaAll_AnReadWrite, IeeOperation_AddA, Pairability_pOEP_Or_sOEP, }, // Shadows ADD <ea>,Dn
	{ true, 0xf1c0, 0xd1c0, "ADDA.L <ea>,An", OpWordClass_EncodedSize_SrcEaAll_AnReadWrite, IeeOperation_AddA, Pairability_pOEP_Or_sOEP, }, // Shadows ADD Dn,<ea>

	{ true, 0xf100, 0xd000, "ADD <ea>,Dn", OpWordClass_EncodedSize_SrcEaAll_DnReadWrite, IeeOperation_Add, Pairability_pOEP_Or_sOEP, },

	{ true, 0xf100, 0xd100, "ADD Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea_1, IeeOperation_Add, Pairability_pOEP_Or_sOEP, },

	{ true, 0xff00, 0x0600, "ADDI #imm,<ea>", OpWordClass_EncodedSize_Imm_Ea_ReadWrite, IeeOperation_Add, Pairability_pOEP_Or_sOEP, },
	{ true, 0xf138, 0x5008, "ADDQ #imm,An", OpWordClass_SrcImm3Bit_DestEa_Alterable, IeeOperation_AddA, Pairability_pOEP_Or_sOEP, },  // Shadows ADDQ #imm,<ea>
	{ true, 0xf100, 0x5000, "ADDQ #imm,<ea>", OpWordClass_SrcImm3Bit_DestEa_Alterable, IeeOperation_Add, Pairability_pOEP_Or_sOEP, },
	
	{ false, 0xf1f8, 0xc140, "EXG Dm,Dn", OpWordClass_NoExtraWords, }, // Shadows AND
	{ false, 0xf1f8, 0xc148, "EXG Am,An", OpWordClass_NoExtraWords, }, // Shadows AND
	{ false, 0xf1f8, 0xc188, "EXG Dm,An", OpWordClass_NoExtraWords, }, // Shadows AND
	{ false, 0xf1c0, 0xc1c0, "MULS.W <ea>,Dn", OpWordClass_Word_SrcEa, }, // Shadows AND
	{ false, 0xf1c0, 0xc0c0, "MULU.W <ea>,Dn", OpWordClass_Word_SrcEa, }, // Shadows AND
	{ false, 0xf100, 0xc000, "AND <ea>,Dn", OpWordClass_EncodedSize_SrcEaData_Dn, },
	{ false, 0xf100, 0xc100, "AND Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea_1, },

	{ false, 0xffff, 0x023c, "ANDI #imm,CCR", OpWordClass_ImmediateByte, }, // Shadows ANDI #imm,<ea>
	{ false, 0xff00, 0x0200, "ANDI #imm,<ea>", OpWordClass_EncodedSize_Imm_Ea_ReadWrite, },

	{ false, 0xffc0, 0xeac0, "BFCHG <ea>{Do:Dw}", OpWordClass_Bitfield_ReadWriteEa, }, // Shadows ASL/ASR #imm/Dm,Dn
	{ false, 0xffc0, 0xeec0, "BFSET <ea>{Do:Dw}", OpWordClass_Bitfield_ReadWriteEa, }, // Shadows ASL/ASR #imm/Dm,Dn

	{ false, 0xfec0, 0xe0c0, "ASL/ASR <ea>", OpWordClass_DestEa_MemoryAlterable, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn
	{ false, 0xfec0, 0xe2c0, "LSL/LSR <ea>", OpWordClass_DestEa_MemoryAlterable, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn
	{ false, 0xfec0, 0xe4c0, "ROXL/ROXR <ea>", OpWordClass_DestEa_MemoryAlterable, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn
	{ false, 0xfec0, 0xe6c0, "ROL/ROR <ea>", OpWordClass_DestEa_MemoryAlterable, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn

	{ false, 0xff00, 0x6100, "BSR <relative address>", OpWordClass_RelativeBranch, }, // Shadows Bcc
	{ false, 0xf000, 0x6000, "Bcc <relative address>", OpWordClass_RelativeBranch, },
	{ false, 0xf138, 0x0108, "MOVEP Dx <-> d16(An)", OpWordClass_D16An, }, // Shadows BCLR/BCHG Dn,<ea>
	{ false, 0xf1c0, 0x0140, "BCHG Dn,<ea>", OpWordClass_DestEa_DataAlterable, },
	{ false, 0xffc0, 0x0840, "BCHG #imm,<ea>", OpWordClass_BitImmInstruction_ReadWrite, },
	{ false, 0xf1c0, 0x0180, "BCLR Dn,<ea>", OpWordClass_DestEa_DataAlterable, },
	{ false, 0xffc0, 0x0880, "BCLR #imm,<ea>", OpWordClass_BitImmInstruction_ReadWrite, },
	{ false, 0xfff8, 0x4848, "BKPT #imm", OpWordClass_NoExtraWords, }, // Shadows PEA
	{ false, 0xf1c0, 0x01c0, "BSET Dn,<ea>", OpWordClass_DestEa_DataAlterable, },
	{ false, 0xffc0, 0x08c0, "BSET #imm,<ea>", OpWordClass_BitImmInstruction_ReadWrite, },
	{ false, 0xf1c0, 0x0100, "BTST Dn,<ea>", OpWordClass_Byte_DestEa_Data, },
	{ false, 0xffc0, 0x0800, "BTST #imm,<ea>", OpWordClass_BitImmInstruction_Read, },
	{ false, 0xffc0, 0xecc0, "BFCLR <ea>{Do:Dw}", OpWordClass_Bitfield_ReadWriteEa, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn
	{ false, 0xffc0, 0xebc0, "BFEXTS <ea>{Do:Dw},Dn", OpWordClass_Bitfield_ReadEa, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn
	{ false, 0xffc0, 0xe9c0, "BFEXTU <ea>{Do:Dw},Dn", OpWordClass_Bitfield_ReadEa, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn
	{ false, 0xffc0, 0xedc0, "BFFFO <ea>{Do:Dw},Dn", OpWordClass_Bitfield_ReadEa, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn
	{ false, 0xffc0, 0xefc0, "BFINS Dn,<ea>{Do:Dw}", OpWordClass_Bitfield_ReadWriteEa, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn
	{ false, 0xffc0, 0xe8c0, "BFTST <ea>{Do:Dw}", OpWordClass_Bitfield_ReadEa, }, // Shadows ASL/ASR/LSL/LSR/ROL/ROR/ROXL/ROXR #imm/Dm,Dn
	{ false, 0xf1c0, 0x4180, "CHK.W <ea>,Dn", OpWordClass_Word_SrcEa, },
	{ false, 0xf1c0, 0x4100, "CHK.L <ea>,Dn", OpWordClass_Long_SrcEa, },

	{ false, 0xf018, 0xe000, "ASL/ASR #imm/Dm,Dn", OpWordClass_EncodedSize, },
	{ false, 0xf018, 0xe008, "LSL/LSR #imm/Dm,Dn", OpWordClass_EncodedSize, },
	{ false, 0xf018, 0xe010, "ROXL/ROXR #imm/Dm,Dn", OpWordClass_EncodedSize, },
	{ false, 0xf018, 0xe018, "ROL/ROR #imm/Dm,Dn", OpWordClass_EncodedSize, },
	
	{ false, 0xffc0, 0x42c0, "MOVE CCR,<ea>", OpWordClass_DestEa_DataAlterable, }, // Shadows CLR

	{ false, 0xff00, 0x4200, "CLR <ea>", OpWordClass_EncodedSize_DestEa_ReadWrite, },
	{ false, 0xf138, 0xb108, "CMPM (Ax)+,(Ay)+", OpWordClass_NoExtraWords, }, // Shadows EOR
	{ false, 0xf1c0, 0xb100, "EOR.B Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea_2, }, // Shadows CMP
	{ false, 0xf1c0, 0xb140, "EOR.W Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea_2, }, // Shadows CMP
	{ false, 0xf1c0, 0xb180, "EOR.L Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea_2, }, // Shadows CMP
	{ true, 0xf1c0, 0xb0c0, "CMPA.W <ea>,An", OpWordClass_EncodedSize_SrcEaAll_AnReadOnly, IeeOperation_CmpA, Pairability_pOEP_Or_sOEP, }, // Shadows CMP <ea>,Dn -- Perhaps possible to collapse with CMPA.L?
	{ true, 0xf1c0, 0xb1c0, "CMPA.L <ea>,An", OpWordClass_EncodedSize_SrcEaAll_AnReadOnly, IeeOperation_CmpA, Pairability_pOEP_Or_sOEP, }, // Shadows CMP <ea>,Dn
	{ true, 0xf000, 0xb000, "CMP <ea>,Dn", OpWordClass_EncodedSize_SrcEaAll_DnReadOnly, IeeOperation_Cmp, Pairability_pOEP_Or_sOEP, },

	{ false, 0xff00, 0x0c00, "CMPI #imm,<ea>", OpWordClass_EncodedSize_Imm_Ea_Read, },
	{ false, 0xf1c0, 0x81c0, "DIVS.W <ea>,Dn", OpWordClass_Word_SrcEa, },
	{ false, 0xffc0, 0x4c40, "DIVS/DIVU.L <ea>,Dr:Dq (can be 32bit or 64bit division)", OpWordClass_LongMulDiv, },
	{ false, 0xf1c0, 0x80c0, "DIVU.W <ea>,Dn", OpWordClass_Word_SrcEa, },
	{ false, 0xffff, 0x0a3c, "EORI #imm,CCR", OpWordClass_ImmediateByte, }, // Shadows EORI #imm,<ea>
	{ false, 0xff00, 0x0a00, "EORI #imm,<ea>", OpWordClass_EncodedSize_Imm_Ea_ReadWrite, },
	{ false, 0xfff8, 0x4880, "EXT.W Dn", OpWordClass_NoExtraWords, },
	{ false, 0xfff8, 0x48c0, "EXT.L Dn", OpWordClass_NoExtraWords, },
	{ false, 0xfff8, 0x49c0, "EXTB.L Dn", OpWordClass_NoExtraWords, },
	{ false, 0xffff, 0x4afc, "ILLEGAL", OpWordClass_NoExtraWords, },
	{ false, 0xffc0, 0x4ec0, "JMP <ea>", OpWordClass_Control, },
	{ false, 0xffc0, 0x4e80, "JSR <ea>", OpWordClass_Control, },
	{ false, 0xf1c0, 0x41c0, "LEA <ea>,An", OpWordClass_Control, },
	{ false, 0xfff8, 0x4e50, "LINK.W An,#imm", OpWordClass_ImmediateWord, },
	{ false, 0xfff8, 0x4808, "LINK.L An,#imm", OpWordClass_ImmediateLong, },

	{ false, 0xf000, 0x1000, "MOVE.B <ea>,<ea>", OpWordClass_Move_B, },
	{ false, 0xf000, 0x2000, "MOVE.L <ea>,<ea>", OpWordClass_Move_L, }, // Combined MOVE.L & MOVEA.L
	{ false, 0xf000, 0x3000, "MOVE.W <ea>,<ea>", OpWordClass_Move_W, }, // Combined MOVE.W & MOVEA.W

	{ false, 0xffc0, 0x44c0, "MOVE <ea>,CCR", OpWordClass_Byte_SrcEa, },

	{ false, 0xffc0, 0x40c0, "MOVE SR,<ea>", OpWordClass_DestEa_DataAlterable, },

	{ false, 0xfff8, 0xf620, "MOVE16 (Ax)+,(Ay)+", OpWordClass_1SpecialWord, },
	{ false, 0xffe0, 0xf600, "MOVE16 (An <-> xxx.L)", OpWordClass_2SpecialWords, },

	{ false, 0xff80, 0x4880, "MOVEM reglist,<ea>", OpWordClass_Movem_RegistersToMemory, },
	{ false, 0xff80, 0x4c80, "MOVEM <ea>,reglist", OpWordClass_Movem_MemoryToRegisters, },

	{ false, 0xf100, 0x7000, "MOVEQ #imm,Dn", OpWordClass_NoExtraWords, },

	{ false, 0xffc0, 0x4c00, "MULS/MULU.L <ea>,Dm:Dn (can be 32bit or 64bit multiply)", OpWordClass_LongMulDiv, },

	{ false, 0xffc0, 0x4800, "NBCD <ea>", OpWordClass_DestEa_DataAlterable, },
	{ false, 0xff00, 0x4400, "NEG <ea>", OpWordClass_EncodedSize_DestEa_ReadWrite, },
	{ false, 0xff00, 0x4000, "NEGX <ea>", OpWordClass_EncodedSize_DestEa_ReadWrite, },

	{ false, 0xffff, 0x4e71, "NOP", OpWordClass_NoExtraWords, },

	{ false, 0xff00, 0x4600, "NOT <ea>", OpWordClass_EncodedSize_DestEa_ReadWrite, },

	{ false, 0xf1f8, 0x8140, "PACK Dm,Dn,#imm", OpWordClass_ImmediateWord, }, // Shadows OR
	{ false, 0xf1f8, 0x8148, "PACK -(Am),-(An),#imm", OpWordClass_ImmediateWord, }, // Shadows OR
	{ false, 0xf1f8, 0x8180, "UNPK Dm,Dn,#imm", OpWordClass_ImmediateWord, }, // Shadows OR
	{ false, 0xf1f8, 0x8188, "UNPK -(Am),-(An),#imm", OpWordClass_ImmediateWord, }, // Shadows OR

	{ false, 0xf1f8, 0x8100, "SBCD Dx,Dy", OpWordClass_NoExtraWords, }, // Shadows OR
	{ false, 0xf1f8, 0x8108, "SBCD -(Ax),-(Ay)", OpWordClass_NoExtraWords, }, // Shadows OR

	{ false, 0xf100, 0x8000, "OR <ea>,Dn", OpWordClass_EncodedSize_SrcEaData_Dn, },
	{ false, 0xf100, 0x8100, "OR Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea_1, },

	{ false, 0xffff, 0x003c, "ORI #imm,CCR", OpWordClass_ImmediateByte, }, // Shadows ANDI #imm,<ea>
	{ false, 0xff00, 0x0000, "ORI #imm,<ea>", OpWordClass_EncodedSize_Imm_Ea_ReadWrite, },

	{ false, 0xfff8, 0x4840, "SWAP Dn", OpWordClass_NoExtraWords, }, // Shadows PEA

	{ false, 0xffc0, 0x4840, "PEA <ea>", OpWordClass_Control, },


	{ false, 0xffff, 0x4e74, "RTD #imm", OpWordClass_ImmediateWord, },
	{ false, 0xffff, 0x4e77, "RTR", OpWordClass_NoExtraWords, },
	{ false, 0xffff, 0x4e75, "RTS", OpWordClass_NoExtraWords, },

	{ true, 0xf138, 0x9100, "SUBX Dx,Dy", OpWordClass_EncodedSize_SrcDn_DestDn, IeeOperation_SubX, Pairability_pOEP_Only, }, // Shadows SUB Dn,<ea>
	{ true, 0xf138, 0x9108, "SUBX -(Ax),-(Ay)", OpWordClass_EncodedSize_SrcAnPreDecrement_DestAnPreDecrement, IeeOperation_SubX, Pairability_pOEP_Only, }, // Shadows SUB Dn,<ea>
	{ true, 0xf1c0, 0x90c0, "SUBA.W <ea>,An", OpWordClass_EncodedSize_SrcEaAll_AnReadWrite, IeeOperation_SubA, Pairability_pOEP_Or_sOEP, }, // Shadows SUB <ea>,Dn
	{ true, 0xf1c0, 0x91c0, "SUBA.L <ea>,An", OpWordClass_EncodedSize_SrcEaAll_AnReadWrite, IeeOperation_SubA, Pairability_pOEP_Or_sOEP, }, // Shadows SUB Dn,<ea>
	{ true, 0xf100, 0x9000, "SUB <ea>,Dn", OpWordClass_EncodedSize_SrcEaAll_DnReadWrite, IeeOperation_Sub, Pairability_pOEP_Or_sOEP, },
	{ true, 0xf100, 0x9100, "SUB Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea_1, IeeOperation_Sub, Pairability_pOEP_Or_sOEP, },
	{ true, 0xff00, 0x0400, "SUBI #imm,<ea>", OpWordClass_EncodedSize_Imm_Ea_ReadWrite, IeeOperation_Sub, Pairability_pOEP_Or_sOEP, },
	{ true, 0xf138, 0x5108, "SUBQ #imm,An", OpWordClass_SrcImm3Bit_DestEa_Alterable, IeeOperation_SubA, Pairability_pOEP_Or_sOEP, }, // Shadows SUBQ #imm,<ea>
	{ true, 0xf100, 0x5100, "SUBQ #imm,<ea>", OpWordClass_SrcImm3Bit_DestEa_Alterable, IeeOperation_Sub, Pairability_pOEP_Or_sOEP, },

	{ false, 0xffc0, 0x4ac0, "TAS <ea>", OpWordClass_DestEa_DataAlterable, },

	{ false, 0xfff8, 0x4e40, "TRAP #imm", OpWordClass_NoExtraWords, },
	{ false, 0xffff, 0x4e76, "TRAPV", OpWordClass_NoExtraWords, },

	{ false, 0xff00, 0x4a00, "TST <ea>", OpWordClass_EncodedSize_DestEa_Read, },

	{ false, 0xfff8, 0x4e58, "UNLK An", OpWordClass_NoExtraWords, },

	{ true, 0, 0, "Unknown instruction", },
};

const OpWordClassInfo* getOpWordClassInformation(OpWordClass opWordClass)
{
	return &s_opWordClassInformation[opWordClass];
}

const OpWordDecodeInfo* getOpWordDecodeInformation(uint16_t opWord)
{
	const OpWordDecodeInfo* opWordDecodeInfo = s_opWordDecodeInformation;
	
	while ((opWord & opWordDecodeInfo->mask) != opWordDecodeInfo->match)
	{
		opWordDecodeInfo++;
	}
	
	if (opWordDecodeInfo->mask == 0 && opWordDecodeInfo->match == 0)
		return 0;
	else
		return opWordDecodeInfo;
}
