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

struct trace
{
	int operand;
	unsigned long addr;
};


struct cache_line
{
	int tag_bits;
	int lru_bits;
	int mesi_bits;
};


struct trace test_input;


struct cache_line **cache_ln = NULL;

unsigned long NCR = 0;
unsigned long NCW = 0;
unsigned long NCH = 0;
unsigned long NCM = 0;

int add_cache_line(unsigned long addr, int mesi_bits);



int main(int argc, char *argv[])
{
	int L2cache_status = 0;
	int L2cache_state = 0;
	int snoop_ret;

#ifdef	DEBUG3
	printf("\nEntering function %s\n",__func__);
#endif
	init_cache();


	/* 
	   _______________________________________________________________________________
	   |	test_input    Test case                                               	  |
	   --------------------------------------------------------------------------------
	   |           0          Read request from L1 data cache                      | 
	   |                                                                            |
	   |           1          write request from L1 data cache                     |
	   |                                                                            |
	   |           2          Read request from L1 instruction cache               |
	   |                                                                            |
	   |           3          Snooped invalidate command                           |
	   |                                                                            |
	   |           4          Snooped read request                                 |
	   |                                                                            |
	   |           5          Snooped write request                                |
	   |                                                                            |
	   |           6          Snooped read with intent to modify                   |
	   |                                                                            |
	   |           8          clear the cache and reset all state                  |
	   |                                                                            |
	   |           9          print contents and state of each valid cache line    |
	   _______________________________________________________________________________
	 */


	if(argv[1] == NULL)
	{
		printf("\n\n");
		printf("\n************************************************************************************************\n");
		printf("\n           Please provide input trace file ( Refer README for options ) \n");
		printf("\n************************************************************************************************\n");
		printf("\n\n");
		return 0;

	}

	/* test file parsing part */
	FILE *fp;
	fp = fopen(argv[1],"r");



	while(fscanf(fp,"%d %lx",&test_input.operand,&test_input.addr)!=EOF)
	{

#ifdef	DEBUG3
		printf("\ncommand : %d\t Address : 0x%lx",test_input.operand,test_input.addr);
#endif


		switch(test_input.operand)
		{

			//   0     Read request from L1 data cache   

			case 0:

#ifdef	DEBUG3
				printf("\nRead request from L1 data cache\t%lx",test_input.addr);
#endif

				NCR++;
				L2cache_status = check_in_cache(test_input.addr);
				if(L2cache_status == HIT)
				{
#ifdef	DEBUG3
					printf("\n L2 cache : HIT\t%lx",test_input.addr);
#endif
					NCH++;
					update_lru(test_input.addr);
				}
				else
				{
#ifdef	DEBUG3
					printf("\n L2 cache : MISS\t%lx",test_input.addr);
#endif
					NCM++;
					snoop_ret = BusOperation(READ, test_input.addr);

					switch(snoop_ret)
					{
						case NOHIT :
							add_cache_line(test_input.addr, EXCLUSIVE);
							break;

						case HIT :
							add_cache_line(test_input.addr, SHARED);
							break;

						case HITM :
							add_cache_line(test_input.addr, SHARED);
							break;

					}

				}   
				break;



				//     1  write request from L1 data cache

			case 1:
#ifdef	DEBUG3
				printf("\nWrite request from L1 data cache\t%lx",test_input.addr);
#endif

				NCW++;
				L2cache_status = check_in_cache(test_input.addr);

				if(L2cache_status == HIT)
				{
#ifdef	DEBUG3
					printf("\n L2 cache : HIT\t%lx",test_input.addr);
#endif
					NCH++;
					mesi_protocol(test_input.addr, WRITE, MODIFIED);
					update_lru(test_input.addr);
				}
				else
				{
#ifdef	DEBUG3
					printf("\n L2 cache : MISS\t%lx",test_input.addr);
#endif
					NCM++;
					snoop_ret = BusOperation(RWIM, test_input.addr);

					switch(snoop_ret)
					{
						case NOHIT :
							add_cache_line(test_input.addr, MODIFIED);
							break;

						case HIT :
							add_cache_line(test_input.addr, MODIFIED);
							break;

						case HITM :
							add_cache_line(test_input.addr, MODIFIED);
							break;

					}

				}   
				break;



				//    2     Read request from L1 instruction cache  

			case 2:
#ifdef	DEBUG3
				printf("\nRead request from L1 instruction cache\t%lx",test_input.addr);
#endif
				NCR++;
				L2cache_status = check_in_cache(test_input.addr);
				if(L2cache_status == HIT)
				{
#ifdef	DEBUG3
					printf("\n L2 cache : HIT\t%lx",test_input.addr);
#endif
					NCH++;
					update_lru(test_input.addr);
				}
				else
				{
#ifdef	DEBUG3
					printf("\n L2 cache : MISS\t%lx",test_input.addr);
#endif
					NCM++;
					snoop_ret = BusOperation(READ, test_input.addr);

					switch(snoop_ret)
					{
						case NOHIT :
							add_cache_line(test_input.addr, EXCLUSIVE);
							break;

						case HIT :
							add_cache_line(test_input.addr, SHARED);
							break;

						case HITM :
							add_cache_line(test_input.addr, SHARED);
							break;

					}

				}   

				break;



				//   3    Snooped invalidate command    

			case 3:
#ifdef	DEBUG3
				printf("\nSnooped invalidate command\t%lx",test_input.addr);
#endif
				mesi_protocol(test_input.addr, INVALIDATE, INVALID);

				break;



				//     4          Snooped read request   

			case 4:
#ifdef	DEBUG3
				printf("\nSnooped read request\t%lx",test_input.addr);
#endif
				L2cache_state = check_in_cache_state(test_input.addr);

				if(L2cache_state == INVALID)
				{
					PutSnoopResult(test_input.addr, NOHIT);
				}

				if(L2cache_state == SHARED)
				{
					PutSnoopResult(test_input.addr, HIT);
				}


				if(L2cache_state == EXCLUSIVE)
				{
					PutSnoopResult(test_input.addr, HIT);
					mesi_protocol(test_input.addr, SNOOP_READ, SHARED);
				}

				if(L2cache_state == MODIFIED)
				{
					PutSnoopResult(test_input.addr, HITM);
					mesi_protocol(test_input.addr, SNOOP_READ, SHARED);
				}


				break;



				//   5      Snooped write request    

			case 5:
#ifdef	DEBUG3
				printf("\nSnooped write request\t%lx",test_input.addr);
#endif
				break;



				//   6     Snooped read with intent to modify   

			case 6:
#ifdef	DEBUG3
				printf("\nSnooped read with intent to modify\t%lx",test_input.addr);
#endif



				L2cache_state = check_in_cache_state(test_input.addr);

				if(L2cache_state == INVALID)
				{
					PutSnoopResult(test_input.addr, NOHIT);
				}

				if( (L2cache_state == SHARED) || (L2cache_state == EXCLUSIVE) )
				{
					PutSnoopResult(test_input.addr, HIT);
					mesi_protocol(test_input.addr, SNOOP_RWIM, INVALID);
				}


				if(L2cache_state == MODIFIED)
				{
					PutSnoopResult(test_input.addr, HITM);
					mesi_protocol(test_input.addr, SNOOP_RWIM, INVALID);
				}



				break;




				//   8     clear the cache and reset all state      

			case 8:
#ifdef	DEBUG3
				printf("\nClear the cache and reset all state\t%lx",test_input.addr);
#endif
				reset_cache();
				break;



				//   9     print contents and state of each valid cache line  

			case 9:
#ifdef	DEBUG3
				printf("\nPrint contents and state of each valid cache line\t%lx",test_input.addr);
#endif
				print_cache();
				break;

			default:
				printf("\n************************************************************************************************\n");
				printf("\nPrint contents of invalid test line addr : %lx, operand : %d \n",test_input.addr,test_input.operand);
				printf("\n************************************************************************************************\n");
				break;


		}

	}


	fclose(fp);

	// Print final cache stats
	cache_stats();



#ifdef	DEBUG3
	printf("\nExiting function %s\n",__func__);
#endif

	return 0;

}




