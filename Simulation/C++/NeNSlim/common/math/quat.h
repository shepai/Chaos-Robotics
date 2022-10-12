
#ifndef QUAT_H
#define QUAT_H


#include "mat33.h"

struct QUAT
{
	float r, i, j, k;


	void Set( float R, float I, float J, float K )
	{
		r=R;	i=I;	j=J;	k=K;
	}

	MAT33 Quat_to_Mat( void );
	void Norm(void);
	
	QUAT	operator*( QUAT );
	QUAT	operator*( float );
	QUAT	operator/( float );

};








#endif