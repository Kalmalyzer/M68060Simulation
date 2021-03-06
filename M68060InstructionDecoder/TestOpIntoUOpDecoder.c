
#include "M68060DecodeOpIntoUOps.h"
#include "../Types.h"

#include <stdio.h>

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
																														{ "ADD <ea>,Dn",			0xd200, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D0,				ExecutionResource_D1,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D1,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.B D6,D0", 1, { 0xd006, }, 1, {
																														{ "ADD <ea>,Dn",			0xd006, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D6,				ExecutionResource_D0,				OperationSize_Byte,	IeeOperation_Add,			ExecutionResource_D0,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.W D0,D0", 1, { 0xd040, }, 1, {
																														{ "ADD <ea>,Dn",			0xd040, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D0,				ExecutionResource_D0,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D0,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.L D0,D0", 1, { 0xd080, }, 1, {
																														{ "ADD <ea>,Dn",			0xd080, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D0,				ExecutionResource_D0,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D0,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.B D0,D1", 1, { 0xd200, }, 1, {
																														{ "ADD <ea>,Dn",			0xd200, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D0,				ExecutionResource_D1,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D1,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.B D0,D2", 1, { 0xd400, }, 1, {
																														{ "ADD <ea>,Dn",			0xd400, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D0,				ExecutionResource_D2,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D2,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.B D0,D7", 1, { 0xde00, }, 1, {
																														{ "ADD <ea>,Dn",			0xde00, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D0,				ExecutionResource_D7,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D7,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.B D1,D0", 1, { 0xd001, }, 1, {
																														{ "ADD <ea>,Dn",			0xd001, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D1,				ExecutionResource_D0,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D0,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.B D2,D0", 1, { 0xd002, }, 1, {
																														{ "ADD <ea>,Dn",			0xd002, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D2,				ExecutionResource_D0,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D0,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "ADD.B D3,D0", 1, { 0xd003, }, 1, {
																														{ "ADD <ea>,Dn",			0xd003, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D3,				ExecutionResource_D0,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D0,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.B D6,D0", 1, { 0xd006, }, 1, {
																														{ "ADD <ea>,Dn",			0xd006, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D6,				ExecutionResource_D0,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D0,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.W A3,D5", 1, { 0xda4b, }, 1, {
																														{ "ADD <ea>,Dn",			0xda4b, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_A3,				ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.W (A3),D5", 1, { 0xda53, }, 1, {
																														{ "ADD <ea>,Dn",			0xda53, { 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.W (A3)+,D5", 1, { 0xda5b, }, 1, {
																														{ "ADD <ea>,Dn",			0xda5b, { 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PostIncrement,			ExecutionResource_A3,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.W -(A3),D5", 1, { 0xda63, }, 1, {
																														{ "ADD <ea>,Dn",			0xda63, { 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrement,			ExecutionResource_A3,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.W ($1234,A3),D5", 2, { 0xda6b, 0x1234, }, 1, {
																														{ "ADD <ea>,Dn",			0xda6b, { 0x1234, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.W $12(A1,D3.W*4),D5", 2, { 0xda71, 0x3412, }, 1, {
																														{ "ADD <ea>,Dn",			0xda71, { 0x0012, 0x0000, },	ExecutionResource_A1,				ExecutionResource_D3,				2,	AguIndexSize_Word,	AguDisplacementSize_S8,		AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.W $1234.W,D5", 2, { 0xda78, 0x1234, }, 1, {
																														{ "ADD <ea>,Dn",			0xda78, { 0x1234, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.W $12345678.L,D5", 3, { 0xda79, 0x1234, 0x5678, }, 1, {
																														{ "ADD <ea>,Dn",			0xda79, { 0x1234, 0x5678, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.W ($1234,PC),D5", 2, { 0xda7a, 0x1234, }, 1, {
																														{ "ADD <ea>,Dn",			0xda7a, { 0x1234, 0x0000, },	ExecutionResource_PC,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.L $12(PC,D3.W*4),D5", 2, { 0xdabb, 0x3412, }, 1, {
																														{ "ADD <ea>,Dn",			0xdabb, { 0x0012, 0x0000, },	ExecutionResource_PC,				ExecutionResource_D3,				2,	AguIndexSize_Word,	AguDisplacementSize_S8,		AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.W #$1234,D5", 2, { 0xda7c, 0x1234, }, 1, {
																														{ "ADD <ea>,Dn",			0xda7c, { 0x1234, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpWord0,			ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "ADD.L ($1234.W,A0,D0.L),D3", 3, { 0xd6b0, 0x0920, 0x1234, }, 2, {
																														{ "LEA",					0x0000, { 0x1234, 0x0000, },	ExecutionResource_A0,				ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "ADD <ea>,Dn",			0xd6b0, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.L ($12345678.L,A0,ZD0),D3", 4, { 0xd6b0, 0x0170, 0x1234, 0x5678, }, 2, {
																														{ "LEA",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_A0,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "ADD <ea>,Dn",			0xd6b0, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.L ($12345678.L,A0,D0.L),D3", 4, { 0xd6b0, 0x0930, 0x1234, 0x5678, }, 2, {
																														{ "LEA",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_A0,				ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "ADD <ea>,Dn",			0xd6b0, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.L ([$12345678.L,A0],D0.L),D3", 4, { 0xd6b0, 0x0935, 0x1234, 0x5678, }, 3, {
																														{ "LOAD",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_A0,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "ADD <ea>,Dn",			0xd6b0, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.L ([$12345678.L,A0],D0.L,$1234.W),D3", 5, { 0xd6b0, 0x0936, 0x1234, 0x5678, 0x1234, }, 3, {
																														{ "LOAD",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_A0,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x1234, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "ADD <ea>,Dn",			0xd6b0, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.L ([$12345678.L,A0],D0.L,$12345678.L),D3", 6, { 0xd6b0, 0x0937, 0x1234, 0x5678, 0x1234, 0x5678, }, 3, {
																														{ "LOAD",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_A0,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_AguTemp,			ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "ADD <ea>,Dn",			0xd6b0, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.L ([$1234.W,A0],D0.L,$12345678.L),D3", 5, { 0xd6b0, 0x0927, 0x1234, 0x1234, 0x5678, }, 3, {
																														{ "LOAD",					0x0000, { 0x1234, 0x0000, },	ExecutionResource_A0,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_AguTemp,			ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "ADD <ea>,Dn",			0xd6b0, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "ADD.L ([A0,D0.L],$12345678.L),D3", 4, { 0xd6b0, 0x0913, 0x1234, 0x5678, }, 3, {
																														{ "LOAD",					0x0000, { 0x0000, 0x0000, },	ExecutionResource_A0,				ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "ADD <ea>,Dn",			0xd6b0, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.W D5,(A3)", 1, { 0xdb53, }, 1, {
																														{ "ADD Dn,<ea>",			0xdb53, { 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D5,				ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.W D5,(A3)+", 1, { 0xdb5b, }, 1, {
																														{ "ADD Dn,<ea>",			0xdb5b, { 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PostIncrement,			ExecutionResource_A3,				true,	ExecutionResource_D5,				ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADDA.W #$1234,A1", 2, { 0xd2fc, 0x1234, },  1, {
																														{ "ADDA.W <ea>,An",			0xd2fc, { 0x1234, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpWord0,			ExecutionResource_A1,				OperationSize_Word,	IeeOperation_AddA,			ExecutionResource_A1,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADDA.L #$12345678,A1", 3, { 0xd3fc, 0x1234, 0x5678, }, 1, {
																														{ "ADDA.L <ea>,An",			0xd3fc, { 0x1234, 0x5678, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpLong,			ExecutionResource_A1,				OperationSize_Long,	IeeOperation_AddA,			ExecutionResource_A1,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.L D3,(A1)+", 1, { 0xd799, }, 1, {
																														{ "ADD Dn,<ea>",			0xd799, { 0x0000, 0x0000, },	ExecutionResource_A1,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PostIncrement,			ExecutionResource_A1,				true,	ExecutionResource_D3,				ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.L D3,-(A2)", 1, { 0xd7a2, }, 1, {
																														{ "ADD Dn,<ea>",			0xd7a2, { 0x0000, 0x0000, },	ExecutionResource_A2,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrement,			ExecutionResource_A2,				true,	ExecutionResource_D3,				ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.W D3,($1234,A1)", 2, { 0xd769, 0x1234, }, 1, {
																														{ "ADD Dn,<ea>",			0xd769, { 0x1234, 0x0000, },	ExecutionResource_A1,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D3,				ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.W D5,$12(A1,D3.W*4)", 2, { 0xdb71, 0x3412, }, 1, {
																														{ "ADD Dn,<ea>",			0xdb71, { 0x0012, 0x0000, },	ExecutionResource_A1,				ExecutionResource_D3,				2,	AguIndexSize_Word,	AguDisplacementSize_S8,		AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D5,				ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.B D1,$1234.W", 2, { 0xd338, 0x1234, }, 1, {
																														{ "ADD Dn,<ea>",			0xd338, { 0x1234, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D1,				ExecutionResource_MemoryOperand,	OperationSize_Byte, IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.W D1,$12345678.L", 3, { 0xd379, 0x1234, 0x5678, }, 1, {
																														{ "ADD Dn,<ea>",			0xd379, { 0x1234, 0x5678, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D1,				ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "ADD.L D3,($12345678.L,A0,D0.L)", 4, { 0xd7b0, 0x0930, 0x1234, 0x5678, }, 2, {
																														{ "LEA",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_A0,				ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "ADD Dn,<ea>",			0xd7b0, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D3,				ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "ADD.L D3,([$12345678.L,A0],D0.L)", 4, { 0xd7b0, 0x0935, 0x1234, 0x5678, }, 3, {
																														{ "LOAD",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_A0,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "ADD Dn,<ea>",			0xd7b0, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D3,				ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.L D3,([$12345678.L,A0],D0.L,$1234.W)", 5, { 0xd7b0, 0x0936, 0x1234, 0x5678, 0x1234, }, 3, {
																														{ "LOAD",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_A0,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x1234, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "ADD Dn,<ea>",			0xd7b0, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D3,				ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.L D3,([$12345678.L,A0],D0.L,$12345678.L)", 6, { 0xd7b0, 0x0937, 0x1234, 0x5678, 0x1234, 0x5678, }, 3, {
																														{ "LOAD",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_A0,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_AguTemp,			ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "ADD Dn,<ea>",			0xd7b0, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D3,				ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "ADD.L D3,([$1234.W,A0],D0.L,$12345678.L)", 5, { 0xd7b0, 0x0927, 0x1234, 0x1234, 0x5678, }, 3, {
																														{ "LOAD",					0x0000, { 0x1234, 0x0000, },	ExecutionResource_A0,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_AguTemp,			ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "ADD Dn,<ea>",			0xd7b0, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D3,				ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.L D3,([A0,D0.L],$12345678.L)", 4, { 0xd7b0, 0x0913, 0x1234, 0x5678, }, 3, {
																														{ "LOAD",					0x0000, { 0x0000, 0x0000, },	ExecutionResource_A0,				ExecutionResource_D0,				0,	AguIndexSize_Long,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "ADD Dn,<ea>",			0xd7b0, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D3,				ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.W (SP)+,D5", 1, { 0xda5f, }, 1, {
																														{ "ADD <ea>,Dn",			0xda5f, { 0x0000, 0x0000, },	ExecutionResource_A7,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PostIncrementSP,		ExecutionResource_A7,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADD.W -(SP),D5", 1, { 0xda67, }, 1, {
																														{ "ADD <ea>,Dn",			0xda67, { 0x0000, 0x0000, },	ExecutionResource_A7,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrementSP,		ExecutionResource_A7,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

};

static const InstructionTestCase immediateTests[] =
{
	{ "ADDI.B #$12,D2", 2, { 0x0602, 0x0012, }, 1, {
																														{ "ADDI #imm,<ea>",			0x0602, { 0x0012, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpByte0,			ExecutionResource_D2,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D2,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADDI.W #$1234,D2", 2, { 0x0642, 0x1234, }, 1, {
																														{ "ADDI #imm,<ea>",			0x0642, { 0x1234, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpWord0,			ExecutionResource_D2,				OperationSize_Word,	IeeOperation_Add,			ExecutionResource_D2,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADDI.L #$12345678,D2", 3, { 0x0682, 0x1234, 0x5678, }, 1, {
																														{ "ADDI #imm,<ea>",			0x0682, { 0x1234, 0x5678, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpLong,			ExecutionResource_D2,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D2,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADDI.W #$1234,$1234(A1,D2.L)", 4, { 0x0671, 0x1234, 0x2920, 0x1234, }, 3, {
																														{ "SOURCEUOP",				0x0000, { 0x1234, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpWord0,			ExecutionResource_None,				OperationSize_Word,	IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x1234, 0x0000, },	ExecutionResource_A1,				ExecutionResource_D2,				0,	AguIndexSize_Long,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "ADDI #imm,<ea>",			0x0671, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADDI.W #$1234,$12345678.L", 4, { 0x0679, 0x1234, 0x1234, 0x5678, }, 2, {
																														{ "SOURCEUOP",				0x0000, { 0x1234, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpWord0,			ExecutionResource_None,				OperationSize_Word,	IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		},
																														{ "ADDI #imm,<ea>",			0x0679, { 0x1234, 0x5678, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "ADDI.W #$1234,([$12345678,A1,D2.L])", 6, { 0x0671, 0x1234, 0x2931, 0x1234, 0x5678, }, 4, {
																														{ "SOURCEUOP",				0x0000, { 0x1234, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpWord0,			ExecutionResource_None,				OperationSize_Word,	IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LOAD",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_A1,				ExecutionResource_D2,				0,	AguIndexSize_Long,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "ADDI #imm,<ea>",			0x0671, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADDQ.L #2,D3", 1, { 0x5483, }, 1, {
																														{ "ADDQ #imm,<ea>",			0x5483, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_ImmOpWord3Bit,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADDQ.L #7,D3", 1, { 0x5e83, }, 1, {
																														{ "ADDQ #imm,<ea>",			0x5e83, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_ImmOpWord3Bit,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADDQ.L #8,D3", 1, { 0x5083, }, 1, {
																														{ "ADDQ #imm,<ea>",			0x5083, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_ImmOpWord3Bit,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

};

static const InstructionTestCase relativeBranchTests[] =
{
	{ "BRA.S *+$12", 1, { 0x6012, }, 1, {
																														{ "BRA <relative address>",	0x6012, { 0x0012, 0x0000, },	ExecutionResource_PC,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S8,		AguOperation_OffsetBaseIndexScale,	ExecutionResource_PC,				false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
	}, },
	{ "BNE.W *+$1234", 2, { 0x6600, 0x1234, }, 1, {
																														{ "Bcc <relative address>",	0x6600, { 0x1234, 0x0000, },	ExecutionResource_PC,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_PC,				false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_Bcc,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
	}, },
	{ "BEQ.L *+$12345678", 3, { 0x67ff, 0x1234, 0x5678, }, 1, {
																														{ "Bcc <relative address>",	0x67ff, { 0x1234, 0x5678, },	ExecutionResource_PC,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_PC,				false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_Bcc,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
	}, },
	{ "BSR.S *+$12", 1, { 0x6112, }, 2, {
																														{ "BSR <relative address>",	0x6112, { 0x0012, 0x0000, },	ExecutionResource_PC,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S8,		AguOperation_OffsetBaseIndexScale,	ExecutionResource_PC,				false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "PUSH NEXT PC",			0x0000, { 0x0000, 0x0000, },	ExecutionResource_A7,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrementSP,		ExecutionResource_A7,				false,	ExecutionResource_uOpLong,			ExecutionResource_PC,				OperationSize_Long,	IeeOperation_AddA,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Only,		},
	}, },
	{ "BSR.W *+$1234", 2, { 0x6100, 0x1234, }, 2, {
																														{ "BSR <relative address>",	0x6100, { 0x1234, 0x0000, },	ExecutionResource_PC,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_PC,				false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "PUSH NEXT PC",			0x0000, { 0x0000, 0x0002, },	ExecutionResource_A7,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrementSP,		ExecutionResource_A7,				false,	ExecutionResource_uOpLong,			ExecutionResource_PC,				OperationSize_Long,	IeeOperation_AddA,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Only,		},
	}, },
	{ "BSR.L *+$12345678", 3, { 0x61ff, 0x1234, 0x5678, }, 2, {
																														{ "BSR <relative address>",	0x61ff, { 0x1234, 0x5678, },	ExecutionResource_PC,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_PC,				false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "PUSH NEXT PC",			0x0000, { 0x0000, 0x0004, },	ExecutionResource_A7,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrementSP,		ExecutionResource_A7,				false,	ExecutionResource_uOpLong,			ExecutionResource_PC,				OperationSize_Long,	IeeOperation_AddA,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Only,		},
	}, },
//	{ "DBRA *+$12", { 0x51c9, 0x1234, }, 2, "DBra <relative address>" },
//	{ "DBEQ *+$12", { 0x57ca, 0x1234, }, 2, "DBcc <relative address>" },
};

static const InstructionTestCase absoluteJumpTests[] =
{
	{ "JMP $1234(A3)", 2, { 0x4eeb, 0x1234, }, 1, {
																														{ "JMP <ea>",				0x4eeb, { 0x1234, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_PC,				false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
	}, },
	{ "JSR ([$12345678.L,PC])", 4, { 0x4ebb, 0x0171, 0x1234, 0x5678, }, 4, {
																														{ "LOAD",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_PC,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "JSR <ea>",				0x4ebb, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_PC,				false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "PUSH NEXT PC",			0x0000, { 0x0000, 0x0006, },	ExecutionResource_A7,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrementSP,		ExecutionResource_A7,				false,	ExecutionResource_uOpLong,			ExecutionResource_PC,				OperationSize_Long,	IeeOperation_AddA,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Only,		},
	}, },
};

static const InstructionTestCase singleBitInstructionTests[] =
{
	{ "BTST D1,$12345678.L", 3, { 0x0339, 0x1234, 0x5678, }, 1, {
																														{ "BTST Dn,<ea>",			0x0339, { 0x1234, 0x5678, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D1,				ExecutionResource_MemoryOperand,	OperationSize_Byte, IeeOperation_BTst,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
	}, },
	
	{ "BCLR #3,$12345678.L", 4, { 0x08b9, 0x0003, 0x1234, 0x5678, }, 2, {
																														{ "SOURCEUOP",				0x0000, { 0x0003, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpByte0,			ExecutionResource_None,				OperationSize_Byte,	IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		},
																														{ "BCLR #imm,<ea>",			0x08b9, { 0x1234, 0x5678, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Byte, IeeOperation_BClr,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "BCHG D1,D3", 1, { 0x0343, }, 1, {
																														{ "BCHG Dn,Dm",				0x0343, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D1,				ExecutionResource_D3,				OperationSize_Long, IeeOperation_BChg,			ExecutionResource_D3,				false,	Pairability_pOEP_Only,		},
	}, },
	
	{ "BSET #31,D2", 2, { 0x08c2, 0x001f, 0x1111, 0x2222, 0x3333, }, 1, {
																														{ "BSET #imm,Dm",			0x08c2, { 0x001f, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpByte0,			ExecutionResource_D2,				OperationSize_Long, IeeOperation_BSet,			ExecutionResource_D2,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },
};

static const InstructionTestCase bcdArithmeticTests[] =
{
	{ "ABCD D2,D3", 1, { 0xc702, }, 1, { 
																														{ "ABCD Dx,Dy",				0xc702, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D2,				ExecutionResource_D3,				OperationSize_Byte, IeeOperation_Abcd,			ExecutionResource_D3,				false,	Pairability_pOEP_Only,		},
	}, },

	{ "ABCD -(A1),-(A3)", 1, { 0xc709, }, 2, {
																														{ "SOURCEUOP",				0x0000, { 0x0000, 0x0000, },	ExecutionResource_A1,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrement,			ExecutionResource_A1,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Byte, IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		},
																														{ "ABCD -(Ax),-(Ay)",		0xc709, { 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrement,			ExecutionResource_A3,				true,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Byte, IeeOperation_Abcd,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Only,		},
	}, },

	{ "NBCD (A3)", 1, { 0x4813, }, 1, {
																														{ "NBCD <ea>",				0x4813, { 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_None,				ExecutionResource_MemoryOperand,	OperationSize_Byte, IeeOperation_Nbcd,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Only,		},
	}, },

	{ "SBCD D2,D3", 1, { 0x8702, }, 1, {
																														{ "SBCD Dx,Dy",				0x8702, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D2,				ExecutionResource_D3,				OperationSize_Byte, IeeOperation_Sbcd,			ExecutionResource_D3,				false,	Pairability_pOEP_Only,		},
	}, },

	{ "SBCD -(A1),-(A3)", 1, { 0x8709, }, 2, {
																														{ "SOURCEUOP",				0x0000, { 0x0000, 0x0000, },	ExecutionResource_A1,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrement,			ExecutionResource_A1,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Byte, IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		},
																														{ "SBCD -(Ax),-(Ay)",		0x8709, { 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrement,			ExecutionResource_A3,				true,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Byte, IeeOperation_Sbcd,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Only,		},
	}, },

//	{ "PACK D1,D2,#$1234", { 0x8541, 0x1234, }, 2, "PACK Dm,Dn,#imm" },
//	{ "PACK -(A2),-(A1),#$1234", { 0x834a, 0x1234, }, 2, "PACK -(Am),-(An),#imm" },

//	{ "UNPK D2,D3,#$1234", { 0x8782, 0x1234, }, 2, "UNPK Dm,Dn,#imm" },
//	{ "UNPK -(A7),-(A6),#$1234", { 0x8d8f, 0x1234, }, 2, "UNPK -(Am),-(An),#imm" },
};

static const InstructionTestCase integerArithmeticTests[] =
{
	{ "ADD.B D1,D3", 1, { 0xd601, }, 1, {
																														{ "ADD <ea>,Dn",			0xd601, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D1,				ExecutionResource_D3,				OperationSize_Byte, IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "ADD.L D1,D3", 1, { 0xd681, }, 1, {
																														{ "ADD <ea>,Dn",			0xd681, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D1,				ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "ADDA.W D4,A5", 1, { 0xdac4, }, 1, {
																														{ "ADDA.W <ea>,An",			0xdac4, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D4,				ExecutionResource_A5,				OperationSize_Word,	IeeOperation_AddA,			ExecutionResource_A5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "ADDA.L (A2),A5", 1, { 0xdbd2, }, 1, {
																														{ "ADDA.L <ea>,An",			0xdbd2, { 0x0000, 0x0000, },	ExecutionResource_A2,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_A5,				OperationSize_Long,	IeeOperation_AddA,			ExecutionResource_A5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADDQ.L #2,D3", 1, { 0x5483, }, 1, {
																														{ "ADDQ #imm,<ea>",			0x5483, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_ImmOpWord3Bit,	ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Add,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADDQ.W #5,A3", 1, { 0x5a4b, }, 1, {
																														{ "ADDQ #imm,An",			0x5a4b, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_ImmOpWord3Bit,	ExecutionResource_A3,				OperationSize_Word,	IeeOperation_AddA,			ExecutionResource_A3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "ADDQ.B #1,$12(A3,D2.L)", 2, { 0x5233, 0x2812, }, 1, {
																														{ "ADDQ #imm,<ea>",			0x5233, { 0x0012, 0x0000, },	ExecutionResource_A3,				ExecutionResource_D2,				0,	AguIndexSize_Long,	AguDisplacementSize_S8,		AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_ImmOpWord3Bit,	ExecutionResource_MemoryOperand,	OperationSize_Byte, IeeOperation_Add,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ADDX.W D2,D3", 1, { 0xd742, }, 1, {
																														{ "ADDX Dx,Dy",				0xd742, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D2,				ExecutionResource_D3,				OperationSize_Word, IeeOperation_AddX,			ExecutionResource_D3,				false,	Pairability_pOEP_Only,		},
	}, },

	{ "ADDX.L -(A1),-(A3)", 1, { 0xd789, }, 2, {
																														{ "SOURCEUOP",				0x0000, { 0x0000, 0x0000, },	ExecutionResource_A1,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrement,			ExecutionResource_A1,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long, IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		},
																														{ "ADDX -(Ax),-(Ay)",		0xd789, { 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrement,			ExecutionResource_A3,				true,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Long, IeeOperation_AddX,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Only,		},
	}, },

	{ "CMP.L (A3),D1", 1, { 0xb293, }, 1, {
																														{ "CMP <ea>,Dn",			0xb293, { 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D1,				OperationSize_Long,	IeeOperation_Cmp,			ExecutionResource_None,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "CMPA.L D1,A1", 1, { 0xb3c1, }, 1, {
																														{ "CMPA.L <ea>,An",			0xb3c1, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D1,				ExecutionResource_A1,				OperationSize_Long,	IeeOperation_CmpA,			ExecutionResource_None,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "CMPI.L #$12345678,D1", 3, { 0x0c81, 0x1234, 0x5678, }, 1, {
																														{ "CMPI #imm,<ea>",			0x0c81, { 0x1234, 0x5678, },	ExecutionResource_None,				ExecutionResource_None,				0, AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpLong,			ExecutionResource_D1,				OperationSize_Long,	IeeOperation_Cmp,			ExecutionResource_None,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "CMPM.B (A5)+,(A4)+", 1, { 0xb90d, }, 2, {
																														{ "SOURCEUOP",				0x0000, { 0x0000, 0x0000, },	ExecutionResource_A5,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PostIncrement,			ExecutionResource_A5,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Byte, IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		},
																														{ "CMPM (Ax)+,(Ay)+",		0xb90d, { 0x0000, 0x0000, },	ExecutionResource_A4,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PostIncrement,			ExecutionResource_A4,				true,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Byte, IeeOperation_Cmp,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
	}, },

	{ "CMPM.L (A3)+,(A2)+", 1, { 0xb58b, }, 2, {
																														{ "SOURCEUOP",				0x0000, { 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PostIncrement,			ExecutionResource_A3,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long, IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		},
																														{ "CMPM (Ax)+,(Ay)+",		0xb58b, { 0x0000, 0x0000, },	ExecutionResource_A2,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PostIncrement,			ExecutionResource_A2,				true,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Long, IeeOperation_Cmp,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
	}, },

	// TODO: Add MULS/MULU/MULSL
	// TODO: Add DIVS/DIVU/DIVSL
	
	{ "NEG.W D7", 1, { 0x4447, }, 1, {
																														{ "NEG <ea>",				0x4447, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_None,				ExecutionResource_D7,				OperationSize_Word, IeeOperation_Neg,			ExecutionResource_D7,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "NEGX.L D5", 1, { 0x4085, }, 1, {
																														{ "NEGX <ea>",				0x4085, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_None,				ExecutionResource_D5,				OperationSize_Long, IeeOperation_NegX,			ExecutionResource_D5,				false,	Pairability_pOEP_Only,		},
	}, },

	{ "SUB.B D1,D2", 1, { 0x9401, }, 1, {
																														{ "SUB <ea>,Dn",			0x9401, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D1,				ExecutionResource_D2,				OperationSize_Byte, IeeOperation_Sub,			ExecutionResource_D2,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "SUBA.W D4,A5", 1, { 0x9ac4, }, 1, {
																														{ "SUBA.W <ea>,An",			0x9ac4, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D4,				ExecutionResource_A5,				OperationSize_Word,	IeeOperation_SubA,			ExecutionResource_A5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "SUBA.W (A2),A5", 1, { 0x9ad2, }, 1, {
																														{ "SUBA.W <ea>,An",			0x9ad2, { 0x0000, 0x0000, },	ExecutionResource_A2,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_A5,				OperationSize_Word,	IeeOperation_SubA,			ExecutionResource_A5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "SUBA.L (A2),A5", 1, { 0x9bd2, }, 1, {
																														{ "SUBA.L <ea>,An",			0x9bd2, { 0x0000, 0x0000, },	ExecutionResource_A2,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_A5,				OperationSize_Long,	IeeOperation_SubA,			ExecutionResource_A5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "SUB.L D2,-(A6)", 1, { 0x95a6, }, 1, {
																														{ "SUB Dn,<ea>",			0x95a6, { 0x0000, 0x0000, },	ExecutionResource_A6,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrement,			ExecutionResource_A6,				true,	ExecutionResource_D2,				ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_Sub,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "SUBI.W #$1234,D5", 2, { 0x0445, 0x1234, }, 1, {
																														{ "SUBI #imm,<ea>",			0x0445, { 0x1234, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpWord0,			ExecutionResource_D5,				OperationSize_Word,	IeeOperation_Sub,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "SUBQ.L #4,D0", 1, { 0x5980, }, 1, {
																														{ "SUBQ #imm,<ea>",			0x5980, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_ImmOpWord3Bit,	ExecutionResource_D0,				OperationSize_Long,	IeeOperation_Sub,			ExecutionResource_D0,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "SUBQ.B #3,([A5])", 2, { 0x5735, 0x0151, }, 3, {
																														{ "LOAD",					0x0000, { 0x0000, 0x0000, },	ExecutionResource_A5,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "SUBQ #imm,<ea>",			0x5735, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_ImmOpWord3Bit,	ExecutionResource_MemoryOperand,	OperationSize_Byte, IeeOperation_Sub,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "SUBX.W D1,D1", 1, { 0x9341, }, 1, {
																														{ "SUBX Dx,Dy",				0x9341, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D1,				ExecutionResource_D1,				OperationSize_Word, IeeOperation_SubX,			ExecutionResource_D1,				false,	Pairability_pOEP_Only,		},
	}, },

	{ "SUBX.B -(A4),-(A3)", 1, { 0x970c, }, 2, {
																														{ "SOURCEUOP",				0x0000, { 0x0000, 0x0000, },	ExecutionResource_A4,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrement,			ExecutionResource_A4,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Byte, IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		},
																														{ "SUBX -(Ax),-(Ay)",		0x970c, { 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrement,			ExecutionResource_A3,				true,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Byte, IeeOperation_SubX,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Only,		},
	}, },

	{ "TST.B D7", 1, { 0x4a07, }, 1, {
																														{ "TST <ea>",				0x4a07, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D7,				ExecutionResource_None,				OperationSize_Byte,	IeeOperation_Tst,			ExecutionResource_None,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "TST.L $1234(A4)", 2, { 0x4aac, 0x1234, }, 1, {
																														{ "TST <ea>",				0x4aac, { 0x1234, 0x0000, },	ExecutionResource_A4,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_Tst,			ExecutionResource_None,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "TST.W (A0)+", 1, { 0x4a58, }, 1, {
																														{ "TST <ea>",				0x4a58, { 0x0000, 0x0000, },	ExecutionResource_A0,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PostIncrement,			ExecutionResource_A0,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Word,	IeeOperation_Tst,			ExecutionResource_None,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },
};

static const InstructionTestCase shiftRotateTests[] =
{
	{ "ASL.W D0,D1", 1, { 0xe161, }, 1, {
																														{ "ASL Dm,Dn",				0xe161, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D0,				ExecutionResource_D1,				OperationSize_Word,	IeeOperation_Asl,			ExecutionResource_D1,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ASL.L #3,D2", 1, { 0xe782, }, 1, {
																														{ "ASL #imm,Dn",			0xe782, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_ImmOpWord3Bit,	ExecutionResource_D2,				OperationSize_Long,	IeeOperation_Asl,			ExecutionResource_D2,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ASR.B #1,D1", 1, { 0xe201, }, 1, {
																														{ "ASR #imm,Dn",			0xe201, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_ImmOpWord3Bit,	ExecutionResource_D1,				OperationSize_Byte,	IeeOperation_Asr,			ExecutionResource_D1,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ASR.W $1234(A5)", 2, { 0xe0ed, 0x1234, }, 1, {
																														{ "ASR <ea>",				0xe0ed, { 0x1234, 0x0000, },	ExecutionResource_A5,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_Constant1,		ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Asr,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "LSL.L D2,D3", 1, { 0xe5ab, }, 1, {
																														{ "LSL Dm,Dn",				0xe5ab, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D2,				ExecutionResource_D3,				OperationSize_Long,	IeeOperation_Lsl,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "LSR.W #2,D7", 1, { 0xe44f, }, 1, {
																														{ "LSR #imm,Dn",			0xe44f, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_ImmOpWord3Bit,	ExecutionResource_D7,				OperationSize_Word,	IeeOperation_Lsr,			ExecutionResource_D7,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "LSL.W ([A4])", 2, { 0xe3f4, 0x0151, }, 3, {
																														{ "LOAD",					0x0000, { 0x0000, 0x0000, },	ExecutionResource_A4,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "LSL <ea>",				0xe3f4, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_Constant1,		ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Lsl,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ROL.W #2,D3", 1, { 0xe55b, }, 1, {
																														{ "ROL #imm,Dn",			0xe55b, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_ImmOpWord3Bit,	ExecutionResource_D3,				OperationSize_Word,	IeeOperation_Rol,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "ROR.L D2,D7", 1, { 0xe4bf, }, 1, {
																														{ "ROR Dm,Dn",				0xe4bf, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D2,				ExecutionResource_D7,				OperationSize_Long,	IeeOperation_Ror,			ExecutionResource_D7,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ROL (A2)", 1, { 0xe7d2, }, 1, {
																														{ "ROL <ea>",				0xe7d2, { 0x0000, 0x0000, },	ExecutionResource_A2,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_Constant1,		ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Rol,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ROXL.W #1,D4", 1, { 0xe354, }, 1, {
																														{ "ROXL #imm,Dn",			0xe354, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_ImmOpWord3Bit,	ExecutionResource_D4,				OperationSize_Word,	IeeOperation_Roxl,			ExecutionResource_D4,				false,	Pairability_pOEP_Only,		},
	}, },

	{ "ROXL ([$12345678,A5])", 4, { 0xe5f5, 0x0171, 0x1234, 0x5678, }, 3, {
																														{ "LOAD",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_A5,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "ROXL <ea>",				0xe5f5, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_Constant1,		ExecutionResource_MemoryOperand,	OperationSize_Word,	IeeOperation_Roxl,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Only,		},
	}, },

	{ "ROXR.L D2,D2", 1, { 0xe4b2, }, 1, {
																														{ "ROXR Dm,Dn",				0xe4b2, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D2,				ExecutionResource_D2,				OperationSize_Long,	IeeOperation_Roxr,			ExecutionResource_D2,				false,	Pairability_pOEP_Only,		},
	}, },

};

static const InstructionTestCase integerLogicTests[] =
{
	{ "AND.W $1234(A1),D3", 2, { 0xc669, 0x1234, }, 1, {
																														{ "AND <ea>,Dn",			0xc669, { 0x1234, 0x0000, },	ExecutionResource_A1,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D3,				OperationSize_Word, IeeOperation_And,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },
	
	{ "AND.L D3,$1234(A1)", 2, { 0xc7a9, 0x1234, }, 1, {
																														{ "AND Dn,<ea>",			0xc7a9, { 0x1234, 0x0000, },	ExecutionResource_A1,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D3,				ExecutionResource_MemoryOperand,	OperationSize_Long, IeeOperation_And,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ANDI.L #$12345678,($12345678,D2.W)", 6, { 0x02b0, 0x1234, 0x5678, 0x21b0, 0x1234, 0x5678, }, 3, {
																														{ "SOURCEUOP",				0x0000, { 0x1234, 0x5678, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpLong,			ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_None,				ExecutionResource_D2,				0,	AguIndexSize_Word,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "ANDI #imm,<ea>",			0x02b0, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Long,	IeeOperation_And,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ANDI #$12,CCR", 2, { 0x023c, 0x0012, }, 1, {
																														{ "ANDI #imm,CCR",			0x023c, { 0x0012, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpByte0,			ExecutionResource_CCR,				OperationSize_Byte, IeeOperation_AndToCcr,		ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
	}, },
	
	{ "EOR.L D1,(A3)", 1, { 0xb393, }, 1, {
																														{ "EOR.L Dn,<ea>",			0xb393, { 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D1,				ExecutionResource_MemoryOperand,	OperationSize_Long, IeeOperation_Eor,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "EORI.L #$12345678,$1234(A3)", 4, { 0x0aab, 0x1234, 0x5678, 0x1234, }, 2, {
																														{ "SOURCEUOP",				0x0000, { 0x1234, 0x5678, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpLong,			ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		},
																														{ "EORI #imm,<ea>",			0x0aab, { 0x1234, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_IeeTemp,			ExecutionResource_MemoryOperand,	OperationSize_Long, IeeOperation_Eor,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "EORI #$12,CCR", 2, { 0x0a3c, 0x0012, }, 1, {
																														{ "EORI #imm,CCR",			0x0a3c, { 0x0012, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpByte0,			ExecutionResource_CCR,				OperationSize_Byte, IeeOperation_EorToCcr,		ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
	}, },

	{ "NOT.W -(A5)", 1, { 0x4665, }, 1, {
																														{ "NOT <ea>",				0x4665, { 0x0000, 0x0000, },	ExecutionResource_A5,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_PreDecrement,			ExecutionResource_A5,				true,	ExecutionResource_None,				ExecutionResource_MemoryOperand,	OperationSize_Word, IeeOperation_Not,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "OR.L $1234(A3),D2", 2, { 0x84ab, 0x1234, }, 1, {
																														{ "OR <ea>,Dn",				0x84ab, { 0x1234, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D2,				OperationSize_Long, IeeOperation_Or,			ExecutionResource_D2,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "OR.B D2,([A4])", 2, { 0x8534, 0x0151, }, 3, {
																														{ "LOAD",					0x0000, { 0x0000, 0x0000, },	ExecutionResource_A4,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "OR Dn,<ea>",				0x8534, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_D2,				ExecutionResource_MemoryOperand,	OperationSize_Byte,	IeeOperation_Or,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ORI.W #$1234,D5", 2, { 0x0045, 0x1234, }, 1, {
																														{ "ORI #imm,<ea>",			0x0045, { 0x1234, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpWord0,			ExecutionResource_D5,				OperationSize_Word, IeeOperation_Or,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "ORI #$12,CCR", 2, { 0x003c, 0x0012, }, 1, {
																														{ "ORI #imm,CCR",			0x003c, { 0x0012, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpByte0,			ExecutionResource_CCR,				OperationSize_Byte, IeeOperation_OrToCcr,		ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
	}, },

};

static const InstructionTestCase moveTests[] =
{
	{ "MOVE.W D3,D4", 1, { 0x3803, }, 1, {
																														{ "MOVE.W <ea>,<ea>",		0x3803, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D3,				ExecutionResource_D4,				OperationSize_Word, IeeOperation_Move,			ExecutionResource_D4,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "MOVE.B #$12,D2", 2, { 0x143c, 0x0012, }, 1, {
																														{ "MOVE.B <ea>,<ea>",		0x143c, { 0x0012, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpByte0,			ExecutionResource_D2,				OperationSize_Byte, IeeOperation_Move,			ExecutionResource_D2,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "MOVE.W #$1234,(A6)", 2, { 0x3cbc, 0x1234, }, 1, {
																														{ "MOVE.W <ea>,<ea>",		0x3cbc, { 0x1234, 0x0000, },	ExecutionResource_A6,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				false,	ExecutionResource_uOpWord0,			ExecutionResource_None,				OperationSize_Word, IeeOperation_Move,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "MOVE.L #$12345678,(A6)", 3, { 0x2cbc, 0x1234, 0x5678, }, 1, {
																														{ "MOVE.L <ea>,<ea>",		0x2cbc, { 0x1234, 0x5678, },	ExecutionResource_A6,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				false,	ExecutionResource_uOpLong,			ExecutionResource_None,				OperationSize_Long, IeeOperation_Move,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "MOVE.W (A7),D5", 1, { 0x3a17, }, 1, {
																														{ "MOVE.W <ea>,<ea>",		0x3a17, { 0x0000, 0x0000, },	ExecutionResource_A7,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_D5,				OperationSize_Word, IeeOperation_Move,			ExecutionResource_D5,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "MOVE.L #$12345678,([$12345678.L,A1])", 6, { 0x23bc, 0x1234, 0x5678, 0x0171, 0x1234, 0x5678, }, 4, {
																														{ "SOURCEUOP",				0x0000, { 0x1234, 0x5678, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_uOpLong,			ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LOAD",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_A1,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "MOVE.L <ea>,<ea>",		0x23bc, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				false,	ExecutionResource_IeeTemp,			ExecutionResource_None,				OperationSize_Long,	IeeOperation_Move,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "MOVE.L ([$12345678.L,A1]),([$12345678.L,A2])", 7, { 0x25b1, 0x0171, 0x1234, 0x5678, 0x0171, 0x1234, 0x5678, }, 6, {
																														{ "LOAD",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_A1,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "SOURCEUOP",				0x0000, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LOAD",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_A2,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "MOVE.L <ea>,<ea>",		0x25b1, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				false,	ExecutionResource_IeeTemp,			ExecutionResource_None,				OperationSize_Long,	IeeOperation_Move,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "MOVE.L ([$12345678.L,PC]),([D0.W],$12345678.L)", 7, { 0x21bb, 0x0171, 0x1234, 0x5678, 0x0193, 0x1234, 0x5678, }, 6, {
																														{ "LOAD",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_PC,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "SOURCEUOP",				0x0000, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_IeeTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LOAD",					0x0000, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_D0,				0,	AguIndexSize_Word,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x1234, 0x5678, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S32,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "MOVE.L <ea>,<ea>",		0x21bb, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				false,	ExecutionResource_IeeTemp,			ExecutionResource_None,				OperationSize_Long,	IeeOperation_Move,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "MOVEA.W D2,A2", 1, { 0x3442, }, 1, {
																														{ "MOVEA.W <ea>,<ea>",		0x3442, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_D2,				ExecutionResource_None,				OperationSize_Word, IeeOperation_MoveA,			ExecutionResource_A2,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "MOVE.L A2,D3", 1, { 0x260a, }, 1, {
																														{ "MOVE.L <ea>,<ea>",		0x260a, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_A2,				ExecutionResource_None,				OperationSize_Long, IeeOperation_Move,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "MOVE CCR,$1234(A3)", 2, { 0x42eb, 0x1234, }, 1, {
																														{ "MOVE CCR,<ea>",			0x42eb, { 0x1234, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				false,	ExecutionResource_CCR,				ExecutionResource_None,				OperationSize_Word, IeeOperation_MoveFromCcr,	ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "MOVE $1234(A2),CCR", 2, { 0x44ea, 0x1234, }, 1, {
																														{ "MOVE <ea>,CCR",			0x44ea, { 0x1234, 0x0000, },	ExecutionResource_A2,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Word, IeeOperation_MoveToCcr,		ExecutionResource_None,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "MOVEQ #$12,D2", 1, { 0x7412, }, 1, {
																														{ "MOVEQ #imm,Dn",			0x7412, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_ImmOpWord8Bit,	ExecutionResource_None,				OperationSize_Long, IeeOperation_Move,			ExecutionResource_D2,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },
};

static const InstructionTestCase miscellaneousTests[] =
{
//	{ "BKPT #4", { 0x484c, }, 1, "BKPT #imm" },
//	{ "CHK.W $1234(A1),D1", { 0x43a9, 0x1234, }, 2, "CHK.W <ea>,Dn" },
//	{ "CHK.L #$12345678,D1", { 0x433c, 0x1234, 0x5678, }, 3, "CHK.L <ea>,Dn" },
	{ "CLR.W $1234(A1)", 2, { 0x4269, 0x1234, }, 1, {
																														{ "CLR <ea>",				0x4269, { 0x1234, 0x0000, },	ExecutionResource_A1,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_Word, IeeOperation_Clr,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "EXT.W D2", 1, { 0x4882, }, 1, {
																														{ "EXT.W Dn",				0x4882, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_None,				ExecutionResource_D2,				OperationSize_Word, IeeOperation_Ext,			ExecutionResource_D2,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "EXT.L D3", 1, { 0x48c3, }, 1, {
																														{ "EXT.L Dn",				0x48c3, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_None,				ExecutionResource_D3,				OperationSize_Long, IeeOperation_Ext,			ExecutionResource_D3,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "EXTB.L D4", 1, { 0x49c4, }, 1, {
																														{ "EXTB.L Dn",				0x49c4, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_None,				ExecutionResource_D4,				OperationSize_Byte, IeeOperation_Ext,			ExecutionResource_D4,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

//	{ "ILLEGAL", { 0x4afc, }, 1, "ILLEGAL" },
	
	{ "LEA $1234(A1),A2", 2, { 0x45e9, 0x1234, }, 1, {
																														{ "LEA <ea>,An",			0x45e9, { 0x1234, 0x0000, },	ExecutionResource_A1,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_A2,				false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_Long, IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

	{ "LEA ([A3,D2.L*4],$1234.W),A2", 3, { 0x45f3, 0x2d12, 0x1234, }, 3, {
																														{ "LOAD",					0x0000, { 0x0000, 0x0000, },	ExecutionResource_A3,				ExecutionResource_D2,				2,	AguIndexSize_Long,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				true,	ExecutionResource_MemoryOperand,	ExecutionResource_None,				OperationSize_Long,	IeeOperation_ForwardIeeA,	ExecutionResource_AguTemp,			false,	Pairability_pOEP_Only,		},
																														{ "LEA",					0x0000, { 0x1234, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_AguTemp,			false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_None,	IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Only,		},
																														{ "LEA <ea>,An",			0x45f3, { 0x0000, 0x0000, },	ExecutionResource_AguTemp,			ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_A2,				false,	ExecutionResource_None,				ExecutionResource_None,				OperationSize_Long, IeeOperation_None,			ExecutionResource_None,				false,	Pairability_pOEP_Or_sOEP,	},
	}, },

//	{ "LINK.W A3,#$1234", { 0x4e53, 0x1234, }, 2, "LINK.W An,#imm" },
//	{ "LINK.L A3,#$12345678", { 0x480c, 0x1234, 0x5678, }, 3, "LINK.L An,#imm" },

//	{ "NOP", { 0x4e71, }, 1, "NOP" },

//	{ "PEA $12(A3,D1.W)", { 0x4873, 0x1012, }, 2, "PEA <ea>" },
	
//	{ "RTD #$1234", { 0x4e74, 0x1234, }, 2, "RTD #imm" },
//	{ "RTR", { 0x4e77, }, 1, "RTR" },
//	{ "RTS", { 0x4e75, }, 1, "RTS" },

	{ "ST D7", 1, { 0x50c7, }, 1, {
																														{ "Scc <ea>",				0x50c7, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_CCR,				ExecutionResource_D7,				OperationSize_Byte, IeeOperation_Scc,			ExecutionResource_D7,				false,	Pairability_pOEP_But_Allows_sOEP,	},
	}, },

	{ "SNE (A2)", 1, { 0x56d2, }, 1, {
																														{ "Scc <ea>",				0x56d2, { 0x0000, 0x0000, },	ExecutionResource_A2,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				false,	ExecutionResource_CCR,				ExecutionResource_None,				OperationSize_Byte, IeeOperation_Scc,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_But_Allows_sOEP,	},
	}, },

	{ "SEQ $1234(A3)", 2, { 0x57eb, 0x1234, }, 1, {
																														{ "Scc <ea>",				0x57eb, { 0x1234, 0x0000, },	ExecutionResource_A3,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_S16,	AguOperation_OffsetBaseIndexScale,	ExecutionResource_None,				false,	ExecutionResource_CCR,				ExecutionResource_None,				OperationSize_Byte, IeeOperation_Scc,			ExecutionResource_MemoryOperand,	true,	Pairability_pOEP_But_Allows_sOEP,	},
	}, },

	{ "SWAP D3", 1, { 0x4843, }, 1, {
																														{ "SWAP Dn",				0x4843, { 0x0000, 0x0000, },	ExecutionResource_None,				ExecutionResource_None,				0,	AguIndexSize_None,	AguDisplacementSize_None,	AguOperation_None,					ExecutionResource_None,				false,	ExecutionResource_None,				ExecutionResource_D3,				OperationSize_Long, IeeOperation_Swap,			ExecutionResource_D3,				false,	Pairability_pOEP_Only,				},
	}, },

//	{ "TAS D2", { 0x4ac2, }, 1, "TAS <ea>" },
//	{ "TAS $12(A5,D3.L)", { 0x4af5, 0x3812, }, 2, "TAS <ea>" },

//	{ "TRAP #3", { 0x4e43, }, 1, "TRAP #imm" },
//	{ "TRAPNE", { 0x56fc, }, 1, "TRAPcc" },
//	{ "TRAPGE.W #$1234", { 0x5cfa, 0x1234, }, 2, "TRAPcc.W #imm" },
//	{ "TRAPLT.L #$12345678", { 0x5dfb, 0x1234, 0x5678, }, 3, "TRAPcc.L #imm" },
//	{ "TRAPV", { 0x4e76, }, 1, "TRAPV" },
	
//	{ "UNLK A3", { 0x4e5b, }, 1, "UNLK An" },
};

TestSuite testSuites[] =
{
	{ "6-bit EA decoding tests", ea6BitTests, (sizeof ea6BitTests / sizeof ea6BitTests[0]) },
	{ "Immediate source operand tests", immediateTests, (sizeof immediateTests / sizeof immediateTests[0]) },
	{ "Relative branch tests", relativeBranchTests, (sizeof relativeBranchTests / sizeof relativeBranchTests[0]) },
	{ "Absolute jump tests", absoluteJumpTests, (sizeof absoluteJumpTests / sizeof absoluteJumpTests[0]) },
	{ "Single-bit instruction tests", singleBitInstructionTests, (sizeof singleBitInstructionTests / sizeof singleBitInstructionTests[0]) },
	{ "BCD arithmetic tests", bcdArithmeticTests, (sizeof bcdArithmeticTests / sizeof bcdArithmeticTests[0]) },
	{ "Integer arithmetic tests", integerArithmeticTests, (sizeof integerArithmeticTests / sizeof integerArithmeticTests[0]) },
	{ "Shift/rotate tests", shiftRotateTests, (sizeof shiftRotateTests / sizeof shiftRotateTests[0]) },
	{ "Integer logic tests", integerLogicTests, (sizeof integerLogicTests / sizeof integerLogicTests[0]) },
	{ "Move/Exchange tests", moveTests, (sizeof moveTests / sizeof moveTests[0]) },
	{ "Miscellaneous tests", miscellaneousTests, (sizeof miscellaneousTests / sizeof miscellaneousTests[0]) },
};

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
			uint UOp;

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
					{
						printf("UOp %d:\n", UOp);
						printUOp(&InstructionTestCase->UOps[UOp]);
					}

					printf("...but yielded the following UOps:\n");

					for (UOp = 0; UOp < numUOps; ++UOp)
					{
						printf("UOp %d:\n", UOp);
						printUOp(&UOps[UOp]);
					}
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