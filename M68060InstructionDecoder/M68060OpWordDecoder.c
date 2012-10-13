
#include "M68060OpWordDecoder.h"

#include "../Types.h"
#include "../Assert.h"
#include "M68060InstructionDecoderTypes.h"

typedef enum
{
	OperandBehavior_Unknown,
	OperandBehavior_Read_EAOperand_ReadWrite_DnOperand,
	OperandBehavior_Read_DnOperand_ReadWrite_EAOperand,

} OperandBehavior;

typedef struct 
{
	uint16_t mask;
	uint16_t match;

	const char* mnemonic;
	PairingType standardInstructionPairingType;
	OperandBehavior operandBehavior;

} OpWordDecodeInfo;

static OpWordDecodeInfo opWordDecodeInformation[] =
{
	{ 0xf1f8, 0xc100, "ABCD Rx,Ry", PairingType_pOEP_Only },
	{ 0xf1f8, 0xc108, "ABCD -(Ax),-(Ay)", PairingType_pOEP_Only },
	{ 0xd000, 0xf100, "ADD <ea>,Dn", PairingType_pOEP_Or_sOEP, OperandBehavior_Read_EAOperand_ReadWrite_DnOperand },
	{ 0xd100, 0xf100, "ADD Dn,<ea>", PairingType_pOEP_Or_sOEP, OperandBehavior_Read_DnOperand_ReadWrite_EAOperand },
	{ 0, 0, "Unknown instruction", PairingType_pOEP_But_Allows_sOEP },
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
	EA6BitMode_Lower3Bits_Mem_Immediate,

} EA6BitMode_Lower3Bits;

void decodeEA6BitResources(uint16_t operationWord, ExecutionResource* aguBase, ExecutionResource* aguIndex, bool* hasMemoryReference, ExecutionResource* iee)
{
	uint16_t upper3Bits = (operationWord >> 3) & 7;
	uint16_t lower3Bits = operationWord & 7;

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
				default:
					M68060_ERROR("Not implemented");
			}
	}
}

ExecutionResource decodeRegisterDnResource(uint16_t operationWord)
{
	return (ExecutionResource) (ExecutionResource_D0 + (operationWord & 7));
}


void decodeOpWord(uint16_t operationWord)
{
	int i;
	OpWordDecodeInfo* opWordDecodeInfo = opWordDecodeInformation;
	ExecutionResource aguBase, aguIndex, ieeA, ieeB;
	bool hasMemoryReference;

	while ((operationWord & opWordDecodeInfo->mask) != opWordDecodeInfo->match)
		opWordDecodeInfo++;

	switch (opWordDecodeInfo->operandBehavior)
	{
		case OperandBehavior_Read_EAOperand_ReadWrite_DnOperand:
			decodeEA6BitResources(operationWord, &aguBase, &aguIndex, &hasMemoryReference, &ieeA);
			ieeB = decodeRegisterDnResource(operationWord >> 9);
			break;
		case OperandBehavior_Read_DnOperand_ReadWrite_EAOperand:
			decodeEA6BitResources(operationWord, &aguBase, &aguIndex, &hasMemoryReference, &ieeB);
			ieeA = decodeRegisterDnResource(operationWord >> 9);
			break;
		default:
			M68060_ERROR("OperandBehavior not supported");
	}
}
