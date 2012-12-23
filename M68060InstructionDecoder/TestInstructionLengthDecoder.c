
#include "M68060InstructionLengthDecoder.h"
#include "../Types.h"

typedef struct
{
	const char* instruction;
	const uint16_t instructionWords[16];
	uint expectedNumInstructionWords;
	const char* expectedDescription;
} InstructionLengthDecoderTest;

typedef struct
{
	const char* description;
	const InstructionLengthDecoderTest* tests;
	uint numTests;
} TestSuite;

static const InstructionLengthDecoderTest ea6BitTests[] =
{
	{ "ADD.B D1,D3", { 0xd601, }, 1, "ADD <ea>,Dn" },
	{ "ADD.L D1,D3", { 0xd681, }, 1, "ADD <ea>,Dn" },
	{ "ADD.L A1,D3", { 0xd689, }, 1, "ADD <ea>,Dn" },
	{ "ADD.L (A1),D3", { 0xd691, }, 1, "ADD <ea>,Dn" },
	{ "ADD.L (A1)+,D3", { 0xd699, }, 1, "ADD <ea>,Dn" },
	{ "ADD.L -(A2),D3", { 0xd6a2, }, 1, "ADD <ea>,Dn" },
	{ "ADD.W ($1234,A1),D3", { 0xd27c, 0x1234, }, 2, "ADD <ea>,Dn" },
	{ "ADD.W $12(A1,D3.W*4),D5", { 0xda71, 0x3412, }, 2, "ADD <ea>,Dn" },
	{ "ADD.B #$12,D1", { 0xd23c, 0x0012, }, 2, "ADD <ea>,Dn" },
	{ "ADD.W #$1234,D1", { 0xd27c, 0x1234, }, 2, "ADD <ea>,Dn" },
	{ "ADD.L #$12345678,D1", { 0xd2bc, 0x1234, 0x5678, }, 3, "ADD <ea>,Dn" },
	{ "ADDA.W #$1234,A1", { 0xd2fc, 0x1234, }, 2, "ADDA.W <ea>,An" },
	{ "ADDA.L #$12345678,A1", { 0xd3fc, 0x1234, 0x5678, }, 3, "ADDA.L <ea>,An" },
	{ "ADD.B $1234.W,D1", { 0xd238, 0x1234, }, 2, "ADD <ea>,Dn" },
	{ "ADD.W $1234.W,D1", { 0xd278, 0x1234, }, 2, "ADD <ea>,Dn" },
	{ "ADD.L $1234.W,D1", { 0xd2b8, 0x1234, }, 2, "ADD <ea>,Dn" },
	{ "ADD.B $12345678.L,D1", { 0xd239, 0x1234, 0x5678, }, 3, "ADD <ea>,Dn" },
	{ "ADD.W $12345678.L,D1", { 0xd279, 0x1234, 0x5678, }, 3, "ADD <ea>,Dn" },
	{ "ADD.L $12345678.L,D1", { 0xd2b9, 0x1234, 0x5678, }, 3, "ADD <ea>,Dn" },
	{ "ADD.L ($1234,PC),D3", { 0xd6ba, 0x1234, }, 2, "ADD <ea>,Dn" },
	{ "ADD.L $12(PC,D3.W*4),D5", { 0xdabb, 0x3412, }, 2, "ADD <ea>,Dn" },
	{ "ADD.L ($1234.W,A0,D0.L),D3", { 0xd6b0, 0x0920, 0x1234, }, 3, "ADD <ea>,Dn" },
	{ "ADD.L ($12345678.L,A0,ZD0),D3", { 0xd6b0, 0x0170, 0x1234, 0x5678, }, 4, "ADD <ea>,Dn" },
	{ "ADD.L ($12345678.L,A0,D0.L),D3", { 0xd6b0, 0x0930, 0x1234, 0x5678, }, 4, "ADD <ea>,Dn" },
	{ "ADD.L ([$12345678.L,A0],D0.L),D3", { 0xd6b0, 0x0935, 0x1234, 0x5678, }, 4, "ADD <ea>,Dn" },
	{ "ADD.L ([$12345678.L,A0],D0.L,$1234.W),D3", { 0xd6b0, 0x0936, 0x1234, 0x5678, 0x1234, }, 5, "ADD <ea>,Dn" },
	{ "ADD.L ([$12345678.L,A0],D0.L,$12345678.L),D3", { 0xd6b0, 0x0937, 0x1234, 0x5678, 0x1234, 0x5678, }, 6, "ADD <ea>,Dn" },
	{ "ADD.L ([$1234.W,A0],D0.L,$12345678.L),D3", { 0xd6b0, 0x0927, 0x1234, 0x1234, 0x5678, }, 5, "ADD <ea>,Dn" },
	{ "ADD.L ([A0,D0.L],$12345678.L),D3", { 0xd6b0, 0x0913, 0x1234, 0x5678, }, 4, "ADD <ea>,Dn" },

	{ "ADD.L D3,(A1)+", { 0xd799, }, 1, "ADD Dn,<ea>" },
	{ "ADD.L D3,-(A2)", { 0xd7a2, }, 1, "ADD Dn,<ea>" },
	{ "ADD.W D3,($1234,A1)", { 0xd769, 0x1234, }, 2, "ADD Dn,<ea>" },
	{ "ADD.W D5,$12(A1,D3.W*4)", { 0xdb71, 0x3412, }, 2, "ADD Dn,<ea>" },
	{ "ADD.B D1,$1234.W", { 0xd338, 0x1234, }, 2, "ADD Dn,<ea>" },
	{ "ADD.W D1,$12345678.L", { 0xd379, 0x1234, 0x5678, }, 3, "ADD Dn,<ea>" },
	{ "ADD.L D3,($12345678.L,A0,D0.L)", { 0xd7b0, 0x0930, 0x1234, 0x5678, }, 4, "ADD Dn,<ea>" },
	{ "ADD.L D3,([$12345678.L,A0],D0.L)", { 0xd7b0, 0x0935, 0x1234, 0x5678, }, 4, "ADD Dn,<ea>" },
	{ "ADD.L D3,([$12345678.L,A0],D0.L,$1234.W)", { 0xd7b0, 0x0936, 0x1234, 0x5678, 0x1234, }, 5, "ADD Dn,<ea>" },
	{ "ADD.L D3,([$12345678.L,A0],D0.L,$12345678.L)", { 0xd7b0, 0x0937, 0x1234, 0x5678, 0x1234, 0x5678, }, 6, "ADD Dn,<ea>" },
	{ "ADD.L D3,([$1234.W,A0],D0.L,$12345678.L)", { 0xd7b0, 0x0927, 0x1234, 0x1234, 0x5678, }, 5, "ADD Dn,<ea>" },
	{ "ADD.L D3,([A0,D0.L],$12345678.L)", { 0xd7b0, 0x0913, 0x1234, 0x5678, }, 4, "ADD Dn,<ea>" },

};

