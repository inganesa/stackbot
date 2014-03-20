#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"
#include "fcntl.h"
#include "../frametx.h"

int thx1= 50,thx2= 50,thy1= 60,thy2= 50;
unsigned char max = 255;
extern int finalcenx,finalceny;
extern int finalintensity;

int temp1;
int N = 40;

#define size 40


 /* Write RGB data with Green Centroid Marker*/

/* Write RGB data with Green Centroid Marker*/
void applyMarker(char *data , int *centx, int *centy)
{
	int c = 10;
	int i, j;
	int cenx, ceny;
   for(i=0; i<76800; i++)
   {
   	for(j = 0; j < c; j++)
   	{
   		cenx = centx[j];
   		ceny = centy[j];
		 if((cenx!= 0) || (ceny != 0))
		 {
				if((i % 320) == ceny)
				{
				  data[i] = max;
				}
				else if((i/320) == cenx)
				{
					 data[i] = max;
				}
				else
				{
				   data[i] = data[i];
				}
		 }
   	 }
   	}
}

/*Compute the centroid of the frame*/
void multipleCentroid(char* data, int print)
{
	long stripI[size]={0};
	long stripX[size]={0};
	long stripY[size]={0};
	long obj[size]={0};
	long objX[size]={0};
	long objY[size]={0};
	long ob[10] = {0};
	int centx[size]={0};
	int centy[size]={0};
	int cenx[10]= {0};
	int ceny[10] = {0};
	int count=0;
	
	int i,j,k;
/*long sum1=0,sum2=0,sum3=1;*/

    for(i=thx1;i<240-thx2;i++)
    {
   	 for(j=thy1;j<320-thy2;j++)
   	 {
   		 if(data[320*i+j]!=0)
   		 {
   			/* sum1=sum1+i;
   			 sum2=sum2+j;
   			 sum3++;*/
   			 
   			 temp1=(int)(j/(320/N));
   			 stripI[temp1]=stripI[temp1]+1;
   			 stripX[temp1]=stripX[temp1]+i;
   			 stripY[temp1]=stripY[temp1]+j;
   			 
   		 }
   	 }
    }
    
	for (k =0 ;k < N; k++)
	{
		if(stripI[k]>5)
		{
			
			obj[count] = obj[count]+stripI[k];
			objX[count]=objX[count]+stripX[k];
			objY[count]=objY[count]+stripY[k];
		}
		else
		{
			count++;
		}
	}
	
	for(k=0;k<count-1;k++)
		{
			centx[k]=objX[k]/(obj[k]+1);
			centy[k]=objY[k]/(obj[k]+1);
		}
	
	i = 0;	
	for(k = 0; k < N; k++)
	{
		if(( centx[k] != 0) && (centy[k]!= 0 ) && (i < 10)&&(obj[k]>100))
		/*if(( centx[k] != 0) && (centy[k]!= 0 ) && (i < 10))*/
		{
			cenx[i] = centx[k];
			ob[i] = obj[k];
			ceny[i] = centy[k];
			i++;
		}
		
	}


    applyMarker(data ,cenx,ceny);
    
    if(print)
    {
    	finalcenx = -1;
    	finalceny = -1;
		for(i=0;i<10;i++)
		{
			if(cenx[i]!= 0 || ceny[i]!= 0)
			{
				/*if(13 + 0.14285*(cenx[i]*cos(0.02707)-ceny[i]*sin(0.02707))<=10)*/
				/*logMsg("Intensity of object = %ld\n", ob[i], 0,0,0,0,0);*/
				/*logMsg("\nCentxxxxxxxxxxxxxxxx = %ld\n Centyyyyyyyyy = %ld\n", cenx[i], ceny[i],0,0,0,0);*/
				

			/*printf("\nCentxxxxxxxxxxxxx %ld\n",cenx[i]);
			printf("Centyyyyyyyyyyyyyyyy %ld\n",ceny[i]);*/
				/*if(cenx[i]<120){*/
					finalcenx = cenx[i];
					finalceny = ceny[i];
					finalintensity = ob[i];
				/*}*/
			
			}
		}
		
    }
   
   
    
}


    
 

