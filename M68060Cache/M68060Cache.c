
#include "M68060Cache.h"

#include <assert.h>

static M68060Cache Cache;

typedef struct
{
	uint32_t address;
	int size;
} CacheLineAccess;


// Split an address into tag / set index / offset

static void splitAddress(uint32_t address, int* tag, int* set, int* offset)
{
	*offset = (address & M68060CacheAddressOffsetMask) >> M68060CacheAddressOffsetShift;
	*set = (address & M68060CacheAddressSetMask) >> M68060CacheAddressSetShift;
	*tag = (address & M68060CacheAddressTagMask) >> M68060CacheAddressTagShift;
}

// Split an access to an arbitrary address + size into one or two cache line accesses

static void splitAccess(uint32_t address, int size, int* numAccesses, CacheLineAccess* accesses)
{
	uint32_t firstByte = address;
	uint32_t lastByte = address + size - 1;
	if ((firstByte & ~M68060CacheAddressOffsetMask) != (lastByte & ~M68060CacheAddressOffsetMask))
	{
		*numAccesses = 2;
		accesses[0].address = firstByte;
		accesses[0].size = (-((int) firstByte)) & M68060CacheAddressOffsetMask;
		accesses[1].address = (firstByte + M68060CacheAddressOffsetMask + 1) | M68060CacheAddressOffsetMask;
		accesses[1].size = size - accesses[0].size;
	}
	else
	{
		*numAccesses = 1;
		accesses[0].address = address;
		accesses[0].size = size;
	}
}

static int generateRandomLineId(void)
{
	// The 68060 has a deterministic random number generator. It is based on various events from various places
	// in the CPU. The exact events are unknown so we can't really do better than picking an arbitrary, half-decent RNG instead.
	return rand() % M68060CacheLinesPerSet;
}

// Determine whether an access to a certain address results in a cache hit or cache miss

static int checkForHit(M68060Cache* cache, int addressTag, int setId)
{
	M68060CacheSet* set = &cache->sets[setId];
	int lineId;
	
	for (lineId = 0; lineId < M68060CacheLinesPerSet; ++lineId)
	{
		if (set->lines[lineId].tag == addressTag)
			return lineId;
	}

	return -1;
}

// Evict a currently-allocated line from the cache

static void evictLine(M68060Cache* cache, int setId, int lineId)
{
	M68060CacheSet* set = &cache->sets[setId];
	M68060CacheLine* line = &set->lines[lineId];
	assert(line->valid && "evictLine requires that the cache line be currently valid");
	line->valid = false;
	line->dirty = false;
}

// Mark a currently-allocated line as dirty
// Returns true if the line used to be clean, and got marked as dirty by the call

static bool dirtyLine(M68060Cache* cache, int setId, int lineId)
{
	M68060CacheSet* set = &cache->sets[setId];
	M68060CacheLine* line = &set->lines[lineId];
	bool changedFromCleanToDirty = !line->dirty;
	assert(line->valid && "dirtyLine requires that the cache line be currently valid");
	line->dirty = true;
	return changedFromCleanToDirty;
}

// Find an unallocated line in the given set
// Returns line index or -1 if all are allocated

static int locateFreeLine(M68060Cache* cache, int setId)
{	
	M68060CacheSet* set = &cache->sets[setId];
	int lineId;

	for (lineId = 0; lineId < M68060CacheLinesPerSet; ++lineId)
	{
		M68060CacheLine* line = &set->lines[lineId];

		if (!line->valid)
			return lineId;
	}
	
	return -1;
}

// Allocate a cache line to cache a specific address; evict another line if necessary
// The given address must not already be cached by a line

static void allocateLine(M68060Cache* cache, int addressTag, int setId, int* lineId, bool* evictedCleanLine, bool* evictedDirtyLine)
{
	M68060CacheSet* set = &cache->sets[setId];
	M68060CacheLine* line;
	
	*lineId = locateFreeLine(cache, setId);

	if (*lineId == -1)
		*lineId = generateRandomLineId();

	line = &set->lines[*lineId];

	if (line->valid)
	{
		if (line->dirty)
		{
			*evictedCleanLine = false;
			*evictedDirtyLine = true;
			cache->metrics.dirtyLineEvictions++;
		}
		else
		{
			*evictedCleanLine = true;
			*evictedDirtyLine = false;
			cache->metrics.cleanLineEvictions++;
		}
		evictLine(cache, setId, *lineId);
	}
	else
	{
		*evictedCleanLine = false;
		*evictedDirtyLine = false;
	}

	cache->metrics.lineAllocations++;
	line->valid = true;
	line->tag = addressTag;
}

// Perform the cache operations required to complete a memory read or write access

static void access(M68060Cache* cache, uint32_t address, int size, M68060CacheAccessResult* result, bool writeAccess)
{
	CacheLineAccess accesses[2];
	int numAccesses;
	int accessId;

	splitAccess(address, size, &numAccesses, accesses);

	result->numAccesses = numAccesses;

	for (accessId = 0; accessId < numAccesses; ++accessId)
	{
		CacheLineAccess* access = &accesses[accessId];
		M68060CacheLineAccessResult* accessResult = &result->accesses[accessId];
		uint32_t addressTag;
		int setId;
		int offset;
		int lineId;
		
		splitAddress(access->address, &addressTag, &setId, &offset);
		lineId = checkForHit(cache, addressTag, setId);

		memset(accessResult, 0, sizeof(*accessResult));
		
		if (lineId != -1)
		{
			if (writeAccess)
				cache->metrics.lineWriteHits++;
			else
				cache->metrics.lineReadHits++;
			
			accessResult->hit = true;
		}
		else
		{
			if (writeAccess)
				cache->metrics.lineWriteMisses++;
			else
				cache->metrics.lineReadMisses++;

			accessResult->hit = false;
	
			allocateLine(cache, addressTag, setId, &lineId, &accessResult->evictedCleanLine, &accessResult->evictedDirtyLine);
			accessResult->allocatedNewLine = true;
		}

		if (writeAccess)
			accessResult->dirtiedLine = dirtyLine(cache, setId, lineId);
	
		accessResult->set = setId;
		accessResult->line = lineId;
	}
}

void M68060Cache_read(M68060Cache* cache, uint32_t address, int size, M68060CacheAccessResult* result)
{
	access(cache, address, size, result, false);
}

void M68060Cache_write(M68060Cache* cache, uint32_t address, int size, M68060CacheAccessResult* result)
{
	access(cache, address, size, result, true);
}

void M68060Cache_init(M68060Cache* cache)
{
	memset(cache, 0, sizeof(*cache));
}

void M68060Cache_invalidate(M68060Cache* cache)
{
	memset(cache->sets, 0, sizeof(cache->sets));
}
