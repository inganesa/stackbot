
#include "inetLib.h"
#include "sockLib.h"
#include "netinet/tcp.h"
#include "ioLib.h"
#include "wvLib.h"
#include "stdlib.h"
#include "stdio.h"

#include "frametx.h"


/* GLOBALS */

unsigned char frame_seq_cnt = 0;

extern int FrameSockFd;
#define DISPLAY_IP "172.21.74.235"
#define DISPLAY_PORT 50007

int init_frametx(void) 
{
  struct sockaddr_in serverAddr;
  int sockAddrSize;
  struct linger opt;
  int sockarg;
   
  printf("Reached init - 1 \n");
  /* create client's socket */
  if ( (FrameSockFd = socket( AF_INET, SOCK_STREAM, 0 )) == ERROR )
  {
    perror("connect() failed in frametx_init");
    close( FrameSockFd );
    return ERROR;
  }
  printf("Reached init - 2 \n");
  /* turn on zero linger time so that undelivered data is discarded when
     socket is closed
   */
  opt.l_onoff = 1;
  opt.l_linger = 0;

  sockarg = 1;

  setsockopt(FrameSockFd, SOL_SOCKET, SO_LINGER, (char*) &opt, sizeof(opt));
  setsockopt(FrameSockFd, IPPROTO_TCP, TCP_NODELAY, (char *)&sockarg, sizeof(int));
  printf("Reached init - 3 \n");
  /* build server socket address */
  /* build server socket address */
  sockAddrSize = sizeof (struct sockaddr_in);
  bzero( (char *)&serverAddr, sockAddrSize );
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(DISPLAY_PORT);
  serverAddr.sin_addr.s_addr = inet_addr(DISPLAY_IP);

  /* connect to server */
  if (connect(FrameSockFd, (struct sockaddr *)(&serverAddr), sockAddrSize) == ERROR)
  {
    perror("connect() failed in framegrab_init");
    close( FrameSockFd );
    return ERROR;
  }
  printf("Reached init - 4 \n");
  return OK;

}


void shutdown_frametx(void) 
{
  close( FrameSockFd );
}


int xga_frame_to_net(unsigned short *frameptr, int length)
{
  int bytes_written=0;
  int total_bytes_written=0;
  int bytes=0;
  unsigned char c = FRAME_ID;
  unsigned short pixel;

  c = frame_seq_cnt;
  /*bytes = write(FrameSockFd, &c, 2);*/
  total_bytes_written += bytes;

  frame_seq_cnt = (frame_seq_cnt+1) % 255;
  
  c = FRAME_ID;
  /*bytes = write(FrameSockFd, &c, 1);*/
  total_bytes_written += bytes;

 while(bytes_written < length)
  {
	pixel = *frameptr;
    bytes = write(FrameSockFd, (char *)&pixel, 2);
    bytes_written += bytes;
    frameptr++;
  }

 return total_bytes_written;
 
}



