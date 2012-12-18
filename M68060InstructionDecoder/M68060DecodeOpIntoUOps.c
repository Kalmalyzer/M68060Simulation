
#ifndef M68060OPWORDDECODER_H
#define M68060OPWORDDECODER_H

#include "M68060DecodeOpIntoUOps.h"
#include "../Assert.h"
#include "../Types.h"
#include "M68060EA.h"
#include "M68060InstructionDecoderTypes.h"
#include "M68060InstructionLengthDecoder.h"
#include "M68060OpWord.h"
#include "M68060OpWordDecodeInformation.h"
#include "M68060UOp.h"

typedef struct
{
	UOp* UOps;
	uint numUOps;

} UOpWriteBuffer;

static void writeUOp(UOpWriteBuffer* UOpWriteBuffer, UOp* UOp)
{
	UOpWriteBuffer->UOps[UOpWriteBuffer->numUOps] = *UOp;
	UOpWriteBuffer->numUOps++;
}

static bool decodeIeeOperationSize(uint16_t opWord, SizeEncoding sizeEncoding, OperationSize* ieeOperationSize)
{
	switch (sizeEncoding)
	{
		case SizeEncoding_None:
			*ieeOperationSize = OperationSize_None;
			return true;
		case SizeEncoding_Byte:
			*ieeOperationSize = OperationSize_Byte;
			return true;
		case SizeEncoding_Word:
			*ieeOperationSize = OperationSize_Word;
			return true;
		case SizeEncoding_Long:
			*ieeOperationSize = OperationSize_Long;
			return true;
		case SizeEncoding_DefaultOpModeEncoding:
			{
				OpMode opMode = (opWord & OpWord_OpMode_Mask) >> OpWord_OpMode_Shift;
				switch (opMode)
				{
					case OpMode_MemoryToDataRegister_Byte:
						*ieeOperationSize = OperationSize_Byte;
						return true;
					case OpMode_MemoryToDataRegister_Word:
						*ieeOperationSize = OperationSize_Word;
						return true;
					case OpMode_MemoryToDataRegister_Long:
						*ieeOperationSize = OperationSize_Long;
						return true;
					case OpMode_MemoryToAddressRegister_Word:
						*ieeOperationSize = OperationSize_Word;
						return true;
					case OpMode_DataRegisterToMemory_Byte:
						*ieeOperationSize = OperationSize_Byte;
						return true;
					case OpMode_DataRegisterToMemory_Word:
						*ieeOperationSize = OperationSize_Word;
						return true;
					case OpMode_DataRegisterToMemory_Long:
						*ieeOperationSize = OperationSize_Long;
						return true;
					case OpMode_MemoryToAddressRegister_Long:
						*ieeOperationSize = OperationSize_Long;
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
						*ieeOperationSize = OperationSize_Byte;
						return true;
					case OpSize_Word:
						*ieeOperationSize = OperationSize_Word;
						return true;
					case OpSize_Long:
						*ieeOperationSize = OperationSize_Long;
						return true;
					default:
						return false;
				}
			}
		case SizeEncoding_RelativeBranchEncoding:
			{
				uint displacement8Bit = (opWord & 0xff);
				if (displacement8Bit == 0x00)
					*ieeOperationSize = OperationSize_Word;
				else if (displacement8Bit == 0xff)
					*ieeOperationSize = OperationSize_Long;
				else
					*ieeOperationSize = OperationSize_Byte;
				return true;
			}
		default:
			M68060_ERROR("Unsupported SizeEncoding");
			return false;
	}
}

