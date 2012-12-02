
#ifndef M68060DECOMPOSEOPINTOUOPS_H
#define M68060DECOMPOSEOPINTOUOPS_H

#include "../Types.h"
#include "M68060uOP.h"

bool decomposeOpIntouOPs(uint16_t* instructionStream, uint numInstructionWords, uOP* uOPs, uint* numuOPs);


#endif
