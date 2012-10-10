
enum
{
	ExecutionResource_D0,
	ExecutionResource_D1,
	ExecutionResource_D2,
	ExecutionResource_D3,
	ExecutionResource_D4,
	ExecutionResource_D5,
	ExecutionResource_D6,
	ExecutionResource_D7,
	ExecutionResource_A0,
	ExecutionResource_A1,
	ExecutionResource_A2,
	ExecutionResource_A3,
	ExecutionResource_A4,
	ExecutionResource_A5,
	ExecutionResource_A6,
	ExecutionResource_A7,
	ExecutionResource_aguResult,
	ExecutionResource_MemoryOperand,
	ExecutionResource_ImmediateOperand,
	ExecutionResource_PC,
};

enum PairingType
{
	PairingType_pOEP_Or_sOEP,
	PairingType_pOEP_But_Allows_sOEP,
	PairingType_pOEP_Only,
	PairingType_pOEP_Until_Last
};

struct DecodedInstruction
{
	enum { MaxLength = 16 };
	int length;
	uint8_t instruction[MaxLength];
};

bool pairingTypesAllowDualIssue(PairingType pairingType1, PairingType pairingType2)
{
	if (pairingType2 != PairingType_pOEP_Or_sOEP)
		return false;

	if (pairingType1 == PairingType_pOEP_Only)
		return false;
		
	return true;
}

struct DecodedInstructions
{
	enum { MaxDecodedInstructions = 2 };

	int numDecodedInstructions;
	DecodedInstruction decodedInstructions[MaxDecodedInstructions];
};

enum OperandBehavior
{
	OperandBehavior_Unknown,
	OperandBehavior_Read_EAOperand_ReadWrite_DnOperand,
	OperandBehavior_Read_DnOperand_ReadWrite_EAOperand,
};

PreDecodeInstructionInfo
{
	uint16_t mask;
	uint16_t match;

	const char* mnemonic;
	PairingType standardInstructionPairingType;
	OperandBehavior operandBehavior;
};

static PreDecodeInstructionInfo preDecodeInformation[] =
{
	{ 0xf1f8, 0xc100, "ABCD Rx,Ry", PairingType_pOEP_Only },
	{ 0xf1f8, 0xc108, "ABCD -(Ax),-(Ay)", PairingType_pOEP_Only },
	{ 0xd000, 0xf100, "ADD <ea>,Dn", PairingType_pOEP_Or_sOEP, OperandBehavior_Read_EAOperand_ReadWrite_DnOperand },
	{ 0xd100, 0xf100, "ADD Dn,<ea>", PairingType_pOEP_Or_sOEP, OperandBehavior_Read_DnOperand_ReadWrite_EAOperand },
	{ 0, 0, "Unknown instruction", PairingType_pOEP_But_Allows_sOEP },
};

enum EA6BitMode_Upper3Bits
{
	EA6BitMode_Upper3Bits_Dn,
	EA6BitMode_Upper3Bits_An,
	EA6BitMode_Upper3Bits_Mem_An,
	EA6BitMode_Upper3Bits_Mem_An_PostIncrement,
	EA6BitMode_Upper3Bits_Mem_PreDecrement_An,
	EA6BitMode_Upper3Bits_Mem_D16_An,
	EA6BitMode_Upper3Bits_Mem_D8_An_Xn,
	EA6BitMode_Upper3Bits_CheckLower3Bits
};

enum EA6BitMode_Lower3Bits
{
	EA6BitMode_Lower3Bits_Mem_Absolute_Word,
	EA6BitMode_Lower3Bits_Mem_Absolute_Long,
	EA6BitMode_Lower3Bits_Mem_D16_PC,
	EA6BitMode_Lower3Bits_Mem_D8_PC_Xn,
	EA6BitMode_Lower3Bits_Mem_Immediate
}

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
		case EA6BitMode_Upper3Bits_Mem_CheckLower3Bits:
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


void preDecodeInstruction(uint16_t operationWord)
{
	int i;
	PreDecodeInstructionInfo* preDecodeInfo = preDecodeInformation;
	ExecuteResource aguBase, aguIndex, ieeA, ieeB;
	bool hasMemoryReference;

	while ((operationWord & preDecodeInfo->mask) != preDecodeInfo->match)
		preDecodeInfo++;

	switch (preDecodeInfo->operandBehavior)
	{
		case OperandBehavior_Read_EAOperand_ReadWrite_DnOperand:
			decodeEA6BitResources(operationWord, &aguBase, &aguIndex, &hasMemoryReference, &ieeA);
			ieeB = decodeRegisterDnResource(operationWord >> 9);
			break;
		case OperandBehavior_Read_DnOperand_ReadWrite_EaOperand:
			decodeEA6BitResources(operationWord, &aguBase, &aguIndex, &hasMemoryReference, &ieeB);
			ieeA = decodeRegisterDnResource(operationWord >> 9);
			break;
		default:
			;
	}

}

void decodeInstructionLengths(uint8_t* instructionBytes, Decoded)
{
	uint16_t* instructionWords = (uint16_t*) instructionBytes;
	
	uint16_t opWord0 = *instructionWords++;

	preDecodeInstruction(opWord0);
	
}