
#include "M68060OpWordDecoder.h"

#include "../Types.h"
#include "../Assert.h"
#include "M68060InstructionDecoderTypes.h"

typedef enum
{
	OperandBehavior_None,
	OperandBehavior_Read_EAOperand_ReadWrite_DnOperand,
	OperandBehavior_Read_DnOperand_ReadWrite_EAOperand,

} OperandBehavior;

typedef enum
{
	SizeEncoding_None,
	SizeEncoding_OpMode,

} SizeEncoding;

typedef struct 
{
	uint16_t mask;
	uint16_t match;

	const char* mnemonic;
	PairingType standardInstructionPairingType;
	OperandBehavior operandBehavior;
	SizeEncoding sizeEncoding;

} OpWordDecodeInfo;

static OpWordDecodeInfo opWordDecodeInformation[] =
{
	{ 0xf1f8, 0xc100, "ABCD Rx,Ry", PairingType_pOEP_Only, OperandBehavior_None, SizeEncoding_None },
	{ 0xf1f8, 0xc108, "ABCD -(Ax),-(Ay)", PairingType_pOEP_Only, OperandBehavior_None, SizeEncoding_None },
	{ 0xf100, 0xd000, "ADD <ea>,Dn", PairingType_pOEP_Or_sOEP, OperandBehavior_Read_EAOperand_ReadWrite_DnOperand, SizeEncoding_OpMode },
	{ 0xf100, 0xd100, "ADD Dn,<ea>", PairingType_pOEP_Or_sOEP, OperandBehavior_Read_DnOperand_ReadWrite_EAOperand, SizeEncoding_OpMode },
	{ 0, 0, "Unknown instruction", PairingType_pOEP_But_Allows_sOEP, OperandBehavior_None, SizeEncoding_None },
};

typedef enum
{
	EA6BitMode_Upper3Bits_Dn,
	EA6BitMode_Upper3Bits_An,
	EA6BitMode_Upper3Bits_Mem_An,
	EA6BitMode_Upper3Bits_Mem_An_PostIncrement,
	EA6BitMode_Upper3Bits_Mem_PreDecrement_An,
	EA6BitMode_Upper3Bits_Mem_D16_An,
	EA6BitMode_Upper3Bits_Mem_D8_An_Xn,
	EA6BitMode_Upper3Bits_CheckLower3Bits,

} EA6BitMode_Upper3Bits;

typedef enum
{
	EA6BitMode_Lower3Bits_Mem_Absolute_Word,
	EA6BitMode_Lower3Bits_Mem_Absolute_Long,
	EA6BitMode_Lower3Bits_Mem_D16_PC,
	EA6BitMode_Lower3Bits_Mem_D8_PC_Xn,
	EA6BitMode_Lower3Bits_Immediate,

} EA6BitMode_Lower3Bits;


OpMode decodeOpMode(uint16_t opModeBits)
{
	static OpMode opModes[] =
	{
		OpMode_EaToRegister_Data_Byte,
		OpMode_EaToRegister_Data_Word,
		OpMode_EaToRegister_Data_Long,
		OpMode_EaToRegister_Address_WordWithSignExtension,
		OpMode_RegisterToEa_Data_Byte,
		OpMode_RegisterToEa_Data_Word,
		OpMode_RegisterToEa_Data_Long,
		OpMode_RegisterToEa_Address_Long,
	};

	return opModes[opModeBits & 7];
}

