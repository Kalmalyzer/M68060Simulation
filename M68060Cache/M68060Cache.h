
#ifndef M68060Cache_h
#define M68060Cache_h

#include "../Types.h"
#include "M68060CacheMetrics.h"

// The 68060 has two caches: an instruction cache and a datacache.
// The instruction cache and data cache are identical except that the instruction cache is read-only while
// the datacache also supports write accesses.


// A 68060 cache is 8kB in size. Those 8kB of storage can be broken down as follows:
// Each cache line is 16 bytes in size.
// A group of 4 cache lines make up one set.
// There are 128 sets in a full cache.
// 16 bytes/line * 4 lines/set * 128 sets/cache = 8kB/cache

enum { M68060CacheAddressOffsetBits = 4 };
enum { M68060CacheAddressSetBits = 7 };
enum { M68060CacheAddressTagBits = 32 - M68060CacheAddressSetBits - M68060CacheAddressOffsetBits };

enum { M68060CacheLinesPerSet = 4 };
enum { M68060CacheSets = 1 << M68060CacheAddressSetBits };

enum { M68060CacheAddressOffsetShift = 0 };
enum { M68060CacheAddressSetShift = M68060CacheAddressOffsetBits };
enum { M68060CacheAddressTagShift = M68060CacheAddressSetShift + M68060CacheAddressSetBits };

enum { M68060CacheAddressOffsetMask = (1 << M68060CacheAddressOffsetBits) - 1 };
enum { M68060CacheAddressSetMask = ((1 << M68060CacheAddressSetBits) - 1) << M68060CacheAddressSetShift };
enum { M68060CacheAddressTagMask = ((1 << M68060CacheAddressTagBits) - 1) << M68060CacheAddressTagShift };

// A cache line is the smallest unit of memory that the cache deals with when caching.
// A cache line can cache one 16-byte-aligned 16-bytes-large chunk of memory.
// Exactly which address which lines can cache which chunks of memory is also dependent on which sets they are part of.

typedef struct 
{
	// The tag indicates the exact memory location which the cache line is assigned to
	uint32_t tag;

	// A cache line has three valid states:
	// valid = false, dirty = false:	the cache line is unused
	// valid = true,  dirty = false:	the cache line contains data that has been read from memory
	// valid = true,  dirty = true:		the cache line contains data that has been read from memory, and then modified

	// The state always applies to the whole cache line. That is; there is no way to cache half a cache line,
	// or to mark a portion of a cache line as dirty.
	
	bool valid;
	bool dirty;
	
} M68060CacheLine;

// A set is a group of cache lines which are able to cache the exact same memory locations.
// Each set is responsible for caching a unique subset of all memory locations.

typedef struct
{
	M68060CacheLine lines[M68060CacheLinesPerSet];

} M68060CacheSet;

// An entire cache.

typedef struct 
{
	// Datacache contents
	M68060CacheSet sets[M68060CacheSets];

	// Set of metrics which track cache performance
	M68060CacheMetrics metrics;
} M68060Cache;

// The CacheLineAccessResult describes which actions were taken while accessing one cache line.

typedef struct
{
	bool hit;				// true = the access hit a valid cache line
	bool evictedCleanLine;	// true = a clean line was evicted (because all cache lines in the set were occupied, and one was needed for re-allocation)
	bool evictedDirtyLine;	// true = a dirty line was evicted (because all cache lines in the set were occupied, and one was needed for re-allocation)
	bool allocatedNewLine;	// true = a cache line was allocated to cache the target memory location
	bool dirtiedLine;		// true = this access turned the cache line from being non-dirty to dirty; i.e. this was the first write access to the cache line
	int set;				// Set ID accessed
	int line;				// Line ID accessed
} M68060CacheLineAccessResult;

// The CacheAccessResult describes which cache actions were taken while performing one memory access.

typedef struct
{
	// Normally, a memory access results in one cache line access.
	// However, if the memory access is misaligned across a 16-byte boundary, it will result in two cache line accesses.

	int numAccesses;
	M68060CacheLineAccessResult accesses[2];
} M68060CacheAccessResult;

// Initialize cache to an empty state. This also resets all metrics associated with the cache.
void M68060Cache_init(M68060Cache* cache);

// Invalidate all cache contents, without writing back any dirty lines
void M68060Cache_invalidate(M68060Cache* cache);

// Perform read access against cache
void M68060Cache_read(M68060Cache* cache, uint32_t address, int size, M68060CacheAccessResult* result);

// Perform write access against cache
void M68060Cache_write(M68060Cache* cache, uint32_t address, int size, M68060CacheAccessResult* result);

#endif
