
#include <malloc.h>
#include "oscillator.h"



DOYALEARN::DOYALEARN()
{
	num_weight = 0;
	
	// dynamic alloc
	w = dw = inp = 0x00000000l;
	
	x = exin = 0x00000000l;
	
	TCI = wRate = bRate = fbGain = NULL;
}

DOYALEARN::~DOYALEARN()
{
	free( w );
	free( dw );
	free( inp );
}


void DOYALEARN::Add_Weight_Ref( double* yW , double* weight_V , double* afferent )
{
	if( num_weight < 1 )
	{
		w	= (double**)malloc( sizeof(double*) );
		dw	= (double**)malloc( sizeof(double*) );
		inp = (double**)malloc( sizeof(double*) );
	}
	else
	{
		w	= (double**)realloc( w	 , sizeof(double*) * (num_weight+1) );
		dw	= (double**)realloc( dw	 , sizeof(double*) * (num_weight+1) );
		inp = (double**)realloc( inp , sizeof(double*) * (num_weight+1) );		
	}


	w[num_weight]  = yW;
	dw[num_weight] = weight_V;
	inp[num_weight] = afferent;

	num_weight++;
	
}



void DOYALEARN::Learn( void )
{

	double pLearnt = 0.0;
	double pFeedback = 0.0;		// if RK4 alpha update

	for( int j=0 ; j<num_weight ; j++ )
	{
		if( inp[j] )	// check damage
		{
			pLearnt	  += ( *w[j] ) * ( *inp[j] );
			pFeedback += ( *w[j] ) * ( *inp[j] );
		}
	}

	pFeedback = pLearnt;


	double yError = *x - pLearnt;
	double yW_Chunk = (*wRate) * yError * (*TCI);

	for( int j=0 ; j<num_weight-1 ; j++ )
	{
		if( inp[j] )
		{
			// Ia weight learning is turned on only when stable regime
			*dw[j] = yW_Chunk * ( *inp[j] );
		}
		else
		{
			*dw[j] = *w[j] = 0.0;	// decay if damaged
		}
	}

	// bias rate
	*dw[num_weight-1] = (*bRate) * yError * (*TCI);

		
	// If alpha RK4 update --> moved to oscil_Alpha_RK.cpp
//	*exin = fbGain * ( pFeedback - (*x) );
	*exin = (*fbGain) * ( -yError );


}



