


#include "matNN.h"
#include <math.h>
#include <malloc.h>

#include <stdarg.h>


static VECTORN *a, *e;	// for GS QR decomp.

static int NUM_DIM;
void SetNumDimension_NxN( int n )
{
	NUM_DIM = n;

	a = new VECTORN[NUM_DIM];
	e = new VECTORN[NUM_DIM];
}

void Delete_Global_Variables_NxN( void )
{
	delete [] a;
	delete [] e;
}



void MATRIXNXN::QR_Factorisation_LCE( MATRIXNXN& Q , MATRIXNXN& R )
{

	for( int i=0 ; i<NUM_DIM ; i++ )	a[0].val[i] = val[i][0];

	e[0] = a[0];
	e[0].Normalize();
	
	R.val[0][0] = e[0]*a[0];
	for( int i=0 ; i<NUM_DIM ; i++ )	Q.val[i][0] = e[0].val[i];


	for( int j=1 ; j<NUM_DIM ; j++ )
	{
		for( int i=0 ; i<NUM_DIM ; i++ )	a[j].val[i] = val[i][j];

		VECTORN S;	S.Zero();
		for( int k=0 ; k<j ; k++ )
		{
			S = S + ( e[k]*(a[j]*e[k]) );
		}

		e[j] = a[j] - S;
		e[j].Normalize();


		R.val[j][j] = e[j]*a[j];
		for( int i=0 ; i<NUM_DIM ; i++ )	Q.val[i][j] = e[j].val[i];
	}

}



void MATRIXNXN::QR_Factorisation( MATRIXNXN& Q , MATRIXNXN& R )
{

	for( int j=0 ; j<NUM_DIM ; j++ )
		for( int i=0 ; i<NUM_DIM ; i++ )
			a[j].val[i] = val[i][j];


	e[0] = a[0];
	e[0].Normalize();

	for( int j=1 ; j<NUM_DIM ; j++ )
	{
		VECTORN S;	S.Zero();
		for( int k=0 ; k<j ; k++ )
		{
			S = S + ( e[k]*(a[j]*e[k]) );
		}

		e[j] = a[j] - S;
		e[j].Normalize();
	}


	for( int j=0 ; j<NUM_DIM ; j++ )
		for( int i=0 ; i<NUM_DIM ; i++ )
		{
			Q.val[i][j] = e[j].val[i];
		}

//	R.Zero(); // This is done outside

	for( int i=0 ; i<NUM_DIM ; i++ )
		for( int j=i ; j<NUM_DIM ; j++ )
		{
			R.val[i][j] = e[i]*a[j];
		}
}


MATRIXNXN::MATRIXNXN()
{
	val = (double**)malloc( sizeof(double*) * NUM_DIM );
	for( int i=0 ; i<NUM_DIM ; i++ )
	{
		val[i] = (double*)malloc( sizeof(double) * NUM_DIM );

		for( int j=0 ; j<NUM_DIM ; j++ )	val[i][j] = 0.0;
	}
}


MATRIXNXN::MATRIXNXN( double **s ) // 생성자
{
//	len = strlen(s);
//	str = new char[len+1];
//	strcpy(str,s);

	val = (double**)malloc( sizeof(double*) * NUM_DIM );
	for( int i=0 ; i<NUM_DIM ; i++ )
	{
		val[i] = (double*)malloc( sizeof(double) * NUM_DIM );

		for( int j=0 ; j<NUM_DIM ; j++ )	val[i][j] = s[i][j];
	}

}


MATRIXNXN::~MATRIXNXN()
{
	for( int i=0 ; i<NUM_DIM ; i++ )	free( val[i] );
	free( val );
}




void MATRIXNXN::Set( double arg, ... )
{
	va_list ap;

	va_start( ap , arg );

	double firstVal = arg;

	
	for( int i=0 ; i<NUM_DIM ; i++ )
	{
		double x;

		for( int j=0 ; j<NUM_DIM ; j++ )
		{
			if( i == 0 && j == 0 )	x = firstVal;
			else					x = va_arg( ap , double );

			val[i][j] = x;
		}
	}
}


void MATRIXNXN::Set_Identity( void )
{
	Zero( );

	for( int s=0 ; s<NUM_DIM ; s++ )	val[s][s] = 1.0;
}



void MATRIXNXN::Zero( void )
{
	for( int i=0 ; i<NUM_DIM ; i++ )
		for( int j=0 ; j<NUM_DIM ; j++ )
			val[i][j] = 0.0;
}



void MATRIXNXN::CopyFrom( MATRIXNXN src )
{
	for( int i=0 ; i<NUM_DIM ; i++ )
		for( int j=0 ; j<NUM_DIM ; j++ )
			val[i][j] = src.val[i][j];
}




MATRIXNXN MATRIXNXN::operator+( const MATRIXNXN &m )
{
	double** x = (double**)malloc( sizeof(double*) * NUM_DIM );
	for( int i=0 ; i<NUM_DIM ; i++ )	x[i] = (double*)malloc( sizeof(double) * NUM_DIM );
	
	for( int i=0 ; i<NUM_DIM ; i++ )
		for( int j=0 ; j<NUM_DIM ; j++ )
			x[i][j] = val[i][j] + m.val[i][j];
	
	return MATRIXNXN(x);

}

MATRIXNXN MATRIXNXN::operator-( const MATRIXNXN &m )
{
	double** x = (double**)malloc( sizeof(double*) * NUM_DIM );
	for( int i=0 ; i<NUM_DIM ; i++ )	x[i] = (double*)malloc( sizeof(double) * NUM_DIM );
	
	for( int i=0 ; i<NUM_DIM ; i++ )
		for( int j=0 ; j<NUM_DIM ; j++ )
			x[i][j] = val[i][j] - m.val[i][j];
	
	return MATRIXNXN(x);

}


MATRIXNXN MATRIXNXN::operator*( const MATRIXNXN &m )
{
	double** x = (double**)malloc( sizeof(double*) * NUM_DIM );
	for( int i=0 ; i<NUM_DIM ; i++ )
	{
		x[i] = (double*)malloc( sizeof(double) * NUM_DIM );

		for( int j=0 ; j<NUM_DIM ; j++ )	x[i][j] = 0.0;
	}

	
	for( int i=0 ; i<NUM_DIM ; i++ )
	{
		for( int j=0 ; j<NUM_DIM ; j++ )
		{
			for( int k=0 ; k<NUM_DIM ; k++ )	x[i][j] += val[i][k] * m.val[k][j];
		}
	}
	
	return MATRIXNXN(x);

}


MATRIXNXN &MATRIXNXN::operator=( const MATRIXNXN &m ) // = 연산자 오버로딩
{	
	for( int i=0 ; i<NUM_DIM ; i++ )
		for( int j=0 ; j<NUM_DIM ; j++ )
			val[i][j] = m.val[i][j]; 

	return *this;
}


/*
MATRIXNXN &MATRIXNXN::operator=( MATRIXNXN &v )
{	
	free( val );
	val = (double*)malloc( sizeof(double) * NUM_DIM );

	for( int i=0 ; i<NUM_DIM ; i++ )	val[i] = v.val[i];
	
	return *this;
}
*/



void MATRIXNXN::Transpose( void )
{
	MATRIXNXN T;

	for( int i=0 ; i<NUM_DIM ; i++ )
		for( int j=0 ; j<NUM_DIM ; j++ )
			T.val[i][j] = val[j][i]; 



}