static const InstructionLengthDecoderTest immediateTests[] =
{
	{ "ADDI.B #$12,D2", { 0x0602, 0x0012, }, 2, "ADDI #imm,<ea>" },
	{ "ADDI.W #$1234,D2", { 0x0642, 0x1234, }, 2, "ADDI #imm,<ea>" },
	{ "ADDI.L #$12345678,D2", { 0x0682, 0x1234, 0x5678, }, 3, "ADDI #imm,<ea>" },
	{ "ADDI.W #$1234,$1234(A1,D2.L)", { 0x0671, 0x1234, 0x2920, 0x1234, }, 4, "ADDI #imm,<ea>" },
	{ "ADDI.W #$1234,$12345678.L", { 0x0679, 0x1234, 0x1234, 0x5678, }, 4, "ADDI #imm,<ea>" },
	{ "ADDI.W #$1234,([$12345678,A1,D2.L])", { 0x0671, 0x1234, 0x2931, 0x1234, 0x5678, }, 5, "ADDI #imm,<ea>" },
};

static const InstructionLengthDecoderTest relativeBranchTests[] =
{
	{ "BRA.S *+$12", { 0x6012, }, 1, "Bcc <relative address>" },
	{ "BNE.W *+$1234", { 0x6600, 0x1234, }, 2, "Bcc <relative address>" },
	{ "BEQ.L *+$12345678", { 0x67ff, 0x1234, 0x5678, }, 3, "Bcc <relative address>" },
	{ "BSR.S *+$12", { 0x6112, }, 1, "BSR <relative address>" },
	{ "BSR.W *+$1234", { 0x6100, 0x1234, }, 2, "BSR <relative address>" },
	{ "BSR.L *+$12345678", { 0x61ff, 0x1234, 0x5678, }, 3, "BSR <relative address>" },
	{ "DBRA *+$12", { 0x51c9, 0x1234, }, 2, "DBcc <relative address>" },
	{ "DBEQ *+$12", { 0x57ca, 0x1234, }, 2, "DBcc <relative address>" },
};

