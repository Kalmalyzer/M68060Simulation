
#ifndef M68060PAIRABILITYTESTS_H
#define M68060PAIRABILITYTESTS_H

#include "M68060OpWordDecoder.h"

typedef enum
{
	PairabilityTestResult_Success,
	PairabilityTestResult_Test2Failure_FirstInstructionIs_pOEPOnly,
	PairabilityTestResult_Test2Failure_SecondInstructionIsNot_pOEPOrsOEP,
} PairabilityTestResult;

const char* PairabilityTestResultToString(PairabilityTestResult pairabilityTestResult);

PairabilityTestResult checkPairability(DecodedOpWord* opWord0, DecodedOpWord* opWord1);

#endif
