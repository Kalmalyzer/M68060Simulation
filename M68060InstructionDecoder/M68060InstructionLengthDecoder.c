#include "M68060InstructionLengthDecoder.h"
#include "../Assert.h"

#include "M68060EA.h"
#include "M68060InstructionDecoderTypes.h"
#include "M68060OpWord.h"
#include "M68060OpWordDecodeInformation.h"

//Define this if you want decoding of some illegal EAs to not outright reject the instruction.
// but to ignore any base/outer displacements instead; this is what Musashi seems to do.
//#define MUSASHI_COMPATIBILITY_MODE


static bool decodeBriefOrFullExtensionWordLength(uint16_t firstExtensionWord, uint* numExtensionWords_)
{
	uint numExtensionWords = 1;
		
	if (firstExtensionWord & ExtensionWord_FullWord_Mask)
	{
		DisplacementSize baseDisplacementSize = (firstExtensionWord & FullExtensionWord_BaseDisplacementSize_Mask) >> FullExtensionWord_BaseDisplacementSize_Shift;
		IS_IIS is_iis = (firstExtensionWord & FullExtensionWord_IS_IIS_Mask);

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
			case DisplacementSize_Reserved:
#ifdef MUSASHI_COMPATIBILITY_MODE
				break;
#else
				return false;
#endif
			default:
				M68060_ERROR("Unsupported BaseDisplacementSize");
		}

		switch (is_iis)
		{
			case IS_IIS_PreIndexed_NoMemoryIndirect:
			case IS_IIS_NonIndexed_NoMemoryIndirect:
			case IS_IIS_PreIndexed_Indirect_NullOuterDisplacement:
			case IS_IIS_PostIndexed_Indirect_NullOuterDisplacement:
			case IS_IIS_NonIndexed_Indirect_NullOuterDisplacement:
				break;
			case IS_IIS_PreIndexed_Indirect_WordOuterDisplacement:
			case IS_IIS_PostIndexed_Indirect_WordOuterDisplacement:
			case IS_IIS_NonIndexed_Indirect_WordOuterDisplacement:
				numExtensionWords++;
				break;
			case IS_IIS_PreIndexed_Indirect_LongOuterDisplacement:
			case IS_IIS_PostIndexed_Indirect_LongOuterDisplacement:
			case IS_IIS_NonIndexed_Indirect_LongOuterDisplacement:
				numExtensionWords += 2;
				break;
			default:
#ifdef MUSASHI_COMPATIBILITY_MODE
				break;
#else
				return false;
#endif
		}
	}
	
	*numExtensionWords_ = numExtensionWords;

	return true;
}

static bool decodeEA6BitMode(EA6BitMode_Upper3Bits eaUpper3Bits, EA6BitMode_Lower3Bits eaLower3Bits, EAMode* eaMode)
{
	switch (eaUpper3Bits)
	{
		case EA6BitMode_Upper3Bits_Dn:
			*eaMode = EAMode_Dn;
			return true;
		case EA6BitMode_Upper3Bits_An:
			*eaMode = EAMode_An;
			return true;
		case EA6BitMode_Upper3Bits_Mem_An:
			*eaMode = EAMode_Mem_An;
			return true;
		case EA6BitMode_Upper3Bits_Mem_An_PostIncrement:
			*eaMode = EAMode_Mem_An_PostIncrement;
			return true;
		case EA6BitMode_Upper3Bits_Mem_PreDecrement_An:
			*eaMode = EAMode_Mem_PreDecrement_An;
			return true;
		case EA6BitMode_Upper3Bits_Mem_D16_An:
			*eaMode = EAMode_Mem_D16_An;
			return true;
		case EA6BitMode_Upper3Bits_Mem_BriefOrFullExtensionWord_An:
			*eaMode = EAMode_Mem_BriefOrFullExtensionWord_An;
			return true;
		case EA6BitMode_Upper3Bits_CheckLower3Bits:
			switch (eaLower3Bits)
			{
				case EA6BitMode_Lower3Bits_Mem_Absolute_Word:
					*eaMode = EAMode_Mem_Absolute_Word;
					return true;
				case EA6BitMode_Lower3Bits_Mem_Absolute_Long:
					*eaMode = EAMode_Mem_Absolute_Long;
					return true;
				case EA6BitMode_Lower3Bits_Mem_D16_PC:
					*eaMode = EAMode_Mem_D16_PC;
					return true;
				case EA6BitMode_Lower3Bits_Mem_BriefOrFullExtensionWord_PC:
					*eaMode = EAMode_Mem_BriefOrFullExtensionWord_PC;
					return true;
				case EA6BitMode_Lower3Bits_Immediate:
					*eaMode = EAMode_Immediate;
					return true;
				default:
					return false;
			}
			break;
		default:
			M68060_ERROR("Invalid 6-bit EA");
			return false;
	}
}

typedef enum
{
	DecodeOperandLengthResult_InsufficientData,
	DecodeOperandLengthResult_InvalidInstruction,
	DecodeOperandLengthResult_ValidInstruction,

} DecodeOperandLengthResult;