int init_cache()
{

#ifdef	DEBUG3
	printf("\nEntering function %s\n",__func__);
#endif
	int i,j;

	NCR = 0;
	NCW = 0;
	NCH = 0;
	NCM = 0;


#ifdef	DEBUG3
	printf("\nEntering function %s\n",__func__);
	printf("SETS = %d \n ", (int)SETS);
	printf("ROWS_IN_SET = %d \n ", (int)ROWS_IN_SET);
#endif

	cache_ln = (struct cache_line **)malloc( SETS  * sizeof(struct cache_line *));

	if(cache_ln == NULL)
	{
		printf("out of memory\n");
		return 1;
	}

	for(i = 0; i < SETS ; i++)
	{
		cache_ln[i] = (struct cache_line *)malloc(ROWS_IN_SET * sizeof(struct cache_line));
		if(cache_ln[i] == NULL)
		{
			printf("out of memory\n");
			return 1;
		}
	}


	for(i = 0; i < SETS; i++)
	{
		for(j = 0; j < ROWS_IN_SET; j++)
		{
			cache_ln[i][j].tag_bits = 0x0; 
			cache_ln[i][j].lru_bits = 0x7; 
			cache_ln[i][j].mesi_bits = INVALID; 

#ifdef	DEBUG9
			printf("(set = %d row = %d) cache line tag_bits %d \n",i, j, cache_ln[i][j].tag_bits);
			printf("(set = %d row = %d) cache line lru_bits = %d \n",i, j, cache_ln[i][j].lru_bits);
			printf("(set = %d row = %d) cache line mesi_bits = %d \n",i, j, cache_ln[i][j].mesi_bits);
#endif
		}
	}


#ifdef	DEBUG3
	printf("\nExiting function %s\n",__func__);
#endif
	return 0;

}




