
#include "M68060DecodeOpIntoUOps.h"
#include "../Types.h"

typedef struct 
{
	const char* description;
	uint numInstructionWords;
	uint16_t instructionWords[16];
	
	uint16_t numUOps;
	const UOp UOps[16];
	
} InstructionTestCase;

typedef struct
{
	const char* description;
	const InstructionTestCase* tests;
	uint numTests;
} TestSuite;

static const InstructionTestCase ea6BitTests[] =
{
	{ "ADD.B D0,D1", 1, { 0xd200, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D0,				ExecutionResource_D1,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D1,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.B D6,D0", 1, { 0xd006, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D6,				ExecutionResource_D0,				OperationSize_Byte,	IeeOperation_Add,			ExecutionResource_D0,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.W D0,D0", 1, { 0xd040, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D0,				ExecutionResource_D0,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D0,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.L D0,D0", 1, { 0xd080, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D0,				ExecutionResource_D0,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D0,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.B D0,D1", 1, { 0xd200, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D0,				ExecutionResource_D1,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D1,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.B D0,D2", 1, { 0xd400, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D0,				ExecutionResource_D2,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D2,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.B D0,D7", 1, { 0xde00, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D0,				ExecutionResource_D7,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D7,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.B D1,D0", 1, { 0xd001, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D1,				ExecutionResource_D0,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D0,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.B D2,D0", 1, { 0xd002, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D2,				ExecutionResource_D0,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D0,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },
	
	{ "ADD.B D3,D0", 1, { 0xd003, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D3,				ExecutionResource_D0,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D0,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.B D6,D0", 1, { 0xd006, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D6,				ExecutionResource_D0,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D0,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.W A3,D5", 1, { 0xda4b, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_A3,				ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.W (A3),D5", 1, { 0xda53, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.W (A3)+,D5", 1, { 0xda5b, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PostIncrement,			ExecutionResource_A3,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.W -(A3),D5", 1, { 0xda63, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrement,			ExecutionResource_A3,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.W ($1234,A3),D5", 2, { 0xda6b, 0x1234, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x1234, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.W $12(A1,D3.W*4),D5", 2, { 0xda71, 0x3412, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x0012, 0x0000, },	ExecutionResource_A1,				ExecutionResource_D3,				2,	AguIndexSize_Word,	AguDisplacementSize_S8,		AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.W $1234.W,D5", 2, { 0xda78, 0x1234, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x1234, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.W $12345678.L,D5", 3, { 0xda79, 0x1234, 0x5678, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x1234, 0x5678, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.W ($1234,PC),D5", 2, { 0xda7a, 0x1234, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x1234, 0x0000, },	ExecutionResource_PC,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.L $12(PC,D3.W*4),D5", 2, { 0xdabb, 0x3412, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x0012, 0x0000, },	ExecutionResource_PC,				ExecutionResource_D3,				2,	AguIndexSize_Word,	AguDisplacementSize_S8,		AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.W #$1234,D5", 2, { 0xda7c, 0x1234, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x1234, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpWord0,			ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },
	
	{ "ADD.L ($1234.W,A0,D0.L),D3", 3, { 0xd6b0, 0x0920, 0x1234, }, 2, {
																														{ "LEA",					{ 0x1234, 0x0000, },	ExecutionResource_A0,				ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.L ($12345678.L,A0,ZD0),D3", 4, { 0xd6b0, 0x0170, 0x1234, 0x5678, }, 2, {
																														{ "LEA",					{ 0x1234, 0x5678, },	ExecutionResource_A0,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.L ($12345678.L,A0,D0.L),D3", 4, { 0xd6b0, 0x0930, 0x1234, 0x5678, }, 2, {
																														{ "LEA",					{ 0x1234, 0x5678, },	ExecutionResource_A0,				ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.L ([$12345678.L,A0],D0.L),D3", 4, { 0xd6b0, 0x0935, 0x1234, 0x5678, }, 3, {
																														{ "LOAD",					{ 0x1234, 0x5678, },	ExecutionResource_A0,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LEA",					{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.L ([$12345678.L,A0],D0.L,$1234.W),D3", 5, { 0xd6b0, 0x0936, 0x1234, 0x5678, 0x1234, }, 3, {
																														{ "LOAD",					{ 0x1234, 0x5678, },	ExecutionResource_A0,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LEA",					{ 0x1234, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.L ([$12345678.L,A0],D0.L,$12345678.L),D3", 6, { 0xd6b0, 0x0937, 0x1234, 0x5678, 0x1234, 0x5678, }, 3, {
																														{ "LOAD",					{ 0x1234, 0x5678, },	ExecutionResource_A0,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LEA",					{ 0x1234, 0x5678, },	ExecutionResource_AguTemp,			ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.L ([$1234.W,A0],D0.L,$12345678.L),D3", 5, { 0xd6b0, 0x0927, 0x1234, 0x1234, 0x5678, }, 3, {
																														{ "LOAD",					{ 0x1234, 0x0000, },	ExecutionResource_A0,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LEA",					{ 0x1234, 0x5678, },	ExecutionResource_AguTemp,			ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },
	
	{ "ADD.L ([A0,D0.L],$12345678.L),D3", 4, { 0xd6b0, 0x0913, 0x1234, 0x5678, }, 3, {
																														{ "LOAD",					{ 0x0000, 0x0000, },	ExecutionResource_A0,				ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LEA",					{ 0x1234, 0x5678, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.W D5,(A3)", 1, { 0xdb53, }, 1, {
																														{ "ADD Dn,<ea>",			{ 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D5,				ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.W D5,(A3)+", 1, { 0xdb5b, }, 1, {
																														{ "ADD Dn,<ea>",			{ 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PostIncrement,			ExecutionResource_A3,				true,	ExecutionResource_D5,				ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADDA.W #$1234,A1", 2, { 0xd2fc, 0x1234, },  1, {
																														{ "ADDA.W <ea>,An",			{ 0x1234, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpWord0,			ExecutionResource_A1,				OperationSize_Word,	IeeOperation_AddA,			ExecutionResource_A1,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADDA.L #$12345678,A1", 3, { 0xd3fc, 0x1234, 0x5678, }, 1, {
																														{ "ADDA.L <ea>,An",			{ 0x1234, 0x5678, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpLong,			ExecutionResource_A1,				OperationSize_Long,	IeeOperation_AddA,			ExecutionResource_A1,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.L D3,(A1)+", 1, { 0xd799, }, 1, {
																														{ "ADD Dn,<ea>",			{ 0x0000, 0x0000, },	ExecutionResource_A1,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PostIncrement,			ExecutionResource_A1,				true,	ExecutionResource_D3,				ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.L D3,-(A2)", 1, { 0xd7a2, }, 1, {
																														{ "ADD Dn,<ea>",			{ 0x0000, 0x0000, },	ExecutionResource_A2,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrement,			ExecutionResource_A2,				true,	ExecutionResource_D3,				ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.W D3,($1234,A1)", 2, { 0xd769, 0x1234, }, 1, {
																														{ "ADD Dn,<ea>",			{ 0x1234, 0x0000, },	ExecutionResource_A1,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D3,				ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.W D5,$12(A1,D3.W*4)", 2, { 0xdb71, 0x3412, }, 1, {
																														{ "ADD Dn,<ea>",			{ 0x0012, 0x0000, },	ExecutionResource_A1,				ExecutionResource_D3,				2,	AguIndexSize_Word,	AguDisplacementSize_S8,		AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D5,				ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.B D1,$1234.W", 2, { 0xd338, 0x1234, }, 1, {
																														{ "ADD Dn,<ea>",			{ 0x1234, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D1,				ExecutionResource_MemoryOperand,	OperationSize_Byte, IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.W D1,$12345678.L", 3, { 0xd379, 0x1234, 0x5678, }, 1, {
																														{ "ADD Dn,<ea>",			{ 0x1234, 0x5678, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D1,				ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },
	
	{ "ADD.L D3,($12345678.L,A0,D0.L)", 4, { 0xd7b0, 0x0930, 0x1234, 0x5678, }, 2, {
																														{ "LEA",					{ 0x1234, 0x5678, },	ExecutionResource_A0,				ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "ADD Dn,<ea>",			{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D3,				ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },
	
	{ "ADD.L D3,([$12345678.L,A0],D0.L)", 4, { 0xd7b0, 0x0935, 0x1234, 0x5678, }, 3, {
																														{ "LOAD",					{ 0x1234, 0x5678, },	ExecutionResource_A0,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LEA",					{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "ADD Dn,<ea>",			{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D3,				ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.L D3,([$12345678.L,A0],D0.L,$1234.W)", 5, { 0xd7b0, 0x0936, 0x1234, 0x5678, 0x1234, }, 3, {
																														{ "LOAD",					{ 0x1234, 0x5678, },	ExecutionResource_A0,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LEA",					{ 0x1234, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "ADD Dn,<ea>",			{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D3,				ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.L D3,([$12345678.L,A0],D0.L,$12345678.L)", 6, { 0xd7b0, 0x0937, 0x1234, 0x5678, 0x1234, 0x5678, }, 3, {
																														{ "LOAD",					{ 0x1234, 0x5678, },	ExecutionResource_A0,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LEA",					{ 0x1234, 0x5678, },	ExecutionResource_AguTemp,			ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "ADD Dn,<ea>",			{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D3,				ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },
	
	{ "ADD.L D3,([$1234.W,A0],D0.L,$12345678.L)", 5, { 0xd7b0, 0x0927, 0x1234, 0x1234, 0x5678, }, 3, {
																														{ "LOAD",					{ 0x1234, 0x0000, },	ExecutionResource_A0,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LEA",					{ 0x1234, 0x5678, },	ExecutionResource_AguTemp,			ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "ADD Dn,<ea>",			{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D3,				ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADD.L D3,([A0,D0.L],$12345678.L)", 4, { 0xd7b0, 0x0913, 0x1234, 0x5678, }, 3, {
																														{ "LOAD",					{ 0x0000, 0x0000, },	ExecutionResource_A0,				ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LEA",					{ 0x1234, 0x5678, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "ADD Dn,<ea>",			{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D3,				ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

};

static const InstructionTestCase immediateTests[] =
{
	{ "ADDI.B #$12,D2", 2, { 0x0602, 0x0012, }, 1, {
																														{ "ADDI #imm,<ea>",			{ 0x0012, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpByte0,			ExecutionResource_D2,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D2,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADDI.W #$1234,D2", 2, { 0x0642, 0x1234, }, 1, {
																														{ "ADDI #imm,<ea>",			{ 0x1234, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpWord0,			ExecutionResource_D2,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D2,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADDI.L #$12345678,D2", 3, { 0x0682, 0x1234, 0x5678, }, 1, {
																														{ "ADDI #imm,<ea>",			{ 0x1234, 0x5678, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpLong,			ExecutionResource_D2,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D2,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADDI.W #$1234,$1234(A1,D2.L)", 4, { 0x0671, 0x1234, 0x2920, 0x1234, }, 3, {
																														{ "SOURCEUOP",				{ 0x1234, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpWord0,			ExecutionResource_None,				OperationSize_Word,	IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LEA",					{ 0x1234, 0x0000, },	ExecutionResource_A1,				ExecutionResource_D2,				0,	AguIndexSize_Long,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "ADDI #imm,<ea>",			{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADDI.W #$1234,$12345678.L", 4, { 0x0679, 0x1234, 0x1234, 0x5678, }, 2, {
																														{ "SOURCEUOP",				{ 0x1234, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpWord0,			ExecutionResource_None,				OperationSize_Word,	IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "ADDI #imm,<ea>",			{ 0x1234, 0x5678, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },
	
	{ "ADDI.W #$1234,([$12345678,A1,D2.L])", 6, { 0x0671, 0x1234, 0x2931, 0x1234, 0x5678, }, 4, {
																														{ "SOURCEUOP",				{ 0x1234, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpWord0,			ExecutionResource_None,				OperationSize_Word,	IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LOAD",					{ 0x1234, 0x5678, },	ExecutionResource_A1,				ExecutionResource_D2,				0,	AguIndexSize_Long,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LEA",					{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "ADDI #imm,<ea>",			{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADDQ.L #2,D3", 1, { 0x5483, }, 1, {
																														{ "ADDQ #imm,<ea>",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_Imm3Bit,			ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	2, },
	}, },

	{ "ADDQ.L #7,D3", 1, { 0x5e83, }, 1, {
																														{ "ADDQ #imm,<ea>",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_Imm3Bit,			ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	7, },
	}, },

	{ "ADDQ.L #8,D3", 1, { 0x5083, }, 1, {
																														{ "ADDQ #imm,<ea>",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_Imm3Bit,			ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	8, },
	}, },

};

static const InstructionTestCase integerArithmeticTests[] =
{
	{ "ADD.B D1,D3", 1, { 0xd601, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D1,				ExecutionResource_D3,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },
	
	{ "ADD.L D1,D3", 1, { 0xd681, }, 1, {
																														{ "ADD <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D1,				ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },
	
	{ "ADDA.W D4,A5", 1, { 0xdac4, }, 1, {
																														{ "ADDA.W <ea>,An",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D4,				ExecutionResource_A5,				OperationSize_Word,	IeeOperation_AddA,			ExecutionResource_A5,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },
	
	{ "ADDA.L (A2),A5", 1, { 0xdbd2, }, 1, {
																														{ "ADDA.L <ea>,An",			{ 0x0000, 0x0000, },	ExecutionResource_A2,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_A5,				OperationSize_Long,	IeeOperation_AddA,			ExecutionResource_A5,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "ADDQ.L #2,D3", 1, { 0x5483, }, 1, {
																														{ "ADDQ #imm,<ea>",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_Imm3Bit,			ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	2, },
	}, },

	{ "ADDQ.W #5,A3", 1, { 0x5a4b, }, 1, {
																														{ "ADDQ #imm,An",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_Imm3Bit,			ExecutionResource_A3,				OperationSize_Word,	IeeOperation_AddA,			ExecutionResource_A3,				false,	Pairability_pOEP_Or_sOEP,	2, },
	}, },
	
	{ "ADDQ.B #1,$12(A3,D2.L)", 2, { 0x5233, 0x2812, }, 1, {
																														{ "ADDQ #imm,<ea>",			{ 0x0012, 0x0000, },	ExecutionResource_A3,				ExecutionResource_D2,				0,	AguIndexSize_Long,	AguDisplacementSize_S8,		AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_Imm3Bit,			ExecutionResource_MemoryOperand,	OperationSize_Byte, IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	2, },
	}, },

	{ "ADDX.W D2,D3", 1, { 0xd742, }, 1, {
																														{ "ADDX Dx,Dy",				{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D2,				ExecutionResource_D3,				OperationSize_Word, IeeOperation_AddX,			ExecutionResource_D3,				false,	Pairability_pOEP_Only,	0,	},
	}, },

	{ "ADDX.L -(A1),-(A3)", 1, { 0xd789, }, 2, {
																														{ "SOURCEUOP",				{ 0x0000, 0x0000, },	ExecutionResource_A1,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrement,			ExecutionResource_A1,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long, IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,	0,	},
																														{ "ADDX -(Ax),-(Ay)",		{ 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrement,			ExecutionResource_A3,				true,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Long, IeeOperation_AddX,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Only,	0,	},
	}, },

	{ "CMP.L (A3),D1", 1, { 0xb293, }, 1, {
																														{ "CMP <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D1,				OperationSize_Long,	IeeOperation_Cmp,			ExecutionResource_None,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "CMPA.L D1,A1", 1, { 0xb3c1, }, 1, {
																														{ "CMPA.L <ea>,An",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D1,				ExecutionResource_A1,				OperationSize_Long,	IeeOperation_CmpA,			ExecutionResource_None,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "CMPI.L #$12345678,D1", 3, { 0x0c81, 0x1234, 0x5678, }, 1, {
																														{ "CMPI #imm,<ea>",			{ 0x1234, 0x5678, },	ExecutionResource_None,				ExecutionResource_None,				0, AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpLong,			ExecutionResource_D1,				OperationSize_Long,	IeeOperation_Cmp,			ExecutionResource_None,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "CMPM.B (A5)+,(A4)+", 1, { 0xb90d, }, 2, {
																														{ "SOURCEUOP",				{ 0x0000, 0x0000, },	ExecutionResource_A5,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PostIncrement,			ExecutionResource_A5,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Byte, IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "CMPM (Ax)+,(Ay)+",		{ 0x0000, 0x0000, },	ExecutionResource_A4,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PostIncrement,			ExecutionResource_A4,				true,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Byte, IeeOperation_Cmp,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
	}, },

	{ "CMPM.L (A3)+,(A2)+", 1, { 0xb58b, }, 2, {
																														{ "SOURCEUOP",				{ 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PostIncrement,			ExecutionResource_A3,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long, IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "CMPM (Ax)+,(Ay)+",		{ 0x0000, 0x0000, },	ExecutionResource_A2,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PostIncrement,			ExecutionResource_A2,				true,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Long, IeeOperation_Cmp,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
	}, },

	{ "NEG.W D7", 1, { 0x4447, }, 1, {
																														{ "NEG <ea>",				{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_None,				ExecutionResource_D7,				OperationSize_Word, IeeOperation_Neg,			ExecutionResource_D7,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "NEGX.L D5", 1, { 0x4085, }, 1, {
																														{ "NEGX <ea>",				{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_None,				ExecutionResource_D5,				OperationSize_Long, IeeOperation_NegX,			ExecutionResource_D5,				false,	Pairability_pOEP_Only,		0,	},
	}, },

	{ "SUB.B D1,D2", 1, { 0x9401, }, 1, {
																														{ "SUB <ea>,Dn",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D1,				ExecutionResource_D2,				OperationSize_Byte, IeeOperation_Sub,			ExecutionResource_D2,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "SUBA.W D4,A5", 1, { 0x9ac4, }, 1, {
																														{ "SUBA.W <ea>,An",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D4,				ExecutionResource_A5,				OperationSize_Word,	IeeOperation_SubA,			ExecutionResource_A5,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },
	
	{ "SUBA.W (A2),A5", 1, { 0x9ad2, }, 1, {
																														{ "SUBA.W <ea>,An",			{ 0x0000, 0x0000, },	ExecutionResource_A2,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_A5,				OperationSize_Word,	IeeOperation_SubA,			ExecutionResource_A5,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },
	
	{ "SUBA.L (A2),A5", 1, { 0x9bd2, }, 1, {
																														{ "SUBA.L <ea>,An",			{ 0x0000, 0x0000, },	ExecutionResource_A2,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_A5,				OperationSize_Long,	IeeOperation_SubA,			ExecutionResource_A5,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },
	
	{ "SUB.L D2,-(A6)", 1, { 0x95a6, }, 1, {
																														{ "SUB Dn,<ea>",			{ 0x0000, 0x0000, },	ExecutionResource_A6,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrement,			ExecutionResource_A6,				true,	ExecutionResource_D2,				ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Sub,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },
	
	{ "SUBI.W #$1234,D5", 2, { 0x0445, 0x1234, }, 1, {
																														{ "SUBI #imm,<ea>",			{ 0x1234, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpWord0,			ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Sub,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },
	
	{ "SUBQ.L #4,D0", 1, { 0x5980, }, 1, {
																														{ "SUBQ #imm,<ea>",			{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_Imm3Bit,			ExecutionResource_D0,				OperationSize_Long,	IeeOperation_Sub,			ExecutionResource_D0,				false,	Pairability_pOEP_Or_sOEP,	4, },
	}, },
	
	{ "SUBQ.B #3,([A5])", 2, { 0x5735, 0x0151, }, 3, {
																														{ "LOAD",					{ 0x0000, 0x0000, },	ExecutionResource_A5,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LEA",					{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "SUBQ #imm,<ea>",			{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_Imm3Bit,			ExecutionResource_MemoryOperand,	OperationSize_Byte, IeeOperation_Sub,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },
	
	{ "SUBX.W D1,D1", 1, { 0x9341, }, 1, {
																														{ "SUBX Dx,Dy",				{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D1,				ExecutionResource_D1,				OperationSize_Word, IeeOperation_SubX,			ExecutionResource_D1,				false,	Pairability_pOEP_Only,	0,	},
	}, },

	{ "SUBX.B -(A4),-(A3)", 1, { 0x970c, }, 2, {
																														{ "SOURCEUOP",				{ 0x0000, 0x0000, },	ExecutionResource_A4,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrement,			ExecutionResource_A4,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Byte, IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,	0,	},
																														{ "SUBX -(Ax),-(Ay)",		{ 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrement,			ExecutionResource_A3,				true,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Byte, IeeOperation_SubX,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Only,	0,	},
	}, },
};

static const InstructionTestCase moveTests[] =
{
	{ "MOVE.W D3,D4", 1, { 0x3803, }, 1, {
																														{ "MOVE.W <ea>,<ea>",		{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D3,				ExecutionResource_D4,				OperationSize_Word, IeeOperation_Move,			ExecutionResource_D4,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "MOVE.B #$12,D2", 2, { 0x143c, 0x0012, }, 1, {
																														{ "MOVE.B <ea>,<ea>",		{ 0x0012, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpByte0,			ExecutionResource_D2,				OperationSize_Byte, IeeOperation_Move,			ExecutionResource_D2,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "MOVE.L #$12345678,(A6)", 3, { 0x2cbc, 0x1234, 0x5678, }, 1, {
																														{ "MOVE.L <ea>,<ea>",		{ 0x1234, 0x5678, },	ExecutionResource_A6,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				false,	ExecutionResource_uOpLong,			ExecutionResource_None,				OperationSize_Long, IeeOperation_Move,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "MOVE.W (A7),D5", 1, { 0x3a17, }, 1, {
																														{ "MOVE.W <ea>,<ea>",		{ 0x0000, 0x0000, },	ExecutionResource_A7,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word, IeeOperation_Move,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "MOVE.L #$12345678,([$12345678.L,A1])", 6, { 0x23bc, 0x1234, 0x5678, 0x0171, 0x1234, 0x5678, }, 4, {
																														{ "SOURCEUOP",				{ 0x1234, 0x5678, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpLong,			ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LOAD",					{ 0x1234, 0x5678, },	ExecutionResource_A1,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LEA",					{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "MOVE.L <ea>,<ea>",		{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				false,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Move,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "MOVE.L ([$12345678.L,A1]),([$12345678.L,A2])", 7, { 0x25b1, 0x0171, 0x1234, 0x5678, 0x0171, 0x1234, 0x5678, }, 6, {
																														{ "LOAD",					{ 0x1234, 0x5678, },	ExecutionResource_A1,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LEA",					{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "SOURCEUOP",				{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LOAD",					{ 0x1234, 0x5678, },	ExecutionResource_A2,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LEA",					{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "MOVE.L <ea>,<ea>",		{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				false,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Move,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "MOVE.L ([$12345678.L,PC]),([D0.W],$12345678.L)", 7, { 0x21bb, 0x0171, 0x1234, 0x5678, 0x0193, 0x1234, 0x5678, }, 6, {
																														{ "LOAD",					{ 0x1234, 0x5678, },	ExecutionResource_PC,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LEA",					{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "SOURCEUOP",				{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LOAD",					{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_D0,				0,	AguIndexSize_Word,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		0,	},
																														{ "LEA",					{ 0x1234, 0x5678, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		0,	},
																														{ "MOVE.L <ea>,<ea>",		{ 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				false,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Move,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "MOVEA.W D2,A2", 1, { 0x3442, }, 1, {
																														{ "MOVEA.W <ea>,<ea>",		{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D2,				ExecutionResource_A2,				OperationSize_Word, IeeOperation_MoveA,			ExecutionResource_A2,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },

	{ "MOVE.L A2,D3", 1, { 0x260a, }, 1, {
																														{ "MOVE.L <ea>,<ea>",		{ 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_A2,				ExecutionResource_D3,				OperationSize_Long, IeeOperation_Move,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	0,	},
	}, },
};


TestSuite testSuites[] =
{
	{ "6-bit EA decoding tests", ea6BitTests, (sizeof ea6BitTests / sizeof ea6BitTests[0]) },
	{ "Immediate source operand tests", immediateTests, (sizeof immediateTests / sizeof immediateTests[0]) },
	{ "Integer arithmetic tests", integerArithmeticTests, (sizeof integerArithmeticTests / sizeof integerArithmeticTests[0]) },
//	{ "Move/Exchange tests", moveTests, (sizeof moveTests / sizeof moveTests[0]) },
};

bool areUOpsEquivalent(const UOp* a, const UOp* b)
{
	return
		!strcmp(a->mnemonic, b->mnemonic)
		&& a->extensionWords[0] == b->extensionWords[0]
		&& a->extensionWords[1] == b->extensionWords[1]
		&& a->aguBase == b->aguBase
		&& a->aguIndex == b->aguIndex
		&& a->aguIndexShift == b->aguIndexShift
		&& a->aguIndexSize == b->aguIndexSize
		&& a->aguDisplacementSize == b->aguDisplacementSize
		&& a->aguOperation == b->aguOperation
		&& a->aguResult == b->aguResult
		&& a->memoryRead == b->memoryRead
		&& a->ieeA == b->ieeA
		&& a->ieeB == b->ieeB
		&& a->ieeOperationSize == b->ieeOperationSize
		&& a->ieeOperation == b->ieeOperation
		&& a->ieeResult == b->ieeResult
		&& a->memoryWrite == b->memoryWrite
		&& a->pairability == b->pairability;
}

void printUOp(uint id, const UOp* UOp)
{
	printf("    UOp %d: %s\n", id, UOp->mnemonic);
	printf("      ExtensionWords: %04x,%04x\n", UOp->extensionWords[0], UOp->extensionWords[1]);
	printf("      Agu: Base %s, Index %s, IndexShift %d, IndexSize %s, DisplacementSize %s, Operation %s, Result %s\n",
		ExecutionResourceToString(UOp->aguBase), ExecutionResourceToString(UOp->aguIndex),
		UOp->aguIndexShift, AguIndexSizeToString(UOp->aguIndexSize), AguDisplacementSizeToString(UOp->aguDisplacementSize),
		AguOperationToString(UOp->aguOperation), ExecutionResourceToString(UOp->aguResult));
	printf("      MemoryRead: %s\n", UOp->memoryRead ? "yes" : "no");
	printf("      Iee: A %s, B %s, OperationSize %s, Operation %s, Result %s\n",
		ExecutionResourceToString(UOp->ieeA), ExecutionResourceToString(UOp->ieeB),
		OperationSizeToString(UOp->ieeOperationSize), IeeOperationToString(UOp->ieeOperation), ExecutionResourceToString(UOp->ieeResult));
	printf("      MemoryWrite: %s\n", UOp->memoryWrite ? "yes" : "no");
	printf("      Pairability: %s\n", PairabilityToString(UOp->pairability));
}

void runTestSuite(const InstructionTestCase* tests, uint numTests, bool printSuccess, bool printFailure, uint* accumulatedSuccessfulTests, uint* accumulatedTotalTests)
{
	uint numSuccessfulTests = 0;
	uint numTotalTests = 0;
	uint testId;
	
	for (testId = 0; testId < numTests; ++testId)
	{
		bool success = true;

		const InstructionTestCase* InstructionTestCase = &tests[testId];
		UOp UOps[16];
		uint numUOps;
		bool decodeSuccess = decomposeOpIntoUOps(InstructionTestCase->instructionWords, InstructionTestCase->numInstructionWords, UOps, &numUOps);

		if (!decodeSuccess)
		{
			success = false;
			if (printFailure)
				printf("failure: unable to decode %s\n", InstructionTestCase->description);
		}
		else
		{
			bool decodedUOpsMatchReference = true;
			int UOp;

			if (numUOps != InstructionTestCase->numUOps)
				decodedUOpsMatchReference = false;
			else
			{
				for (UOp = 0; UOp < numUOps; ++UOp)
				{
					if (!areUOpsEquivalent(&UOps[UOp], &InstructionTestCase->UOps[UOp]))
						decodedUOpsMatchReference = false;
				}
			}
				
			if (decodedUOpsMatchReference)
			{
				if (printSuccess)
					printf("success: decoded %s into %d UOps properly\n", InstructionTestCase->description, InstructionTestCase->numUOps);
			}
			else
			{
				success = false;
				if (printFailure)
				{
					printf("failure: decoding %s should yield the following UOps:\n", InstructionTestCase->description);

					for (UOp = 0; UOp < InstructionTestCase->numUOps; ++UOp)
						printUOp(UOp, &InstructionTestCase->UOps[UOp]);

					printf("  but yielded the following UOps:\n");

					for (UOp = 0; UOp < numUOps; ++UOp)
						printUOp(UOp, &UOps[UOp]);
				}
			}
		}

		if (success)
			numSuccessfulTests++;
		numTotalTests++;
	}

	*accumulatedSuccessfulTests += numSuccessfulTests;
	*accumulatedTotalTests += numTotalTests;
}


int main(void)
{
	bool printSuccess = true;
	bool printFailure = true;
	
	uint numSuccessfulTests = 0;
	uint numTotalTests = 0;

	uint suite = 0;

	printf("Testing M68060 Op -> UOp decoder\n");
	printf("\n");

	for (suite = 0; suite < (sizeof testSuites / sizeof testSuites[0]); ++suite)
	{
		printf("Running %s\n", testSuites[suite].description);
		runTestSuite(testSuites[suite].tests, testSuites[suite].numTests, printSuccess, printFailure, &numSuccessfulTests, &numTotalTests);
	}

	printf("\n");
	printf("%u out of %u tests succeeded.\n", numSuccessfulTests, numTotalTests);
	
	return 0;
}