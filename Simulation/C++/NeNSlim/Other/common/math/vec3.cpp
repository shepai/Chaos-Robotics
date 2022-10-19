




#include "vec3.h"
#include <math.h>



void VEC3::Zero( void )
{
	x = 0.0;	y = 0.0;	z = 0.0;
}

void VEC3::Set( double X, double Y, double Z )
{
	x = X;	y = Y;	z = Z;
}


void VEC3::Set( VEC3 v )
{
	x = v.x;	y = v.y;	z = v.z;
}

void VEC3::Negate( void )
{
	x = -x;		y = -y;		z = -z;
}

void VEC3::Normalize( void )
{
	double mag = sqrt( x*x+y*y+z*z );

	if( mag < 1E-14 )
	{
		x = 0.0;	y = 0.0;	z = 0.0;
	}
	else
	{
		x /= mag;	y /= mag;	z /= mag;
	}
}


double VEC3::Mag( void )
{
	return sqrt( x*x+y*y+z*z );
}


double VEC3::Mag2( void )
{
	return x*x+y*y+z*z;
}


VEC3 VEC3::operator+( VEC3 v )
{
	VEC3 res;
	
	res.x = x + v.x;
	res.y = y + v.y;
	res.z = z + v.z;

	return res;
}


VEC3 VEC3::operator-( VEC3 v )
{
	VEC3 res;
	
	res.x = x - v.x;
	res.y = y - v.y;
	res.z = z - v.z;

	return res;
}

double VEC3::operator*( VEC3 v )
{
	return x*v.x + y*v.y + z*v.z;
}

VEC3 VEC3::operator^( VEC3 v )
{
	VEC3 res;
	
	res.x = y*v.z - z*v.y;
	res.y = z*v.x - x*v.z;
	res.z = x*v.y - y*v.x;

	return res;
}


VEC3 VEC3::operator/( double a )
{
	VEC3 res;

	res.x = x / a;
	res.y = y / a;
	res.z = z / a;

	return res;
}

VEC3 VEC3::operator*( double a )
{
	VEC3 res;

	res.x = x * a;
	res.y = y * a;
	res.z = z * a;

	return res;
}



void	VEC3::RotateABS( VEC3 vec , double a )
{
	double _11,_12,_13,_21,_22,_23,_31,_32,_33;
	double tx, ty, tz;

	tx = x; ty = y; tz = z;

	_11 = vec.x*vec.x*(1.0-cos(a))+cos(a);
	_12 = vec.x*vec.y*(1.0-cos(a))-vec.z*sin(a);
	_13 = vec.x*vec.z*(1.0-cos(a))+vec.y*sin(a);
	
	_21 = vec.y*vec.x*(1.0-cos(a))+vec.z*sin(a);
	_22 = vec.y*vec.y*(1.0-cos(a))+cos(a);
	_23 = vec.y*vec.z*(1.0-cos(a))-vec.x*sin(a);
	
	_31 = vec.z*vec.x*(1.0-cos(a))-vec.y*sin(a);
	_32 = vec.z*vec.y*(1.0-cos(a))+vec.x*sin(a);
	_33 = vec.z*vec.z*(1.0-cos(a))+cos(a);

	x = _11*tx + _12*ty + _13*tz;
	y = _21*tx + _22*ty + _23*tz;
	z = _31*tx + _32*ty + _33*tz;

}



/*
void MAT33::RotateMatrixAbs( VEC3 vec , double a )
{
	_11 = vec.x*vec.x*(1.0f-(double)cos(a))+(double)cos(a);
	_12 = vec.x*vec.y*(1.0f-(double)cos(a))-vec.z*(double)sin(a);
	_13 = vec.x*vec.z*(1.0f-(double)cos(a))+vec.y*(double)sin(a);
	

	_21 = vec.y*vec.x*(1.0f-(double)cos(a))+vec.z*(double)sin(a);
	_22 = vec.y*vec.y*(1.0f-(double)cos(a))+(double)cos(a);
	_23 = vec.y*vec.z*(1.0f-(double)cos(a))-vec.x*(double)sin(a);
	

	_31 = vec.z*vec.x*(1.0f-(double)cos(a))-vec.y*(double)sin(a);
	_32 = vec.z*vec.y*(1.0f-(double)cos(a))+vec.x*(double)sin(a);
	_33 = vec.z*vec.z*(1.0f-(double)cos(a))+(double)cos(a);

}
*/


