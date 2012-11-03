
#ifndef M68060OPWORD_H
#define M68060OPWORD_H

typedef enum
{
	OperationSize_Reserved,
	OperationSize_Byte,
	OperationSize_Word,
	OperationSize_Long,
} OperationSize;

enum { OpWord_OperationSize_Shift = 9 };
enum { OpWord_OperationSize_Bits = 2 };
enum { OpWord_OperationSize_Mask = ((1 << OpWord_OperationSize_Bits) - 1) << OpWord_OperationSize_Shift };

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

#endif
