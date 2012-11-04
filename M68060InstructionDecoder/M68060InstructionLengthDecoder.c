#include "M68060InstructionLengthDecoder.h"
#include "../Assert.h"

#include "M68060EA.h"
#include "M68060OpWord.h"

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
	OpWordClass_EncodedSize_Ea_Rn,
	OpWordClass_LongMulDiv,
	OpWordClass_Bitfield_ReadEa,
	OpWordClass_SrcEa,
	OpWordClass_Word_SrcEa,
	OpWordClass_BitImmInstruction,
	OpWordClass_EncodedSize_Imm_Ea,
	OpWordClass_EncodedSize_Rn_Ea,
	OpWordClass_EncodedSize_Dn_Ea,
	OpWordClass_DestEa,
	OpWordClass_EncodedSize,
	OpWordClass_ImmediateByte,
	OpWordClass_ImmediateWord,
	OpWordClass_ImmediateLong,
	OpWordClass_RelativeBranch,
	OpWordClass_D16An,
	OpWordClass_1SpecialWord,
	OpWordClass_2SpecialWords,
	OpWordClass_Movem,
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
	EAEncoding destinationEAEncoding;
} OpWordClassInfo;

static OpWordClassInfo opWordClassInformation[] =
{
	{ 0, SizeEncoding_None, EAEncoding_None, EAEncoding_None, }, // OpWordClass_NoExtraWords
	{ 0, SizeEncoding_Byte, EAEncoding_DefaultEALocation, EAEncoding_MoveDestinationEALocation }, // OpWordClass_Move_B,
	{ 0, SizeEncoding_Word, EAEncoding_DefaultEALocation, EAEncoding_MoveDestinationEALocation, }, // OpWordClass_Move_W,
	{ 0, SizeEncoding_Long, EAEncoding_DefaultEALocation, EAEncoding_MoveDestinationEALocation, }, // OpWordClass_Move_L,
	{ 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_DefaultEALocation, EAEncoding_None, }, // OpWordClass_EncodedSize_Ea_Rn,
	{ 1, SizeEncoding_Long, EAEncoding_DefaultEALocation, EAEncoding_None, }, // OpWordClass_LongMulDiv,
	{ 1, SizeEncoding_None, EAEncoding_DefaultEALocation, EAEncoding_None, }, // OpWordClass_Bitfield_ReadEa,
	{ 0, SizeEncoding_None, EAEncoding_DefaultEALocation, EAEncoding_None, }, // OpWordClass_SrcEa,
	{ 0, SizeEncoding_Word, EAEncoding_DefaultEALocation, EAEncoding_None, }, // OpWordClass_Word_SrcEa,
	{ 0, SizeEncoding_Byte, EAEncoding_Immediate, EAEncoding_DefaultEALocation, }, // OpWordClass_BitImmInstruction,
	{ 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_Immediate, EAEncoding_DefaultEALocation, }, // OpWordClass_EncodedSize_Imm_Ea,
	{ 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_None, EAEncoding_DefaultEALocation, }, // OpWordClass_EncodedSize_Rn_Ea,
	{ 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_None, EAEncoding_DefaultEALocation, }, // OpWordClass_EncodedSize_Dn_Ea,
	{ 0, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, }, // OpWordClass_DestEa,
	{ 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_None, EAEncoding_None, }, // OpWordClass_EncodedSize,
	{ 0, SizeEncoding_Byte, EAEncoding_Immediate, EAEncoding_None, }, // OpWordClass_ImmediateByte,
	{ 0, SizeEncoding_Word, EAEncoding_Immediate, EAEncoding_None, }, // OpWordClass_ImmediateWord,
	{ 0, SizeEncoding_Long, EAEncoding_Immediate, EAEncoding_None, }, // OpWordClass_ImmediateLong,
	{ 0, SizeEncoding_RelativeBranchEncoding, EAEncoding_RelativeBranch, EAEncoding_None, }, // OpWordClass_RelativeBranch,
	{ 0, SizeEncoding_Word, EAEncoding_D16, EAEncoding_None, }, // OpWordClass_D16An
	{ 1, SizeEncoding_None, EAEncoding_None, EAEncoding_None, }, // OpWordClass_1SpecialWord
	{ 2, SizeEncoding_None, EAEncoding_None, EAEncoding_None, }, // OpWordClass_2SpecialWords
	{ 1, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, }, // OpWordClass_Movem
	{ 1, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, }, // OpWordClass_Bitfield_ReadWriteEa
};

