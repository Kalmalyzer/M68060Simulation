
#ifndef M68060FLAGSMODIFIER_H
#define M68060FLAGSMODIFIER_H

#include "../Types.h"
#include "../M68060ISA/M68060Flags.h"

typedef struct
{
	uint andFlags;
	uint orFlags;

} FlagsModifier;

Flags applyFlagsModifier(Flags flags, const FlagsModifier* modifier);

#endif