static void decodeBriefOrFullExtensionWord(const uint16_t* operandSpecifierWords, UOp* mainUOp, UOpWriteBuffer* UOpWriteBuffer, ExecutionResource baseRegister)
{
	uint16_t firstExtensionWord = operandSpecifierWords[0];
		
	if (firstExtensionWord & ExtensionWord_FullWord_Mask)
	{
		DisplacementSize baseDisplacementSize = (firstExtensionWord & FullExtensionWord_BaseDisplacementSize_Mask) >> FullExtensionWord_BaseDisplacementSize_Shift;
		DisplacementSize outerDisplacementSize = (firstExtensionWord & FullExtensionWord_OuterDisplacementSize_Mask) >> FullExtensionWord_OuterDisplacementSize_Shift;
		IS_IIS is_iis = (firstExtensionWord & FullExtensionWord_IS_IIS_Mask);

		UOp firstOp = { 0 };
		UOp secondOp = { 0 };
		UOp* indexOp = 0;

		uint numInjectedUOps = 0;
		
		bool baseRegisterSuppressed = (firstExtensionWord & FullExtensionWord_BaseRegisterSuppress_Mask);
		bool indexRegisterSuppressed = (firstExtensionWord & FullExtensionWord_IndexSuppress_Mask);
		
		bool hasDoubleIndirection = (outerDisplacementSize != DisplacementSize_Reserved);

		uint operandSpecifierImmediateOffset = 1;
		
		if (!baseRegisterSuppressed)
			firstOp.aguBase = baseRegister;


		if (hasDoubleIndirection)
		{
			firstOp.mnemonic = "LOAD";
			firstOp.memoryRead = true;
			firstOp.aguOperation = AguOperation_OffsetBaseIndexScale;
			firstOp.ieeA = ExecutionResource_MemoryOperand;
			firstOp.ieeOperation = IeeOperation_ForwardIeeA;
			firstOp.ieeResult = ExecutionResource_AguTemp;
			firstOp.pairability = Pairability_pOEP_Only;

			secondOp.mnemonic = "LEA";
			secondOp.aguBase = ExecutionResource_AguTemp;
			secondOp.aguOperation = AguOperation_OffsetBaseIndexScale;
			secondOp.aguResult = ExecutionResource_AguTemp;
			secondOp.pairability = Pairability_pOEP_Only;
		}
		else
		{
			firstOp.mnemonic = "LEA";
			firstOp.aguOperation = AguOperation_OffsetBaseIndexScale;
			firstOp.aguResult = ExecutionResource_AguTemp;
			firstOp.pairability = Pairability_pOEP_Only;
		}
		
		switch (baseDisplacementSize)
		{
			case DisplacementSize_Null:
				break;
			case DisplacementSize_Word:
				firstOp.aguDisplacementSize = AguDisplacementSize_S16;
				firstOp.extensionWords[0] = operandSpecifierWords[operandSpecifierImmediateOffset++];
				break;
			case DisplacementSize_Long:
				firstOp.aguDisplacementSize = AguDisplacementSize_S32;
				firstOp.extensionWords[0] = operandSpecifierWords[operandSpecifierImmediateOffset++];
				firstOp.extensionWords[1] = operandSpecifierWords[operandSpecifierImmediateOffset++];
				break;
			default:
				M68060_ERROR("BaseDisplacementSize not supported");
		}

		switch (outerDisplacementSize)
		{
			case DisplacementSize_Null:
				break;
			case DisplacementSize_Word:
				secondOp.aguDisplacementSize = AguDisplacementSize_S16;
				secondOp.extensionWords[0] = operandSpecifierWords[operandSpecifierImmediateOffset++];
				break;
			case DisplacementSize_Long:
				secondOp.aguDisplacementSize = AguDisplacementSize_S32;
				secondOp.extensionWords[0] = operandSpecifierWords[operandSpecifierImmediateOffset++];
				secondOp.extensionWords[1] = operandSpecifierWords[operandSpecifierImmediateOffset++];
				break;
			case DisplacementSize_Reserved:
				break;
			default:
				M68060_ERROR("OuterDisplacementSize not supported");
		}

		switch (is_iis)
		{
			case IS_IIS_PreIndexed_NoMemoryIndirect:
			case IS_IIS_PreIndexed_Indirect_NullOuterDisplacement:
			case IS_IIS_PreIndexed_Indirect_WordOuterDisplacement:
			case IS_IIS_PreIndexed_Indirect_LongOuterDisplacement:
				indexOp = &firstOp;
				break;
				
			case IS_IIS_PostIndexed_Indirect_NullOuterDisplacement:
			case IS_IIS_PostIndexed_Indirect_WordOuterDisplacement:
			case IS_IIS_PostIndexed_Indirect_LongOuterDisplacement:
				indexOp = &secondOp;
				break;

			case IS_IIS_NonIndexed_NoMemoryIndirect:
			case IS_IIS_NonIndexed_Indirect_NullOuterDisplacement:
			case IS_IIS_NonIndexed_Indirect_WordOuterDisplacement:
			case IS_IIS_NonIndexed_Indirect_LongOuterDisplacement:
				break;

			default:
				M68060_ERROR("IS_IIS not yet implemented");
				break;
		}

		if (!indexRegisterSuppressed && indexOp)
		{
			ExecutionResource indexRegisterType = (firstExtensionWord & ExtensionWord_DA_Mask) ? ExecutionResource_A0 : ExecutionResource_D0;
			ExecutionResource indexRegister = indexRegisterType + ((firstExtensionWord & ExtensionWord_Register_Mask) >> ExtensionWord_Register_Shift);
			indexOp->aguIndex = indexRegister;
			indexOp->aguIndexShift = (firstExtensionWord & ExtensionWord_Scale_Mask) >> ExtensionWord_Scale_Shift;
			indexOp->aguIndexSize = (firstExtensionWord & ExtensionWord_WL_Mask) ? AguIndexSize_Long : AguIndexSize_Word;
		}

		writeUOp(UOpWriteBuffer, &firstOp);

		if (hasDoubleIndirection)
			writeUOp(UOpWriteBuffer, &secondOp);

		mainUOp->aguBase = ExecutionResource_AguTemp;
		mainUOp->aguOperation = AguOperation_OffsetBaseIndexScale;
	}
	else
	{
		ExecutionResource indexRegisterType = (firstExtensionWord & ExtensionWord_DA_Mask) ? ExecutionResource_A0 : ExecutionResource_D0;
		ExecutionResource indexRegister = indexRegisterType + ((firstExtensionWord & ExtensionWord_Register_Mask) >> ExtensionWord_Register_Shift);
		mainUOp->aguBase = baseRegister;
		mainUOp->aguIndex = indexRegister;
		mainUOp->aguIndexShift = (firstExtensionWord & ExtensionWord_Scale_Mask) >> ExtensionWord_Scale_Shift;
		mainUOp->aguIndexSize = (firstExtensionWord & ExtensionWord_WL_Mask) ? AguIndexSize_Long : AguIndexSize_Word;
		mainUOp->aguDisplacementSize = AguDisplacementSize_S8;
		mainUOp->aguOperation = AguOperation_OffsetBaseIndexScale;
		mainUOp->extensionWords[0] = firstExtensionWord & 0xff;
	}
}