int reset_cache()
{

#ifdef	DEBUG3
	printf("\nEntering function %s\n",__func__);
#endif

	int i,j;

	NCR = 0;
	NCW = 0;
	NCH = 0;
	NCM = 0;


#ifdef	DEBUG3
	printf("\nEntering function %s\n",__func__);
	printf("SETS = %d \n ", (int)SETS);
	printf("ROWS_IN_SET = %d \n ", (int)ROWS_IN_SET);
#endif


	for(i = 0; i < SETS; i++)
	{
		for(j = 0; j < ROWS_IN_SET; j++)
		{
			cache_ln[i][j].tag_bits = 0x0; 
			cache_ln[i][j].lru_bits = 0x7; 
			cache_ln[i][j].mesi_bits = INVALID; 

#ifdef	DEBUG9
			printf("(set = %d row = %d) cache line tag_bits %d \n",i, j, cache_ln[i][j].tag_bits);
			printf("(set = %d row = %d) cache line lru_bits = %d \n",i, j, cache_ln[i][j].lru_bits);
			printf("(set = %d row = %d) cache line mesi_bits = %d \n",i, j, cache_ln[i][j].mesi_bits);
#endif
		}
	}


#ifdef	DEBUG3
	printf("\nExiting function %s\n",__func__);
#endif
	return 0;

}






int cache_stats()
{

	float hit_ratio = 0;

	if((NCH + NCM) != 0)
	{
		hit_ratio = ((float)NCH/((float)NCH + (float)NCM));
	}

#ifdef	DEBUG3
	printf("\nEntering function %s\n",__func__);
#endif


	printf("\n");
	printf("#################################################################\n"              );
	printf("#                                                               \n"              );
	printf("#   Number of Cache Reads   : %ld                               \n",          NCR);
	printf("#   Number of Cache Writes  : %ld                               \n",          NCW);
	printf("#   Number of Cache Hits    : %ld                               \n",          NCH);
	printf("#   Number of Cache Misses  : %ld                               \n",          NCM);
	printf("#   Cache Hit Ratio         : %lf                               \n",    hit_ratio);
	printf("#                                                               \n"              );
	printf("################################################################\n");
	printf("\n");





#ifdef	DEBUG3
	printf("\nExiting function %s\n",__func__);
#endif
	return 0;

}




