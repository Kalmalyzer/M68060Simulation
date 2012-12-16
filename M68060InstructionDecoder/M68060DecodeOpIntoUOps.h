
#ifndef M68060DECOMPOSEOPINTOUOPS_H
#define M68060DECOMPOSEOPINTOUOPS_H

#include "../Types.h"
#include "M68060UOp.h"

bool decomposeOpIntoUOps(const uint16_t* instructionStream, uint numInstructionWords, UOp* UOps, uint* numUOps);


#endif
