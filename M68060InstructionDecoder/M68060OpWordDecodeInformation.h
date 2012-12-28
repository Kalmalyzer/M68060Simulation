
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
	EAEncoding_Immediate_Byte,
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
	OpWordClass_Long_SrcImm8Bit_DestDn,
	OpWordClass_Byte_DestDnReadWrite,
	OpWordClass_Word_DestDnReadWrite,
	OpWordClass_Long_DestDnReadWrite,
	OpWordClass_EncodedSize_SrcDn_DestDn,
	OpWordClass_EncodedSize_SrcDn_DestDn_2,
	OpWordClass_EncodedSize_SrcAnPostIncrement_DestAnPostIncrementReadOnly,
	OpWordClass_EncodedSize_SrcAnPreDecrement_DestAnPreDecrement,
	OpWordClass_Move_B,
	OpWordClass_Move_W,
	OpWordClass_Move_L,
	OpWordClass_EncodedSize_SrcEaData_Dn,
	OpWordClass_EncodedSize_SrcEaAll_DnReadOnly,
	OpWordClass_EncodedSize_SrcEaAll_DnReadWrite,
	OpWordClass_EncodedSize_SrcEaAll_AnReadOnly,
	OpWordClass_EncodedSize_SrcEaAll_AnReadWrite,
	OpWordClass_LongMulDiv,
	OpWordClass_Bitfield_ReadEa,
	OpWordClass_Control,
	OpWordClass_SrcEaReference_DestAnAguResult,
	OpWordClass_Byte_SrcEa,
	OpWordClass_Word_SrcEa,
	OpWordClass_Long_SrcEa,
	OpWordClass_Byte_SrcImm_DestEa_DataAlterable_ReadOnly,
	OpWordClass_Long_SrcImmByte_DestEa_DataAlterable_ReadOnly,
	OpWordClass_Byte_SrcImm_DestEa_DataAlterable_ReadWrite,
	OpWordClass_Long_SrcImmByte_DestEa_DataAlterable_ReadWrite,
	OpWordClass_EncodedSize_Imm_Ea_ReadOnly,
	OpWordClass_EncodedSize_Imm_Ea_ReadWrite,
	OpWordClass_EncodedSize_Rn_Ea_1,
	OpWordClass_EncodedSize_Rn_Ea_2,
	OpWordClass_EncodedSize_SrcEaAll_ReadOnly,
	OpWordClass_EncodedSize_DestEa_ReadWrite,
	OpWordClass_EncodedSize_DestEa_WriteOnly_If_WholeOperand,
	OpWordClass_SrcImm3Bit_DestEa_Alterable,
	OpWordClass_Byte_SrcDn_DestEa_Data_ReadOnly,
	OpWordClass_Long_SrcDn_DestEa_Data_ReadOnly,
	OpWordClass_DestEa_DataAlterable,
	OpWordClass_Byte_SrcDn_DestEa_DataAlterable_ReadWrite,
	OpWordClass_Long_SrcDn_DestEa_DataAlterable_ReadWrite,
	OpWordClass_Word_SrcImmValue1_DestEa_MemoryAlterable,
	OpWordClass_EncodedSize_SrcImm3Bit_DestDn,
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
	DecodeOperand_DefaultEAReferenceLocation,
	DecodeOperand_DefaultDnLocation,
	DecodeOperand_DefaultPostIncrementAnLocation,
	DecodeOperand_DefaultPreDecrementAnLocation,
	DecodeOperand_SecondaryDnLocation,
	DecodeOperand_SecondaryAnLocation,
	DecodeOperand_SecondaryPostIncrementAnLocation,
	DecodeOperand_SecondaryPreDecrementAnLocation,
	DecodeOperand_SecondaryAnAguResultLocation,
	DecodeOperand_MoveDestinationEALocation,
	DecodeOperand_Immediate,
	DecodeOperand_Immediate_ExtensionWords_Byte,
	DecodeOperand_Imm3Bit,
	DecodeOperand_Imm3BitValue1,
	DecodeOperand_Imm8Bit,

} DecodeOperand;

typedef enum
{
	DestinationOperandAccessType_None,
	DestinationOperandAccessType_ReadOnly,
	DestinationOperandAccessType_ReadWrite,
	DestinationOperandAccessType_WriteOnly_If_WholeOperand,
	DestinationOperandAccessType_WriteOnly,

} DestinationOperandAccessType;

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
	DestinationOperandAccessType destinationOperandAccessType;
	
} OpWordClassInfo;

const OpWordClassInfo* getOpWordClassInformation(OpWordClass opWordClass);
const OpWordDecodeInfo* getOpWordDecodeInformation(uint16_t opWord);

#endif
