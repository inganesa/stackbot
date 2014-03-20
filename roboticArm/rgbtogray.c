
#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"
#include "fcntl.h"

typedef enum  color { Red = 1, Green = 2, Blue = 3} color;

void rgbToGray(char *pWriteBuf, char*savebuffbyteptr, color c)
{
    char agg;
    int i;
    double rmult=0.3, gmult=0.59, bmult=0.11;

   

    switch(c){
        case Red :  
        {
          rmult=1.0; gmult=0.0; bmult=0.0;
        
        }
       case Green:
        {
          rmult=0.0; gmult=1.0; bmult=0.0;
         }
       case Blue:
      {
          rmult=0.0; gmult=0.0; bmult=1.0;
        
      }
     default: 
      {
    	  rmult=0.3; gmult=0.59; bmult=0.11;
      } 
    }

    /* Write RGB data*/
    for(i=0; i<76800; i++)
    {
        pWriteBuf[i] = (rmult * savebuffbyteptr[2]) + (gmult * savebuffbyteptr[1]) + (bmult * savebuffbyteptr[0]); /* 2== R, 1===G, 0===B*/
        
        /* pWriteBuf++;
		 *pWriteBuf =agg;
         pWriteBuf++;
         *pWriteBuf =agg;
         pWriteBuf++;*/
         savebuffbyteptr+=4;
    }
}

