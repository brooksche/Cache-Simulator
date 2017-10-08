/////////////////////////////////////////////////////////////

#ifndef __CACHE_H__
#define __CACHE_H__


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "test.h"

/*
 * Bus Operation types
 */
#define READ 		1 /* Bus Read */
#define WRITE 		2 /* Bus Write */
#define INVALIDATE 	3 /* Bus Invalidate */
#define RWIM 		4 /* Bus Read With Intent to Modify */
#define SNOOP_READ 	5 /* Bus snoop Read */
#define SNOOP_RWIM	6 /* Bus snoop Read With Intent to Modify */


/*
 * MESI states
 */
#define MODIFIED 	0 /* Modified state */
#define EXCLUSIVE 	1 /* exclusive state */
#define SHARED 		2 /* shared state */
#define INVALID 	3 /* Invalid state */

/*
 * Snoop Result types
 */
#define NOHIT 	0	 /* No hit */
#define HIT 	1	 /* Hit */
#define HITM 	2	 /* Hit to modified line */



#define MASK_BITS
//#define ALL_HITM


unsigned long NCR;
unsigned long NCW;
unsigned long NCH;
unsigned long NCM;



extern struct cache_line **cache_ln;



/*
   Used to simulate a bus operation and to capture the snoop results of last
   level caches of other processors
 */
int BusOperation(int BusOp, unsigned long addr);


/*
   Used to simulate the reporting of snoop results by other caches
 */
int GetSnoopResult(unsigned long addr);


/*
   Used to report the result of our snooping bus operations by other caches
 */
void PutSnoopResult(unsigned long addr, int SnoopResult);



/*
   Used to simulate communication to our upper level cache
 */
void MessageToL1Cache(int BusOp, unsigned long addr);


/*
   Used to update the LRU bits
 */
int update_LRU(unsigned long addr);

/*
   Used to initialize the LRU bits
 */
int init_LRU(unsigned long addr);

/*
   Used to get the set number
 */
int getsetno(unsigned long addr);



int get_tag_bits(unsigned long addr);


int get_set_number(unsigned long addr);


int mesi_protocol(unsigned long addr, int opcode, int mesi_bits);

int update_lru(unsigned long addr);

int check_in_cache(unsigned long addr);

int add_cache_line(unsigned long addr, int mesi_bits);

int reset_cache();

int print_cache();

int cache_stats();

int init_cache();

int check_in_cache_state(unsigned long addr);

#endif     //// __CACHE_H__
