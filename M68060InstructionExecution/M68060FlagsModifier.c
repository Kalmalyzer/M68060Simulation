
#include "M68060FlagsModifier.h"

Flags applyFlagsModifier(Flags flags, const FlagsModifier* modifier)
{
	return (flags & modifier->andFlags) | modifier->orFlags;
}
