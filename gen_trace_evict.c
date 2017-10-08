//Determining the Associvity/Setsize= 8 Locations;
// By Making Condition
//-  Accessing all the Cache lines within a Set
//- Using 8 different Tag bits pointing for the Same Set.
//32bit CPU address		
// 6bits for Byteselect
// 15 bits for Index
// 11 bits for Tagbits
#include<stdio.h>
int main()
{
	FILE *fp;
	int i,j,k,zero=0;
	int ii,jj,kk;
	fp = fopen("trace_evict.txt", "w");
	//By giving operation and address to the Cache.

	j=0;	 // for a particular Set

	for(i=0;i<7;i++)    //For generating 8 different Tags
	{
		for(k=0;k<64;k++)   //For ByteSelect
		{

			ii=i<<21;
			jj=j<<6;
			kk= ii+ jj +k;
			fprintf(fp,"%d %x\n",zero,kk); //output is printed to the tracefile with Operation<space>Hexadecimal addresses
		}
	}
}
