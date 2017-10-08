
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEST_H__
#define __TEST_H__


//#undef DEBUG 
//#define DEBUG

//#define SILENT

//#define DEBUG1
//#define DEBUG3
//#define DEBUG6


#ifdef DEBUG    //DEBUG

#define DEBUG1
#define DEBUG3
#define DEBUG6
//#define DEBUG9

#endif         // DEBUG

/*
 * Cache size related parameters
 */
#define CACHE_SIZE		(16 * 1024 *1024)ULL
#define SETS			(32 * 1024)
#define CACHE_LINE_SIZE		(64)

#define ASSOCIATIVITY		(8)
#define ROWS_IN_SET 		(8)


#define BYTE_SELECT_BITS	(6)
#define INDEX_BITS		(15)
#define TAG_BITS		(11)


/*
 * SNOOP reurn policies
#define POLICY1
#define ALL_HIT		(1)
 */





















#endif     //// __TEST_H__
