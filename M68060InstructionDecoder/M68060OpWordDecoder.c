
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
		OpMode_EaToRegister_Address_Long,
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
			*iee = ExecutionResource_MemoryOperand;
			return;
		case EA6BitMode_Upper3Bits_Mem_D8_An_Xn:
			*aguBase = (ExecutionResource) (ExecutionResource_A0 + lower3Bits);
			*hasMemoryReference = true;
			*aguIndex = ExecutionResource_Unknown;
			*iee = ExecutionResource_MemoryOperand;
			return;
		case EA6BitMode_Upper3Bits_CheckLower3Bits:
			switch (lower3Bits)
			{
				case EA6BitMode_Lower3Bits_Mem_Absolute_Word:
					*aguBase = ExecutionResource_AbsoluteMemoryAddress;
					*hasMemoryReference = true;
					*iee = ExecutionResource_MemoryOperand;
					return;
				case EA6BitMode_Lower3Bits_Mem_Absolute_Long:
					*aguBase = ExecutionResource_AbsoluteMemoryAddress;
					*hasMemoryReference = true;
					*iee = ExecutionResource_MemoryOperand;
					return;
				case EA6BitMode_Lower3Bits_Mem_D16_PC:
					*aguBase = ExecutionResource_PC;
					*hasMemoryReference = true;
					*iee = ExecutionResource_MemoryOperand;
					return;
				case EA6BitMode_Lower3Bits_Mem_D8_PC_Xn:
					*aguBase = ExecutionResource_PC;
					*aguIndex = ExecutionResource_Unknown;
					*hasMemoryReference = true;
					*iee = ExecutionResource_MemoryOperand;
					return;
				case EA6BitMode_Lower3Bits_Immediate:
					*hasMemoryReference = false;
					*iee = ExecutionResource_ImmediateOperand;
					return;
			}
	}

	M68060_ERROR("EA6BitResourceDecoder case not implemented");
}

AguOperation decodeEA6BitAguOperation(uint16_t resourceBits)
{
	uint16_t upper3Bits = (resourceBits >> 3) & 7;
	uint16_t lower3Bits = resourceBits & 7;

	switch (upper3Bits)
	{
		case EA6BitMode_Upper3Bits_Dn:
		case EA6BitMode_Upper3Bits_An:
			return AguOperation_None;
		case EA6BitMode_Upper3Bits_Mem_An:
			return AguOperation_OffsetBaseIndexScale;
		case EA6BitMode_Upper3Bits_Mem_An_PostIncrement:
			return AguOperation_PostIncrement;
		case EA6BitMode_Upper3Bits_Mem_PreDecrement_An:
			return AguOperation_PreDecrement;
		case EA6BitMode_Upper3Bits_Mem_D16_An:
		case EA6BitMode_Upper3Bits_Mem_D8_An_Xn:
			return AguOperation_OffsetBaseIndexScale;
		case EA6BitMode_Upper3Bits_CheckLower3Bits:
			switch (lower3Bits)
			{
				case EA6BitMode_Lower3Bits_Mem_Absolute_Word:
				case EA6BitMode_Lower3Bits_Mem_Absolute_Long:
				case EA6BitMode_Lower3Bits_Mem_D16_PC:
				case EA6BitMode_Lower3Bits_Mem_D8_PC_Xn:
					return AguOperation_OffsetBaseIndexScale;
				case EA6BitMode_Lower3Bits_Immediate:
					return AguOperation_None;
			}
	}

	M68060_ERROR("EA6BitAguOperationDecoder case not implemented");
	return AguOperation_None;
}

AguOffset decodeEA6BitAguOffset(uint16_t resourceBits)
{
	uint16_t upper3Bits = (resourceBits >> 3) & 7;
	uint16_t lower3Bits = resourceBits & 7;

	switch (upper3Bits)
	{
		case EA6BitMode_Upper3Bits_Dn:
		case EA6BitMode_Upper3Bits_An:
		case EA6BitMode_Upper3Bits_Mem_An:
		case EA6BitMode_Upper3Bits_Mem_An_PostIncrement:
		case EA6BitMode_Upper3Bits_Mem_PreDecrement_An:
			return AguOffset_None;
		case EA6BitMode_Upper3Bits_Mem_D16_An:
			return AguOffset_D16;
		case EA6BitMode_Upper3Bits_Mem_D8_An_Xn:
			return AguOffset_D8;
		case EA6BitMode_Upper3Bits_CheckLower3Bits:
			switch (lower3Bits)
			{
				case EA6BitMode_Lower3Bits_Mem_Absolute_Word:
					return AguOffset_D16;
				case EA6BitMode_Lower3Bits_Mem_Absolute_Long:
					return AguOffset_D32;
				case EA6BitMode_Lower3Bits_Mem_D16_PC:
					return AguOffset_D16;
				case EA6BitMode_Lower3Bits_Mem_D8_PC_Xn:
					return AguOffset_D8;
				case EA6BitMode_Lower3Bits_Immediate:
					return AguOffset_None;
			}
	}

	M68060_ERROR("EA6BitAguOffsetDecoder case not implemented");
	return AguOffset_None;
}

