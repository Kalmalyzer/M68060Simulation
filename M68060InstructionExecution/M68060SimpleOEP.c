
#include "M68060SimpleOEP.h"
#include "M68060IntegerRegisterFile.h"
#include "M68060AguAlu.h"
#include "M68060IeeAlu.h"
#include "DummyMemorySubSystem.h"
#include "../M68060InstructionDecoder/M68060InstructionDecoderTypes.h"
#include "../Assert.h"


static uint32_t s_aguResult;
static uint32_t s_memoryOperand;
static uint32_t s_aguTemp;
static uint32_t s_immediateTemp;

static uint32_t readExecutionResource(ExecutionResource executionResource, const UOp* UOp)
{
	switch (executionResource)
	{
		case ExecutionResource_None:
			return 0;
		case ExecutionResource_D0:
		case ExecutionResource_D1:
		case ExecutionResource_D2:
		case ExecutionResource_D3:
		case ExecutionResource_D4:
		case ExecutionResource_D5:
		case ExecutionResource_D6:
		case ExecutionResource_D7:
		case ExecutionResource_A0:
		case ExecutionResource_A1:
		case ExecutionResource_A2:
		case ExecutionResource_A3:
		case ExecutionResource_A4:
		case ExecutionResource_A5:
		case ExecutionResource_A6:
		case ExecutionResource_A7:
			return readIntegerRegister(executionResource);
		case ExecutionResource_AguResult:
			return s_aguResult;
		case ExecutionResource_MemoryOperand:
			return s_memoryOperand;
		case ExecutionResource_uOpByte0:
			return (uint32_t) (int32_t) (int8_t) UOp->extensionWords[0];
		case ExecutionResource_uOpWord0:
			return (uint32_t) (int32_t) (int16_t) UOp->extensionWords[0];
		case ExecutionResource_uOpLong:
			return (UOp->extensionWords[0] << 16) + UOp->extensionWords[1];
		case ExecutionResource_AguTemp:
			return s_aguTemp;
		case ExecutionResource_ImmediateTemp:
			return s_immediateTemp;
		case ExecutionResource_Imm3Bit:
			return UOp->imm3Bit;
		default:
			M68060_ERROR("ExecutionResource not supported");
			return 0;
	}
}

static void writeExecutionResource(ExecutionResource executionResource, uint32_t value)
{
	switch (executionResource)
	{
		case ExecutionResource_None:
			break;
		case ExecutionResource_D0:
		case ExecutionResource_D1:
		case ExecutionResource_D2:
		case ExecutionResource_D3:
		case ExecutionResource_D4:
		case ExecutionResource_D5:
		case ExecutionResource_D6:
		case ExecutionResource_D7:
		case ExecutionResource_A0:
		case ExecutionResource_A1:
		case ExecutionResource_A2:
		case ExecutionResource_A3:
		case ExecutionResource_A4:
		case ExecutionResource_A5:
		case ExecutionResource_A6:
		case ExecutionResource_A7:
			writeIntegerRegister(executionResource, value);
			break;
		case ExecutionResource_AguTemp:
			s_aguTemp = value;
			break;
		case ExecutionResource_ImmediateTemp:
			s_immediateTemp = value;
			break;
		case ExecutionResource_MemoryOperand:
			M68060_WARNING("Ignored MemoryOperand write for now");
			break;
		case ExecutionResource_AguResult:
			M68060_WARNING("Ignored AguResult write for now");
			break;
		default:
			M68060_ERROR("ExecutionResource not supported");
			break;
	}
}

static ExecutionResource translateAguDisplacementSizeToExecutionResource(AguDisplacementSize aguDisplacementSize)
{
	switch (aguDisplacementSize)
	{
		case AguDisplacementSize_None:
			return ExecutionResource_None;
		case AguDisplacementSize_S8:
			return ExecutionResource_uOpByte0;
		case AguDisplacementSize_S16:
			return ExecutionResource_uOpWord0;
		case AguDisplacementSize_S32:
			return ExecutionResource_uOpLong;
		default:
			M68060_ERROR("Unsupported AguDisplacementSize");
			return ExecutionResource_None;
	}
}

void executeUOp(const UOp* UOp)
{
	uint32_t memoryReference = 0;
	uint32_t ieeResultValue = 0;

	// AG stage
	
	{
		uint32_t aguBaseValue = readExecutionResource(UOp->aguBase, UOp);
		uint32_t aguIndexValue = readExecutionResource(UOp->aguIndex, UOp);
		uint32_t aguDisplacementValue = readExecutionResource(translateAguDisplacementSizeToExecutionResource(UOp->aguDisplacementSize), UOp);
		evaluateAguAluOperation(UOp->aguOperation, UOp->ieeOperationSize, aguBaseValue, aguIndexValue, UOp->aguIndexShift, UOp->aguIndexSize,
			aguDisplacementValue, UOp->aguDisplacementSize, &s_aguResult, &memoryReference);
	}
		
	// OC stage

	if (UOp->memoryRead)
		s_memoryOperand = readMemory(memoryReference, UOp->ieeOperationSize);

	// EX stage
	
	{
		uint32_t ieeAValue = readExecutionResource(UOp->ieeA, UOp);
		uint32_t ieeBValue = readExecutionResource(UOp->ieeB, UOp);
		evaluateIeeAluOperation(UOp->ieeOperation, UOp->ieeOperationSize, ieeAValue, ieeBValue, &ieeResultValue);
	}

	// WB stage
	
	if (UOp->memoryWrite)
		writeMemory(memoryReference, UOp->ieeOperationSize, ieeResultValue);
		
	if (UOp->aguResult != ExecutionResource_None)
		writeExecutionResource(UOp->aguResult, s_aguResult);
	if (UOp->ieeResult != ExecutionResource_None)
		writeExecutionResource(UOp->ieeResult, ieeResultValue);

}
