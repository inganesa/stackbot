
#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"
#include "fcntl.h"

unsigned char G[76800];
unsigned char convG[76800];

#define K 1
unsigned char PSF[9] = {-K/8, -K/8, -K/8, -K/8, K+1, -K/8, -K/8, -K/8, -K/8};

void sharpenGS(char *convG, char *G, unsigned char newK)
{
	int filt = 1;
    int i,j;
        PSF[0] = -newK/8;
        PSF[1] = -newK/8;
        PSF[2] = -newK/8;
        PSF[3] = -newK/8;
        PSF[4] = newK+1;
        PSF[5] = -newK/8;
        PSF[6] = -newK/8;
        PSF[7] = -newK/8;
        PSF[8] = -newK/8;

    /* Read G data */
    for(i=0; i<76800; i++)
    {
     convG[i]=G[i];
    }

    /*Skip first and last row, no neighbors to convolve with*/
    for(i=1; i<239; i++)
    {

        for(j=1; j<319; j++)
        {
            convG[(i*320)+j]=0;
            convG[(i*320)+j] += PSF[0] * G[((i-1)*320)+j-1];
            convG[(i*320)+j] += PSF[1] * G[((i-1)*320)+j];
            convG[(i*320)+j] += PSF[2] * G[((i-1)*320)+j+1];
            convG[(i*320)+j] += PSF[3] * G[((i)*320)+j-1];
            convG[(i*320)+j] += PSF[4] * G[((i)*320)+j];
            convG[(i*320)+j] += PSF[5] * G[((i)*320)+j+1];
            convG[(i*320)+j] += PSF[6] * G[((i+1)*320)+j-1];
            convG[(i*320)+j] += PSF[7] * G[((i+1)*320)+j];
            convG[(i*320)+j] += PSF[8] * G[((i+1)*320)+j+1];

            if(filt && ((convG[(i*320)+j] > 140) || (convG[(i*320)+j] < 100)))
            {
                convG[(i*320)+j]=0;
            }
        }
    }
 
}

