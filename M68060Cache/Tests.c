
#include "M68060Cache.h"

bool withinRange(uint32_t value, uint32_t ref, float minRatio, float maxRatio)
{
	return (float) value >= (ref * minRatio) && (float) value <= (ref * maxRatio);
}

void test7(M68060Cache* cache)
{
	int totalLineAccesses = 0;
	int totalLineHits = 0;
	int totalLineMisses = 0;
	int totalLineReads = 0;
	int totalLineWriteBacks = 0;
	uint32_t i;
	int start = 0x10000;
	int size = 4;
	int stride = 4;
	int count = 2048;

	M68060Cache_invalidate(cache);
	
	for (i = 0; i < count; ++i)
	{
		uint32_t address = start + i * stride;
		M68060CacheAccessResult result;
		M68060Cache_write(cache, address, size, &result);

		{
			int numLineAccesses = result.numAccesses;
			int numLineHits = (result.numAccesses > 1) ? ((int) result.accesses[0].hit + (int) result.accesses[1].hit) : (int) result.accesses[0].hit;
			int numLineMisses = result.numAccesses - numLineHits;
			int numLineReads = (result.numAccesses > 1) ? ((int) result.accesses[0].allocatedNewLine + (int) result.accesses[1].allocatedNewLine) : (int) result.accesses[0].allocatedNewLine;
			int numLineWriteBacks = (result.numAccesses > 1) ? ((int) result.accesses[0].evictedDirtyLine + (int) result.accesses[1].evictedDirtyLine) : (int) result.accesses[0].evictedDirtyLine;
			
			printf("Reading from 0x%08x - line accesses: %d - line hits: %d - line misses: %d - line reads: %d - line writebacks: %d\n",
				address, numLineAccesses, numLineHits, numLineMisses, numLineReads, numLineWriteBacks);
				
			totalLineAccesses += numLineAccesses;
			totalLineHits += numLineHits;
			totalLineMisses += numLineMisses;
			totalLineReads += numLineReads;
			totalLineWriteBacks += numLineWriteBacks;
		}
	}

	{
		float misAlignedAccessRatio = (float) (totalLineAccesses - count) / count;
		float lineHitRatio = (float) totalLineHits / totalLineAccesses;
		float lineReadRatio = (float) totalLineReads / totalLineAccesses;
		float lineWriteBackRatio = (float) totalLineWriteBacks / totalLineAccesses;
		
		printf("Misaligned access ratio: %.2f%% - Line hit ratio: %.2f%% - Line read ratio: %.2f%% - Line writeback ratio: %.2f%%\n",
			100.f * misAlignedAccessRatio, 100.f * lineHitRatio, 100.f * lineReadRatio, 100.f * lineWriteBackRatio);
	}
} 

void readStride(M68060Cache* cache, uint32_t start, int size, int stride, int count)
{
	uint32_t i;

	for (i = 0; i < count; ++i)
	{
		uint32_t address = start + i * stride;
		M68060CacheAccessResult result;
		M68060Cache_read(cache, address, size, &result);
	}
} 

void writeStride(M68060Cache* cache, uint32_t start, int size, int stride, int count)
{
	uint32_t i;

	for (i = 0; i < count; ++i)
	{
		uint32_t address = start + i * stride;
		M68060CacheAccessResult result;
		M68060Cache_write(cache, address, size, &result);
	}
} 

void primeCacheWithReads(M68060Cache* cache, uint32_t baseAddress)
{
	uint32_t address;
	M68060CacheAccessResult result;

	M68060Cache_invalidate(cache);
	readStride(cache, baseAddress, 1, 16, 512);
}

bool test1(M68060Cache* cache)
{
	M68060CacheMetrics oldMetrics;
	M68060CacheMetrics metricsDiff;
	int i;

	primeCacheWithReads(cache, 0x10000);

	memcpy(&oldMetrics, &cache->metrics, sizeof(oldMetrics));
	for (i = 0; i < 10; i++)
		readStride(cache, 0x10000, 4, 4, 2048);
	
	M68060CacheMetrics_diff(&metricsDiff, &oldMetrics, &cache->metrics);

	if (metricsDiff.lineReadHits != i*2048
		|| metricsDiff.lineReadMisses != 0
		|| metricsDiff.lineWriteHits != 0
		|| metricsDiff.lineWriteMisses != 0
		|| metricsDiff.cleanLineEvictions != 0
		|| metricsDiff.dirtyLineEvictions != 0
		|| metricsDiff.lineAllocations != 0)
	{
		printf("In-cache 8kB read test failed. Metrics for %d repeats:\n", i);
		M68060CacheMetrics_print(&metricsDiff);
		return false;
	}
	else
	{
		printf("In-cache 8kB read test succeeded.\n");
		return true;
	}
}

