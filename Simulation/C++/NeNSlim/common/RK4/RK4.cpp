

#include <stdio.h>

#include "RK4.h"


RK4::RK4( SIMULATION* sss )
{
	sim = sss;

	Clean_Var();

	h = 0.0;
			
}


RK4::~RK4()
{		
	
			
}


void RK4::Add_Var( double* var_x, double* var_y, double* var_v )
{
	x[num_rk4_var] = var_x;
	y[num_rk4_var] = var_y;
	v[num_rk4_var] = var_v;

	num_rk4_var++;
}



void RK4::Add_Var_VEC3( VEC3* X, VEC3* Y, VEC3* V )
{
/*
	x[num_rk4_var] = &X->x;	y[num_rk4_var] = &Y->x;	v[num_rk4_var] = &V->x;		num_rk4_var++;
	x[num_rk4_var] = &X->y;	y[num_rk4_var] = &Y->y;	v[num_rk4_var] = &V->y;		num_rk4_var++;
	x[num_rk4_var] = &X->z;	y[num_rk4_var] = &Y->z;	v[num_rk4_var] = &V->z;		num_rk4_var++;
*/
	Add_Var( &X->x , &Y->x , &V->x );
	Add_Var( &X->y , &Y->y , &V->y );
	Add_Var( &X->z , &Y->z , &V->z );
}


void RK4::Del_Var( double* var_x )
{
	for( int i=0 ; i<num_rk4_var ; i++ )
	{
		if( x[i] == var_x )
		{
			for( int d=i ; d<num_rk4_var ; d++ )
			{
				x[d] = x[d+1];
				y[d] = y[d+1];
				v[d] = v[d+1];
			}

			num_rk4_var--;
			break;
		}
	}
}

void RK4::Del_Var_VEC3( VEC3* X )
{
	Del_Var( &X->x );
	Del_Var( &X->y );
	Del_Var( &X->z );
}


void RK4::Clean_Var( void )
{
	for( int i=0 ; i<MAX_RK4_VAR ; i++ )
	{
		x[i] = y[i] = v[i] = 0;		// null pointer
		k1[i] = k2[i] = k3[i] = 0.0;
	}

	num_rk4_var = 0;
}



void RK4::Zero( void )
{
	for( int i=0 ; i<num_rk4_var ; i++ )
	{
		(*y[i]) = (*v[i]) = 0.0;
		k1[i] = k2[i] = k3[i] = 0.0;
	}
}


#define DIV_SIX		0.166666666666667
#define DIV_THREE	0.333333333333333

void RK4::RK4_STEP( int idx )
{
	switch( idx )
	{
		case 0:
				for( int i=0 ; i<num_rk4_var ; i++ )	(*y[i]) = (*x[i]);
		break;

		case 1:
				for( int i=0 ; i<num_rk4_var ; i++ )
				{
					k1[i] = (*v[i]);
					(*y[i]) = (*x[i]) + 0.5*h*k1[i];
				}
		break;

		case 2:
				for( int i=0 ; i<num_rk4_var ; i++ )
				{
					k2[i] = (*v[i]);
					(*y[i]) = (*x[i]) + 0.5*h*k2[i];
				}
		break;

		case 3:
				for( int i=0 ; i<num_rk4_var ; i++ )
				{
					k3[i] = (*v[i]);
					(*y[i]) = (*x[i]) +		h*k3[i];
				}
		break;

		case 4:
				for( int i=0 ; i<num_rk4_var ; i++ )
					(*v[i]) = ( k1[i]+(*v[i]) )*DIV_SIX  +  (k2[i]+k3[i])*DIV_THREE;

				//	(*v[i]) = ( k1[i] + 2.0*k2[i] + 2.0*k3[i] + (*v[i]) ) / 6.0;
		break;
	}
}


void RK4::Progress( void )
{
	for( int i=0 ; i<num_rk4_var ; i++ )
	{
		(*x[i]) += h * (*v[i]);
	}
}