static const InstructionLengthDecoderTest absoluteJumpTests[] =
{
	{ "JMP $1234(A3)", { 0x4eeb, 0x1234, }, 2, "JMP <ea>" },
	{ "JSR ([$12345678.L,PC])", { 0x4ebb, 0x0171, 0x1234, 0x5678, }, 4, "JSR <ea>" },
};

static const InstructionLengthDecoderTest singleBitInstructionTests[] =
{
	{ "BTST D1,$12345678.L", { 0x0339, 0x1234, 0x5678, }, 3, "BTST Dn,<ea>" },
	{ "BCLR #3,$12345678.L", { 0x08b9, 0x0003, 0x1234, 0x5678, }, 4, "BCLR #imm,<ea>" },
	{ "BCHG D1,D3", { 0x0343, }, 1, "BCHG Dn,<ea>" },
	{ "BSET #31,D2", { 0x08c2, 0x001f, }, 2, "BSET #imm,<ea>" },
};

static const InstructionLengthDecoderTest bitFieldInstructionTests[] =
{
	{ "BFCHG $1234(A1,D1.l){D2:D3}", { 0xeaf1, 0x08a3, 0x1920, 0x1234, }, 4, "BFCHG <ea>{Do:Dw}" },
	{ "BFCHG D1{D2:D3}", { 0xeac1, 0x08a3, }, 2, "BFCHG <ea>{Do:Dw}" },
	{ "BFCLR $1234(A1,D1.l){D2:D3}", { 0xecf1, 0x08a3, 0x1920, 0x1234, }, 4, "BFCLR <ea>{Do:Dw}" },
	{ "BFEXTS ($12345678.L,A1,D1.l){D2:D3},D4", { 0xebf1, 0x48a3, 0x1930, 0x1234, 0x5678, }, 5, "BFEXTS <ea>{Do:Dw},Dn" },
	{ "BFEXTU ($1234.w,A1){D2:D3},D4", { 0xe9e9, 0x48a3, 0x1234, }, 3, "BFEXTU <ea>{Do:Dw},Dn" },
	{ "BFFFO (A1){D2:D3},D4", { 0xedd1, 0x48a3, }, 2, "BFFFO <ea>{Do:Dw},Dn" },
	{ "BFINS D1,($1234.w,A1){D2:D3}", { 0xefe9, 0x18a3, 0x1234, }, 3, "BFINS Dn,<ea>{Do:Dw}" },
	{ "BFSET D1{D2:D3}", { 0xeec1, 0x08a3, }, 2, "BFSET <ea>{Do:Dw}" },
	{ "BFTST $1234(A1,D1.l){D2:D3}", { 0xe8f1, 0x08a3, 0x1920, 0x1234, }, 4, "BFTST <ea>{Do:Dw}" },
};

