//##########################################################
//##                      R O B O T I S                   ##
//##         SyncWrite Example code for Dynamixel.        ##
//##                                           2009.11.10 ##
//##########################################################
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <termio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <signal.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <dynamixel.h>
#define NSTRS 3



#define PI	3.141592f
#define l1 16.5
#define l2 17
#define l3 24
#define NUM_ACTUATOR		7

// Control table address
#define P_GOAL_POSITION_L	30
#define P_GOAL_POSITION_H	31
#define P_GOAL_SPEED_L		32
#define P_GOAL_SPEED_H		33

// Defulat setting
#define DEFAULT_BAUDNUM		1 // 1Mbps
#define NUM_ACTUATOR		7 // Number of actuator
#define STEP_THETA			(PI / 100.0f) // Large value is more fast
#define CONTROL_PERIOD		(10000) // usec (Large value is more slow) 

//Robotic arm Sync Write declarations
int myPos[7] = {0,200,1023-200,300,1023-300,512,0};
int id[NUM_ACTUATOR];
float a,b,c;
float theta1,theta2,phi,x,y,z;
float r;
int CommStatus;
void PrintCommStatus(int CommStatus);
void PrintErrorCode(void);
void StepMove();
int centx, centy;
int move;

//TCP/IP server declarations
extern int errno;
extern void int_handler();
extern void broken_pipe_handler();
extern void serve_clients();

static int server_sock, client_sock;
static int fromlen;
//static char c;
static struct sockaddr_in server_sockaddr, client_sockaddr;
int num=5;
float theta;

void recv_data();
void serve_clients();
void inverse(float X,float Y,float Z);
void tcpipinit();

int main()
{	
	int pick=0,mid=0;
	int byte1,byte2;
	
	int baudnum = 1;
	int deviceIndex =0;
	float phase[NUM_ACTUATOR];
	
	//int AmpPos = 2048; // for EX series
	int i;
	
	
	printf( "\n\nSyncWrite example for Linux\n\n" );

	
	// Initialize id and phase
	for( i= 0; i<NUM_ACTUATOR; i++ )
	{
		id[i] = i+1;
		phase[i] = 2*PI * (float)i / (float)NUM_ACTUATOR;
	}

	///////// Open USB2Dynamixel ////////////
	if( dxl_initialize(deviceIndex, baudnum) == 0 )
	{
		printf( "Failed to open USB2Dynamixel!\n" );
		printf( "Press Enter key to terminate...\n" );
		getchar();
		return 0;
	}
	else
		printf( "Succeed to open USB2Dynamixel!\n" );
	//Initialize socket connection
	tcpipinit();
	// Set goal speed
	dxl_write_word( BROADCAST_ID, P_GOAL_SPEED_L, 60);
	//dxl_write_byte( BROADCAST_ID, 32, 20);
	//dxl_write_byte( BROADCAST_ID, 33, 3);
	usleep(CONTROL_PERIOD);
	// Set goal position
	//dxl_write_word( BROADCAST_ID, P_GOAL_POSITION_L, AmpPos );
	byte1=dxl_read_byte( BROADCAST_ID, 38);
	byte2=dxl_read_byte( BROADCAST_ID, 39);
	usleep(CONTROL_PERIOD);
	do{
		CommStatus = dxl_get_result();	
	}while(CommStatus != COMM_RXSUCCESS );
	printf("Byte1=%d,Byte2=%d",byte1,byte2);
	while(1)
	{	
		//while(1){
   		//printf( "Press Enter key to continue!(press ESC and Enter to quit)\n" );
		//if(getchar() == 0x1b)
		//break;

		//theta = 0;
		//printf ("enter goal position in x,y,z \n");
		
		//scanf ("%f" , &a);
		//scanf ("%f" , &b);
		//scanf ("%f" , &c);
		
		//if(pick!=1)
		//myPos[6]=10;
				
		//OneMove();
		
		//usleep(CONTROL_PERIOD);
		
		//printf("Enter 1 to pick object");
		//scanf("%d",&pick);
		//if(pick==1){
		//dxl_write_word(7,P_GOAL_POSITION_L,600);
		//mid=1;	
		//}
		//}	
		while(1){
		recv_data();
		
		//if(move==1){			//Move robotic arm only when move signal received from socket connx
			a=centx;
			b=centy;
			StepMove();
			usleep(CONTROL_PERIOD);
		//}
			
		}
	}

	dxl_terminate();
	printf( "Press Enter key to terminate...\n" );
	getchar();

	return 0;
}

