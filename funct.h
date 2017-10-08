////////////////////////////////////////////////////////////////////////////////


#ifndef __FUNCT_H__
#define __FUNCT_H__


/*
Used to simulate a bus operation and to capture the snoop results of last
level caches of other processors
*/
void BusOperation(char BusOp, unsigned int Address, char *SnoopResult);


/*
Used to simulate the reporting of snoop results by other caches
*/
char GetSnoopResult(unsigned int Address);


/*
Used to report the result of our snooping bus operations by other caches
*/
void PutSnoopResult(unsigned int Address, char SnoopResult);



/*
Used to simulate communication to our upper level cache
*/
void MessageToL2Cache(char BusOp, unsigned int Address);


/*
Used to update the LRU bits
*/
int update_LRU(unsigned int Address);


#endif     //// __FUNCT_H__
