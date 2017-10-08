#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "cache.h"


extern struct cache_line **cache_ln;

struct addr_arr
{
	unsigned long addr;
	unsigned int lrubits;
	unsigned int mesibits;
};

struct addr_arr *ptrlru;

int update_lru(unsigned long addr)
{

	int i;
	int set_num = 0;
	int tag_bits = 0;
	int line_num = 0;
	int lru_cmp = 0;

	tag_bits = get_tag_bits(addr);	
	set_num = get_set_number(addr);	


	for(line_num = 0; line_num < ROWS_IN_SET; line_num++)
	{

		if(cache_ln[set_num][line_num].tag_bits == tag_bits)
		{      
			lru_cmp = cache_ln[set_num][line_num].lru_bits;

			for(i = 0; i < ROWS_IN_SET; i++)
			{

				if(cache_ln[set_num][i].lru_bits < lru_cmp)
				{      
					cache_ln[set_num][i].lru_bits++;
				}

			}


			cache_ln[set_num][line_num].lru_bits = 0;


			return 0;
		}

	}




#ifdef	DEBUG6
	printf("\n");
#endif

	return 0;
}

/*
//FIXME not needed ; remove the code

int init_LRU(unsigned long addr)
{

ptrlru = (struct addr_arr *)malloc(sizeof(struct addr_arr) * 8);
int i;
for(i=0;i<8;i++)
{
ptrlru[i].lrubits = 7;
}
return 0;
}
 */