static const InstructionLengthDecoderTest bcdArithmeticTests[] =
{
	{ "ABCD D2,D3", { 0xc702, }, 1, "ABCD Dx,Dy" },
	{ "ABCD -(A1),-(A3)", { 0xc709, }, 1, "ABCD -(Ax),-(Ay)"},

	{ "SBCD D2,D3", { 0x8702, }, 1, "SBCD Dx,Dy" },
	{ "SBCD -(A1),-(A3)", { 0x8709, }, 1, "SBCD -(Ax),-(Ay)" },

	{ "PACK D1,D2,#$1234", { 0x8541, 0x1234, }, 2, "PACK Dm,Dn,#imm" },
	{ "PACK -(A2),-(A1),#$1234", { 0x834a, 0x1234, }, 2, "PACK -(Am),-(An),#imm" },

	{ "UNPK D2,D3,#$1234", { 0x8782, 0x1234, }, 2, "UNPK Dm,Dn,#imm" },
	{ "UNPK -(A7),-(A6),#$1234", { 0x8d8f, 0x1234, }, 2, "UNPK -(Am),-(An),#imm" },
};

static const InstructionLengthDecoderTest integerArithmeticTests[] =
{
	{ "ADD.B D1,D3", { 0xd601, }, 1, "ADD <ea>,Dn" },
	{ "ADD.L D1,D3", { 0xd681, }, 1, "ADD <ea>,Dn" },
	{ "ADDA.W D4,A5", { 0xdac4, }, 1, "ADDA.W <ea>,An" },
	{ "ADDA.L (A2),A5", { 0xdbd2, }, 1, "ADDA.L <ea>,An" },
	{ "ADDQ.L #5,([A2])", { 0x5ab2, 0x0151, }, 2, "ADDQ #imm,<ea>" },
	{ "ADDX.W D2,D3", { 0xd742, }, 1, "ADDX Dx,Dy" },
	{ "ADDX.L -(A1),-(A3)", { 0xd789, }, 1, "ADDX -(Ax),-(Ay)" },
	{ "CMP.L (A3),D1", { 0xb293, }, 1, "CMP <ea>,Dn" },
	{ "CMPA.L D1,A1", { 0xb3c1, }, 1, "CMPA.L <ea>,An" },
	{ "CMPI.L #$12345678,D1", { 0x0c81, 0x1234, 0x5678, }, 3, "CMPI #imm,<ea>" },
	{ "CMPM.B (A5)+,(A4)+", { 0xb90d, }, 1, "CMPM (Ax)+,(Ay)+" },
	{ "CMPM.L (A3)+,(A2)+", { 0xb58b, }, 1, "CMPM (Ax)+,(Ay)+" },

	{ "DIVS.W #$1234,D1", { 0x83fc, 0x1234, }, 2, "DIVS.W <ea>,Dn" },
	{ "DIVS.W $1234(A1),D1", { 0x83e9, 0x1234, }, 2, "DIVS.W <ea>,Dn" },
	{ "DIVU.W $12(A1,D2.L),D3", { 0x86f1, 0x2812, }, 2, "DIVU.W <ea>,Dn" },
	{ "DIVS.L $1234(A1),D0", { 0x4c69, 0x0800, 0x1234 }, 3, "DIVS/DIVU.L <ea>,Dr:Dq (can be 32bit or 64bit division)" },
	{ "DIVS.L $1234(A1),D0:D2", { 0x4c69, 0x2c00, 0x1234 }, 3, "DIVS/DIVU.L <ea>,Dr:Dq (can be 32bit or 64bit division)" },
	{ "DIVUL.L $1234(A1),D0:D2", { 0x4c69, 0x2000, 0x1234 }, 3, "DIVS/DIVU.L <ea>,Dr:Dq (can be 32bit or 64bit division)" },

	{ "MULS.W #$1234,D1", { 0xc3fc, 0x1234, }, 2, "MULS.W <ea>,Dn" },
	{ "MULS.W $1234(A1),D1", { 0xc3e9, 0x1234, }, 2, "MULS.W <ea>,Dn" },
	{ "MULU.W $12(A1,D2.L),D3", { 0xc6f1, 0x2812, }, 2, "MULU.W <ea>,Dn" },
	{ "MULS.L $1234(A1),D0", { 0x4c29, 0x0800, 0x1234 }, 3, "MULS/MULU.L <ea>,Dm:Dn (can be 32bit or 64bit multiply)" },
	{ "MULS.L $1234(A1),D0:D2", { 0x4c29, 0x2c00, 0x1234 }, 3, "MULS/MULU.L <ea>,Dm:Dn (can be 32bit or 64bit multiply)" },

	{ "NBCD (A3)", { 0x4813, }, 1, "NBCD <ea>" },
	{ "NEG.W D7", { 0x4447, }, 1, "NEG <ea>" },
	{ "NEGX.L D5", { 0x4085, }, 1, "NEGX <ea>" },

	{ "SUB.B D1,D2", { 0x9401, }, 1, "SUB <ea>,Dn" },
	{ "SUBA.W D4,A5", { 0x9ac4, }, 1, "SUBA.W <ea>,An" },
	{ "SUBA.W (A2),A5", { 0x9ad2, }, 1, "SUBA.W <ea>,An" },
	{ "SUBA.L (A2),A5", { 0x9bd2, }, 1, "SUBA.L <ea>,An" },
	{ "SUB.L D2,-(A6)", { 0x95a6, }, 1, "SUB Dn,<ea>" },
	{ "SUBI.W #$1234,D5", { 0x0445, 0x1234, }, 2, "SUBI #imm,<ea>" },
	{ "SUBQ.L #4,D0", { 0x5980, }, 1, "SUBQ #imm,<ea>" },
	{ "SUBQ.B #3,([A5])", { 0x5735, 0x0151, }, 2, "SUBQ #imm,<ea>" },

	{ "SUBX.W D1,D1", { 0x9341, }, 1, "SUBX Dx,Dy" },
	{ "SUBX.B -(A4),-(A3)", { 0x970c, }, 1, "SUBX -(Ax),-(Ay)" },

	{ "TST.B D7", { 0x4a07, }, 1, "TST <ea>" },
	{ "TST.L $1234(A4)", { 0x4aac, 0x1234, }, 2, "TST <ea>" },
	{ "TST.W (A0)+", { 0x4a58, }, 1, "TST <ea>" },
};

