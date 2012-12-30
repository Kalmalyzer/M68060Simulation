
#ifndef M68060OPWORD_H
#define M68060OPWORD_H

typedef enum
{
	OpSize_Byte,
	OpSize_Word,
	OpSize_Long,
} OpSize;

enum { OpWord_OpSize_Shift = 6 };
enum { OpWord_OpSize_Bits = 2 };
enum { OpWord_OpSize_Mask = ((1 << OpWord_OpSize_Bits) - 1) << OpWord_OpSize_Shift };

typedef enum
{
	OpMode_MemoryToDataRegister_Byte,
	OpMode_MemoryToDataRegister_Word,
	OpMode_MemoryToDataRegister_Long,
	OpMode_MemoryToAddressRegister_Word,
	OpMode_DataRegisterToMemory_Byte,
	OpMode_DataRegisterToMemory_Word,
	OpMode_DataRegisterToMemory_Long,
	OpMode_MemoryToAddressRegister_Long,
} OpMode;

enum { OpWord_OpMode_Shift = 6 };
enum { OpWord_OpMode_Bits = 3 };
enum { OpWord_OpMode_Mask = ((1 << OpWord_OpMode_Bits) - 1) << OpWord_OpMode_Shift };

enum { OpWord_DefaultRegisterEncoding_Shift = 0 };
enum { OpWord_DefaultRegisterEncoding_Bits = 3 };
enum { OpWord_DefaultRegisterEncoding_Mask = ((1 << OpWord_DefaultRegisterEncoding_Bits) - 1) << OpWord_DefaultRegisterEncoding_Shift };

enum { OpWord_SecondaryRegisterEncoding_Shift = 9 };
enum { OpWord_SecondaryRegisterEncoding_Bits = 3 };
enum { OpWord_SecondaryRegisterEncoding_Mask = ((1 << OpWord_SecondaryRegisterEncoding_Bits) - 1) << OpWord_SecondaryRegisterEncoding_Shift };

enum { OpWord_DefaultImm3BitEncoding_Shift = 9 };
enum { OpWord_DefaultImm3BitEncoding_Bits = 3 };
enum { OpWord_DefaultImm3BitEncoding_Mask = ((1 << OpWord_DefaultImm3BitEncoding_Bits) - 1) << OpWord_DefaultImm3BitEncoding_Shift };

enum { OpWord_DefaultImm8BitEncoding_Shift = 0 };
enum { OpWord_DefaultImm8BitEncoding_Bits = 8 };
enum { OpWord_DefaultImm8BitEncoding_Mask = ((1 << OpWord_DefaultImm8BitEncoding_Bits) - 1) << OpWord_DefaultImm8BitEncoding_Shift };

enum { OpWord_DefaultConditionCodeEncoding_Shift = 8 };
enum { OpWord_DefaultConditionCodeEncoding_Bits = 4 };
enum { OpWord_DefaultConditionCodeEncoding_Mask = ((1 << OpWord_DefaultConditionCodeEncoding_Bits) - 1) << OpWord_DefaultConditionCodeEncoding_Shift };

#endif
