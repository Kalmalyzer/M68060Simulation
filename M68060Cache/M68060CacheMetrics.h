
#ifndef M68060CacheMetrics_h
#define M68060CacheMetrics_h

#include "Types.h"

typedef struct
{
	uint32_t lineReadHits;			// Cache line reads that have hit an existing cache line
	uint32_t lineReadMisses;		// Cache line reads that have missed all existing cache lines
	uint32_t lineWriteHits;			// Cache line writes that have hit an existing cache line
	uint32_t lineWriteMisses;		// Cache line writes that have missed all existing cache lines
	uint32_t cleanLineEvictions;	// Evictions of clean cache lines
	uint32_t dirtyLineEvictions;	// Evictions of dirty cache lines; each eviction results in writeback of the old cache line contents
	uint32_t lineAllocations;		// Allocations/re-allocations of cache lines
} M68060CacheMetrics;

void M68060CacheMetrics_diff(M68060CacheMetrics* result, M68060CacheMetrics* oldMetrics, M68060CacheMetrics* newMetrics);

void M68060CacheMetrics_print(M68060CacheMetrics* metrics);

#endif
