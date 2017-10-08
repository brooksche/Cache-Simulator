#include "cache.h"

extern struct cache_line **cache_ln;

int get_tag_bits(unsigned long addr)
{
	int tag_bits = 0;
	tag_bits = ( addr >> 21) & 0x3FF;

	return tag_bits;

}


int get_set_number(unsigned long addr)
{
	int index_bits = 0;
	index_bits = ( addr >> 6) & 0x7FFF;

	return index_bits;

}


/*
   Used to simulate communication to our upper level cache
 */
void MessageToL1Cache(int BusOp, unsigned long addr)
{

#ifndef	SILENT
	if(BusOp == 1)
	{
		printf("L1: READ 0x%lx \n",addr);
	}

	if(BusOp == 2)
	{
		printf("L1: WRITE 0x%lx \n",addr);
	}

	if(BusOp == 3)
	{
		printf("L1: INVALIDATE 0x%lx \n",addr);
	}

	if(BusOp == 4)
	{
		printf("L1: RWIM 0x%lx \n",addr);
	}

#endif

}