static void decodeEA6BitMode(EA6BitMode_Upper3Bits eaUpper3Bits, EA6BitMode_Lower3Bits eaLower3Bits, OperationSize immediateSize, const uint16_t* operandSpecifierWords, UOp* mainUOp, ExecutionResource* ieeInput, UOpWriteBuffer* UOpWriteBuffer, bool* hasMemoryReference)
{
	switch (eaUpper3Bits)
	{
		case EA6BitMode_Upper3Bits_Dn:
			*ieeInput = ExecutionResource_D0 + (uint) eaLower3Bits;
			*hasMemoryReference = false;
			return;
		case EA6BitMode_Upper3Bits_An:
			*ieeInput = ExecutionResource_A0 + (uint) eaLower3Bits;
			*hasMemoryReference = false;
			return;
		case EA6BitMode_Upper3Bits_Mem_An:
			mainUOp->aguBase = ExecutionResource_A0 + (uint) eaLower3Bits;
			mainUOp->aguOperation = AguOperation_OffsetBaseIndexScale;
			*ieeInput = ExecutionResource_MemoryOperand;
			*hasMemoryReference = true;
			return;
		case EA6BitMode_Upper3Bits_Mem_An_PostIncrement:
			mainUOp->aguBase = ExecutionResource_A0 + (uint) eaLower3Bits;
			mainUOp->aguOperation = AguOperation_PostIncrement;
			mainUOp->aguResult = ExecutionResource_A0 + (uint) eaLower3Bits;
			*ieeInput = ExecutionResource_MemoryOperand;
			*hasMemoryReference = true;
			return;
		case EA6BitMode_Upper3Bits_Mem_PreDecrement_An:
			mainUOp->aguBase = ExecutionResource_A0 + (uint) eaLower3Bits;
			mainUOp->aguOperation = AguOperation_PreDecrement;
			mainUOp->aguResult = ExecutionResource_A0 + (uint) eaLower3Bits;
			*ieeInput = ExecutionResource_MemoryOperand;
			*hasMemoryReference = true;
			return;
		case EA6BitMode_Upper3Bits_Mem_D16_An:
			mainUOp->aguBase = ExecutionResource_A0 + (uint) eaLower3Bits;
			mainUOp->aguDisplacementSize = AguDisplacementSize_S16;
			mainUOp->extensionWords[0] = operandSpecifierWords[0];
			mainUOp->aguOperation = AguOperation_OffsetBaseIndexScale;
			*ieeInput = ExecutionResource_MemoryOperand;
			*hasMemoryReference = true;
			return;
		case EA6BitMode_Upper3Bits_Mem_BriefOrFullExtensionWord_An:
			{
				ExecutionResource an = ExecutionResource_A0 + (uint) eaLower3Bits;
				*ieeInput = ExecutionResource_MemoryOperand;
				*hasMemoryReference = true;
				decodeBriefOrFullExtensionWord(operandSpecifierWords, mainUOp, UOpWriteBuffer, an);
				return;
			}
		case EA6BitMode_Upper3Bits_CheckLower3Bits:
			switch (eaLower3Bits)
			{
				case EA6BitMode_Lower3Bits_Mem_Absolute_Word:
					mainUOp->aguDisplacementSize = AguDisplacementSize_S16;
					mainUOp->aguOperation = AguOperation_OffsetBaseIndexScale;
					mainUOp->extensionWords[0] = operandSpecifierWords[0];
					*ieeInput = ExecutionResource_MemoryOperand;
					*hasMemoryReference = true;
					return;
				case EA6BitMode_Lower3Bits_Mem_Absolute_Long:
					mainUOp->aguDisplacementSize = AguDisplacementSize_S32;
					mainUOp->extensionWords[0] = operandSpecifierWords[0];
					mainUOp->extensionWords[1] = operandSpecifierWords[1];
					mainUOp->aguOperation = AguOperation_OffsetBaseIndexScale;
					*ieeInput = ExecutionResource_MemoryOperand;
					*hasMemoryReference = true;
					return;
				case EA6BitMode_Lower3Bits_Mem_D16_PC:
					mainUOp->aguBase = ExecutionResource_PC;
					mainUOp->aguDisplacementSize = AguDisplacementSize_S16;
					mainUOp->extensionWords[0] = operandSpecifierWords[0];
					mainUOp->aguOperation = AguOperation_OffsetBaseIndexScale;
					*ieeInput = ExecutionResource_MemoryOperand;
					*hasMemoryReference = true;
					return;
				case EA6BitMode_Lower3Bits_Mem_BriefOrFullExtensionWord_PC:
					*ieeInput = ExecutionResource_MemoryOperand;
					*hasMemoryReference = true;
					decodeBriefOrFullExtensionWord(operandSpecifierWords, mainUOp, UOpWriteBuffer, ExecutionResource_PC);
					return;
				case EA6BitMode_Lower3Bits_Immediate:
					switch (immediateSize)
					{
						case OperationSize_Byte:
							mainUOp->extensionWords[0] = operandSpecifierWords[0] & 0xff;
							*ieeInput = ExecutionResource_uOpByte0;
							*hasMemoryReference = false;
							return;
						case OperationSize_Word:
							mainUOp->extensionWords[0] = operandSpecifierWords[0];
							*ieeInput = ExecutionResource_uOpWord0;
							*hasMemoryReference = false;
							return;
						case OperationSize_Long:
							mainUOp->extensionWords[0] = operandSpecifierWords[0];
							mainUOp->extensionWords[1] = operandSpecifierWords[1];
							*ieeInput = ExecutionResource_uOpLong;
							*hasMemoryReference = false;
							return;
						default:
							M68060_ERROR("Invalid operation size");
					}
					return;
				default:
					M68060_ERROR("Invalid 6-bit EA");
					return;
			}
			break;
		default:
			M68060_ERROR("Invalid 6-bit EA");
			return;
	}
}