static DecodeOperandLengthResult decodeOperandLength(EAMode eaMode, bool firstExtensionWordAvailable, uint16_t firstExtensionWord, OperationSize operationSize, uint* numExtensionWords)
{
	switch (eaMode)
	{
		case EAMode_None:
		case EAMode_Dn:
		case EAMode_An:
		case EAMode_Mem_An:
		case EAMode_Mem_An_PostIncrement:
		case EAMode_Mem_PreDecrement_An:
			*numExtensionWords = 0;
			break;
		case EAMode_Mem_D16_An:
			*numExtensionWords = 1;
			break;
		case EAMode_Mem_BriefOrFullExtensionWord_An:
			if (!firstExtensionWordAvailable)
				return DecodeOperandLengthResult_InsufficientData;
			if (!decodeBriefOrFullExtensionWordLength(firstExtensionWord, numExtensionWords))
				return DecodeOperandLengthResult_InvalidInstruction;
			break;
		case EAMode_Mem_Absolute_Word:
			*numExtensionWords = 1;
			break;
		case EAMode_Mem_Absolute_Long:
			*numExtensionWords = 2;
			break;
		case EAMode_Mem_D16_PC:
			*numExtensionWords = 1;
			break;
		case EAMode_Mem_BriefOrFullExtensionWord_PC:
			if (!firstExtensionWordAvailable)
				return DecodeOperandLengthResult_InsufficientData;
			if (!decodeBriefOrFullExtensionWordLength(firstExtensionWord, numExtensionWords))
				return DecodeOperandLengthResult_InvalidInstruction;
			break;
		case EAMode_Immediate:
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
		case EAMode_RelativeBranch:
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
			break;
		default:
			M68060_ERROR("Invalid EAMode");
	}
	
	return DecodeOperandLengthResult_ValidInstruction;
}

static bool decodeOperand(uint16_t opWord, EAEncoding eaEncoding, EAMode* eaMode)
{
	switch (eaEncoding)
	{
		case EAEncoding_None:
			*eaMode = EAMode_None;
			return true;
		case EAEncoding_DefaultEALocation:
		{
			uint ea6BitMode = opWord & 0x3f;
			EA6BitMode_Upper3Bits eaUpper3Bits = (ea6BitMode >> 3) & 7;
			EA6BitMode_Lower3Bits eaLower3Bits = ea6BitMode & 7;
			return decodeEA6BitMode(eaUpper3Bits, eaLower3Bits, eaMode);
		}
		case EAEncoding_MoveDestinationEALocation:
		{
			uint ea6BitMode = (opWord >> 6) & 0x3f;
			EA6BitMode_Upper3Bits eaUpper3Bits = ea6BitMode & 7;
			EA6BitMode_Lower3Bits eaLower3Bits = (ea6BitMode >> 3) & 7;
			return decodeEA6BitMode(eaUpper3Bits, eaLower3Bits, eaMode);
		}
		case EAEncoding_Immediate:
			*eaMode = EAMode_Immediate;
			return true;
		case EAEncoding_D16:
			*eaMode = EAMode_Mem_D16_An;
			return true;
		case EAEncoding_RelativeBranch:
			*eaMode = EAMode_RelativeBranch;
			return true;
		default:
			M68060_ERROR("Unsupported EAEncoding");
			return false;
	}
}

static bool decodeOperationSize(uint16_t opWord, SizeEncoding sizeEncoding, OperationSize* operationSize)
{
	switch (sizeEncoding)
	{
		case SizeEncoding_None:
			*operationSize = OperationSize_None;
			return true;
		case SizeEncoding_Byte:
			*operationSize = OperationSize_Byte;
			return true;
		case SizeEncoding_Word:
			*operationSize = OperationSize_Word;
			return true;
		case SizeEncoding_Long:
			*operationSize = OperationSize_Long;
			return true;
		case SizeEncoding_DefaultOpModeEncoding:
			{
				OpMode opMode = (opWord & OpWord_OpMode_Mask) >> OpWord_OpMode_Shift;
				switch (opMode)
				{
					case OpMode_MemoryToDataRegister_Byte:
						*operationSize = OperationSize_Byte;
						return true;
					case OpMode_MemoryToDataRegister_Word:
						*operationSize = OperationSize_Word;
						return true;
					case OpMode_MemoryToDataRegister_Long:
						*operationSize = OperationSize_Long;
						return true;
					case OpMode_MemoryToAddressRegister_Word:
						*operationSize = OperationSize_Word;
						return true;
					case OpMode_DataRegisterToMemory_Byte:
						*operationSize = OperationSize_Byte;
						return true;
					case OpMode_DataRegisterToMemory_Word:
						*operationSize = OperationSize_Word;
						return true;
					case OpMode_DataRegisterToMemory_Long:
						*operationSize = OperationSize_Long;
						return true;
					case OpMode_MemoryToAddressRegister_Long:
						*operationSize = OperationSize_Long;
						return true;
					default:
						M68060_ERROR("Unsupported OpMode");
						return false;
				}
			}
		case SizeEncoding_DefaultOpSizeEncoding:
			{
				OpSize opSize = (opWord & OpWord_OpSize_Mask) >> OpWord_OpSize_Shift;
				switch (opSize)
				{
					case OpSize_Byte:
						*operationSize = OperationSize_Byte;
						return true;
					case OpSize_Word:
						*operationSize = OperationSize_Word;
						return true;
					case OpSize_Long:
						*operationSize = OperationSize_Long;
						return true;
					default:
						return false;
				}
			}
		case SizeEncoding_RelativeBranchEncoding:
			{
				uint displacement8Bit = (opWord & 0xff);
				if (displacement8Bit == 0x00)
					*operationSize = OperationSize_Word;
				else if (displacement8Bit == 0xff)
					*operationSize = OperationSize_Long;
				else
					*operationSize = OperationSize_Byte;
				return true;
			}
		default:
			M68060_ERROR("Unsupported SizeEncoding");
			return false;
	}
}

