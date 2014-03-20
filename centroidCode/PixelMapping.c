#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"
#include "fcntl.h"
#include "../frametx.h"
#include <math.h>

#define C 0.14285 
#define OX 13 
#define OY 28.3
#define theta 0.02707 

extern float xcor,ycor;

void PixtoRobotic( float xp, float yp)
{
	float temp;
	
	if(xp>=0)
	{	
	temp=xp;
	xp=160-yp;
	yp=temp-120;
	xcor = OX + C*(1+0.00018345*pow(C,2)*(pow(xp,2)+pow(yp,2)))*(xp*cos(theta)-yp*sin(theta));
	ycor = OY + C*(1+0.00018345*pow(C,2)*(pow(xp,2)+pow(yp,2)))*(xp*sin(theta)+yp*cos(theta));		
	/*xcor = OX + C*(xp*cos(theta)-yp*sin(theta));
	ycor = OY + C*(xp*sin(theta)+yp*cos(theta));*/
	}
	else
	{
		xcor = -1;
		ycor = -1;
	}
	/*logMsg("xxxxxxx-coordinate = %d\n yyyy-coordinate = %d\n", (int)xcor, (int)ycor,0,0,0,0);*/
	
	
}



