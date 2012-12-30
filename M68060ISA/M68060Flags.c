
#include "M68060Flags.h"
#include "../Assert.h"

const char* flagsToString(Flags flags)
{
	const char* flagStrings[32] = {
		"(none)", "C", "V", "VC", "Z", "ZC", "ZV", "ZVC", "N", "NC", "NV", "NVC", "NZ", "NZC", "NZV", "NZVC",
		"X", "XC", "XV", "XVC", "XZ", "XZC", "XZV", "XZVC", "XN", "XNC", "XNV", "XNVC", "XNZ", "XNZC", "XNZV", "XNZVC",
	};

	M68060_ASSERT(flags < (sizeof flagStrings / sizeof flagStrings[0]), "Unsupported flags value");
	
	return flagStrings[flags];
}