static void decodeImmediateOperand(OperationSize immediateSize, const uint16_t* operandSpecifierWords, UOp* mainUOp, ExecutionResource* ieeInput, bool* hasMemoryReference)
{
	switch (immediateSize)
	{
		case OperationSize_Byte:
			mainUOp->extensionWords[0] = operandSpecifierWords[0] & 0xff;
			*ieeInput = ExecutionResource_uOpByte0;
			break;
		case OperationSize_Word:
			mainUOp->extensionWords[0] = operandSpecifierWords[0];
			*ieeInput = ExecutionResource_uOpWord0;
			break;
		case OperationSize_Long:
			mainUOp->extensionWords[0] = operandSpecifierWords[0];
			mainUOp->extensionWords[1] = operandSpecifierWords[1];
			*ieeInput = ExecutionResource_uOpLong;
			break;
	}
	*hasMemoryReference = false;
}

static bool needsExtensionWords(EA6BitMode_Upper3Bits eaUpper3Bits, EA6BitMode_Lower3Bits eaLower3Bits)
{
	switch (eaUpper3Bits)
	{
		case EA6BitMode_Upper3Bits_Dn:
		case EA6BitMode_Upper3Bits_An:
		case EA6BitMode_Upper3Bits_Mem_An:
		case EA6BitMode_Upper3Bits_Mem_An_PostIncrement:
		case EA6BitMode_Upper3Bits_Mem_PreDecrement_An:
			return false;
		case EA6BitMode_Upper3Bits_Mem_D16_An:
		case EA6BitMode_Upper3Bits_Mem_BriefOrFullExtensionWord_An:
			return true;
		case EA6BitMode_Upper3Bits_CheckLower3Bits:
			switch (eaLower3Bits)
			{
				case EA6BitMode_Lower3Bits_Mem_Absolute_Word:
				case EA6BitMode_Lower3Bits_Mem_Absolute_Long:
				case EA6BitMode_Lower3Bits_Mem_D16_PC:
				case EA6BitMode_Lower3Bits_Mem_BriefOrFullExtensionWord_PC:
				case EA6BitMode_Lower3Bits_Immediate:
					return true;
				default:
					M68060_ERROR("Invalid 6-bit EA");
					return false;
			}
			break;
		default:
			M68060_ERROR("Invalid 6-bit EA");
			return false;
	}
}

