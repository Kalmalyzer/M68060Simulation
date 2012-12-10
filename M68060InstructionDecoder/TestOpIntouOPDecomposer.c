
#include "M68060DecomposeOpIntouOPs.h"
#include "../Types.h"

typedef struct 
{
	const char* description;
	uint numInstructionWords;
	uint16_t instructionWords[16];
	
	uint16_t numuOPs;
	const uOP uOPs[16];
	
} TestInstruction;

TestInstruction instructionStream[] =
{
	{ "ADD.B D0,D1", 1, { 0xd200, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_None,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				false,

				ExecutionResource_D0,
				ExecutionResource_D1,
				OperationSize_Byte,
				ExecutionResource_D1,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.B D6,D0", 1, { 0xd006, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_None,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				false,

				ExecutionResource_D6,
				ExecutionResource_D0,
				OperationSize_Byte,
				ExecutionResource_D0,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.W D0,D0", 1, { 0xd040, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_None,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				false,

				ExecutionResource_D0,
				ExecutionResource_D0,
				OperationSize_Word,
				ExecutionResource_D0,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.L D0,D0", 1, { 0xd080, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_None,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				false,

				ExecutionResource_D0,
				ExecutionResource_D0,
				OperationSize_Long,
				ExecutionResource_D0,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.B D0,D1", 1, { 0xd200, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_None,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				false,

				ExecutionResource_D0,
				ExecutionResource_D1,
				OperationSize_Byte,
				ExecutionResource_D1,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.B D0,D2", 1, { 0xd400, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_None,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				false,

				ExecutionResource_D0,
				ExecutionResource_D2,
				OperationSize_Byte,
				ExecutionResource_D2,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.B D0,D7", 1, { 0xde00, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_None,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				false,

				ExecutionResource_D0,
				ExecutionResource_D7,
				OperationSize_Byte,
				ExecutionResource_D7,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.B D1,D0", 1, { 0xd001, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_None,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				false,

				ExecutionResource_D1,
				ExecutionResource_D0,
				OperationSize_Byte,
				ExecutionResource_D0,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.B D2,D0", 1, { 0xd002, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_None,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				false,

				ExecutionResource_D2,
				ExecutionResource_D0,
				OperationSize_Byte,
				ExecutionResource_D0,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},
	
	{ "ADD.B D3,D0", 1, { 0xd003, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_None,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				false,

				ExecutionResource_D3,
				ExecutionResource_D0,
				OperationSize_Byte,
				ExecutionResource_D0,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.B D6,D0", 1, { 0xd006, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_None,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				false,

				ExecutionResource_D6,
				ExecutionResource_D0,
				OperationSize_Byte,
				ExecutionResource_D0,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.W A3,D5", 1, { 0xda4b, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_None,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				false,

				ExecutionResource_A3,
				ExecutionResource_D5,
				OperationSize_Word,
				ExecutionResource_D5,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.W (A3),D5", 1, { 0xda53, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_A3,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_D5,
				OperationSize_Word,
				ExecutionResource_D5,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.W (A3)+,D5", 1, { 0xda5b, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_A3,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_A3,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_D5,
				OperationSize_Word,
				ExecutionResource_D5,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.W -(A3),D5", 1, { 0xda63, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_A3,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_A3,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_D5,
				OperationSize_Word,
				ExecutionResource_D5,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.W ($1234,A3),D5", 2, { 0xda6b, 0x1234, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0x1234, 0, },

				ExecutionResource_A3,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_S16,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_D5,
				OperationSize_Word,
				ExecutionResource_D5,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.W $12(A1,D3.W*4),D5", 2, { 0xda71, 0x3412, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0x12, 0, },

				ExecutionResource_A1,
				ExecutionResource_D3,
				2,
				AguIndexSize_Word,
				AguDisplacementSize_S8,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_D5,
				OperationSize_Word,
				ExecutionResource_D5,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.W $1234.W,D5", 2, { 0xda78, 0x1234, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0x1234, 0, },

				ExecutionResource_None,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_S16,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_D5,
				OperationSize_Word,
				ExecutionResource_D5,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.W $12345678.L,D5", 3, { 0xda79, 0x1234, 0x5678, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0x1234, 0x5678, },

				ExecutionResource_None,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_S32,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_D5,
				OperationSize_Word,
				ExecutionResource_D5,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.W ($1234,PC),D5", 2, { 0xda7a, 0x1234, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0x1234, 0, },

				ExecutionResource_PC,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_S16,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_D5,
				OperationSize_Word,
				ExecutionResource_D5,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.L $12(PC,D3.W*4),D5", 2, { 0xdabb, 0x3412, },
		1, 
		{
			{
				"ADD <ea>,Dn",
				{ 0x12, 0, },

				ExecutionResource_PC,
				ExecutionResource_D3,
				2,
				AguIndexSize_Word,
				AguDisplacementSize_S8,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_D5,
				OperationSize_Long,
				ExecutionResource_D5,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.L ($1234.W,A0,D0.L),D3", 3, { 0xd6b0, 0x0920, 0x1234, },
		2, 
		{
			{
				"LEA",
				{ 0x1234, 0, },

				ExecutionResource_A0,
				ExecutionResource_D0,
				0,
				AguIndexSize_Long,
				AguDisplacementSize_S16,
				AguOperation_None,
				ExecutionResource_AguTemp,

				false,

				ExecutionResource_None,
				ExecutionResource_None,
				OperationSize_None,
				ExecutionResource_None,

				false,
				Pairability_pOEP_Or_sOEP,
			},
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_AguTemp,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_D3,
				OperationSize_Long,
				ExecutionResource_D3,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.L ($12345678.L,A0,ZD0),D3", 4, { 0xd6b0, 0x0170, 0x1234, 0x5678, },
		2, 
		{
			{
				"LEA",
				{ 0x1234, 0x5678, },

				ExecutionResource_A0,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_S32,
				AguOperation_None,
				ExecutionResource_AguTemp,

				false,

				ExecutionResource_None,
				ExecutionResource_None,
				OperationSize_None,
				ExecutionResource_None,

				false,
				Pairability_pOEP_Or_sOEP,
			},
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_AguTemp,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_D3,
				OperationSize_Long,
				ExecutionResource_D3,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.L ($12345678.L,A0,D0.L),D3", 4, { 0xd6b0, 0x0930, 0x1234, 0x5678, },
		2, 
		{
			{
				"LEA",
				{ 0x1234, 0x5678, },

				ExecutionResource_A0,
				ExecutionResource_D0,
				0,
				AguIndexSize_Long,
				AguDisplacementSize_S32,
				AguOperation_None,
				ExecutionResource_AguTemp,

				false,

				ExecutionResource_None,
				ExecutionResource_None,
				OperationSize_None,
				ExecutionResource_None,

				false,
				Pairability_pOEP_Or_sOEP,
			},
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_AguTemp,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_D3,
				OperationSize_Long,
				ExecutionResource_D3,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.L ([$12345678.L,A0],D0.L),D3", 4, { 0xd6b0, 0x0935, 0x1234, 0x5678, },
		3, 
		{
			{
				"LOAD",
				{ 0x1234, 0x5678, },

				ExecutionResource_A0,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_S32,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_None,
				OperationSize_None,
				ExecutionResource_AguTemp,

				false,
				Pairability_pOEP_Or_sOEP,
			},
			{
				"LEA",
				{ 0, 0, },

				ExecutionResource_AguTemp,
				ExecutionResource_D0,
				0,
				AguIndexSize_Long,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_AguTemp,

				false,

				ExecutionResource_None,
				ExecutionResource_None,
				OperationSize_None,
				ExecutionResource_None,

				false,
				Pairability_pOEP_Or_sOEP,
			},
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_AguTemp,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_D3,
				OperationSize_Long,
				ExecutionResource_D3,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.L ([$12345678.L,A0],D0.L,$1234.W),D3", 5, { 0xd6b0, 0x0936, 0x1234, 0x5678, 0x1234, },
		3, 
		{
			{
				"LOAD",
				{ 0x1234, 0x5678, },

				ExecutionResource_A0,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_S32,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_None,
				OperationSize_None,
				ExecutionResource_AguTemp,

				false,
				Pairability_pOEP_Or_sOEP,
			},
			{
				"LEA",
				{ 0x1234, 0, },

				ExecutionResource_AguTemp,
				ExecutionResource_D0,
				0,
				AguIndexSize_Long,
				AguDisplacementSize_S16,
				AguOperation_None,
				ExecutionResource_AguTemp,

				false,

				ExecutionResource_None,
				ExecutionResource_None,
				OperationSize_None,
				ExecutionResource_None,

				false,
				Pairability_pOEP_Or_sOEP,
			},
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_AguTemp,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_D3,
				OperationSize_Long,
				ExecutionResource_D3,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.L ([$12345678.L,A0],D0.L,$12345678.L),D3", 6, { 0xd6b0, 0x0937, 0x1234, 0x5678, 0x1234, 0x5678, },
		3, 
		{
			{
				"LOAD",
				{ 0x1234, 0x5678, },

				ExecutionResource_A0,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_S32,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_None,
				OperationSize_None,
				ExecutionResource_AguTemp,

				false,
				Pairability_pOEP_Or_sOEP,
			},
			{
				"LEA",
				{ 0x1234, 0x5678, },

				ExecutionResource_AguTemp,
				ExecutionResource_D0,
				0,
				AguIndexSize_Long,
				AguDisplacementSize_S32,
				AguOperation_None,
				ExecutionResource_AguTemp,

				false,

				ExecutionResource_None,
				ExecutionResource_None,
				OperationSize_None,
				ExecutionResource_None,

				false,
				Pairability_pOEP_Or_sOEP,
			},
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_AguTemp,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_D3,
				OperationSize_Long,
				ExecutionResource_D3,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	{ "ADD.L ([$1234.W,A0],D0.L,$12345678.L),D3", 5, { 0xd6b0, 0x0927, 0x1234, 0x1234, 0x5678, },
		3, 
		{
			{
				"LOAD",
				{ 0x1234, 0, },

				ExecutionResource_A0,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_S16,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_None,
				OperationSize_None,
				ExecutionResource_AguTemp,

				false,
				Pairability_pOEP_Or_sOEP,
			},
			{
				"LEA",
				{ 0x1234, 0x5678, },

				ExecutionResource_AguTemp,
				ExecutionResource_D0,
				0,
				AguIndexSize_Long,
				AguDisplacementSize_S32,
				AguOperation_None,
				ExecutionResource_AguTemp,

				false,

				ExecutionResource_None,
				ExecutionResource_None,
				OperationSize_None,
				ExecutionResource_None,

				false,
				Pairability_pOEP_Or_sOEP,
			},
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_AguTemp,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_D3,
				OperationSize_Long,
				ExecutionResource_D3,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},
	
	{ "ADD.L ([A0,D0.L],$12345678.L),D3", 4, { 0xd6b0, 0x0913, 0x1234, 0x5678, },
		3, 
		{
			{
				"LOAD",
				{ 0, 0, },

				ExecutionResource_A0,
				ExecutionResource_D0,
				0,
				AguIndexSize_Long,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_None,
				OperationSize_None,
				ExecutionResource_AguTemp,

				false,
				Pairability_pOEP_Or_sOEP,
			},
			{
				"LEA",
				{ 0x1234, 0x5678, },

				ExecutionResource_AguTemp,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_S32,
				AguOperation_None,
				ExecutionResource_AguTemp,

				false,

				ExecutionResource_None,
				ExecutionResource_None,
				OperationSize_None,
				ExecutionResource_None,

				false,
				Pairability_pOEP_Or_sOEP,
			},
			{
				"ADD <ea>,Dn",
				{ 0, 0, },

				ExecutionResource_AguTemp,
				ExecutionResource_None,
				0,
				AguIndexSize_None,
				AguDisplacementSize_None,
				AguOperation_None,
				ExecutionResource_None,

				true,

				ExecutionResource_MemoryOperand,
				ExecutionResource_D3,
				OperationSize_Long,
				ExecutionResource_D3,

				false,
				Pairability_pOEP_Or_sOEP,
			},
		},
	},

	//{ "ADD.W #$1234,D5", { 0xda7c, 0x1234, }, 2, },
	//{ "ADD.W D5,(A3)", { 0xdb53, }, 1, },
	//{ "ADD.W D5,(A3)+", { 0xdb5b, }, 1, },
};

bool areuOPsEquivalent(const uOP* a, const uOP* b)
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
		&& a->ieeResult == b->ieeResult
		&& a->memoryWrite == b->memoryWrite
		&& a->pairability == b->pairability;
}

void printuOP(uint id, const uOP* uOP)
{
	printf("    uOP %d: %s\n", id, uOP->mnemonic);
	printf("      ExtensionWords: %04x,%04x\n", uOP->extensionWords[0], uOP->extensionWords[1]);
	printf("      Agu: Base %s, Index %s, IndexShift %d, IndexSize %s, DisplacementSize %s, Operation %s, Result %s\n",
		ExecutionResourceToString(uOP->aguBase), ExecutionResourceToString(uOP->aguIndex),
		uOP->aguIndexShift, AguIndexSizeToString(uOP->aguIndexSize), AguDisplacementSizeToString(uOP->aguDisplacementSize),
		AguOperationToString(uOP->aguOperation), ExecutionResourceToString(uOP->aguResult));
	printf("      MemoryRead: %s\n", uOP->memoryRead ? "yes" : "no");
	printf("      Iee: A %s, B %s, OperationSize %s, Result %s\n", ExecutionResourceToString(uOP->ieeA), ExecutionResourceToString(uOP->ieeB),
		OperationSizeToString(uOP->ieeOperationSize), ExecutionResourceToString(uOP->ieeResult));
	printf("      MemoryWrite: %s\n", uOP->memoryWrite ? "yes" : "no");
}

int main(void)
{
	uint i;

	for (i = 0; i < (sizeof instructionStream / sizeof instructionStream[0]); ++i)
	{
		const TestInstruction* testInstruction = &instructionStream[i];
		uOP uOPs[16];
		uint numuOPs;
		bool success = decomposeOpIntouOPs(testInstruction->instructionWords, testInstruction->numInstructionWords, uOPs, &numuOPs);

		if (!success)
			printf("failure: unable to decode %s\n", testInstruction->description);
		else
		{
			bool identical = true;
			int uOP;

			if (numuOPs != testInstruction->numuOPs)
				identical = false;
			else
			{
				for (uOP = 0; uOP < numuOPs; ++uOP)
				{
					if (!areuOPsEquivalent(&uOPs[uOP], &testInstruction->uOPs[uOP]))
						identical = false;
				}
			}
				
			if (identical)
				printf("success: decoded %s into %d uOPs properly\n", testInstruction->description, testInstruction->numuOPs);
			else
			{
				printf("failure: decoding %s should yield the following uOPs:\n", testInstruction->description);

				for (uOP = 0; uOP < testInstruction->numuOPs; ++uOP)
					printuOP(uOP, &testInstruction->uOPs[uOP]);

				printf("  but yielded the following uOPs:\n");

				for (uOP = 0; uOP < numuOPs; ++uOP)
					printuOP(uOP, &uOPs[uOP]);
			}
		}
	}
	
	return 0;
}