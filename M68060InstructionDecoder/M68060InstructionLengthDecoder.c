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

typedef struct
{
	uint16_t mask;
	uint16_t match;

	const char* mnemonic;
	uint numSpecialOperandSpecifierWords;
	SizeEncoding sizeEncoding;
	EAEncoding sourceEAEncoding;
	EAEncoding destinationEAEncoding;
	
} OpWordLengthInfo;


static OpWordLengthInfo opWordLengthInformation[] =
{

	{ 0xf0ff, 0x50fc, "TRAPcc", }, // Shadows Scc <ea>
	{ 0xf0ff, 0x50fa, "TRAPcc.W #imm", 0, SizeEncoding_Word, EAEncoding_Immediate, EAEncoding_None, }, // Shadows Scc <ea>
	{ 0xf0ff, 0x50fb, "TRAPcc.L #imm", 0, SizeEncoding_Long, EAEncoding_Immediate, EAEncoding_None, }, // Shadows Scc <ea>
	{ 0xf0f8, 0x50c8, "DBcc <relative address>", 1, SizeEncoding_Word, EAEncoding_None, EAEncoding_None, }, // Shadows Scc <ea>
	{ 0xf0c0, 0x50c0, "Scc <ea>", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, }, // Shadows ADDQ/SUBQ

	{ 0xf1f8, 0xc100, "ABCD Dx,Dy", },
	{ 0xf1f8, 0xc108, "ABCD -(Ax),-(Ay)", },
	{ 0xf138, 0xd100, "ADDX Dx,Dy", }, // Shadows ADD Dn,<ea>
	{ 0xf138, 0xd108, "ADDX -(Ax),-(Ay)", }, // Shadows ADD Dn,<ea>
	{ 0xf100, 0xd000, "ADD <ea>,Dn", 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_DefaultEALocation, EAEncoding_None, },
	{ 0xf100, 0xd100, "ADD Dn,<ea>", 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_None, EAEncoding_DefaultEALocation, },
	{ 0xff00, 0x0600, "ADDI #imm,<ea>", 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_Immediate, EAEncoding_DefaultEALocation, },
	{ 0xf100, 0x5000, "ADDQ #imm,<ea>", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, },
	{ 0xf1f8, 0xc140, "EXG Dn,Dn", }, // Shadows AND
	{ 0xf1f8, 0xc148, "EXG Am,An", }, // Shadows AND
	{ 0xf1f8, 0xc188, "EXG Dm,An", }, // Shadows AND
	{ 0xf1c0, 0xc1c0, "MULS.W <ea>,Dn", 0, SizeEncoding_Word, EAEncoding_DefaultEALocation, EAEncoding_None, }, // Shadows AND
	{ 0xf1c0, 0xc0c0, "MULU.W <ea>,Dn", 0, SizeEncoding_Word, EAEncoding_DefaultEALocation, EAEncoding_None, }, // Shadows AND
	{ 0xf100, 0xc000, "AND <ea>,Dn", 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_DefaultEALocation, EAEncoding_None, },
	{ 0xf100, 0xc100, "AND Dn,<ea>", 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_None, EAEncoding_DefaultEALocation, },

	{ 0xffff, 0x023c, "ANDI #imm,CCR", 0, SizeEncoding_Byte, EAEncoding_Immediate, EAEncoding_None, }, // Shadows ANDI #imm,<ea>
	{ 0xff00, 0x0200, "ANDI #imm,<ea>", 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_Immediate, EAEncoding_DefaultEALocation, },

	{ 0xffc0, 0xeac0, "BFCHG <ea>{Do:Dw}", 1, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, }, // Shadows ASL/ASR #imm/Dm,Dn
	{ 0xffc0, 0xeec0, "BFSET <ea>{Do:Dw}", 1, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, }, // Shadows ASL/ASR #imm/Dm,Dn

	{ 0xfec0, 0xe0c0, "ASL/ASR <ea>", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, }, // Shadows ASL/ASR #imm/Dm,Dn
	{ 0xf018, 0xe000, "ASL/ASR #imm/Dm,Dn", 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_None, EAEncoding_None, },
	
	{ 0xff00, 0x6100, "BSR <relative address>", 0, SizeEncoding_RelativeBranchEncoding, EAEncoding_RelativeBranch, EAEncoding_None, }, // Shadows Bcc
	{ 0xf000, 0x6000, "Bcc <relative address>", 0, SizeEncoding_RelativeBranchEncoding, EAEncoding_RelativeBranch, EAEncoding_None, },
	{ 0xf138, 0x0108, "MOVEP Dx <-> d16(An)", 1, SizeEncoding_None, EAEncoding_None, EAEncoding_None, }, // Shadows BCLR/BCHG Dn,<ea>
	{ 0xf1c0, 0x0140, "BCHG Dn,<ea>", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, },
	{ 0xffc0, 0x0840, "BCHG #imm,<ea>", 0, SizeEncoding_Byte, EAEncoding_Immediate, EAEncoding_DefaultEALocation, },
	{ 0xf1c0, 0x0180, "BCLR Dn,<ea>", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, },
	{ 0xffc0, 0x0880, "BCLR #imm,<ea>", 0, SizeEncoding_Byte, EAEncoding_Immediate, EAEncoding_DefaultEALocation, },
	{ 0xfff8, 0x4848, "BKPT #imm", }, // Shadows PEA
	{ 0xf1c0, 0x01c0, "BSET Dn,<ea>", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, },
	{ 0xffc0, 0x08c0, "BSET #imm,<ea>", 0, SizeEncoding_Byte, EAEncoding_Immediate, EAEncoding_DefaultEALocation, },
	{ 0xf1c0, 0x0100, "BTST Dn,<ea>", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, },
	{ 0xffc0, 0x0800, "BTST #imm,<ea>", 0, SizeEncoding_Byte, EAEncoding_Immediate, EAEncoding_DefaultEALocation, },
	{ 0xffc0, 0xecc0, "BFCLR <ea>{Do:Dw}", 1, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, },
	{ 0xffc0, 0xebc0, "BFEXTS <ea>{Do:Dw},Dn", 1, SizeEncoding_None, EAEncoding_DefaultEALocation, EAEncoding_None, },
	{ 0xffc0, 0xe9c0, "BFEXTU <ea>{Do:Dw},Dn", 1, SizeEncoding_None, EAEncoding_DefaultEALocation, EAEncoding_None, },
	{ 0xffc0, 0xedc0, "BFFFO <ea>{Do:Dw},Dn", 1, SizeEncoding_None, EAEncoding_DefaultEALocation, EAEncoding_None, },
	{ 0xffc0, 0xefc0, "BFINS Dn,<ea>{Do:Dw}", 1, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, },
	{ 0xffc0, 0xe8c0, "BFTST <ea>{Do:Dw}", 1, SizeEncoding_None, EAEncoding_DefaultEALocation, },
	{ 0xf1c0, 0x4180, "CHK <ea>,Dn", 0, SizeEncoding_Word, EAEncoding_DefaultEALocation, EAEncoding_None, },
	{ 0xf1c0, 0x4100, "CHK <ea>,Dn", 0, SizeEncoding_Long, EAEncoding_DefaultEALocation, EAEncoding_None, },

	{ 0xffc0, 0x42c0, "MOVE CCR,<ea>", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, }, // Shadows CLR

	{ 0xff00, 0x4200, "CLR <ea>", 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_None, EAEncoding_DefaultEALocation, },
	{ 0xf1c0, 0xb100, "EOR.B Dn,<ea>", 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_None, EAEncoding_DefaultEALocation, }, // Shadows CMP
	{ 0xf1c0, 0xb140, "EOR.W Dn,<ea>", 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_None, EAEncoding_DefaultEALocation, }, // Shadows CMP
	{ 0xf1c0, 0xb180, "EOR.L Dn,<ea>", 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_None, EAEncoding_DefaultEALocation, }, // Shadows CMP
	{ 0xf000, 0xb000, "CMP <ea>,Dn", 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_DefaultEALocation, EAEncoding_None, },
	{ 0xff00, 0x0c00, "CMPI #imm,<ea>", 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_Immediate, EAEncoding_DefaultEALocation, },
	{ 0xf138, 0xb108, "CMPM (Ax)+,(Ay)+", },
	{ 0xf1c0, 0x81c0, "DIVS.W <ea>,Dn", 0, SizeEncoding_Word, EAEncoding_DefaultEALocation, EAEncoding_None, },
	{ 0xffc0, 0x4c40, "DIVS/DIVU.L <ea>,Dr:Dq (can be 32bit or 64bit division)", 1, SizeEncoding_Long, EAEncoding_DefaultEALocation, EAEncoding_None, },
	{ 0xf1c0, 0x80c0, "DIVU.W <ea>,Dn", 0, SizeEncoding_Word, EAEncoding_DefaultEALocation, EAEncoding_None, },
	{ 0xff00, 0x0a00, "EORI #imm,<ea>", 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_Immediate, EAEncoding_DefaultEALocation, },
	{ 0xfff8, 0x4880, "EXT.W Dn", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_None, },
	{ 0xfff8, 0x48c0, "EXT.L Dn", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_None, },
	{ 0xfff8, 0x49c0, "EXTB.L Dn", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_None, },
	{ 0xffff, 0x4afc, "ILLEGAL", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_None, },
	{ 0xffc0, 0x4ec0, "JMP <ea>", 0, SizeEncoding_None, EAEncoding_DefaultEALocation, EAEncoding_None, },
	{ 0xffc0, 0x4e80, "JSR <ea>", 0, SizeEncoding_None, EAEncoding_DefaultEALocation, EAEncoding_None, },
	{ 0xf1c0, 0x41c0, "LEA <ea>,An", 0, SizeEncoding_None, EAEncoding_DefaultEALocation, EAEncoding_None, },
	{ 0xfff8, 0x4e50, "LINK.W An,#imm", 0, SizeEncoding_Word, EAEncoding_Immediate, EAEncoding_None, },
	{ 0xfff8, 0x4808, "LINK.L An,#imm", 0, SizeEncoding_Long, EAEncoding_Immediate, EAEncoding_None, },

	{ 0xfec0, 0xe2c0, "LSL/LSR <ea>", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, }, // Shadows LSL/LSR #imm/Dm,Dn
	{ 0xf018, 0xe008, "LSL/LSR #imm/Dm,Dn", 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_None, EAEncoding_None, },

	{ 0xf000, 0x1000, "MOVE.B <ea>,<ea>", 0, SizeEncoding_Byte, EAEncoding_DefaultEALocation, EAEncoding_MoveDestinationEALocation, },
	{ 0xf000, 0x2000, "MOVE.L <ea>,<ea>", 0, SizeEncoding_Long, EAEncoding_DefaultEALocation, EAEncoding_MoveDestinationEALocation, },
	{ 0xf000, 0x3000, "MOVE.W <ea>,<ea>", 0, SizeEncoding_Word, EAEncoding_DefaultEALocation, EAEncoding_MoveDestinationEALocation, },

	{ 0xffc0, 0x44c0, "MOVE <ea>,CCR", 0, SizeEncoding_None, EAEncoding_DefaultEALocation, EAEncoding_None, },

	{ 0xffc0, 0x40c0, "MOVE SR,<ea>", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, },

	{ 0xfff8, 0xf620, "MOVE16 (Ax)+,(Ay)+", 1, SizeEncoding_None, EAEncoding_None, EAEncoding_None, },
	{ 0xffe0, 0xf600, "MOVE16 (An <-> xxx.L)", 2, SizeEncoding_None, EAEncoding_None, EAEncoding_None, },

	{ 0xfb80, 0x4880, "MOVEM <ea> <-> reglist", 1, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, },

	{ 0xf100, 0x7000, "MOVEQ #imm,Dn", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_None, },

	{ 0xffc0, 0x4c00, "MULS/MULU.L <ea>,Dm:Dn (can be 32bit or 64bit multiply)", 1, SizeEncoding_Long, EAEncoding_DefaultEALocation, EAEncoding_None, },

	{ 0xffc0, 0x4800, "NBCD <ea>", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, },
	{ 0xff00, 0x4400, "NEG <ea>", 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_None, EAEncoding_DefaultEALocation, },
	{ 0xff00, 0x4000, "NEGX <ea>", 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_None, EAEncoding_DefaultEALocation, },

	{ 0xffff, 0x4e71, "NOP", },

	{ 0xff00, 0x4600, "NOT <ea>", 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_None, EAEncoding_DefaultEALocation, },

	{ 0xf1f8, 0x8140, "PACK Dm,Dn,#imm", 0, SizeEncoding_Word, EAEncoding_Immediate, EAEncoding_None, }, // Shadows OR/ORI
	{ 0xf1f8, 0x8148, "PACK -(Am),-(An),#imm", 0, SizeEncoding_Word, EAEncoding_Immediate, EAEncoding_None, }, // Shadows OR/ORI
	{ 0xf1f8, 0x8180, "UNPK Dm,Dn,#imm", 0, SizeEncoding_Word, EAEncoding_Immediate, EAEncoding_None, }, // Shadows OR/ORI
	{ 0xf1f8, 0x8188, "UNPK -(Am),-(An),#imm", 0, SizeEncoding_Word, EAEncoding_Immediate, EAEncoding_None, }, // Shadows OR/ORI

	{ 0xf100, 0x8000, "OR <ea>,Dn", 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_DefaultEALocation, EAEncoding_None, },
	{ 0xf100, 0x8100, "OR Dn,<ea>", 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_None, EAEncoding_DefaultEALocation, },

	{ 0xffff, 0x003c, "ORI #imm,CCR", 0, SizeEncoding_Byte, EAEncoding_Immediate, EAEncoding_None, }, // Shadows ANDI #imm,<ea>
	{ 0xff00, 0x0000, "ORI #imm,<ea>", 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_Immediate, EAEncoding_DefaultEALocation, },

	{ 0xfff8, 0x4840, "SWAP Dn", }, // Shadows PEA

	{ 0xffc0, 0x4840, "PEA <ea>", 0, SizeEncoding_None, EAEncoding_DefaultEALocation, EAEncoding_None, },

	{ 0xfec0, 0xe6c0, "ROL/ROR <ea>", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, }, // Shadows ROL/ROR #imm/Dm,Dn
	{ 0xf018, 0xe018, "ROL/ROR #imm/Dm,Dn", 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_None, EAEncoding_None, },
	
	{ 0xfec0, 0xe4c0, "ROXL/ROXR <ea>", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, }, // Shadows ROXL/ROXR #imm/Dm,Dn
	{ 0xf018, 0xe010, "ROXL/ROXR #imm/Dm,Dn", 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_None, EAEncoding_None, },

	{ 0xffff, 0x4e77, "RTR", },
	{ 0xffff, 0x4e75, "RTS", },

	{ 0xf1f8, 0x8100, "SBCD Dx,Dy", },
	{ 0xf1f8, 0x8108, "SBCD -(Ax),-(Ay)", },

	{ 0xf138, 0x9100, "SUBX Dx,Dy", }, // Shadows SUB Dn,<e>
	{ 0xf138, 0x9108, "SUBX -(Ax),-(Ay)", }, // Shadows SUB Dn,<e>
	{ 0xf100, 0x9000, "SUB <ea>,Dn", 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_DefaultEALocation, EAEncoding_None, },
	{ 0xf100, 0x9100, "SUB Dn,<ea>", 0, SizeEncoding_DefaultOpModeEncoding, EAEncoding_None, EAEncoding_DefaultEALocation, },
	{ 0xff00, 0x0400, "SUBI #imm,<ea>", 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_Immediate, EAEncoding_DefaultEALocation, },
	{ 0xf100, 0x5100, "SUBQ #imm,<ea>", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, },

	{ 0xffc0, 0x4ac0, "TAS <ea>", 0, SizeEncoding_None, EAEncoding_None, EAEncoding_DefaultEALocation, },

	{ 0xfff8, 0x4e40, "TRAP #imm", },
	{ 0xffff, 0x4e76, "TRAPV", },

	{ 0xff00, 0x4a00, "TST <ea>", 0, SizeEncoding_DefaultOpSizeEncoding, EAEncoding_None, EAEncoding_DefaultEALocation, },

	{ 0xfff8, 0x4e58, "UNLK An", },

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

	instructionLength->numSpecialOperandSpecifierWords = opWordLengthInfo->numSpecialOperandSpecifierWords;
	instructionLength->mnemonic = opWordLengthInfo->mnemonic;

	operationSize = decodeOperationSize(opWord, opWordLengthInfo->sizeEncoding);
	
	operandOffset = 1 + instructionLength->numSpecialOperandSpecifierWords;

	{
		bool firstExtensionWordAvailable = (operandOffset < numInstructionWordsAvailable);
		uint16_t firstExtensionWord = (firstExtensionWordAvailable ? instructionWords[operandOffset] : 0);
		if (!decodeOperandLength(opWord, firstExtensionWordAvailable, firstExtensionWord, opWordLengthInfo->sourceEAEncoding, operationSize, &instructionLength->numSourceEAExtensionWords))
			return false;
	}

	operandOffset += instructionLength->numSourceEAExtensionWords;

	{
		bool firstExtensionWordAvailable = (operandOffset < numInstructionWordsAvailable);
		uint16_t firstExtensionWord = (firstExtensionWordAvailable ? instructionWords[operandOffset] : 0);
		if (!decodeOperandLength(opWord, firstExtensionWordAvailable, firstExtensionWord, opWordLengthInfo->destinationEAEncoding, operationSize, &instructionLength->numDestinationEAExtensionWords))
			return false;
	}

	instructionLength->totalWords = 1 + instructionLength->numSpecialOperandSpecifierWords
		 + instructionLength->numSourceEAExtensionWords + instructionLength->numDestinationEAExtensionWords;
	
	return true;
}
