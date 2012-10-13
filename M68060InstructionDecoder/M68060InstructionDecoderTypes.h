
#ifndef M68060INSTRUCTIONDECODERTYPES_H
#define M68060INSTRUCTIONDECODERTYPES_H

typedef enum
{
	ExecutionResource_None,
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
	ExecutionResource_AguResult,
	ExecutionResource_MemoryOperand,
	ExecutionResource_ImmediateOperand,
	ExecutionResource_AbsoluteMemoryAddress,
	ExecutionResource_PC,
} ExecutionResource;

const char* ExecutionResourceToString(ExecutionResource executionResource);

typedef enum
{
	PairingType_pOEP_Or_sOEP,
	PairingType_pOEP_But_Allows_sOEP,
	PairingType_pOEP_Only,
	PairingType_pOEP_Until_Last
} PairingType;

typedef enum
{
	OpMode_None,
	OpMode_EaToRegister_Data_Byte,
	OpMode_EaToRegister_Data_Word,
	OpMode_EaToRegister_Data_Long,
	OpMode_EaToRegister_Address_WordWithSignExtension,
	OpMode_RegisterToEa_Data_Byte,
	OpMode_RegisterToEa_Data_Word,
	OpMode_RegisterToEa_Data_Long,
	OpMode_RegisterToEa_Address_Long,
} OpMode;

const char* OpModeToString(OpMode opMode);

#endif
