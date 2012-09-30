
#include "M68060CacheMetrics.h"

#include <stdio.h>

void M68060CacheMetrics_diff(M68060CacheMetrics* result, M68060CacheMetrics* oldMetrics, M68060CacheMetrics* newMetrics)
{
	result->lineReadHits = newMetrics->lineReadHits - oldMetrics->lineReadHits;
	result->lineReadMisses = newMetrics->lineReadMisses - oldMetrics->lineReadMisses;
	result->lineWriteHits = newMetrics->lineWriteHits - oldMetrics->lineWriteHits;
	result->lineWriteMisses = newMetrics->lineWriteMisses - oldMetrics->lineWriteMisses;
	result->cleanLineEvictions = newMetrics->cleanLineEvictions - oldMetrics->cleanLineEvictions;
	result->dirtyLineEvictions = newMetrics->dirtyLineEvictions - oldMetrics->dirtyLineEvictions;
	result->lineAllocations = newMetrics->lineAllocations - oldMetrics->lineAllocations;
}

void M68060CacheMetrics_print(M68060CacheMetrics* metrics)
{
	printf("lineReadHits: %d\n", metrics->lineReadHits);
	printf("lineReadMisses: %d\n", metrics->lineReadMisses);
	printf("lineWriteHits: %d\n", metrics->lineWriteHits);
	printf("lineWriteMisses: %d\n", metrics->lineWriteMisses);
	printf("cleanLineEvictions: %d\n", metrics->cleanLineEvictions);
	printf("dirtyLineEvictions: %d\n", metrics->dirtyLineEvictions);
	printf("lineAllocations: %d\n", metrics->lineAllocations);
}