int print_cache()
{

#ifdef	DEBUG3
	printf("\nEntering function %s\n",__func__);
#endif
	int i,j;
	int temp = 0;

#ifdef	DEBUG3
	printf("\nEntering function %s\n",__func__);
#endif


	for(i = 0; i < SETS; i++)
	{
		temp = 0;

		for(j = 0; j < ROWS_IN_SET; j++)
		{
			if(temp == 0 && cache_ln[i][j].mesi_bits != INVALID)
			{
				printf("##########################################################################\n");
				printf("   All Valid rows in set ( %d or 0x%x ) \n",i, i);
				temp = 1;
			}
			if(cache_ln[i][j].mesi_bits != INVALID)
			{
				printf("\n");
				printf("   Set = %d ( 0x%x ) \n", i, i);
				printf("   Row = %d ( 0x%x ) \n", j, j);
				printf("   Tag_value = %d ( 0x%x )\n", cache_ln[i][j].tag_bits, cache_ln[i][j].tag_bits);
				printf("   Lru_value = %d ( 0x%x )\n", cache_ln[i][j].lru_bits, cache_ln[i][j].lru_bits);

				if( cache_ln[i][j].mesi_bits == MODIFIED)
				{
					printf("   mesi_value = MODIFIED \n");
				}

				if( cache_ln[i][j].mesi_bits == EXCLUSIVE)
				{
					printf("   mesi_value = EXCLUSIVE \n");
				}

				if( cache_ln[i][j].mesi_bits == SHARED)
				{
					printf("   mesi_value = SHARED \n");
				}


				printf("\n");
			} 

		}

	}


#ifdef	DEBUG3
	printf("\nExiting function %s\n",__func__);
#endif
	return 0;

}





int add_cache_line(unsigned long addr, int mesi_bits)
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
		if(cache_ln[set_num][line_num].mesi_bits == INVALID)
		{
#ifdef	DEBUG3
			printf("Have space in set to add new addr entry\n");	
#endif
			cache_ln[set_num][line_num].tag_bits = tag_bits;
			cache_ln[set_num][line_num].mesi_bits = mesi_bits;

			update_lru(addr);

			return 0;

		}
	}

	// no INVALID line in cache
	// have to evict something

	for(line_num = 0; line_num < ROWS_IN_SET ; line_num++)
	{
		if(cache_ln[set_num][line_num].lru_bits == 0x7)
		{

#ifdef	DEBUG3
			printf("No space in set to add new addr entry, so doing eviction\n");	
#endif

			if(cache_ln[set_num][line_num].mesi_bits == MODIFIED)
			{
#ifndef	SILENT
				printf("Write back to memory Address: 0x%lx \n", (long unsigned int)((cache_ln[set_num][line_num].tag_bits << 21) + set_num));	
#endif
#ifndef	SILENT
				MessageToL1Cache(WRITE , ((cache_ln[set_num][line_num].tag_bits << 21) + set_num));
#endif
			}

			cache_ln[set_num][line_num].tag_bits = tag_bits;
			cache_ln[set_num][line_num].mesi_bits = mesi_bits;

			update_lru(addr);


		}
	}



#ifdef	DEBUG3
	printf("\nExiting function %s\n",__func__);
#endif

	return 0;


}




int check_in_cache_state(unsigned long addr)
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
		if(cache_ln[set_num][line_num].tag_bits == tag_bits)
		{
#ifdef	DEBUG3
			printf(" Cache state  : %d\n",cache_ln[set_num][line_num].mesi_bits);	
#endif
			return cache_ln[set_num][line_num].mesi_bits;

		}
	}


#ifdef	DEBUG3
	printf("\nExiting function %s\n",__func__);
#endif

	return INVALID;

}




