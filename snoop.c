#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cache.h"

extern struct cache_line **cache_ln;

int BusOperation(int BusOp, unsigned long addr)
{
	int SnoopResult;
	SnoopResult = GetSnoopResult(addr);
#ifndef SILENT
	printf("BusOp: %d, Address: %lx, Snoop Result: %d\n",BusOp, addr, SnoopResult);
#endif
	return SnoopResult;
}

int GetSnoopResult(unsigned long addr)
{
	int maskbits;
#ifdef MASK_BITS
	maskbits = ( addr >> 21) & 0xF;
	switch(maskbits)
	{
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
#ifdef DEBUG6
			printf("\nHIT \n");
#endif
			return HIT;

		case 6:
		case 7:
		case 8:
		case 9:
#ifdef DEBUG6
			printf("\nHITM \n");
#endif
			return HITM;

		case 0xA:
		case 0xB:
		case 0xC:
		case 0xD:
		case 0xE:
		case 0xF:
#ifdef DEBUG6
			printf("\nNOHIT \n");
#endif
			return NOHIT;

	}
#endif

#ifdef ALL_HIT
	return HIT;
#endif

#ifdef ALL_NOHIT
	return NOHIT;
#endif

#ifdef ALL_HITM
	return HITM;
#endif

#ifdef SPACE_DIV
	if (addr < 0x3FFFFFFF)
	{
		return HIT;
	}
	else if (addr < 0x80000000)
	{
		return NOHIT;
	}
	else
	{
		return HITM;
	}
#endif




return 0;
}

void PutSnoopResult(unsigned long addr, char SnoopResult)
{
#ifndef SILENT
	printf("SnoopResult: Address %lx, Snoop Result: %d\n",addr, SnoopResult);
#endif
}




