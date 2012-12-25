
#ifndef M68060FLAGS_H
#define M68060FLAGS_H

#include "../Types.h"
#include "../M68060InstructionDecoder/M68060InstructionDecoderTypes.h"

typedef enum
{
	Flags_Carry_Mask = 1,
	Flags_Overflow_Mask = 2,
	Flags_Zero_Mask = 4,
	Flags_Negative_Mask = 8,
	Flags_Extend_Mask = 0x10,
	Flags_All_Mask = (Flags_Extend_Mask | Flags_Negative_Mask | Flags_Zero_Mask | Flags_Overflow_Mask | Flags_Carry_Mask),
};

typedef struct
{
	uint andFlags;
	uint orFlags;

} FlagsModifier;

typedef uint Flags;

Flags applyFlagsModifier(Flags flags, const FlagsModifier* modifier);

const char* flagsToString(Flags flags);

#endif
