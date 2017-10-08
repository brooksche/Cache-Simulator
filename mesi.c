/****************************************************************************************/
/*
   ECE - 585 Microprocessor system design 
   Final project : Simulation of last level cache	
Date : 22nd March 2017

Team mates:
Amit Solapurkar
Kalyani Chawak
Soma Sai Charitha
Haranadh Chintapalli	


 */
/****************************************************************************************/



#include "cache.h"


int check_in_cache(unsigned long addr)
{
	int tag_bits = 0;
	int set_num = 0;
	int line_num = 0;

	tag_bits = get_tag_bits(addr);	
	set_num = get_set_number(addr);	

	for(line_num = 0; line_num < ROWS_IN_SET ; line_num++)
	{
		if((cache_ln[set_num][line_num].tag_bits == tag_bits) && (cache_ln[set_num][line_num].mesi_bits != INVALID))
		{
#ifdef	DEBUG3
			printf("L2 : HIT\n");	
#endif
			return HIT;

		}
	}

	return NOHIT;


}


int mesi_protocol(unsigned long addr, int opcode, int mesi_bits)
{

#ifdef	DEBUG3
	printf("\nEntering function %s\n",__func__);
#endif


	int tag_bits = 0;
	int set_num = 0;
	int line_num = 0;

	tag_bits = get_tag_bits(addr);	
	set_num = get_set_number(addr);	

	for(line_num = 0; line_num < ROWS_IN_SET ; line_num++)
	{
		if((cache_ln[set_num][line_num].tag_bits == tag_bits) && (cache_ln[set_num][line_num].mesi_bits != INVALID))
		{

			if((cache_ln[set_num][line_num].mesi_bits == SHARED) && (opcode == WRITE))
			{

				printf("BusOp: INVALIDATE, Address: %lx\n", addr);

			}

			cache_ln[set_num][line_num].mesi_bits = mesi_bits;
		
			return 0;

		}
	}

	return 0;







#ifdef	DEBUG3
	printf("\nExiting function %s\n",__func__);
#endif

	return 0;

}