static const InstructionLengthDecoderTest shiftRotateTests[] =
{
	{ "ASL.W D0,D1", { 0xe161, }, 1, "ASL Dm,Dn" },
	{ "ASL.L #3,D2", { 0xe782, }, 1, "ASL #imm,Dn" },
	{ "ASR.B #1,D1", { 0xe201, }, 1, "ASR #imm,Dn" },
	{ "ASR.W $1234(A5)", { 0xe0ed, 0x1234, }, 2, "ASR <ea>" },

	{ "LSL.L D2,D3", { 0xe5ab, }, 1, "LSL Dm,Dn" },
	{ "LSR.W #2,D7", { 0xe44f, }, 1, "LSR #imm,Dn" },
	{ "LSL.W ([A4])", { 0xe3f4, 0x0151, }, 2, "LSL <ea>" },

	{ "ROL.W #2,D3", { 0xe55b, }, 1, "ROL #imm,Dn" },
	{ "ROR.L D2,D7", { 0xe4bf, }, 1, "ROR Dm,Dn" },
	{ "ROL (A2)", { 0xe7d2, }, 1, "ROL <ea>" },

	{ "ROXL.W #1,D4", { 0xe354, }, 1, "ROXL #imm,Dn" },
	{ "ROXL ([$12345678,A5])", { 0xe5f5, 0x0171, 0x1234, 0x5678, }, 4, "ROXL <ea>" },
	{ "ROXR.L D2,D2", { 0xe4b2, }, 1, "ROXR Dm,Dn" },
};