// Print communication result
void PrintCommStatus(int CommStatus)
{
	switch(CommStatus)
	{
	case COMM_TXFAIL:
		printf("COMM_TXFAIL: Failed transmit instruction packet!\n");
		break;

	case COMM_TXERROR:
		printf("COMM_TXERROR: Incorrect instruction packet!\n");
		break;

	case COMM_RXFAIL:
		printf("COMM_RXFAIL: Failed get status packet from device!\n");
		break;

	case COMM_RXWAITING:
		printf("COMM_RXWAITING: Now receiving status packet!\n");
		break;

	case COMM_RXTIMEOUT:
		printf("COMM_RXTIMEOUT: There is no status packet!\n");
		break;

	case COMM_RXCORRUPT:
		printf("COMM_RXCORRUPT: Incorrect status packet!\n");
		break;

	default:
		printf("This is unknown error code!\n");
		break;
	}
}

// Print error bit of status packet
void PrintErrorCode()
{
	if(dxl_get_rxpacket_error(ERRBIT_VOLTAGE) == 1)
		printf("Input voltage error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_ANGLE) == 1)
		printf("Angle limit error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERHEAT) == 1)
		printf("Overheat error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_RANGE) == 1)
		printf("Out of range error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_CHECKSUM) == 1)
		printf("Checksum error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1)
		printf("Overload error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_INSTRUCTION) == 1)
		printf("Instruction code error!\n");
}

void inverse(float X,float Y,float Z)
{	float A,B,C;
	float temptheta2;
	r=sqrt(X*X+Y*Y);
	A=r*l2;
	B=(Z-l1)*l2;
	C=(r*r+(Z-l1)*(Z-l1)+l2*l2-l3*l3)/2;
	printf("a=%f, b=%f,c=%f", A,B,C);
	if(X==0)
	phi=PI/2;
	else
	{
	if(X>=0)
	phi=atan(Y/X);
	else
	phi=PI+atan(Y/X);
	}
	theta1=acos((B*C+sqrt(B*B*C*C-(C*C-A*A)*(A*A+B*B)))/(A*A+B*B));
	temptheta2=atan((r-l2*sin(theta1))/(Z-l1-l2*cos(theta1)))-theta1;
	if(temptheta2>=0)
	theta2=temptheta2;
	else
	theta2=temptheta2+PI;
	printf("\n phi = %f, Theta1= %f, Theta2=%f \n", phi, theta1, theta2);
}

void StepMove(){
	int k,i;
	float StepPos[30]={0,30.48,10.16,a,b,10.16,a,b,5.08,a,b,5.08,a,b,10.16,20.32,0,10.16,20.32,0,2.5,20.32,0,2.5,20.32,0,10.16,0,30.48,10.16};//10 Step picking and dropping motion		
	for(k=0;k<10;k++){								
		inverse(StepPos[k*3], StepPos[k*3 + 1],StepPos[k*3 + 2]);		//Move arm to Initial(Reset) Position
		theta1=theta1*195.37;
		theta2=theta2*195.37;
		phi=phi*195.37;
		myPos[0]=phi;
		myPos[1]=512 + (int)theta1;
		myPos[2]=512 - (int)theta1;
		myPos[3]=512 - (int)theta2;
		myPos[4]=512 + (int)theta2;
		myPos[5]=512;
		theta=0;
		do{
		dxl_set_txpacket_id(BROADCAST_ID);
		dxl_set_txpacket_instruction(INST_SYNC_WRITE);
		dxl_set_txpacket_parameter(0, P_GOAL_POSITION_L);
		dxl_set_txpacket_parameter(1, 2);
		for( i=0; i<NUM_ACTUATOR; i++ ){
			dxl_set_txpacket_parameter(2+3*i, id[i]);
			dxl_set_txpacket_parameter(2+3*i+1, dxl_get_lowbyte(myPos[i]));
			dxl_set_txpacket_parameter(2+3*i+2, dxl_get_highbyte(myPos[i]));
		}
		dxl_set_txpacket_length((2+1)*NUM_ACTUATOR+4);
		dxl_txrx_packet();
		CommStatus = dxl_get_result();
		if( CommStatus == COMM_RXSUCCESS ){
			PrintErrorCode();
		}
		else{
			PrintCommStatus(CommStatus);
			break;
		}
		theta += STEP_THETA;
		usleep(CONTROL_PERIOD);
		}while(theta<2*PI);
		if(k==2)					//grab the object on third move
		myPos[6]=150;
					
		if(k==6)
		myPos[6]=0;					//Release the object on the sixth move
	}
}	


void OneMove(){
	int k,i;
	float StepPos[3]={a,b,c};	//1 Step  motion		
									
		inverse(StepPos[0], StepPos[1],StepPos[2]);		//Move arm to Initial(Reset) Position
		theta1=theta1*195.37;
		theta2=theta2*195.37;
		phi=phi*195.37;
		myPos[0]=phi;
		myPos[1]=512 + (int)theta1;
		myPos[2]=512 - (int)theta1;
		myPos[3]=512 - (int)theta2;
		myPos[4]=512 + (int)theta2;
		myPos[5]=512;
		myPos[6]=300;
		theta=0;
		do{
			dxl_set_txpacket_id(BROADCAST_ID);
			dxl_set_txpacket_instruction(INST_SYNC_WRITE);
			dxl_set_txpacket_parameter(0, P_GOAL_POSITION_L);
			dxl_set_txpacket_parameter(1, 2);
			for( i=0; i<NUM_ACTUATOR; i++ ){
				dxl_set_txpacket_parameter(2+3*i, id[i]);
				dxl_set_txpacket_parameter(2+3*i+1, dxl_get_lowbyte(myPos[i]));
				dxl_set_txpacket_parameter(2+3*i+2, dxl_get_highbyte(myPos[i]));
			}
			dxl_set_txpacket_length((2+1)*NUM_ACTUATOR+4);
			dxl_txrx_packet();
		CommStatus = dxl_get_result();
		if( CommStatus == COMM_RXSUCCESS ){
			PrintErrorCode();
		}
		else{
			PrintCommStatus(CommStatus);
			break;
		}
		theta += STEP_THETA;
		usleep(CONTROL_PERIOD);
		}while(theta<2*PI);	
	
}	
//Initialize TCP/IP socket connx
void tcpipinit()
{
   	struct linger opt;
   	int sockarg;
	
	if((server_sock=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("server: socket");
		exit(1);
	}
	bzero((char*) &server_sockaddr, sizeof(server_sockaddr));
	server_sockaddr.sin_family = AF_INET;
	server_sockaddr.sin_port = htons(1234);
	server_sockaddr.sin_addr.s_addr=inet_addr("172.21.74.46");

	/* Bind address to the socket */
	if(bind(server_sock, (struct sockaddr *) &server_sockaddr,
            sizeof(server_sockaddr)) < 0) 
    {
		perror("server: bind");
		exit(1);
	}

    /* turn on zero linger time so that undelivered data is discarded when
       socket is closed
     */
    opt.l_onoff = 1;
    opt.l_linger = 0;

    sockarg = 1;
 
    setsockopt(server_sock, SOL_SOCKET, SO_LINGER, (char*) &opt, sizeof(opt));
    setsockopt(client_sock, SOL_SOCKET, SO_REUSEADDR, (char *)&sockarg, sizeof(int));
    signal(SIGINT, int_handler);
    signal(SIGPIPE, broken_pipe_handler);


    serve_clients();

}

/* Listen and accept function */
void serve_clients()
{
		
		char *bs = (char *) malloc(32);
		
	
		/* Listen on the socket */
		if(listen(server_sock, 5) < 0) {
			perror("server: listen");
			exit(1);
		}
                else
                {
                    printf("Listening to port 1234\n");
                }


		
		printf("Here1\n");
		/* Accept connections */
		if((client_sock=accept(server_sock, 
                               (struct sockaddr *)&client_sockaddr,&fromlen)) < 0) 
        	{
			perror("server: accept");
			exit(1);
		}
                else
                {
                    printf("Connection made on port 1234\n");
                }

		printf("Here2\n");
		x=recv(client_sock,(char *)bs,(3*sizeof(char)), MSG_WAITALL);
		printf("bytes received %d \n",(int)x);
		printf("x= %d y=%d\n mid=%x", bs[0],bs[2], bs[1]);
		centx=bs[0];
		centy=bs[2];
		y=0;
		x=0;
}

void recv_data(){
	char *bs = (char *) malloc(32);
	recv(client_sock,(char *)bs,(3*sizeof(char)), MSG_WAITALL);
	printf("x= %d y=%d\n", bs[0],bs[2]);	
	centx=bs[0];
	centy=bs[2];
	if(bs[1]=='a')		//Send 'a' from vxworks as 2nd byte only when the arm has to move
	move=1;
	else
	move=0;	
}


/* Close sockets after a Ctrl-C interrupt */
void int_handler()
{
	char ch;

	printf("Enter y to close sockets or n to keep open:");
	scanf("%c", &ch);

	if(ch == 'y') {
		printf("\nsockets are being closed\n");
		close(client_sock);
		close(server_sock);
	}

	exit(0);

}

void broken_pipe_handler()
{
	char ch;

	printf("Enter y to continue serving clients or n to halt:");
	scanf("%c", &ch);

	if(ch == 'y') {
		printf("\nwill continue serving clients\n");
		serve_clients();
	}
	else
		exit(0);

}

	
