
#include <stdio.h>
#include <stdint.h>

#include "TimingTools.h"

#include <hardware/custom.h>

extern struct Custom custom;

uint32_t runAsmCode(void);

int main(void)
{
	init();

	startTimer();
	
	uint32_t divisor = runAsmCode();

	uint32_t deltaCycles = stopTimer();

	shutDown();

	printf("Number of cycles per iteration, times 100: %u\n", (deltaCycles * 100) / divisor);

	return 0;
}
