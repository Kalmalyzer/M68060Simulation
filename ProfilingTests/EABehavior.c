
#include <stdio.h>
#include <stdint.h>

#include "TimingTools.h"

uint32_t moveTest(void);

int main(void)
{
	init();

	startTimer();
	
	uint32_t divisor = moveTest();

	uint32_t deltaCycles = stopTimer();

	shutDown();

	printf("Number of cycles per iteration, times 100: %u\n", (deltaCycles * 100) / divisor);

	return 0;
}