int check_in_cache(unsigned long addr)
{
#ifdef	DEBUG3
	printf("\nEntering function %s\n",__func__);
#endif
	int tag_bits = 0;
	int set_num = 0;
	int line_num = 0;

	tag_bits = get_tag_bits(addr);	
	set_num = get_set_number(addr);	

#ifdef	DEBUG3
	printf("tag bits = 0x%x \n",tag_bits);	
	printf("set number = 0x%x \n",set_num);	
#endif


	for(line_num = 0; line_num < ROWS_IN_SET ; line_num++)
	{
		if((cache_ln[set_num][line_num].tag_bits == tag_bits) && (cache_ln[set_num][line_num].mesi_bits != INVALID))
		{
#ifdef	DEBUG3
			printf("tag bits inside for  = 0x%x\n",tag_bits);
			printf("cache.ln tag bits inside for = 0x%x\n",cache_ln[set_num][line_num].tag_bits);
			printf("L2 : HIT\n");	
#endif
			return HIT;

		}
	}


#ifdef	DEBUG3
	printf("\nExiting function %s\n",__func__);
#endif

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

#ifndef	SILENT
				printf(" INVALIDATE, Address: %lx\n", addr);
#endif
				cache_ln[set_num][line_num].mesi_bits = mesi_bits;

			}
			else if(opcode == WRITE)
			{
				cache_ln[set_num][line_num].mesi_bits = mesi_bits;
			}



			if(opcode == INVALIDATE)
			{

#ifndef	SILENT
				printf(" INVALIDATE, Address: %lx\n", addr);
#endif
				cache_ln[set_num][line_num].mesi_bits = mesi_bits;
				cache_ln[set_num][line_num].lru_bits = 0x7;


			}


			if(opcode == SNOOP_READ)
			{

				if(cache_ln[set_num][line_num].mesi_bits == EXCLUSIVE)
				{
					cache_ln[set_num][line_num].mesi_bits = SHARED;
				}

				if(cache_ln[set_num][line_num].mesi_bits == MODIFIED)
				{
					cache_ln[set_num][line_num].mesi_bits = SHARED;
#ifndef	SILENT
					printf("Write back to memory Address: 0x%lx \n", (long unsigned int)((cache_ln[set_num][line_num].tag_bits << 21) + set_num));	
#endif

				}

			}

			if( ( (cache_ln[set_num][line_num].mesi_bits == SHARED) || (cache_ln[set_num][line_num].mesi_bits == EXCLUSIVE) ) && (opcode == SNOOP_RWIM))
			{
				cache_ln[set_num][line_num].mesi_bits = mesi_bits;
				cache_ln[set_num][line_num].lru_bits = 0x7;

			}

			if( ( (cache_ln[set_num][line_num].mesi_bits == MODIFIED) ) && (opcode == SNOOP_RWIM))
			{
				cache_ln[set_num][line_num].mesi_bits = mesi_bits;
				cache_ln[set_num][line_num].lru_bits = 0x7;
#ifndef	SILENT
				printf("Write back to memory Address: 0x%lx \n", (long unsigned int)((cache_ln[set_num][line_num].tag_bits << 21) + set_num));	
#endif

			}




			return 0;

		}
	}

	return 0;







#ifdef	DEBUG3
	printf("\nExiting function %s\n",__func__);
#endif

	return 0;

}


int get_tag_bits(unsigned long addr)
{
#ifdef	DEBUG3
	printf("\nEntering function %s\n",__func__);
#endif
	int tag_bits = 0;
	tag_bits = ( addr >> 21) & 0x1FFFF;


#ifdef	DEBUG3
	printf("\nExiting function %s\n",__func__);
#endif

	return tag_bits;

}


int get_set_number(unsigned long addr)
{
#ifdef	DEBUG3
	printf("\nEntering function %s\n",__func__);
#endif
	int index_bits = 0;
	index_bits = ( addr >> 6) & 0x7FFF;


#ifdef	DEBUG3
	printf("\nExiting function %s\n",__func__);
#endif

	return index_bits;

}


