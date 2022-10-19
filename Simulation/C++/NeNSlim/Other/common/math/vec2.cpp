




#include "vec2.h"
#include <math.h>


void VEC2::Zero( void )
{
	x = 0.0;	y = 0.0;
}

void VEC2::Set( double X, double Y )
{
	x = X;	y = Y;
}


void VEC2::Set( VEC2 v )
{
	x = v.x;	y = v.y;
}

void VEC2::Negate( void )
{
	x = -x;		y = -y;
}

void VEC2::Normalize( void )
{
	double mag = sqrt( x*x+y*y );

	if( mag < 1E-14 )
	{
		x = 0.0;	y = 0.0;
	}
	else
	{
		x /= mag;	y /= mag;
	}
}


double VEC2::Mag( void )
{
	return sqrt( x*x+y*y );;
}


double VEC2::Mag2( void )
{
	return x*x+y*y;
}


VEC2 VEC2::operator+( VEC2 v )
{
	VEC2 res;
	
	res.x = x + v.x;
	res.y = y + v.y;

	return res;
}


VEC2 VEC2::operator-( VEC2 v )
{
	VEC2 res;
	
	res.x = x - v.x;
	res.y = y - v.y;

	return res;
}

double VEC2::operator*( VEC2 v )
{
	return x*v.x + y*v.y;
}



VEC2 VEC2::operator/( double a )
{
	VEC2 res;

	res.x = x / a;
	res.y = y / a;

	return res;
}

VEC2 VEC2::operator*( double a )
{
	VEC2 res;

	res.x = x * a;
	res.y = y * a;

	return res;
}




