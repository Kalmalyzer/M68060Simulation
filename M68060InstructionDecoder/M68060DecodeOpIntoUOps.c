
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
	M68060_ASSERT(UOp->aguResult == ExecutionResource_None || UOp->aguOperation != AguOperation_None, "UOps are not allowed to have an AguResult resource specified, but no AguOperation specified");
	M68060_ASSERT(UOp->ieeResult == ExecutionResource_None || UOp->ieeOperation != IeeOperation_None, "UOps are not allowed to have an IeeResult resource specified, but no IeeOperation specified");

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
			firstOp.ieeOperationSize = OperationSize_Long;
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

static void decodeOperand(uint16_t opWord, DecodeOperand decodeOperand, OperationSize immediateSize, const uint16_t* operandSpecifierWords, UOp* mainUOp, ExecutionResource* ieeInput, UOpWriteBuffer* UOpWriteBuffer, bool* hasMemoryReference)
{
	switch (decodeOperand)
	{
	case DecodeOperand_None:
		{
			*ieeInput = ExecutionResource_None;
			*hasMemoryReference = false;
			break;
		}
	case DecodeOperand_DefaultEALocation:
		{
			uint ea6BitMode = opWord & 0x3f;
			EA6BitMode_Upper3Bits eaUpper3Bits = (ea6BitMode >> 3) & 7;
			EA6BitMode_Lower3Bits eaLower3Bits = ea6BitMode & 7;
			decodeEA6BitMode(eaUpper3Bits, eaLower3Bits, immediateSize, operandSpecifierWords, mainUOp, ieeInput, UOpWriteBuffer, hasMemoryReference);
			break;
		}
	case DecodeOperand_DefaultEAReferenceLocation:
		{
			uint ea6BitMode = opWord & 0x3f;
			EA6BitMode_Upper3Bits eaUpper3Bits = (ea6BitMode >> 3) & 7;
			EA6BitMode_Lower3Bits eaLower3Bits = ea6BitMode & 7;
			bool dummyHasMemoryReference;
			ExecutionResource dummyIeeInput;
			decodeEA6BitMode(eaUpper3Bits, eaLower3Bits, immediateSize, operandSpecifierWords, mainUOp, &dummyIeeInput, UOpWriteBuffer, &dummyHasMemoryReference);
			M68060_ASSERT(dummyHasMemoryReference, "'EA Reference' addressing mode must be a memory reference");
			M68060_ASSERT(dummyIeeInput == ExecutionResource_MemoryOperand, "'EA Reference' addressing mode yield ieeInput = MemoryOperand");
			break;
		}
	case DecodeOperand_DefaultDnLocation:
		{
			ExecutionResource dn = ExecutionResource_D0 + ((opWord & OpWord_DefaultRegisterEncoding_Mask) >> OpWord_DefaultRegisterEncoding_Shift);
			*ieeInput = dn;
			*hasMemoryReference = false;
			break;
		}
	case DecodeOperand_DefaultPostIncrementAnLocation:
		{
			ExecutionResource an = ExecutionResource_A0 + ((opWord & OpWord_DefaultRegisterEncoding_Mask) >> OpWord_DefaultRegisterEncoding_Shift);
			mainUOp->aguBase = an;
			mainUOp->aguOperation = AguOperation_PostIncrement;
			mainUOp->aguResult = an;
			*ieeInput = ExecutionResource_MemoryOperand;
			*hasMemoryReference = true;
			break;
		}
	case DecodeOperand_DefaultPreDecrementAnLocation:
		{
			ExecutionResource an = ExecutionResource_A0 + ((opWord & OpWord_DefaultRegisterEncoding_Mask) >> OpWord_DefaultRegisterEncoding_Shift);
			mainUOp->aguBase = an;
			mainUOp->aguOperation = AguOperation_PreDecrement;
			mainUOp->aguResult = an;
			*ieeInput = ExecutionResource_MemoryOperand;
			*hasMemoryReference = true;
			break;
		}
	case DecodeOperand_SecondaryDnLocation:
		{
			ExecutionResource dn = ExecutionResource_D0 + ((opWord & OpWord_SecondaryRegisterEncoding_Mask) >> OpWord_SecondaryRegisterEncoding_Shift);
			*ieeInput = dn;
			*hasMemoryReference = false;
			break;
		}
	case DecodeOperand_SecondaryAnLocation:
		{
			ExecutionResource an = ExecutionResource_A0 + ((opWord & OpWord_SecondaryRegisterEncoding_Mask) >> OpWord_SecondaryRegisterEncoding_Shift);
			*ieeInput = an;
			*hasMemoryReference = false;
			break;
		}
	case DecodeOperand_SecondaryPostIncrementAnLocation:
		{
			ExecutionResource an = ExecutionResource_A0 + ((opWord & OpWord_SecondaryRegisterEncoding_Mask) >> OpWord_SecondaryRegisterEncoding_Shift);
			mainUOp->aguBase = an;
			mainUOp->aguOperation = AguOperation_PostIncrement;
			mainUOp->aguResult = an;
			*ieeInput = ExecutionResource_MemoryOperand;
			*hasMemoryReference = true;
			break;
		}
	case DecodeOperand_SecondaryPreDecrementAnLocation:
		{
			ExecutionResource an = ExecutionResource_A0 + ((opWord & OpWord_SecondaryRegisterEncoding_Mask) >> OpWord_SecondaryRegisterEncoding_Shift);
			mainUOp->aguBase = an;
			mainUOp->aguOperation = AguOperation_PreDecrement;
			mainUOp->aguResult = an;
			*ieeInput = ExecutionResource_MemoryOperand;
			*hasMemoryReference = true;
			break;
		}
	case DecodeOperand_SecondaryAnAguResultLocation:
		{
			ExecutionResource an = ExecutionResource_A0 + ((opWord & OpWord_SecondaryRegisterEncoding_Mask) >> OpWord_SecondaryRegisterEncoding_Shift);
			mainUOp->aguResult = an;
			*ieeInput = ExecutionResource_None;
			*hasMemoryReference = false;
			break;
		}
	case DecodeOperand_MoveDestinationEALocation:
		{
			uint ea6BitMode = (opWord >> 6) & 0x3f;
			EA6BitMode_Upper3Bits eaUpper3Bits = ea6BitMode & 7;
			EA6BitMode_Lower3Bits eaLower3Bits = (ea6BitMode >> 3) & 7;
			decodeEA6BitMode(eaUpper3Bits, eaLower3Bits, immediateSize, operandSpecifierWords, mainUOp, ieeInput, UOpWriteBuffer, hasMemoryReference);
			break;
		}
	case DecodeOperand_Immediate:
		{
			decodeImmediateOperand(immediateSize, operandSpecifierWords, mainUOp, ieeInput, hasMemoryReference);
			break;
		}
	case DecodeOperand_Immediate_ExtensionWords_Byte:
		{
			decodeImmediateOperand(OperationSize_Byte, operandSpecifierWords, mainUOp, ieeInput, hasMemoryReference);
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
	case DecodeOperand_Imm3BitValue1:
		{
			uint8_t imm3BitValue = (opWord & OpWord_DefaultImm3BitEncoding_Mask) >> OpWord_DefaultImm3BitEncoding_Shift;
			mainUOp->imm3Bit = 1;
			*ieeInput = ExecutionResource_Imm3Bit;
			*hasMemoryReference = false;
			break;
		}
	case DecodeOperand_Imm8Bit:
		{
			uint8_t imm8BitValue = (opWord & OpWord_DefaultImm8BitEncoding_Mask) >> OpWord_DefaultImm8BitEncoding_Shift;
			mainUOp->extensionWords[0] = imm8BitValue;
			*ieeInput = ExecutionResource_uOpByte0;
			*hasMemoryReference = false;
			break;
		}
	default:
		M68060_ERROR("Not yet implemented");
		break;
	}
}

static DestinationOperandAccessType decodeDestinationOperandAccessType(DestinationOperandAccessType originalAccessType, ExecutionResource destinationExecutionResource, OperationSize operationSize)
{
	switch (originalAccessType)
	{
	case DestinationOperandAccessType_None:
		return originalAccessType;
	case DestinationOperandAccessType_ReadOnly:
		return originalAccessType;
	case DestinationOperandAccessType_ReadWrite:
		return originalAccessType;
	case DestinationOperandAccessType_WriteOnly_If_WholeOperand:
		if (destinationExecutionResource == ExecutionResource_MemoryOperand)
			return DestinationOperandAccessType_WriteOnly;
		else if (isAnRegister(destinationExecutionResource)
			|| (isDnRegister(destinationExecutionResource) && operationSize == OperationSize_Long))
			return DestinationOperandAccessType_WriteOnly;
		else
			return DestinationOperandAccessType_ReadWrite;
	default:
		M68060_ERROR("DestinationOperandAccessType not supported");
		return DestinationOperandAccessType_None;
	}
}

static ExecutionResource decodeIeeB(DestinationOperandAccessType accessType, ExecutionResource destinationExecutionResource)
{
	switch (accessType)
	{
	case DestinationOperandAccessType_None:
		return ExecutionResource_None;
	case DestinationOperandAccessType_ReadOnly:
		return destinationExecutionResource;
	case DestinationOperandAccessType_ReadWrite:
		return destinationExecutionResource;
	case DestinationOperandAccessType_WriteOnly:
		return ExecutionResource_None;
	default:
		M68060_ERROR("DestinationOperandAccessType not supported");
		return ExecutionResource_None;
	}
}

static ExecutionResource decodeIeeResult(DestinationOperandAccessType accessType, ExecutionResource destinationExecutionResource)
{
	switch (accessType)
	{
	case DestinationOperandAccessType_None:
		return ExecutionResource_None;
	case DestinationOperandAccessType_ReadOnly:
		return ExecutionResource_None;
	case DestinationOperandAccessType_ReadWrite:
		return destinationExecutionResource;
	case DestinationOperandAccessType_WriteOnly:
		return destinationExecutionResource;
	default:
		M68060_ERROR("DestinationOperandAccessType not supported");
		return ExecutionResource_None;
	}
}

static void decodeMemoryReferences(DestinationOperandAccessType accessType, bool sourceOperandMemoryReference, bool destinationOperandMemoryReference, bool* memoryRead, bool* memoryWrite)
{
	switch (accessType)
	{
	case DestinationOperandAccessType_None:
		*memoryRead = sourceOperandMemoryReference;
		*memoryWrite = false;
		break;
	case DestinationOperandAccessType_ReadOnly:
		*memoryRead = sourceOperandMemoryReference | destinationOperandMemoryReference;
		*memoryWrite = false;
		break;
	case DestinationOperandAccessType_ReadWrite:
		*memoryRead = sourceOperandMemoryReference | destinationOperandMemoryReference;
		*memoryWrite = destinationOperandMemoryReference;
		break;
	case DestinationOperandAccessType_WriteOnly:
		*memoryRead = sourceOperandMemoryReference;
		*memoryWrite = destinationOperandMemoryReference;
		break;
	default:
		M68060_ERROR("DestinationOperandAccessType not supported");
	}
}

static bool ea6BitModeNeedsExtensionWords(EA6BitMode_Upper3Bits eaUpper3Bits, EA6BitMode_Lower3Bits eaLower3Bits)
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

static bool ea6BitModeHasMemoryReference(EA6BitMode_Upper3Bits eaUpper3Bits, EA6BitMode_Lower3Bits eaLower3Bits)
{
	switch (eaUpper3Bits)
	{
		case EA6BitMode_Upper3Bits_Dn:
		case EA6BitMode_Upper3Bits_An:
			return false;
		case EA6BitMode_Upper3Bits_Mem_An:
		case EA6BitMode_Upper3Bits_Mem_An_PostIncrement:
		case EA6BitMode_Upper3Bits_Mem_PreDecrement_An:
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
					return true;
				case EA6BitMode_Lower3Bits_Immediate:
					return false;
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

typedef struct
{
	bool hasMemoryReference;
	bool needsExtensionWords;

} PreDecodedOperand;

static PreDecodedOperand preDecodeOperand(uint16_t opWord, DecodeOperand decodeOperand)
{
	PreDecodedOperand preDecodedOperand = { 0 };
	
	switch (decodeOperand)
	{
	case DecodeOperand_None:
		preDecodedOperand.needsExtensionWords = false;
		preDecodedOperand.hasMemoryReference = false;
		break;
	case DecodeOperand_DefaultEALocation:
		{
			uint ea6BitMode = opWord & 0x3f;
			EA6BitMode_Upper3Bits eaUpper3Bits = (ea6BitMode >> 3) & 7;
			EA6BitMode_Lower3Bits eaLower3Bits = ea6BitMode & 7;
			preDecodedOperand.needsExtensionWords = ea6BitModeNeedsExtensionWords(eaUpper3Bits, eaLower3Bits);
			preDecodedOperand.hasMemoryReference = ea6BitModeHasMemoryReference(eaUpper3Bits, eaLower3Bits);
			break;
		}
	case DecodeOperand_DefaultEAReferenceLocation:
		{
			uint ea6BitMode = opWord & 0x3f;
			EA6BitMode_Upper3Bits eaUpper3Bits = (ea6BitMode >> 3) & 7;
			EA6BitMode_Lower3Bits eaLower3Bits = ea6BitMode & 7;
			preDecodedOperand.needsExtensionWords = ea6BitModeNeedsExtensionWords(eaUpper3Bits, eaLower3Bits);
			preDecodedOperand.hasMemoryReference = false;
			break;
		}
	case DecodeOperand_DefaultDnLocation:
	case DecodeOperand_SecondaryDnLocation:
	case DecodeOperand_SecondaryAnLocation:
	case DecodeOperand_SecondaryAnAguResultLocation:
	case DecodeOperand_Imm3Bit:
	case DecodeOperand_Imm3BitValue1:
	case DecodeOperand_Imm8Bit:
		{
			preDecodedOperand.needsExtensionWords = false;
			preDecodedOperand.hasMemoryReference = false;
			break;
		}
	case DecodeOperand_DefaultPostIncrementAnLocation:
	case DecodeOperand_DefaultPreDecrementAnLocation:
	case DecodeOperand_SecondaryPostIncrementAnLocation:
	case DecodeOperand_SecondaryPreDecrementAnLocation:
		{
			preDecodedOperand.needsExtensionWords = false;
			preDecodedOperand.hasMemoryReference = true;
			break;
		}
	case DecodeOperand_MoveDestinationEALocation:
		{
			uint ea6BitMode = (opWord >> 6) & 0x3f;
			EA6BitMode_Upper3Bits eaUpper3Bits = ea6BitMode & 7;
			EA6BitMode_Lower3Bits eaLower3Bits = (ea6BitMode >> 3) & 7;
			preDecodedOperand.needsExtensionWords = ea6BitModeNeedsExtensionWords(eaUpper3Bits, eaLower3Bits);
			preDecodedOperand.hasMemoryReference = ea6BitModeHasMemoryReference(eaUpper3Bits, eaLower3Bits);
			break;
		}
	case DecodeOperand_Immediate:
		{
			preDecodedOperand.needsExtensionWords = true;
			preDecodedOperand.hasMemoryReference = false;
			break;
		}
	case DecodeOperand_Immediate_ExtensionWords_Byte:
		{
			preDecodedOperand.needsExtensionWords = true;
			preDecodedOperand.hasMemoryReference = false;
			break;
		}
	default:
		M68060_ERROR("Not yet implemented");
		break;
	}
	
	return preDecodedOperand;
}

static bool shouldSplitSourceAndDestOperandReferences(uint16_t opWord, DecodeOperand sourceDecodeOperand, DecodeOperand destinationDecodeOperand)
{
	PreDecodedOperand sourceOperand = preDecodeOperand(opWord, sourceDecodeOperand);
	PreDecodedOperand destinationOperand = preDecodeOperand(opWord, destinationDecodeOperand);
	
	if (sourceOperand.hasMemoryReference && destinationOperand.hasMemoryReference)
		return true;

	if (sourceOperand.needsExtensionWords && destinationOperand.needsExtensionWords)
		return true;
		
	return false;
}

static void decodeUOps(const uint16_t* instructionWords, const InstructionLength* instructionLength, UOpWriteBuffer* UOpWriteBuffer)
{
	uint16_t opWord = *instructionWords;
	const OpWordDecodeInfo* opWordDecodeInfo = getOpWordDecodeInformation(opWord);
	const OpWordClassInfo* opWordClassInfo;
	UOp mainUOp = { 0 };
	bool splitSourceAndDestOperandReferences;
	OperationSize ieeOperationSize;
	bool sourceOperandMemoryReference = false;
	bool destinationOperandMemoryReference = false;
	ExecutionResource destinationExecutionResource = ExecutionResource_None;
	DestinationOperandAccessType destinationOperandAccessType;
	
	M68060_ASSERT(opWordDecodeInfo, "No decoding pattern available for instruction");
	M68060_ASSERT(opWordDecodeInfo->readyForUOpDecoding, "Instruction doesn't support UOp decoding yet");

	opWordClassInfo = getOpWordClassInformation(opWordDecodeInfo->class);

	decodeIeeOperationSize(opWord, opWordClassInfo->sizeEncoding, &ieeOperationSize);

	splitSourceAndDestOperandReferences = shouldSplitSourceAndDestOperandReferences(opWord, opWordClassInfo->sourceDecodeOperand, opWordClassInfo->destinationDecodeOperand);

	if (opWordClassInfo->sourceDecodeOperand != DecodeOperand_None)
	{
			uint sourceExtensionWordsOffset = 1 + instructionLength->numSpecialOperandSpecifierWords;
	
			if (splitSourceAndDestOperandReferences)
			{
				UOp sourceUOp = { 0 };

				decodeOperand(opWord, opWordClassInfo->sourceDecodeOperand, ieeOperationSize, instructionWords + sourceExtensionWordsOffset, &sourceUOp, &sourceUOp.ieeA, UOpWriteBuffer, &sourceUOp.memoryRead);

				sourceUOp.ieeOperation = IeeOperation_ForwardIeeA;
				sourceUOp.ieeOperationSize = ieeOperationSize;
				sourceUOp.ieeResult = ExecutionResource_IeeTemp;
				sourceUOp.mnemonic = "SOURCEUOP";
				sourceUOp.pairability = Pairability_pOEP_Only;
				writeUOp(UOpWriteBuffer, &sourceUOp);

				mainUOp.ieeA = ExecutionResource_IeeTemp;
			}
			else
			{
				decodeOperand(opWord, opWordClassInfo->sourceDecodeOperand, ieeOperationSize, instructionWords + sourceExtensionWordsOffset, &mainUOp, &mainUOp.ieeA, UOpWriteBuffer, &sourceOperandMemoryReference);
			}
	}

	if (opWordClassInfo->destinationDecodeOperand != DecodeOperand_None)
	{
			uint destinationExtensionWordsOffset = 1 + instructionLength->numSpecialOperandSpecifierWords + instructionLength->numSourceEAExtensionWords;
			decodeOperand(opWord, opWordClassInfo->destinationDecodeOperand, ieeOperationSize, instructionWords + destinationExtensionWordsOffset, &mainUOp, &destinationExecutionResource, UOpWriteBuffer, &destinationOperandMemoryReference);
	}

	mainUOp.ieeOperation = opWordDecodeInfo->ieeOperation;
	mainUOp.ieeOperationSize = ieeOperationSize;

	destinationOperandAccessType = decodeDestinationOperandAccessType(opWordClassInfo->destinationOperandAccessType, destinationExecutionResource, ieeOperationSize);
	
	mainUOp.ieeB = decodeIeeB(destinationOperandAccessType, destinationExecutionResource);
	mainUOp.ieeResult = decodeIeeResult(destinationOperandAccessType, destinationExecutionResource);
	decodeMemoryReferences(destinationOperandAccessType, sourceOperandMemoryReference, destinationOperandMemoryReference, &mainUOp.memoryRead, &mainUOp.memoryWrite);
	
	mainUOp.mnemonic = opWordDecodeInfo->description;
	mainUOp.pairability = opWordDecodeInfo->pairability;

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