IeeImmediate decodeEA6BitIeeImmediate(uint16_t resourceBits, OpMode opMode)
{
	uint16_t upper3Bits = (resourceBits >> 3) & 7;
	uint16_t lower3Bits = resourceBits & 7;

	if (upper3Bits == EA6BitMode_Upper3Bits_CheckLower3Bits && lower3Bits == EA6BitMode_Lower3Bits_Immediate)
	{
		switch (opMode)
		{
			case OpMode_EaToRegister_Data_Byte:
				return IeeImmediate_D8;
			case OpMode_EaToRegister_Data_Word:
				return IeeImmediate_D16;
			case OpMode_EaToRegister_Data_Long:
				return IeeImmediate_D32;
			case OpMode_EaToRegister_Address_WordWithSignExtension:
				return IeeImmediate_D16;
			case OpMode_RegisterToEa_Data_Byte:
			case OpMode_RegisterToEa_Data_Word:
			case OpMode_RegisterToEa_Data_Long:
				break; // Register-to-EA does not support #imm source operands
			case OpMode_EaToRegister_Address_Long:
				return IeeImmediate_D32;
		}

		M68060_ERROR("EA6BitIeeImmediateDecoder case not implemented");
		return IeeImmediate_None;
	}
	else
		return IeeImmediate_None;
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

	decodedOpWord.mnemonic = "Unknown instruction";
	decodedOpWord.aguBase = ExecutionResource_None;
	decodedOpWord.aguIndex = ExecutionResource_None;
	decodedOpWord.ieeA = ExecutionResource_None;
	decodedOpWord.ieeB = ExecutionResource_None;
	decodedOpWord.hasMemoryReference = false;
	decodedOpWord.opMode = OpMode_None;
	decodedOpWord.aguOperation = AguOperation_None;
	decodedOpWord.aguOffset = AguOffset_None;
	decodedOpWord.ieeImmediate = IeeImmediate_None;
	
	while ((operationWord & opWordDecodeInfo->mask) != opWordDecodeInfo->match)
	{
		opWordDecodeInfo++;
	}

	if (opWordDecodeInfo->mask == 0 && opWordDecodeInfo->match == 0)
		return decodedOpWord;
		
	decodedOpWord.mnemonic = opWordDecodeInfo->mnemonic;

	switch (opWordDecodeInfo->sizeEncoding)
	{
		case SizeEncoding_None:
			break;
		case SizeEncoding_OpMode:
			decodedOpWord.opMode = decodeOpMode(operationWord >> 6);
			break;
		default:
			M68060_ERROR("SizeEncoding case not implemented");
	}

	switch (opWordDecodeInfo->operandBehavior)
	{
		case OperandBehavior_None:
			break;
		case OperandBehavior_Read_EAOperand_ReadWrite_DnOperand:
			decodeEA6BitResources(operationWord, &decodedOpWord.aguBase, &decodedOpWord.aguIndex, &decodedOpWord.hasMemoryReference, &decodedOpWord.ieeA);
			decodedOpWord.ieeB = decodeRegisterDnResource(operationWord >> 9);

			decodedOpWord.aguOperation = decodeEA6BitAguOperation(operationWord);
			decodedOpWord.aguOffset = decodeEA6BitAguOffset(operationWord);
			decodedOpWord.ieeImmediate = decodeEA6BitIeeImmediate(operationWord, decodedOpWord.opMode);
			break;
		case OperandBehavior_Read_DnOperand_ReadWrite_EAOperand:
			decodeEA6BitResources(operationWord, &decodedOpWord.aguBase, &decodedOpWord.aguIndex, &decodedOpWord.hasMemoryReference, &decodedOpWord.ieeB);
			decodedOpWord.ieeA = decodeRegisterDnResource(operationWord >> 9);

			decodedOpWord.aguOperation = decodeEA6BitAguOperation(operationWord);
			decodedOpWord.aguOffset = decodeEA6BitAguOffset(operationWord);
			decodedOpWord.ieeImmediate = decodeEA6BitIeeImmediate(operationWord, decodedOpWord.opMode);
			break;
		default:
			M68060_ERROR("OperandBehavior case not implemented");
	}


	return decodedOpWord;
}
