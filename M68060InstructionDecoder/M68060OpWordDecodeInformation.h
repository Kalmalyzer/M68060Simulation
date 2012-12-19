
#ifndef M68060OPWORDDECODEINFORMATION_H
#define M68060OPWORDDECODEINFORMATION_H

#include "../Types.h"
#include "M68060EA.h"
#include "M68060InstructionDecoderTypes.h"

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
	OpWordClass_EncodedSize_SrcDn_DestDn,
	OpWordClass_EncodedSize_SrcAnPreDecrement_DestAnPreDecrement,
	OpWordClass_Move_B,
	OpWordClass_Move_W,
	OpWordClass_Move_L,
	OpWordClass_EncodedSize_SrcEaData_Dn,
	OpWordClass_EncodedSize_SrcEaAll_Dn,
	OpWordClass_EncodedSize_SrcEaAll_An,
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
	OpWordClass_SrcImm3Bit_DestEa_Alterable,
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
	bool readyForUOpDecoding;

	uint16_t mask;
	uint16_t match;

	const char* description;
	
	OpWordClass class;
	
	IeeOperation ieeOperation;
	
	Pairability pairability;
} OpWordDecodeInfo;

typedef enum
{
	DecodeOperand_None,
	DecodeOperand_DefaultEALocation,
	DecodeOperand_DefaultDnLocation,
	DecodeOperand_DefaultPreDecrementAnLocation,
	DecodeOperand_SecondaryDnLocation,
	DecodeOperand_SecondaryAnLocation,
	DecodeOperand_SecondaryPreDecrementAnLocation,
	DecodeOperand_Immediate,
	DecodeOperand_Imm3Bit,

} DecodeOperand;

typedef enum
{
	DecodeIeeResult_None,
	DecodeIeeResult_IeeA,
	DecodeIeeResult_IeeB,

} DecodeIeeResult;

typedef struct
{
	uint numSpecialOperandSpecifierWords;
	SizeEncoding sizeEncoding;
	EAEncoding sourceEAEncoding;
	EAModeMask sourceEAModeMask;
	EAEncoding destinationEAEncoding;
	EAModeMask destinationEAModeMask;

	DecodeOperand sourceDecodeOperand;
	DecodeOperand destinationDecodeOperand;
	DecodeIeeResult decodeIeeResult;
	
} OpWordClassInfo;

const OpWordClassInfo* getOpWordClassInformation(OpWordClass opWordClass);
const OpWordDecodeInfo* getOpWordDecodeInformation(uint16_t opWord);

#endif
