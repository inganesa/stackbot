#include <math.h>
#define F 30

void velocities(float *cenx, float *ceny)
{
	int i;
	float vx, vy;
	float Ax = 0, Bx = 0, Ay = 0, By = 0;
	for(i=0; i<5; i++)
	{
		Ax = Ax + i*cenx[i];
		Bx = Bx + i*cenx[i];
		Ay = Ay + i*ceny[i];
		By = By + ceny[i];
	}
	vx = (F/10) * (Ax - 2*Bx);
	vy = (F/10) * (Ay - 2*By);
	
}
