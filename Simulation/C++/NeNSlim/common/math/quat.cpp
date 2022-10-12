


#include "quat.h"
#include <math.h>



MAT33 QUAT::Quat_to_Mat(void)
{
	MAT33 res;

	res._11 = 1.0f - 2.0f*j*j - 2.0f*k*k;	res._12 = 2.0f*i*j - 2.0f*r*k;	res._13 = 2.0f*i*k + 2.0f*r*j;

	res._21 = 2.0f*i*j + 2.0f*r*k;	res._22 = 1.0f - 2.0f*i*i - 2.0f*k*k;	res._23 = 2.0f*j*k - 2.0f*r*i;

	res._31 = 2.0f*i*k - 2.0f*r*j;	res._32 = 2.0f*j*k + 2.0f*r*i;	res._33 = 1.0f - 2.0f*i*i - 2.0f*j*j;

	return res;

}





QUAT	QUAT::operator*( QUAT q )
{
	QUAT res;

	res.r = r*q.r - i*q.i - j*q.j - k*q.k;
	res.i = r*q.i + q.r*i + j*q.k - k*q.j;
	res.j = r*q.j + q.r*j + k*q.i - i*q.k;
	res.k = r*q.k + q.r*k + i*q.j - j*q.i;

	return res;

}


QUAT	QUAT::operator*( float f )
{
	QUAT res;

	res.r = r*f;	res.i = i*f;	res.j = j*f;	res.k = k*f;

	return res;

}

QUAT	QUAT::operator/( float f )
{
	QUAT res;

	res.r = r/f;	res.i = i/f;	res.j = j/f;	res.k = k/f;

	return res;

}


void QUAT::Norm(void)
{	
	float f = (float)sqrt( (double)(r*r + i*i + j*j + k*k) );

	if( f != 0.0f )
	{
		r = r/f;	i = i/f;	j = j/f;	k = k/f;
	}
	
}