void decodeEA6BitResources(uint16_t resourceBits, ExecutionResource* aguBase, ExecutionResource* aguIndex, bool* hasMemoryReference, ExecutionResource* iee)
{
	uint16_t upper3Bits = (resourceBits >> 3) & 7;
	uint16_t lower3Bits = resourceBits & 7;

	*aguBase = ExecutionResource_None;
	*aguIndex = ExecutionResource_None;
	*hasMemoryReference = false;
	*iee = ExecutionResource_None;
	
	switch (upper3Bits)
	{
		case EA6BitMode_Upper3Bits_Dn:
			*iee = (ExecutionResource) (ExecutionResource_D0 + lower3Bits);
			return;
		case EA6BitMode_Upper3Bits_An:
			*iee = (ExecutionResource) (ExecutionResource_A0 + lower3Bits);
			return;
		case EA6BitMode_Upper3Bits_Mem_An:
		case EA6BitMode_Upper3Bits_Mem_An_PostIncrement:
		case EA6BitMode_Upper3Bits_Mem_PreDecrement_An:
		case EA6BitMode_Upper3Bits_Mem_D16_An:
			*aguBase = (ExecutionResource) (ExecutionResource_A0 + lower3Bits);
			*hasMemoryReference = true;
			*iee = ExecutionResource_AguResult;
			return;
		case EA6BitMode_Upper3Bits_Mem_D8_An_Xn:
			*aguBase = (ExecutionResource) (ExecutionResource_A0 + lower3Bits);
			*hasMemoryReference = true;
			// TODO: add Xn reference
			*iee = ExecutionResource_AguResult;
			return;
		case EA6BitMode_Upper3Bits_CheckLower3Bits:
			switch (lower3Bits)
			{
				case EA6BitMode_Lower3Bits_Mem_Absolute_Word:
					*aguBase = ExecutionResource_MemoryOperand;
					*hasMemoryReference = true;
					*iee = ExecutionResource_AguResult;
					return;
				case EA6BitMode_Lower3Bits_Mem_Absolute_Long:
					*aguBase = ExecutionResource_MemoryOperand;
					*hasMemoryReference = true;
					*iee = ExecutionResource_AguResult;
					return;
				case EA6BitMode_Lower3Bits_Mem_D16_PC:
					*aguBase = ExecutionResource_PC;
					*hasMemoryReference = true;
					*iee = ExecutionResource_AguResult;
					return;
				case EA6BitMode_Lower3Bits_Mem_D8_PC_Xn:
					*aguBase = ExecutionResource_PC;
					// TODO: Add Xn reference
					*hasMemoryReference = true;
					*iee = ExecutionResource_AguResult;
					return;
				case EA6BitMode_Lower3Bits_Immediate:
					*hasMemoryReference = false;
					*iee = ExecutionResource_ImmediateOperand;
					return;
				default:
					M68060_ERROR("EA6BitDecoder case not implemented");
			}
	}
}

ExecutionResource decodeRegisterDnResource(uint16_t resourceBits)
{
	return (ExecutionResource) (ExecutionResource_D0 + (resourceBits & 7));
}


DecodedOpWord decodeOpWord(uint16_t operationWord)
{
	int i;
	const OpWordDecodeInfo* opWordDecodeInfo = opWordDecodeInformation;
	DecodedOpWord decodedOpWord;
	bool hasMemoryReference;

	decodedOpWord.mnemonic = "Unknown instruction";
	decodedOpWord.aguBase = ExecutionResource_None;
	decodedOpWord.aguIndex = ExecutionResource_None;
	decodedOpWord.ieeA = ExecutionResource_None;
	decodedOpWord.ieeB = ExecutionResource_None;
	decodedOpWord.opMode = OpMode_None;
	
	while ((operationWord & opWordDecodeInfo->mask) != opWordDecodeInfo->match)
	{
		opWordDecodeInfo++;
	}

	if (opWordDecodeInfo->mask == 0 && opWordDecodeInfo->match == 0)
		return decodedOpWord;
		
	decodedOpWord.mnemonic = opWordDecodeInfo->mnemonic;

	switch (opWordDecodeInfo->operandBehavior)
	{
		case OperandBehavior_None:
			break;
		case OperandBehavior_Read_EAOperand_ReadWrite_DnOperand:
			decodeEA6BitResources(operationWord, &decodedOpWord.aguBase, &decodedOpWord.aguIndex, &hasMemoryReference, &decodedOpWord.ieeA);
			decodedOpWord.ieeB = decodeRegisterDnResource(operationWord >> 9);
			break;
		case OperandBehavior_Read_DnOperand_ReadWrite_EAOperand:
			decodeEA6BitResources(operationWord, &decodedOpWord.aguBase, &decodedOpWord.aguIndex, &hasMemoryReference, &decodedOpWord.ieeB);
			decodedOpWord.ieeA = decodeRegisterDnResource(operationWord >> 9);
			break;
		default:
			M68060_ERROR("OperandBehavior not implemented");
	}

	switch (opWordDecodeInfo->sizeEncoding)
	{
		case SizeEncoding_None:
			break;
		case SizeEncoding_OpMode:
			decodedOpWord.opMode = decodeOpMode(operationWord >> 6);
			break;
		default:
			M68060_ERROR("SizeEncoding not implemented");
	}
	
	return decodedOpWord;
}
