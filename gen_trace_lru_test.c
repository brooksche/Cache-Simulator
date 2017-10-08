//Determining the Linesize=64Bytes
// By Making Condition
		//-  Accessing all the 64Bytes for the Particular- Tag, Index;
//32bit CPU address		
                // 6bits for Byteselect
                // 15 bits for Index
                // 11 bits for Tagbits
		
#include<stdio.h>
int main()
{
FILE *fp;
fp = fopen("trace_lru_test.txt", "w");

int i,j,k,z=0;
int ii,jj,kk;


i=0;j=0;    //Assigning for a particular Tag, Set
	
ii=i<<21;
jj=j<<6;

	for(k=0;k<64;k++)
	{
	    kk = ii + jj + k;
        fprintf(fp,"%d %x\n",z,kk); //output is printed to the tracefile with Operation<space>Hexadecimal addresses.
	}
}
