
#include "M68060Flags.h"
#include "../Assert.h"

Flags applyFlagsModifier(Flags flags, const FlagsModifier* modifier)
{
	return (flags & modifier->andFlags) | modifier->orFlags;
}
