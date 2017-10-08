//Determining the Associvity/Setsize= 8 Locations;
// By Making Condition
		//-  Accessing all the Cache lines within a Setsize
		
		

#include<stdio.h>
int main()
{
FILE *fp;
fp = fopen("trace_evict.txt", "w");

int i,j,k,zero=0;
int ii,jj,kk;

		
//By giving operation and address to the Cache.
		
	


j=1;	//Assigning for a particular Set

for(i=0;i<1;i++)
{
	for(k=0;k<60;k++)
	{

	ii=i<<21;
	jj=j<<6;
	kk= ii+ jj +k;
    fprintf(fp,"%d %d\n",zero,kk);
	}

}


	
}

//Output of this Program:  Provides 8*64 different CPU addresses for a particular set.

//Expected Output of the Cache: The Cache Should give Number of Reads:8*64 

