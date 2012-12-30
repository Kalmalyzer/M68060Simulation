
#include "M68060UOp.h"
#include <stdio.h>
#include <string.h>

bool areUOpsEquivalent(const UOp* a, const UOp* b)
{
	return
		!strcmp(a->description, b->description)
		&& a->opWord == b->opWord
		&& a->extensionWords[0] == b->extensionWords[0]
		&& a->extensionWords[1] == b->extensionWords[1]
		&& a->aguBase == b->aguBase
		&& a->aguIndex == b->aguIndex
		&& a->aguIndexShift == b->aguIndexShift
		&& a->aguIndexSize == b->aguIndexSize
		&& a->aguDisplacementSize == b->aguDisplacementSize
		&& a->aguOperation == b->aguOperation
		&& a->aguResult == b->aguResult
		&& a->memoryRead == b->memoryRead
		&& a->ieeA == b->ieeA
		&& a->ieeB == b->ieeB
		&& a->ieeOperationSize == b->ieeOperationSize
		&& a->ieeOperation == b->ieeOperation
		&& a->ieeResult == b->ieeResult
		&& a->memoryWrite == b->memoryWrite
		&& a->pairability == b->pairability;
}

void printUOp(const UOp* UOp)
{
	printf("OpWord & ExtensionWords: %04x,%04x,%04x\n", UOp->opWord, UOp->extensionWords[0], UOp->extensionWords[1]);
	printf("Agu: Base %s, Index %s, IndexShift %d, IndexSize %s, DisplacementSize %s, Operation %s, Result %s\n",
		ExecutionResourceToString(UOp->aguBase), ExecutionResourceToString(UOp->aguIndex),
		UOp->aguIndexShift, AguIndexSizeToString(UOp->aguIndexSize), AguDisplacementSizeToString(UOp->aguDisplacementSize),
		AguOperationToString(UOp->aguOperation), ExecutionResourceToString(UOp->aguResult));
	printf("MemoryRead: %s\n", UOp->memoryRead ? "yes" : "no");
	printf("Iee: A %s, B %s, OperationSize %s, Operation %s, Result %s\n",
		ExecutionResourceToString(UOp->ieeA), ExecutionResourceToString(UOp->ieeB),
		OperationSizeToString(UOp->ieeOperationSize), IeeOperationToString(UOp->ieeOperation), ExecutionResourceToString(UOp->ieeResult));
	printf("MemoryWrite: %s\n", UOp->memoryWrite ? "yes" : "no");
	printf("Pairability: %s\n", PairabilityToString(UOp->pairability));
}