static bool hasImmediateIeeA(UOp* mainUOp)
{
	return (mainUOp->ieeA == ExecutionResource_uOpByte0)
		|| (mainUOp->ieeA == ExecutionResource_uOpWord0)
		|| (mainUOp->ieeA == ExecutionResource_uOpWord1)
		|| (mainUOp->ieeA == ExecutionResource_uOpLong);
}

static void transferIeeAImmediateToSeparateUOp(UOp* mainUOp, UOpWriteBuffer* UOpWriteBuffer)
{
	UOp loadI = { 0 };
	loadI.mnemonic = "LOADI";
	loadI.extensionWords[0] = mainUOp->extensionWords[0];
	loadI.extensionWords[1] = mainUOp->extensionWords[1];
	loadI.ieeA = mainUOp->ieeA;
	loadI.ieeOperation = IeeOperation_ForwardIeeA;
	loadI.ieeResult = ExecutionResource_ImmediateTemp;
	loadI.pairability = Pairability_pOEP_Only;
	writeUOp(UOpWriteBuffer, &loadI);

	mainUOp->extensionWords[0] = 0;
	mainUOp->extensionWords[1] = 0;
	mainUOp->ieeA = ExecutionResource_ImmediateTemp;
}

static void decodeOperand(uint16_t opWord, DecodeOperand decodeOperand, OperationSize immediateSize, const uint16_t* operandSpecifierWords, UOp* mainUOp, ExecutionResource* ieeInput, UOpWriteBuffer* UOpWriteBuffer, bool* hasMemoryReference)
{
	switch (decodeOperand)
	{
	case DecodeOperand_None:
		break;
	case DecodeOperand_DefaultEALocation:
		{
			uint ea6BitMode = opWord & 0x3f;
			EA6BitMode_Upper3Bits eaUpper3Bits = (ea6BitMode >> 3) & 7;
			EA6BitMode_Lower3Bits eaLower3Bits = ea6BitMode & 7;
			if (needsExtensionWords(eaUpper3Bits, eaLower3Bits) && hasImmediateIeeA(mainUOp))
			{
				transferIeeAImmediateToSeparateUOp(mainUOp, UOpWriteBuffer);
			}
			decodeEA6BitMode(eaUpper3Bits, eaLower3Bits, immediateSize, operandSpecifierWords, mainUOp, ieeInput, UOpWriteBuffer, hasMemoryReference);
			break;
		}
	case DecodeOperand_DefaultDnLocation:
		{
			ExecutionResource dn = ExecutionResource_D0 + ((opWord & OpWord_DefaultRegisterEncoding_Mask) >> OpWord_DefaultRegisterEncoding_Shift);
			*ieeInput = dn;
			*hasMemoryReference = false;
			break;
		}
	case DecodeOperand_DefaultAnLocation:
		{
			ExecutionResource an = ExecutionResource_A0 + ((opWord & OpWord_DefaultRegisterEncoding_Mask) >> OpWord_DefaultRegisterEncoding_Shift);
			*ieeInput = an;
			*hasMemoryReference = false;
			break;
		}
	case DecodeOperand_Immediate:
		{
			decodeImmediateOperand(immediateSize, operandSpecifierWords, mainUOp, ieeInput, hasMemoryReference);
			break;
		}
	case DecodeOperand_Imm3Bit:
		{
			uint8_t imm3BitValue = (opWord & OpWord_DefaultImm3BitEncoding_Mask) >> OpWord_DefaultImm3BitEncoding_Shift;
			mainUOp->imm3Bit = (imm3BitValue ? imm3BitValue : 8);
			*ieeInput = ExecutionResource_Imm3Bit;
			*hasMemoryReference = false;
			break;
		}
	default:
		M68060_ERROR("Not yet implemented");
		break;
	}
}