bool test2(M68060Cache* cache)
{
	M68060CacheMetrics oldMetrics;
	M68060CacheMetrics metricsDiff;
	float minRatio = 0.98f;
	float maxRatio = 1.02f;

	M68060Cache_invalidate(cache);

	memcpy(&oldMetrics, &cache->metrics, sizeof(oldMetrics));
	readStride(cache, 0x10000, 1, 1, 131072);
	M68060CacheMetrics_diff(&metricsDiff, &oldMetrics, &cache->metrics);
	
	
	if (metricsDiff.lineReadHits != 131072*15/16
		|| metricsDiff.lineReadMisses != 131072*1/16
		|| metricsDiff.lineWriteHits != 0
		|| metricsDiff.lineWriteMisses != 0
		|| !withinRange(metricsDiff.cleanLineEvictions, 131072*1/16 - 512, minRatio, maxRatio)
		|| metricsDiff.dirtyLineEvictions != 0
		|| metricsDiff.lineAllocations != 131072*1/16)
	{
		printf("128kB linear read test failed. Metrics:\n");
		M68060CacheMetrics_print(&metricsDiff);
		return false;
	}
	else
	{
		printf("128kB linear read test succeeded.\n");
		return true;
	}
}

bool test3(M68060Cache* cache)
{
	M68060CacheMetrics oldMetrics;
	M68060CacheMetrics metricsDiff;
	int i;
	float thrashRatio;
	int size = 64*1024;
	float minRatio = 0.98f;
	float maxRatio = 1.02f;

	M68060Cache_invalidate(cache);

	memcpy(&oldMetrics, &cache->metrics, sizeof(oldMetrics));
	for (i = 0; i < size; i++)
	{
		M68060CacheAccessResult result;
		uint32_t address1 = size * 1 + i * 1;
		uint32_t address2 = size * 2 + i * 1;
		M68060Cache_read(cache, address1, 1, &result);
		M68060Cache_read(cache, address2, 1, &result);
	}
	
	M68060CacheMetrics_diff(&metricsDiff, &oldMetrics, &cache->metrics);

	thrashRatio = ((float) (metricsDiff.lineReadMisses - size*2*1/16)) / (size*2*1/16);
	
	if (!withinRange(metricsDiff.lineReadHits, size*2*15/16, minRatio, maxRatio)
		|| !withinRange(metricsDiff.lineReadMisses, size*2*1/16*115/100, minRatio, maxRatio)
		|| metricsDiff.lineWriteHits != 0
		|| metricsDiff.lineWriteMisses != 0
		|| !withinRange(metricsDiff.cleanLineEvictions, size*2*1/16*115/100 - 512, minRatio, maxRatio)
		|| metricsDiff.dirtyLineEvictions != 0
		|| !withinRange(metricsDiff.lineAllocations, size*2*1/16*115/100, minRatio, maxRatio))
	{
		printf("64kb dual read-stream test failed. Thrash ratio: %.2f%%. Metrics:\n", 100.f * thrashRatio);
		M68060CacheMetrics_print(&metricsDiff);
		return false;
	}
	else
	{
		printf("64kb dual read-stream test succeeded. Thrash ratio: %.2f%%\n", 100.f * thrashRatio);
		return true;
	}
}