/*
   Used to simulate communication to our upper level cache
 */
void MessageToL1Cache(int BusOp, unsigned long addr)
{
#ifdef	DEBUG3
	printf("\nEntering function %s\n",__func__);
#endif

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


#ifdef	DEBUG3
	printf("\nExiting function %s\n",__func__);
#endif

}



int update_lru(unsigned long addr)
{
#ifdef	DEBUG3
	printf("\nEntering function %s\n",__func__);
#endif


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

#ifdef	DEBUG3
	printf("\nExiting function %s\n",__func__);
#endif


	return 0;
}



int BusOperation(int BusOp, unsigned long addr)
{
#ifdef	DEBUG3
	printf("\nEntering function %s\n",__func__);
#endif

	int SnoopResult;
	SnoopResult = GetSnoopResult(addr);
#ifndef SILENT

	switch (BusOp)
	{
		case READ:

			if(SnoopResult == NOHIT)
			{
				printf("BusOp: READ, Address: %lx, Snoop Result: NOHIT\n",addr);
			}

			if(SnoopResult == HIT)
			{
				printf("BusOp: READ, Address: %lx, Snoop Result: HIT\n",addr);
			}

			if(SnoopResult == HITM)
			{
				printf("BusOp: READ, Address: %lx, Snoop Result: HITM\n",addr);
			}

			break;



		case WRITE:

			if(SnoopResult == NOHIT)
			{
				printf("BusOp: WRITE, Address: %lx, Snoop Result: NOHIT\n",addr);
			}

			if(SnoopResult == HIT)
			{
				printf("BusOp: WRITE, Address: %lx, Snoop Result: HIT\n",addr);
			}

			if(SnoopResult == HITM)
			{
				printf("BusOp: WRITE, Address: %lx, Snoop Result: HITM\n",addr);
			}

			break;


		case INVALIDATE:

			if(SnoopResult == NOHIT)
			{
				printf("BusOp: INVALIDATE, Address: %lx, Snoop Result: NOHIT\n",addr);
			}

			if(SnoopResult == HIT)
			{
				printf("BusOp: INVALIDATE, Address: %lx, Snoop Result: HIT\n",addr);
			}

			if(SnoopResult == HITM)
			{
				printf("BusOp: INVALIDATE, Address: %lx, Snoop Result: HITM\n",addr);
			}

			break;



		case RWIM:

			if(SnoopResult == NOHIT)
			{
				printf("BusOp: RWIM, Address: %lx, Snoop Result: NOHIT\n",addr);
			}

			if(SnoopResult == HIT)
			{
				printf("BusOp: RWIM, Address: %lx, Snoop Result: HIT\n",addr);
			}

			if(SnoopResult == HITM)
			{
				printf("BusOp: RWIM, Address: %lx, Snoop Result: HITM\n",addr);
			}

			break;


	}







#endif

#ifdef	DEBUG3
	printf("\nExiting function %s\n",__func__);
#endif

	return SnoopResult;
}

int GetSnoopResult(unsigned long addr)
{
#ifdef	DEBUG3
	printf("\nEntering function %s\n",__func__);
#endif

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



#ifdef	DEBUG3
	printf("\nExiting function %s\n",__func__);
#endif



	return 0;
}

void PutSnoopResult(unsigned long addr, int SnoopResult)
{
#ifdef	DEBUG3
	printf("\nEntering function %s\n",__func__);
#endif

#ifndef SILENT
	if(SnoopResult == NOHIT)
	{
		printf("SnoopResult: Address %lx, Snoop Result: NOHIT\n",addr);
	}

	if(SnoopResult == HIT)
	{
		printf("SnoopResult: Address %lx, Snoop Result: HIT\n",addr);
	}

	if(SnoopResult == HITM)
	{
		printf("SnoopResult: Address %lx, Snoop Result: HITM\n",addr);
	}

#endif

#ifdef	DEBUG3
	printf("\nExiting function %s\n",__func__);
#endif

}




