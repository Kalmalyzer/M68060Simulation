
#ifndef M68060CONDITIONCODE_H
#define M68060CONDITIONCODE_H

#include "M68060Flags.h"

typedef enum
{
	ConditionCode_T,
	ConditionCode_F,
	ConditionCode_Hi,
	ConditionCode_Ls,
	ConditionCode_Cc,
	ConditionCode_Cs,
	ConditionCode_Ne,
	ConditionCode_Eq,
	ConditionCode_Vc,
	ConditionCode_Vs,
	ConditionCode_Pl,
	ConditionCode_Mi,
	ConditionCode_Ge,
	ConditionCode_Lt,
	ConditionCode_Gt,
	ConditionCode_Le,

} ConditionCode;

const char* ConditionCodeToString(ConditionCode conditionCode);

bool evaluateConditionCode(ConditionCode conditionCode, Flags flags);

#endif
