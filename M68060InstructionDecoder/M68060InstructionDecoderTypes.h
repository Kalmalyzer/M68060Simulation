
#ifndef M68060INSTRUCTIONDECODERTYPES_H
#define M68060INSTRUCTIONDECODERTYPES_H

#include "../Types.h"

typedef enum
{
	ExecutionResource_None,

	ExecutionResource_D0,	// Contents of integer register file
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

	ExecutionResource_AguResult, // Output from AGU

	ExecutionResource_MemoryOperand, // Result from OC stage
	ExecutionResource_PC, // Contents of PC register

	ExecutionResource_uOpByte0, // Lower byte of first uOP extension word
	ExecutionResource_uOpWord0, // First uOP extension word
	ExecutionResource_uOpWord1, // Second uOP extension word
	ExecutionResource_uOpLong, // Both uOP extension words, seen as a 32-bit value

	ExecutionResource_AguTemp, // Temporary register used when decomposing complex addressing modes into multiple uOPs
	ExecutionResource_ImmediateTemp, // Temporary register used when decomposing instructions with both immediates and displacements into multiple uOPs

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
	AguOperation_None,
	AguOperation_PostIncrement,
	AguOperation_PreDecrement,
	AguOperation_OffsetBaseIndexScale,

} AguOperation;

const char* AguOperationToString(AguOperation aguOperation);

typedef enum
{
	AguIndexSize_None,
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
	OperationSize_None,
	OperationSize_Byte,
	OperationSize_Word,
	OperationSize_Long,

} OperationSize;

const char* OperationSizeToString(OperationSize operationSize);

typedef enum
{
	IeeOperation_None,
	IeeOperation_Add,
	IeeOperation_AddA,

} IeeOperation;

const char* IeeOperationToString(IeeOperation ieeOperation);

#endif
