
#include <stdint.h>

#include <proto/exec.h>
#include <hardware/custom.h>
#include <hardware/cia.h>

extern struct Custom custom;
extern struct CIA ciaa;

enum { BusCyclesPerLine = 227 };
enum { LinesPerFrame = 312 };
enum { BusCyclesPerFrame = BusCyclesPerLine * LinesPerFrame };

enum { ChipSetFrequency_PAL	= 3546895 };
enum { ChipSetFrequency_NTSC = 3579545 };

enum { ChipSetFrequency = ChipSetFrequency_PAL };

enum { CpuFrequency = 50*1000*1000 };

void init(void)
{
}

void shutDown(void)
{
}

uint32_t readVhposr(void)
{
	uint16_t vhposrHigh, vhposrLow;

	do
	{
		vhposrHigh = custom.vposr;
		vhposrLow = custom.vhposr;

	} while ((vhposrHigh & 1) != (custom.vposr & 1));

	return ((vhposrHigh << 16) | vhposrLow) & 0x1ffff;
}

uint32_t getBusCyclesIntoFrame(void)
{
	uint32_t vhposr = readVhposr();
	
	uint32_t currentBusCycle = ((vhposr >> 8) * BusCyclesPerLine) + (vhposr & 0xff);
	
	return currentBusCycle;
}

uint32_t readTod(void)
{
	uint8_t todHi = ciaa.ciatodhi;
	uint8_t todMid = ciaa.ciatodmid;
	uint8_t todLow = ciaa.ciatodlow;
	uint32_t tod = (todHi << 16) | (todMid << 8) | todLow;
	
	return tod;
}

uint32_t busCyclesToCpuCycles(uint32_t busCycles)
{
	return (uint32_t) ((((double) busCycles) * CpuFrequency) / ChipSetFrequency);
}

static uint32_t startTimeTod;
static uint32_t startTimeBusCyclesIntoFrame;

void startTimer(void)
{
	Disable();
	uint32_t tod;
	uint32_t busCyclesIntoFrame;
	do
	{
		tod = readTod();
		busCyclesIntoFrame = getBusCyclesIntoFrame();
		
	} while (busCyclesIntoFrame < 10 * BusCyclesPerLine);

	startTimeTod = tod;
	startTimeBusCyclesIntoFrame = busCyclesIntoFrame;
}

uint32_t stopTimer(void)
{
	uint32_t tod;
	uint32_t busCyclesIntoFrame;
	uint32_t deltaFrames;
	uint32_t deltaBusCycles;
	uint32_t deltaCpuCycles;

	busCyclesIntoFrame = getBusCyclesIntoFrame();
	while (getBusCyclesIntoFrame() < 10 * BusCyclesPerLine) ;
	tod = readTod();
	
	deltaFrames = (tod - startTimeTod) & 0xffffff;
	deltaBusCycles = deltaFrames * BusCyclesPerFrame + busCyclesIntoFrame - startTimeBusCyclesIntoFrame;
	deltaCpuCycles = busCyclesToCpuCycles(deltaBusCycles);

	Enable();
	
	return deltaCpuCycles;
}