static bool isValidEAMode(EAMode eaMode, EAModeMask eaModeMask)
{
	return (eaMode == EAMode_None || ((1U << eaMode) & eaModeMask));
}

bool decodeInstructionLengthFromInstructionWords(const uint16_t* instructionWords, uint numInstructionWordsAvailable, InstructionLength* outInstructionLength)
{
	const OpWordDecodeInfo* opWordDecodeInfo;
	const OpWordClassInfo* opWordClassInfo;
	uint16_t opWord;
	uint operandOffset;
	InstructionLength instructionLength = { 0 };
	bool validInstruction = true;

	M68060_ASSERT(numInstructionWordsAvailable > 0, "Must supply at least one instruction word");

	opWord = instructionWords[0];

	opWordDecodeInfo = getOpWordDecodeInformation(opWord);

	if (!opWordDecodeInfo)
	{
		validInstruction = false;
	}
	else
	{
		bool validOperationSize;
		OperationSize operationSize;
		opWordClassInfo = getOpWordClassInformation(opWordDecodeInfo->class);

		instructionLength.numSpecialOperandSpecifierWords = opWordClassInfo->numSpecialOperandSpecifierWords;
		instructionLength.description = opWordDecodeInfo->description;

		validOperationSize = decodeOperationSize(opWord, opWordClassInfo->sizeEncoding, &operationSize);

		if (!validOperationSize)
			validInstruction = false;
		
		operandOffset = 1 + instructionLength.numSpecialOperandSpecifierWords;

		if (validInstruction)
		{
			EAMode eaMode;
			bool validEaMode = decodeOperand(opWord, opWordClassInfo->sourceEAEncoding, &eaMode);
			bool firstExtensionWordAvailable = (operandOffset < numInstructionWordsAvailable);
			uint16_t firstExtensionWord = (firstExtensionWordAvailable ? instructionWords[operandOffset] : 0);

			if (validEaMode && isValidEAMode(eaMode, opWordClassInfo->sourceEAModeMask))
			{
				DecodeOperandLengthResult decodeOperandLengthResult = decodeOperandLength(eaMode, firstExtensionWordAvailable, firstExtensionWord, operationSize, &instructionLength.numSourceEAExtensionWords);

				if (decodeOperandLengthResult == DecodeOperandLengthResult_InsufficientData)
					return false;
				else if (decodeOperandLengthResult == DecodeOperandLengthResult_InvalidInstruction)
					validInstruction = false;
			}
			else
				validInstruction = false;
		}

		if (validInstruction)
		{
		
			operandOffset += instructionLength.numSourceEAExtensionWords;

			{
				EAMode eaMode;
				bool validEaMode = decodeOperand(opWord, opWordClassInfo->destinationEAEncoding, &eaMode);
				bool firstExtensionWordAvailable = (operandOffset < numInstructionWordsAvailable);
				uint16_t firstExtensionWord = (firstExtensionWordAvailable ? instructionWords[operandOffset] : 0);

				if (validEaMode && isValidEAMode(eaMode, opWordClassInfo->destinationEAModeMask))
				{
					DecodeOperandLengthResult decodeOperandLengthResult = decodeOperandLength(eaMode, firstExtensionWordAvailable, firstExtensionWord, operationSize, &instructionLength.numDestinationEAExtensionWords);

					if (decodeOperandLengthResult == DecodeOperandLengthResult_InsufficientData)
						return false;
					else if (decodeOperandLengthResult == DecodeOperandLengthResult_InvalidInstruction)
						validInstruction = false;
				}
				else
					validInstruction = false;
			}
		}
	}

	
	if (!validInstruction)
	{
		InstructionLength invalidInstructionLength = { 0, 0, 0, 0, "invalid instruction" } ;
		
		instructionLength = invalidInstructionLength;
	}

	instructionLength.totalWords = 1 + instructionLength.numSpecialOperandSpecifierWords
		 + instructionLength.numSourceEAExtensionWords + instructionLength.numDestinationEAExtensionWords;
		
	*outInstructionLength = instructionLength;
	
	return true;
}
