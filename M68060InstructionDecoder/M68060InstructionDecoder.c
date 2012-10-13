

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

void decodeInstructionLengths(uint8_t* instructionBytes, Decoded)
{
	uint16_t* instructionWords = (uint16_t*) instructionBytes;
	
	uint16_t opWord0 = *instructionWords++;

	preDecodeInstruction(opWord0);
	
}