static const InstructionLengthDecoderTest integerLogicTests[] =
{
	{ "AND.W $1234(A1),D3", { 0xc669, 0x1234, }, 2, "AND <ea>,Dn" },
	{ "AND.L D3,$1234(A1)", { 0xc7a9, 0x1234, }, 2, "AND Dn,<ea>" },
	{ "ANDI.L #$12345678,($12345678,D2)", { 0x02b0, 0x1234, 0x5678, 0x21b0, 0x1234, 0x5678, }, 6, "ANDI #imm,<ea>" },

	{ "ANDI #$12,CCR", { 0x023c, 0x0012, }, 2, "ANDI #imm,CCR" },

	{ "EOR.L D1,(A3)", { 0xb393, }, 1, "EOR.L Dn,<ea>" },
	{ "EORI.L #$12345678,$1234(A3)", { 0x0aab, 0x1234, 0x5678, 0x1234, }, 4, "EORI #imm,<ea>" },

	{ "EORI #$12,CCR", { 0x0a3c, 0x0012, }, 2, "EORI #imm,CCR" },

	{ "NOT.W -(A5)", { 0x4665, }, 1, "NOT <ea>" },

	{ "OR.L $1234(A3),D2", { 0x84ab, 0x1234, }, 2, "OR <ea>,Dn" },
	{ "OR.B d2,([A4])", { 0x8534, 0x0151, }, 2, "OR Dn,<ea>" },
	{ "ORI.W #$1234,d5", { 0x0045, 0x1234, }, 2, "ORI #imm,<ea>" },

	{ "ORI #$12,CCR", { 0x003c, 0x0012, }, 2, "ORI #imm,CCR" },

};

static const InstructionLengthDecoderTest moveTests[] =
{
	{ "EXG D2,D3", { 0xc543, }, 1, "EXG Dm,Dn" },
	{ "EXG A4,A5", { 0xc94d, }, 1, "EXG Am,An" },
	{ "EXG D3,A7", { 0xc78f, }, 1, "EXG Dm,An" },

	{ "MOVE.W D3,D4", { 0x3803, }, 1, "MOVE.W <ea>,<ea>" },
	{ "MOVE.B #$12,D2", { 0x143c, 0x0012, }, 2, "MOVE.B <ea>,<ea>" },
	{ "MOVE.L #$12345678,(A6)", { 0x2cbc, 0x1234, 0x5678, }, 3, "MOVE.L <ea>,<ea>" },
	{ "MOVE.W (A7),D5", { 0x3a17, }, 1, "MOVE.W <ea>,<ea>" },
	{ "MOVE.L #$12345678,([$12345678.L,A1])", { 0x23bc, 0x1234, 0x5678, 0x0171, 0x1234, 0x5678, }, 6, "MOVE.L <ea>,<ea>" },
	{ "MOVE.L ([$12345678.L,A1]),([$12345678.L,A2])", { 0x25b1, 0x0171, 0x1234, 0x5678, 0x0171, 0x1234, 0x5678, }, 7, "MOVE.L <ea>,<ea>" },
	{ "MOVE.L ([$12345678.L,PC]),([D0.W],$12345678.L)", { 0x21bb, 0x0171, 0x1234, 0x5678, 0x0193, 0x1234, 0x5678, }, 7, "MOVE.L <ea>,<ea>" },
	{ "MOVEA.W D2,A2", { 0x3442, }, 1, "MOVEA.W <ea>,<ea>" },
	{ "MOVEA.L D2,A2", { 0x2442, }, 1, "MOVEA.L <ea>,<ea>" },
	{ "MOVE.L A2,D3", { 0x260a, }, 1, "MOVE.L <ea>,<ea>" },

	{ "MOVE CCR,$1234(A3)", { 0x42eb, 0x1234, }, 2, "MOVE CCR,<ea>" },
	{ "MOVE $1234(A2),CCR", { 0x44ea, 0x1234, }, 2, "MOVE <ea>,CCR" },

	{ "MOVE SR,$1234(A3)", { 0x40eb, 0x1234, }, 2, "MOVE SR,<ea>" },
	
	{ "MOVEQ #$12,D2", { 0x7412, }, 1, "MOVEQ #imm,Dn" },

	{ "MOVE16 (A2)+,(A3)+", { 0xf622, 0xb000, }, 2, "MOVE16 (Ax)+,(Ay)+" },
	{ "MOVE16 $12345678.L,(A1)", { 0xf619, 0x1234, 0x5678, }, 3, "MOVE16 (An <-> xxx.L)" },
	{ "MOVE16 (A2)+,$12345678.L", { 0xf602, 0x1234, 0x5678, }, 3, "MOVE16 (An <-> xxx.L)" },
	
	{ "MOVEM.L D2-D7,-(A7)", { 0x48e7, 0x3f00, }, 2, "MOVEM reglist,<ea>" },
	{ "MOVEM.W (A3),D0/D2", { 0x4c93, 0x0005, }, 2, "MOVEM <ea>,reglist" },

	{ "MOVEP.W D2,$1234(A1)", { 0x0589, 0x1234, }, 2, "MOVEP Dx <-> d16(An)" },
	{ "MOVEP.L $1234(A2),D3", { 0x074a, 0x1234, }, 2, "MOVEP Dx <-> d16(An)" },
};

