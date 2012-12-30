
#ifndef M68060SIMPLEOEP_H
#define M68060SIMPLEOEP_H

#include "../M68060InstructionDecoder/M68060UOp.h"
#include "../M68060ISA/M68060Flags.h"

void executeUOp(const UOp* UOp);

void resetSimpleOEP(void);
Flags readFlags(void);
void writeFlags(Flags flags);

#endif
