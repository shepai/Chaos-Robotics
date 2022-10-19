


#include "vecN.h"
#include <math.h>
#include <malloc.h>


static int NUM_DIM;
void SetNumDimension_VecNd( int n ){	NUM_DIM = n;	}



VECTORN::VECTORN()
{
//	val = (double*)malloc( sizeof(double) * NUM_DIM );
			//	for( int i=0 ; i<NUM_DIM ; i++ )	val[i] = 0.0;
}

VECTORN::VECTORN( double* x )
{
//	val = (double*)malloc( sizeof(double) * NUM_DIM );
	for( int i=0 ; i<NUM_DIM ; i++ )	val[i] = x[i];
//	free(x);
}

VECTORN::~VECTORN()
{
//	free( val );	
}







void VECTORN::Zero( void )
{
	for( int i=0 ; i<NUM_DIM ; i++ )	val[i] = 0.0;
}


void VECTORN::Negate( void )
{
	for( int i=0 ; i<NUM_DIM ; i++ )	val[i] = -val[i];
}

void VECTORN::Normalize( void )
{
	
	double mag = Mag();
	
	if( mag < 1E-15 )	Zero();
	else
	{
		for( int i=0 ; i<NUM_DIM ; i++ )	val[i] = val[i]/mag;
	}
}


double VECTORN::Mag( void )
{
	double mag = 0.0;
	for( int i=0 ; i<NUM_DIM ; i++ )	mag += val[i]*val[i];
	mag = sqrt( mag );

	return mag;
}


VECTORN &VECTORN::operator=( const VECTORN &v )
{	
//	free( val );
//	val = (double*)malloc( sizeof(double) * NUM_DIM );

	for( int i=0 ; i<NUM_DIM ; i++ )
	{
		val[i] = v.val[i];
	}
	
	return *this;
}


VECTORN VECTORN::operator+( const VECTORN &v )
{
//	double* x = (double*)malloc( sizeof(double) * NUM_DIM );

	double x[MAX_NUM_DIM];

	for( int i=0 ; i<NUM_DIM ; i++ )	x[i] = val[i] + v.val[i];
	
	return VECTORN(x);

}


VECTORN VECTORN::operator-( const VECTORN &v )
{
//	double* x = (double*)malloc( sizeof(double) * NUM_DIM );

	double x[MAX_NUM_DIM];
	
	for( int i=0 ; i<NUM_DIM ; i++ )	x[i] = val[i] - v.val[i];
	
	return VECTORN( x );
}

double VECTORN::operator*( const VECTORN &v )
{
	double res = 0.0;

	for( int i=0 ; i<NUM_DIM ; i++ )	res += val[i] * v.val[i];

	return res;
}




VECTORN VECTORN::operator/( double a )
{
//	double* x = (double*)malloc( sizeof(double) * NUM_DIM );

	double x[MAX_NUM_DIM];
	
	for( int i=0 ; i<NUM_DIM ; i++ )	x[i] = val[i]/a;
	
	return VECTORN( x );
}

VECTORN VECTORN::operator*( double a )
{
//	double* x = (double*)malloc( sizeof(double) * NUM_DIM );

	double x[MAX_NUM_DIM];
	
	for( int i=0 ; i<NUM_DIM ; i++ )	x[i] = val[i]*a;
	
	return VECTORN( x );
}