static const InstructionLengthDecoderTest miscellaneousTests[] =
{
	{ "BKPT #4", { 0x484c, }, 1, "BKPT #imm" },
	{ "CHK.W $1234(A1),D1", { 0x43a9, 0x1234, }, 2, "CHK.W <ea>,Dn" },
	{ "CHK.L #$12345678,D1", { 0x433c, 0x1234, 0x5678, }, 3, "CHK.L <ea>,Dn" },
	{ "CLR.W $1234(A1)", { 0x4269, 0x1234, }, 2, "CLR <ea>" },

	{ "EXT.W D2", { 0x4882, }, 1, "EXT.W Dn" },
	{ "EXT.L D3", { 0x48c3, }, 1, "EXT.L Dn" },
	{ "EXTB.L D4", { 0x49c4, }, 1, "EXTB.L Dn" },
	{ "ILLEGAL", { 0x4afc, }, 1, "ILLEGAL" },
	
	{ "LEA ([A3,D2.L*4],$1234.W),A2", { 0x45f3, 0x2d12, 0x1234, }, 3, "LEA <ea>,An" },
	
	{ "LINK.W A3,#$1234", { 0x4e53, 0x1234, }, 2, "LINK.W An,#imm" },
	{ "LINK.L A3,#$12345678", { 0x480c, 0x1234, 0x5678, }, 3, "LINK.L An,#imm" },

	{ "NOP", { 0x4e71, }, 1, "NOP" },

	{ "PEA $12(A3,D1.W)", { 0x4873, 0x1012, }, 2, "PEA <ea>" },
	
	{ "RTD #$1234", { 0x4e74, 0x1234, }, 2, "RTD #imm" },
	{ "RTR", { 0x4e77, }, 1, "RTR" },
	{ "RTS", { 0x4e75, }, 1, "RTS" },

	{ "ST D7", { 0x50c7, }, 1, "Scc <ea>" },
	{ "SNE (A2)", { 0x56d2, }, 1, "Scc <ea>" },
	{ "SEQ $1234(A3)", { 0x57eb, 0x1234, }, 2, "Scc <ea>" },

	{ "SWAP D3", { 0x4843, }, 1, "SWAP Dn" },

	{ "TAS D2", { 0x4ac2, }, 1, "TAS <ea>" },
	{ "TAS $12(A5,D3.L)", { 0x4af5, 0x3812, }, 2, "TAS <ea>" },

	{ "TRAP #3", { 0x4e43, }, 1, "TRAP #imm" },
	{ "TRAPNE", { 0x56fc, }, 1, "TRAPcc" },
	{ "TRAPGE.W #$1234", { 0x5cfa, 0x1234, }, 2, "TRAPcc.W #imm" },
	{ "TRAPLT.L #$12345678", { 0x5dfb, 0x1234, 0x5678, }, 3, "TRAPcc.L #imm" },
	{ "TRAPV", { 0x4e76, }, 1, "TRAPV" },
	
	{ "UNLK A3", { 0x4e5b, }, 1, "UNLK An" },
};

