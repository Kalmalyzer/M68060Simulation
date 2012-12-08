
#ifndef M68060INSTRUCTIONDECODERTYPES_H
#define M68060INSTRUCTIONDECODERTYPES_H

#include "../Types.h"

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
	ExecutionResource_ImmediateOperand, // Deprecated
	ExecutionResource_AbsoluteMemoryAddress, // Deprecated
	ExecutionResource_PC,
	ExecutionResource_uOpWord0,
	ExecutionResource_uOpWord1,
	ExecutionResource_uOpLong,
	ExecutionResource_AguTemp,
	ExecutionResource_Unknown,

} ExecutionResource;

const char* ExecutionResourceToString(ExecutionResource executionResource);

bool isRegister(ExecutionResource executionResource);

typedef enum
{
	Pairability_pOEP_Or_sOEP,
	Pairability_pOEP_But_Allows_sOEP,
	Pairability_pOEP_Only,
	Pairability_pOEP_Until_Last,

} Pairability;

const char* PairabilityToString(Pairability pairability);

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
	OpMode_EaToRegister_Address_Long,

} OpMode;

const char* OpModeToString(OpMode opMode);

typedef enum
{
	AguOperation_None,
	AguOperation_PostIncrement,
	AguOperation_PreDecrement,
	AguOperation_OffsetBaseIndexScale,

} AguOperation;

const char* AguOperationToString(AguOperation aguOperation);

typedef enum
{
	AguIndexSize_Word,
	AguIndexSize_Long,

} AguIndexSize;

const char* AguIndexSizeToString(AguIndexSize aguIndexSize);

typedef enum
{
	AguDisplacementSize_None,
	AguDisplacementSize_S8,
	AguDisplacementSize_S16,
	AguDisplacementSize_S32,

} AguDisplacementSize;

const char* AguDisplacementSizeToString(AguDisplacementSize aguDisplacementSize);

typedef enum
{
	IeeImmediate_None,
	IeeImmediate_D8,
	IeeImmediate_D16,
	IeeImmediate_D32,

} IeeImmediate;

const char* IeeImmediateToString(IeeImmediate ieeImmediate);

#endif