static OpWordLengthInfo opWordLengthInformation[] =
{

	{ 0xf0ff, 0x50fc, "TRAPcc", OpWordClass_NoExtraWords, }, // Shadows Scc <ea>
	{ 0xf0ff, 0x50fa, "TRAPcc.W #imm", OpWordClass_ImmediateWord, }, // Shadows Scc <ea>
	{ 0xf0ff, 0x50fb, "TRAPcc.L #imm", OpWordClass_ImmediateLong, }, // Shadows Scc <ea>
	{ 0xf0f8, 0x50c8, "DBcc <relative address>", OpWordClass_ImmediateWord, }, // Shadows Scc <ea>
	{ 0xf0c0, 0x50c0, "Scc <ea>", OpWordClass_DestEa, }, // Shadows ADDQ/SUBQ

	{ 0xf1f8, 0xc100, "ABCD Dx,Dy", OpWordClass_NoExtraWords, },
	{ 0xf1f8, 0xc108, "ABCD -(Ax),-(Ay)", OpWordClass_NoExtraWords, },
	{ 0xf138, 0xd100, "ADDX Dx,Dy", OpWordClass_NoExtraWords, }, // Shadows ADD Dn,<ea>
	{ 0xf138, 0xd108, "ADDX -(Ax),-(Ay)", OpWordClass_NoExtraWords, }, // Shadows ADD Dn,<ea>
	{ 0xf100, 0xd000, "ADD <ea>,Dn", OpWordClass_EncodedSize_Ea_Rn, },
	{ 0xf100, 0xd100, "ADD Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea, },
	{ 0xff00, 0x0600, "ADDI #imm,<ea>", OpWordClass_EncodedSize_Imm_Ea, },
	{ 0xf100, 0x5000, "ADDQ #imm,<ea>", OpWordClass_DestEa, },
	{ 0xf1f8, 0xc140, "EXG Dn,Dn", OpWordClass_NoExtraWords, }, // Shadows AND
	{ 0xf1f8, 0xc148, "EXG Am,An", OpWordClass_NoExtraWords, }, // Shadows AND
	{ 0xf1f8, 0xc188, "EXG Dm,An", OpWordClass_NoExtraWords, }, // Shadows AND
	{ 0xf1c0, 0xc1c0, "MULS.W <ea>,Dn", OpWordClass_Word_SrcEa, }, // Shadows AND
	{ 0xf1c0, 0xc0c0, "MULU.W <ea>,Dn", OpWordClass_Word_SrcEa, }, // Shadows AND
	{ 0xf100, 0xc000, "AND <ea>,Dn", OpWordClass_EncodedSize_Ea_Rn, },
	{ 0xf100, 0xc100, "AND Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea, },

	{ 0xffff, 0x023c, "ANDI #imm,CCR", OpWordClass_ImmediateByte, }, // Shadows ANDI #imm,<ea>
	{ 0xff00, 0x0200, "ANDI #imm,<ea>", OpWordClass_EncodedSize_Imm_Ea, },

	{ 0xffc0, 0xeac0, "BFCHG <ea>{Do:Dw}", OpWordClass_Bitfield_ReadWriteEa, }, // Shadows ASL/ASR #imm/Dm,Dn
	{ 0xffc0, 0xeec0, "BFSET <ea>{Do:Dw}", OpWordClass_Bitfield_ReadWriteEa, }, // Shadows ASL/ASR #imm/Dm,Dn

	{ 0xfec0, 0xe0c0, "ASL/ASR <ea>", OpWordClass_DestEa, }, // Shadows ASL/ASR #imm/Dm,Dn
	{ 0xf018, 0xe000, "ASL/ASR #imm/Dm,Dn", OpWordClass_EncodedSize, },
	
	{ 0xff00, 0x6100, "BSR <relative address>", OpWordClass_RelativeBranch, }, // Shadows Bcc
	{ 0xf000, 0x6000, "Bcc <relative address>", OpWordClass_RelativeBranch, },
	{ 0xf138, 0x0108, "MOVEP Dx <-> d16(An)", OpWordClass_D16An, }, // Shadows BCLR/BCHG Dn,<ea>
	{ 0xf1c0, 0x0140, "BCHG Dn,<ea>", OpWordClass_DestEa, },
	{ 0xffc0, 0x0840, "BCHG #imm,<ea>", OpWordClass_BitImmInstruction, },
	{ 0xf1c0, 0x0180, "BCLR Dn,<ea>", OpWordClass_DestEa, },
	{ 0xffc0, 0x0880, "BCLR #imm,<ea>", OpWordClass_BitImmInstruction, },
	{ 0xfff8, 0x4848, "BKPT #imm", OpWordClass_NoExtraWords, }, // Shadows PEA
	{ 0xf1c0, 0x01c0, "BSET Dn,<ea>", OpWordClass_DestEa, },
	{ 0xffc0, 0x08c0, "BSET #imm,<ea>", OpWordClass_BitImmInstruction },
	{ 0xf1c0, 0x0100, "BTST Dn,<ea>", OpWordClass_DestEa, },
	{ 0xffc0, 0x0800, "BTST #imm,<ea>", OpWordClass_BitImmInstruction, },
	{ 0xffc0, 0xecc0, "BFCLR <ea>{Do:Dw}", OpWordClass_Bitfield_ReadWriteEa, },
	{ 0xffc0, 0xebc0, "BFEXTS <ea>{Do:Dw},Dn", OpWordClass_Bitfield_ReadEa, },
	{ 0xffc0, 0xe9c0, "BFEXTU <ea>{Do:Dw},Dn", OpWordClass_Bitfield_ReadEa, },
	{ 0xffc0, 0xedc0, "BFFFO <ea>{Do:Dw},Dn", OpWordClass_Bitfield_ReadEa, },
	{ 0xffc0, 0xefc0, "BFINS Dn,<ea>{Do:Dw}", OpWordClass_Bitfield_ReadWriteEa, },
	{ 0xffc0, 0xe8c0, "BFTST <ea>{Do:Dw}", OpWordClass_Bitfield_ReadEa, },
	{ 0xf1c0, 0x4180, "CHK.W <ea>,Dn", OpWordClass_Word_SrcEa, },
	{ 0xf1c0, 0x4100, "CHK.L <ea>,Dn", OpWordClass_ImmediateLong, },

	{ 0xffc0, 0x42c0, "MOVE CCR,<ea>", OpWordClass_DestEa, }, // Shadows CLR

	{ 0xff00, 0x4200, "CLR <ea>", OpWordClass_EncodedSize_Dn_Ea, },
	{ 0xf138, 0xb108, "CMPM (Ax)+,(Ay)+", OpWordClass_NoExtraWords, }, // Shadows EOR
	{ 0xf1c0, 0xb100, "EOR.B Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea, }, // Shadows CMP
	{ 0xf1c0, 0xb140, "EOR.W Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea, }, // Shadows CMP
	{ 0xf1c0, 0xb180, "EOR.L Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea, }, // Shadows CMP
	{ 0xf000, 0xb000, "CMP <ea>,Dn", OpWordClass_EncodedSize_Ea_Rn, },
	{ 0xff00, 0x0c00, "CMPI #imm,<ea>", OpWordClass_EncodedSize_Imm_Ea, },
	{ 0xf1c0, 0x81c0, "DIVS.W <ea>,Dn", OpWordClass_Word_SrcEa, },
	{ 0xffc0, 0x4c40, "DIVS/DIVU.L <ea>,Dr:Dq (can be 32bit or 64bit division)", OpWordClass_LongMulDiv, },
	{ 0xf1c0, 0x80c0, "DIVU.W <ea>,Dn", OpWordClass_Word_SrcEa, },
	{ 0xff00, 0x0a00, "EORI #imm,<ea>", OpWordClass_EncodedSize_Imm_Ea, },
	{ 0xfff8, 0x4880, "EXT.W Dn", OpWordClass_NoExtraWords, },
	{ 0xfff8, 0x48c0, "EXT.L Dn", OpWordClass_NoExtraWords, },
	{ 0xfff8, 0x49c0, "EXTB.L Dn", OpWordClass_NoExtraWords, },
	{ 0xffff, 0x4afc, "ILLEGAL", OpWordClass_NoExtraWords, },
	{ 0xffc0, 0x4ec0, "JMP <ea>", OpWordClass_SrcEa, },
	{ 0xffc0, 0x4e80, "JSR <ea>", OpWordClass_SrcEa, },
	{ 0xf1c0, 0x41c0, "LEA <ea>,An", OpWordClass_SrcEa, },
	{ 0xfff8, 0x4e50, "LINK.W An,#imm", OpWordClass_ImmediateWord, },
	{ 0xfff8, 0x4808, "LINK.L An,#imm", OpWordClass_ImmediateLong, },

	{ 0xfec0, 0xe2c0, "LSL/LSR <ea>", OpWordClass_DestEa, }, // Shadows LSL/LSR #imm/Dm,Dn
	{ 0xf018, 0xe008, "LSL/LSR #imm/Dm,Dn", OpWordClass_EncodedSize, },

	{ 0xf000, 0x1000, "MOVE.B <ea>,<ea>", OpWordClass_Move_B, },
	{ 0xf000, 0x2000, "MOVE.L <ea>,<ea>", OpWordClass_Move_L, },
	{ 0xf000, 0x3000, "MOVE.W <ea>,<ea>", OpWordClass_Move_W, },

	{ 0xffc0, 0x44c0, "MOVE <ea>,CCR", OpWordClass_SrcEa, },

	{ 0xffc0, 0x40c0, "MOVE SR,<ea>", OpWordClass_DestEa, },

	{ 0xfff8, 0xf620, "MOVE16 (Ax)+,(Ay)+", OpWordClass_1SpecialWord, },
	{ 0xffe0, 0xf600, "MOVE16 (An <-> xxx.L)", OpWordClass_2SpecialWords, },

	{ 0xfb80, 0x4880, "MOVEM <ea> <-> reglist", OpWordClass_Movem, },

	{ 0xf100, 0x7000, "MOVEQ #imm,Dn", OpWordClass_NoExtraWords, },

	{ 0xffc0, 0x4c00, "MULS/MULU.L <ea>,Dm:Dn (can be 32bit or 64bit multiply)", OpWordClass_LongMulDiv, },

	{ 0xffc0, 0x4800, "NBCD <ea>", OpWordClass_DestEa, },
	{ 0xff00, 0x4400, "NEG <ea>", OpWordClass_EncodedSize_Dn_Ea, },
	{ 0xff00, 0x4000, "NEGX <ea>", OpWordClass_EncodedSize_Dn_Ea, },

	{ 0xffff, 0x4e71, "NOP", OpWordClass_NoExtraWords, },

	{ 0xff00, 0x4600, "NOT <ea>", OpWordClass_EncodedSize_Dn_Ea, },

	{ 0xf1f8, 0x8140, "PACK Dm,Dn,#imm", OpWordClass_ImmediateWord, }, // Shadows OR
	{ 0xf1f8, 0x8148, "PACK -(Am),-(An),#imm", OpWordClass_ImmediateWord, }, // Shadows OR
	{ 0xf1f8, 0x8180, "UNPK Dm,Dn,#imm", OpWordClass_ImmediateWord, }, // Shadows OR
	{ 0xf1f8, 0x8188, "UNPK -(Am),-(An),#imm", OpWordClass_ImmediateWord, }, // Shadows OR

	{ 0xf1f8, 0x8100, "SBCD Dx,Dy", OpWordClass_NoExtraWords, }, // Shadows OR
	{ 0xf1f8, 0x8108, "SBCD -(Ax),-(Ay)", OpWordClass_NoExtraWords, }, // Shadows OR

	{ 0xf100, 0x8000, "OR <ea>,Dn", OpWordClass_EncodedSize_Ea_Rn, },
	{ 0xf100, 0x8100, "OR Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea, },

	{ 0xffff, 0x003c, "ORI #imm,CCR", OpWordClass_ImmediateByte, }, // Shadows ANDI #imm,<ea>
	{ 0xff00, 0x0000, "ORI #imm,<ea>", OpWordClass_EncodedSize_Imm_Ea, },

	{ 0xfff8, 0x4840, "SWAP Dn", OpWordClass_NoExtraWords, }, // Shadows PEA

	{ 0xffc0, 0x4840, "PEA <ea>", OpWordClass_SrcEa, },

	{ 0xfec0, 0xe6c0, "ROL/ROR <ea>", OpWordClass_DestEa, }, // Shadows ROL/ROR #imm/Dm,Dn
	{ 0xf018, 0xe018, "ROL/ROR #imm/Dm,Dn", OpWordClass_EncodedSize, },
	
	{ 0xfec0, 0xe4c0, "ROXL/ROXR <ea>", OpWordClass_DestEa, }, // Shadows ROXL/ROXR #imm/Dm,Dn
	{ 0xf018, 0xe010, "ROXL/ROXR #imm/Dm,Dn", OpWordClass_EncodedSize, },

	{ 0xffff, 0x4e77, "RTR", OpWordClass_NoExtraWords, },
	{ 0xffff, 0x4e75, "RTS", OpWordClass_NoExtraWords, },

	{ 0xf138, 0x9100, "SUBX Dx,Dy", OpWordClass_NoExtraWords, }, // Shadows SUB Dn,<e>
	{ 0xf138, 0x9108, "SUBX -(Ax),-(Ay)", OpWordClass_NoExtraWords, }, // Shadows SUB Dn,<e>
	{ 0xf100, 0x9000, "SUB <ea>,Dn", OpWordClass_EncodedSize_Ea_Rn, },
	{ 0xf100, 0x9100, "SUB Dn,<ea>", OpWordClass_EncodedSize_Rn_Ea, },
	{ 0xff00, 0x0400, "SUBI #imm,<ea>", OpWordClass_EncodedSize_Imm_Ea, },
	{ 0xf100, 0x5100, "SUBQ #imm,<ea>", OpWordClass_DestEa, },

	{ 0xffc0, 0x4ac0, "TAS <ea>", OpWordClass_DestEa, },

	{ 0xfff8, 0x4e40, "TRAP #imm", OpWordClass_NoExtraWords, },
	{ 0xffff, 0x4e76, "TRAPV", OpWordClass_NoExtraWords, },

	{ 0xff00, 0x4a00, "TST <ea>", OpWordClass_EncodedSize_Dn_Ea, },

	{ 0xfff8, 0x4e58, "UNLK An", OpWordClass_NoExtraWords, },

	{ 0, 0, "Unknown instruction", },
};

uint decodeBriefOrFullExtensionWordLength(uint16_t firstExtensionWord)
{
	uint numExtensionWords = 1;
		
	if (firstExtensionWord & ExtensionWord_FullWord_Mask)
	{
		DisplacementSize baseDisplacementSize = (firstExtensionWord & FullExtensionWord_BaseDisplacementSize_Mask) >> FullExtensionWord_BaseDisplacementSize_Shift;
		DisplacementSize outerDisplacementSize = (firstExtensionWord & FullExtensionWord_OuterDisplacementSize_Mask) >> FullExtensionWord_OuterDisplacementSize_Shift;

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
			default:
				M68060_ERROR("Unsuppored base displacement size");
		}

		// TODO: some outer displacement sizes are unsupported, depending on IndexSuppress bit; these should throw errors
		switch (outerDisplacementSize)
		{
			case DisplacementSize_Null:
				break;
			case DisplacementSize_Word:
				numExtensionWords++;
				break;
			case DisplacementSize_Long:
				numExtensionWords += 2;
				break;
			default:
				break;
		}
	}
	
	return numExtensionWords;
}

bool decodeEA6BitModeLength(EA6BitMode_Upper3Bits eaUpper3Bits, EA6BitMode_Lower3Bits eaLower3Bits, bool firstExtensionWordAvailable, uint16_t firstExtensionWord, OperationSize operationSize, uint* numExtensionWords)
{
	switch (eaUpper3Bits)
	{
		case EA6BitMode_Upper3Bits_Dn:
		case EA6BitMode_Upper3Bits_An:
		case EA6BitMode_Upper3Bits_Mem_An:
		case EA6BitMode_Upper3Bits_Mem_An_PostIncrement:
		case EA6BitMode_Upper3Bits_Mem_PreDecrement_An:
			*numExtensionWords = 0;
			break;
		case EA6BitMode_Upper3Bits_Mem_D16_An:
			*numExtensionWords = 1;
			break;
		case EA6BitMode_Upper3Bits_Mem_BriefOrFullExtensionWord_An:
			if (!firstExtensionWordAvailable)
				return false;
			*numExtensionWords = decodeBriefOrFullExtensionWordLength(firstExtensionWord);
			break;
		case EA6BitMode_Upper3Bits_CheckLower3Bits:
			
			switch (eaLower3Bits)
			{
				case EA6BitMode_Lower3Bits_Mem_Absolute_Word:
					*numExtensionWords = 1;
					break;
				case EA6BitMode_Lower3Bits_Mem_Absolute_Long:
					*numExtensionWords = 2;
					break;
				case EA6BitMode_Lower3Bits_Mem_D16_PC:
					*numExtensionWords = 1;
					break;
				case EA6BitMode_Lower3Bits_Mem_BriefOrFullExtensionWord_PC:
					if (!firstExtensionWordAvailable)
						return false;
					*numExtensionWords = decodeBriefOrFullExtensionWordLength(firstExtensionWord);
					break;
				case EA6BitMode_Lower3Bits_Immediate:
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
				default:
					M68060_ERROR("Invalid 6-bit EA");
			}
			break;
	}
	
	return true;
}

bool decodeOperandLength(uint16_t opWord, bool firstExtensionWordAvailable, uint16_t firstExtensionWord, EAEncoding eaEncoding, OperationSize operationSize, uint* numExtensionWords)
{
	if (eaEncoding == EAEncoding_None)
	{
		*numExtensionWords = 0;
	}
	else if (eaEncoding == EAEncoding_DefaultEALocation)
	{
		uint ea6BitMode = opWord & 0x3f;
		EA6BitMode_Upper3Bits eaUpper3Bits = (ea6BitMode >> 3) & 7;
		EA6BitMode_Lower3Bits eaLower3Bits = ea6BitMode & 7;
		bool success = decodeEA6BitModeLength(eaUpper3Bits, eaLower3Bits, firstExtensionWordAvailable, firstExtensionWord, operationSize, numExtensionWords);
		if (!success)
			return false;
	}
	else if (eaEncoding == EAEncoding_MoveDestinationEALocation)
	{
		uint ea6BitMode = (opWord >> 6) & 0x3f;
		EA6BitMode_Upper3Bits eaUpper3Bits = ea6BitMode & 7;
		EA6BitMode_Lower3Bits eaLower3Bits = (ea6BitMode >> 3) & 7;
		bool success = decodeEA6BitModeLength(eaUpper3Bits, eaLower3Bits, firstExtensionWordAvailable, firstExtensionWord, operationSize, numExtensionWords);
		if (!success)
			return false;
	}
	else if (eaEncoding == EAEncoding_Immediate)
	{
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
	}
	else if (eaEncoding == EAEncoding_D16)
	{
		*numExtensionWords = 1;
	}
	else if (eaEncoding == EAEncoding_RelativeBranch)
	{
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
	}
	else
	{
		M68060_ERROR("Unsupported EAEncoding");
	}
	return true;
}

OperationSize decodeOperationSize(uint16_t opWord, SizeEncoding sizeEncoding)
{
	switch (sizeEncoding)
	{
		case SizeEncoding_None:
			return OperationSize_Reserved;
		case SizeEncoding_Byte:
			return OperationSize_Byte;
		case SizeEncoding_Word:
			return OperationSize_Word;
		case SizeEncoding_Long:
			return OperationSize_Long;
		case SizeEncoding_DefaultOpModeEncoding:
			{
				OpMode opMode = (opWord & OpWord_OpMode_Mask) >> OpWord_OpMode_Shift;
				OperationSize operationSize;
				switch (opMode)
				{
					case OpMode_MemoryToDataRegister_Byte:
						operationSize = OperationSize_Byte;
						break;
					case OpMode_MemoryToDataRegister_Word:
						operationSize = OperationSize_Word;
						break;
					case OpMode_MemoryToDataRegister_Long:
						operationSize = OperationSize_Long;
						break;
					case OpMode_MemoryToAddressRegister_Word:
						operationSize = OperationSize_Word;
						break;
					case OpMode_DataRegisterToMemory_Byte:
						operationSize = OperationSize_Byte;
						break;
					case OpMode_DataRegisterToMemory_Word:
						operationSize = OperationSize_Word;
						break;
					case OpMode_DataRegisterToMemory_Long:
						operationSize = OperationSize_Long;
						break;
					case OpMode_MemoryToAddressRegister_Long:
						operationSize = OperationSize_Long;
						break;
					default:
						M68060_ERROR("Unsupported OpMode");
				}
				return operationSize;
			}
		case SizeEncoding_DefaultOpSizeEncoding:
			{
				OpSize opSize = (opWord & OpWord_OpSize_Mask) >> OpWord_OpSize_Shift;
				OperationSize operationSize;
				switch (opSize)
				{
					case OpSize_Byte:
						operationSize = OperationSize_Byte;
						break;
					case OpSize_Word:
						operationSize = OperationSize_Word;
						break;
					case OpSize_Long:
						operationSize = OperationSize_Long;
						break;
					default:
						M68060_ERROR("Unsupported OpSize");
				}
				return operationSize;
			}
		case SizeEncoding_RelativeBranchEncoding:
			{
				uint displacement8Bit = (opWord & 0xff);
				OperationSize operationSize;
				if (displacement8Bit == 0x00)
					operationSize = OperationSize_Word;
				else if (displacement8Bit == 0xff)
					operationSize = OperationSize_Long;
				else
					operationSize = OperationSize_Byte;
				return operationSize;
			}
		default:
			M68060_ERROR("Unsupported SizeEncoding");
			return OperationSize_Reserved;
	}
}

bool decodeInstructionLengthFromInstructionWords(const uint16_t* instructionWords, uint numInstructionWordsAvailable, InstructionLength* instructionLength)
{
	const OpWordLengthInfo* opWordLengthInfo = opWordLengthInformation;
	const OpWordClassInfo* opWordClassInfo;
	uint16_t opWord;
	OperationSize operationSize;
	uint operandOffset;

	M68060_ASSERT(numInstructionWordsAvailable > 0, "Must supply at least one instruction word");

	opWord = instructionWords[0];
	memset(instructionLength, 0, sizeof *instructionLength);

	while ((opWord & opWordLengthInfo->mask) != opWordLengthInfo->match)
	{
		opWordLengthInfo++;
	}

	if (opWordLengthInfo->mask == 0 && opWordLengthInfo->match == 0)
	{
		instructionLength->totalWords = 1;
		return true;
	}

	opWordClassInfo = &opWordClassInformation[opWordLengthInfo->class];
	
	instructionLength->numSpecialOperandSpecifierWords = opWordClassInfo->numSpecialOperandSpecifierWords;
	instructionLength->description = opWordLengthInfo->description;

	operationSize = decodeOperationSize(opWord, opWordClassInfo->sizeEncoding);
	
	operandOffset = 1 + instructionLength->numSpecialOperandSpecifierWords;

	{
		bool firstExtensionWordAvailable = (operandOffset < numInstructionWordsAvailable);
		uint16_t firstExtensionWord = (firstExtensionWordAvailable ? instructionWords[operandOffset] : 0);
		if (!decodeOperandLength(opWord, firstExtensionWordAvailable, firstExtensionWord, opWordClassInfo->sourceEAEncoding, operationSize, &instructionLength->numSourceEAExtensionWords))
			return false;
	}

	operandOffset += instructionLength->numSourceEAExtensionWords;

	{
		bool firstExtensionWordAvailable = (operandOffset < numInstructionWordsAvailable);
		uint16_t firstExtensionWord = (firstExtensionWordAvailable ? instructionWords[operandOffset] : 0);
		if (!decodeOperandLength(opWord, firstExtensionWordAvailable, firstExtensionWord, opWordClassInfo->destinationEAEncoding, operationSize, &instructionLength->numDestinationEAExtensionWords))
			return false;
	}

	instructionLength->totalWords = 1 + instructionLength->numSpecialOperandSpecifierWords
		 + instructionLength->numSourceEAExtensionWords + instructionLength->numDestinationEAExtensionWords;
	
	return true;
}