bool test4(M68060Cache* cache)
{
	M68060CacheMetrics oldMetrics;
	M68060CacheMetrics metricsDiff;
	int i;

	M68060Cache_invalidate(cache);
	writeStride(cache, 0x10000, 4, 4, 2048);
	
	memcpy(&oldMetrics, &cache->metrics, sizeof(oldMetrics));
	for (i = 0; i < 10; i++)
		writeStride(cache, 0x10000, 4, 4, 2048);
	
	M68060CacheMetrics_diff(&metricsDiff, &oldMetrics, &cache->metrics);

	if (metricsDiff.lineReadHits != 0
		|| metricsDiff.lineReadMisses != 0
		|| metricsDiff.lineWriteHits != i*2048
		|| metricsDiff.lineWriteMisses != 0
		|| metricsDiff.cleanLineEvictions != 0
		|| metricsDiff.dirtyLineEvictions != 0
		|| metricsDiff.lineAllocations != 0)
	{
		printf("In-cache 8kB write test failed. Metrics for %d repeats:\n", i);
		M68060CacheMetrics_print(&metricsDiff);
		return false;
	}
	else
	{
		printf("In-cache 8kB write test succeeded.\n");
		return true;
	}
}

bool test5(M68060Cache* cache)
{
	M68060CacheMetrics oldMetrics;
	M68060CacheMetrics metricsDiff;
	float minRatio = 0.98f;
	float maxRatio = 1.02f;

	M68060Cache_invalidate(cache);

	memcpy(&oldMetrics, &cache->metrics, sizeof(oldMetrics));
	writeStride(cache, 0x10000, 1, 1, 131072);
	M68060CacheMetrics_diff(&metricsDiff, &oldMetrics, &cache->metrics);
	
	
	if (metricsDiff.lineReadHits != 0
		|| metricsDiff.lineReadMisses != 0
		|| metricsDiff.lineWriteHits != 131072*15/16
		|| metricsDiff.lineWriteMisses != 131072*1/16
		|| metricsDiff.cleanLineEvictions != 0
		|| !withinRange(metricsDiff.dirtyLineEvictions, 131072*1/16 - 512, minRatio, maxRatio)
		|| metricsDiff.lineAllocations != 131072*1/16)
	{
		printf("128kB linear write test failed. Metrics:\n");
		M68060CacheMetrics_print(&metricsDiff);
		return false;
	}
	else
	{
		printf("128kB linear write test succeeded.\n");
		return true;
	}
}

bool test6(M68060Cache* cache)
{
	M68060CacheMetrics oldMetrics;
	M68060CacheMetrics metricsDiff;
	int i;
	float thrashRatio;
	int size = 64*1024;
	float minRatio = 0.98f;
	float maxRatio = 1.02f;

	M68060Cache_invalidate(cache);

	memcpy(&oldMetrics, &cache->metrics, sizeof(oldMetrics));
	for (i = 0; i < size; i++)
	{
		M68060CacheAccessResult result;
		uint32_t address1 = size * 1 + i * 1;
		uint32_t address2 = size * 2 + i * 1;
		M68060Cache_write(cache, address1, 1, &result);
		M68060Cache_write(cache, address2, 1, &result);
	}
	
	M68060CacheMetrics_diff(&metricsDiff, &oldMetrics, &cache->metrics);

	thrashRatio = ((float) (metricsDiff.lineWriteMisses - size*2*1/16)) / (size*2*1/16);
	
	if (metricsDiff.lineReadHits != 0
		|| metricsDiff.lineReadMisses != 0
		|| !withinRange(metricsDiff.lineWriteHits, size*2*15/16, minRatio, maxRatio)
		|| !withinRange(metricsDiff.lineWriteMisses, size*2*1/16*115/100, minRatio, maxRatio)
		|| metricsDiff.cleanLineEvictions != 0
		|| !withinRange(metricsDiff.dirtyLineEvictions, size*2*1/16*115/100 - 512, minRatio, maxRatio)
		|| !withinRange(metricsDiff.lineAllocations, size*2*1/16*115/100, minRatio, maxRatio))
	{
		printf("64kb dual write-stream test failed. Thrash ratio: %.2f%%. Metrics:\n", 100.f * thrashRatio);
		M68060CacheMetrics_print(&metricsDiff);
		return false;
	}
	else
	{
		printf("64kb dual write-stream test succeeded. Thrash ratio: %.2f%%\n", 100.f * thrashRatio);
		return true;
	}
}


int main(void)
{
	M68060Cache cache;
	M68060Cache_init(&cache);

	test1(&cache);
	test2(&cache);
	test3(&cache);
	test4(&cache);
	test5(&cache);
	test6(&cache);
	test7(&cache);

	return 0;
}