static ExecutionResource decodeIeeResult(DecodeIeeResult decodeIeeResult, ExecutionResource ieeA, ExecutionResource ieeB)
{
	switch (decodeIeeResult)
	{
	case DecodeIeeResult_None:
		return ExecutionResource_None;
	case DecodeIeeResult_IeeA:
		return ieeA;
	case DecodeIeeResult_IeeB:
		return ieeB;
	default:
		M68060_ERROR("DecodeIeeResult not supported");
		return ExecutionResource_None;
	}
}

static void decodeUOps(const uint16_t* instructionWords, const InstructionLength* instructionLength, UOpWriteBuffer* UOpWriteBuffer)
{
	uint16_t opWord = *instructionWords;
	const OpWordDecodeInfo* opWordDecodeInfo = getOpWordDecodeInformation(opWord);
	const OpWordClassInfo* opWordClassInfo;
	UOp mainUOp = { 0 };

	
	M68060_ASSERT(opWordDecodeInfo, "No decoding pattern available for instruction");
	M68060_ASSERT(opWordDecodeInfo->readyForUOpDecoding, "Instruction doesn't support UOp decoding yet");

	opWordClassInfo = getOpWordClassInformation(opWordDecodeInfo->class);

	decodeIeeOperationSize(opWord, opWordClassInfo->sizeEncoding, &mainUOp.ieeOperationSize);

	if (opWordClassInfo->sourceDecodeOperand != DecodeOperand_None)
	{
			uint operandOffset = 1 + instructionLength->numSpecialOperandSpecifierWords;
			bool hasMemoryReference;
			decodeOperand(opWord, opWordClassInfo->sourceDecodeOperand, mainUOp.ieeOperationSize, instructionWords + operandOffset, &mainUOp, &mainUOp.ieeA, UOpWriteBuffer, &hasMemoryReference);
			mainUOp.memoryRead = hasMemoryReference;
	}
	
	if (opWordClassInfo->destinationDecodeOperand != DecodeOperand_None)
	{
			uint operandOffset = 1 + instructionLength->numSpecialOperandSpecifierWords + instructionLength->numSourceEAExtensionWords;
			bool hasMemoryReference;
			decodeOperand(opWord, opWordClassInfo->destinationDecodeOperand, mainUOp.ieeOperationSize, instructionWords + operandOffset, &mainUOp, &mainUOp.ieeB, UOpWriteBuffer, &hasMemoryReference);
			mainUOp.memoryWrite = hasMemoryReference;
			mainUOp.memoryRead |= mainUOp.memoryWrite;
	}

	mainUOp.ieeOperation = opWordDecodeInfo->ieeOperation;
	mainUOp.ieeResult = decodeIeeResult(opWordClassInfo->decodeIeeResult, mainUOp.ieeA, mainUOp.ieeB);
	
	mainUOp.mnemonic = opWordDecodeInfo->description;
	mainUOp.pairability = Pairability_pOEP_Or_sOEP; // TODO: some UOps should have different classifications

	writeUOp(UOpWriteBuffer, &mainUOp);
}

bool decomposeOpIntoUOps(const uint16_t* instructionWords, uint numInstructionWordsAvailable, UOp* UOps, uint* numUOps)
{
	InstructionLength instructionLength;
	bool success = decodeInstructionLengthFromInstructionWords(instructionWords, numInstructionWordsAvailable, &instructionLength);
	UOpWriteBuffer UOpWriteBuffer;

	if (!success)
		return false;
		
	if (instructionLength.totalWords > numInstructionWordsAvailable)
		return false;

	M68060_ASSERT(instructionLength.description, "Unable to decode illegal instruction into UOps");

	UOpWriteBuffer.UOps = UOps;
	UOpWriteBuffer.numUOps = 0;
	
	decodeUOps(instructionWords, &instructionLength, &UOpWriteBuffer);
	
	*numUOps = UOpWriteBuffer.numUOps;
	
	return true;
		
}

#endif
