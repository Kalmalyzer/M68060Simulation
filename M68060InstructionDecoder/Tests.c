
#include "M68060OpWordDecoder.h"
#include "../Types.h"


int main(void)
{
	uint16_t instructionStream[] = { 0x4e71 };

	decodeOpWord(instructionStream[0]);

	return 0;
}