TestSuite testSuites[] =
{
	{ "6-bit EA decoding tests", ea6BitTests, (sizeof ea6BitTests / sizeof ea6BitTests[0]) },
	{ "Immediate source operand tests", immediateTests, (sizeof immediateTests / sizeof immediateTests[0]) },
	{ "Relative branch tests", relativeBranchTests, (sizeof relativeBranchTests / sizeof relativeBranchTests[0]) },
	{ "Absolute jump tests", absoluteJumpTests, (sizeof absoluteJumpTests / sizeof absoluteJumpTests[0]) },
	{ "Single-bit instruction tests", singleBitInstructionTests, (sizeof singleBitInstructionTests / sizeof singleBitInstructionTests[0]) },
	{ "Bit field instruction tests", bitFieldInstructionTests, (sizeof bitFieldInstructionTests / sizeof bitFieldInstructionTests[0]) },
	{ "BCD arithmetic tests", bcdArithmeticTests, (sizeof bcdArithmeticTests / sizeof bcdArithmeticTests[0]) },
	{ "Integer arithmetic tests", integerArithmeticTests, (sizeof integerArithmeticTests / sizeof integerArithmeticTests[0]) },
	{ "Shift/rotate tests", shiftRotateTests, (sizeof shiftRotateTests / sizeof shiftRotateTests[0]) },
	{ "Integer logic tests", integerLogicTests, (sizeof integerLogicTests / sizeof integerLogicTests[0]) },
	{ "Move/Exchange tests", moveTests, (sizeof moveTests / sizeof moveTests[0]) },
	{ "Miscellaneous tests", miscellaneousTests, (sizeof miscellaneousTests / sizeof miscellaneousTests[0]) },
};

void runTestSuite(const InstructionLengthDecoderTest* tests, uint numTests, bool printSuccess, bool printFailure, uint* accumulatedSuccessfulTests, uint* accumulatedTotalTests)
{
	uint numSuccessfulTests = 0;
	uint numTotalTests = 0;
	uint testId;
	
	for (testId = 0; testId < numTests; ++testId)
	{
		bool success = true;
		const InstructionLengthDecoderTest* test = &tests[testId];
		InstructionLength instructionLength;
		bool correctLength = true;
		bool correctDescription = true;
		
		if (!decodeInstructionLengthFromInstructionWords(test->instructionWords, test->expectedNumInstructionWords, &instructionLength))
			success = false;
		else
		{
			if (instructionLength.totalWords != test->expectedNumInstructionWords)
			{
				correctLength = false;
				success = false;
			}
			if ((!instructionLength.description && test->expectedDescription)
				|| (test->expectedDescription && strcmp(test->expectedDescription, instructionLength.description)))
			{
				correctDescription = false;
				success = false;
			}
		}
		
		if (success && printSuccess)
			printf("success: Decoding %s yields %u words (%s)\n", test->instruction, instructionLength.totalWords, instructionLength.description);
		if (!success && printFailure)
		{
			printf("failure: Decoding %s yields %s", test->instruction, instructionLength.description ? instructionLength.description : "(no description)");
			if (!correctDescription)
				printf(" (should be %s)", test->expectedDescription ? test->expectedDescription : "(no description)");

			printf(", %u words", instructionLength.totalWords);
			if (!correctLength)
				printf(" (should be %u)", test->expectedNumInstructionWords);

			printf("\n");
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

	printf("Testing M68060 Instruction Length Decoder\n");
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