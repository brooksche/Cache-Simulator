//Determining the Cache Size= 16MB
//By Accessing all the Cache lines 
//32bit CPU address		
// 6bits for Byteselect
// 15 bits for Index
// 11 bits for Tagbits

#include <stdio.h>
int main()
{
	FILE *fp;
	int zero= 0;
	int i,j,k;
	int ii,jj,kk;
	fp = fopen("trace_all_addr.txt", "w");
	for( i=0; i<2048; i++)     //  For generating different Tags
	{
		for( j=0;j<32764;j++)  //  For generating different sets
		{
			for( k=0; k<64;k++) // For Byte select
			{
				ii = i << 21;
				jj = j << 6;
				kk = ii + jj + k;
				fprintf(fp,"%d %x\n",zero,kk);  //output is printed to the tracefile with Operation, Hexadecimal addresses
			}

		}
	}
}
