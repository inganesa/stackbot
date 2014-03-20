#include "inetLib.h"
#include "sockLib.h"
#include "netinet/tcp.h"
#include "ioLib.h"
#include "wvLib.h"
#include "stdlib.h"
#include "stdio.h"
/* GLOBALS */

#include "atomTx.h"
extern int AtomSockFd;
#define ATOM_IP "172.21.74.46"
#define ATOM_PORT 1234

int init_atomtx(void) 
{
  struct sockaddr_in clientAddr;
  int sockAddrSize;
  struct linger opt;
  int sockarg;
   
  printf("Reached init - 1 \n");
  /* create client's socket */
  if ( (AtomSockFd = socket( AF_INET, SOCK_STREAM, 0 )) == ERROR )
  {
    perror("connect() failed in Atomtx_init");
    close( AtomSockFd );
    return ERROR;
  }
  printf("Reached init - 2 \n");
  /* turn on zero linger time so that undelivered data is discarded when
     socket is closed
   */
  opt.l_onoff = 1;
  opt.l_linger = 0;

  sockarg = 1;

  setsockopt(AtomSockFd, SOL_SOCKET, SO_LINGER, (char*) &opt, sizeof(opt));
  setsockopt(AtomSockFd, IPPROTO_TCP, TCP_NODELAY, (char *)&sockarg, sizeof(int));
  printf("Reached init - 3 AtomTx \n");
  /* build server socket address */
  /* build server socket address */
  sockAddrSize = sizeof (struct sockaddr_in);
  bzero( (char *)&clientAddr, sockAddrSize );
  clientAddr.sin_family = AF_INET;
  clientAddr.sin_port = htons(ATOM_PORT);
  clientAddr.sin_addr.s_addr = inet_addr(ATOM_IP);

  /* connect to server */
  if (connect(AtomSockFd, (struct sockaddr *)(&clientAddr), sockAddrSize) == ERROR)
  {
    perror("connect() failed in atom_init");
    close( AtomSockFd );
    return ERROR;
  }
  printf("Reached init - 4 \n");
  return OK;

}


void shutdown_atomtx(void) 
{
  close( AtomSockFd );
}

/*
int xga_frame_to_net(unsigned short *frameptr, int length)
{
  int bytes_written=0;
  int total_bytes_written=0;
  int bytes=0;
  unsigned char c = FRAME_ID;
  unsigned short pixel;

  c = frame_seq_cnt;
  bytes = write(FrameSockFd, &c, 2);
  total_bytes_written += bytes;

  frame_seq_cnt = (frame_seq_cnt+1) % 255;
  
  c = FRAME_ID;
  bytes = write(FrameSockFd, &c, 1);
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
*/


 
