
#include "M68060ConditionCode.h"
#include "../Assert.h"


const char* ConditionCodeToString(ConditionCode conditionCode)
{
	const char* conditionCodeStrings[32] = {
		"(none)", "C", "V", "VC", "Z", "ZC", "ZV", "ZVC", "N", "NC", "NV", "NVC", "NZ", "NZC", "NZV", "NZVC",
		"X", "XC", "XV", "XVC", "XZ", "XZC", "XZV", "XZVC", "XN", "XNC", "XNV", "XNVC", "XNZ", "XNZC", "XNZV", "XNZVC",
	};

	M68060_ASSERT(conditionCode < (sizeof conditionCodeStrings / sizeof conditionCodeStrings[0]), "Unsupported condition code value");
	
	return conditionCodeStrings[conditionCode];
}

bool evaluateConditionCode(ConditionCode conditionCode, Flags flags)
{
	bool n = (flags & Flags_Negative_Mask);
	bool z = (flags & Flags_Zero_Mask);
	bool v = (flags & Flags_Overflow_Mask);
	bool c = (flags & Flags_Carry_Mask);
	
	switch (conditionCode)
	{
		case ConditionCode_T:
			return true;
		case ConditionCode_F:
			return false;
		case ConditionCode_Hi:
			return !c && !z;
		case ConditionCode_Ls:
			return c || z;
		case ConditionCode_Cc:
			return !c;
		case ConditionCode_Cs:
			return c;
		case ConditionCode_Ne:
			return !z;
		case ConditionCode_Eq:
			return z;
		case ConditionCode_Vc:
			return !v;
		case ConditionCode_Vs:
			return v;
		case ConditionCode_Pl:
			return !n;
		case ConditionCode_Mi:
			return n;
		case ConditionCode_Ge:
			return (n && v) || (!n && !v);
		case ConditionCode_Lt:
			return (n && !v) || (!n && v);
		case ConditionCode_Gt:
			return (n && v && !z) || (!n && !v && !z);
		case ConditionCode_Le:
			return z || (n && !v) || (!n && v);
		default:
			M68060_ERROR("ConditionCode not supported");
			return false;
	};
}
