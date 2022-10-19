



#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>



void CircleArray( char array[10] )
{
	char last = array[0];

	for( int i=0 ; i<9 ; i++ )
		array[i] = array[i+1];

	array[9] = last;
}


void Init_WELL_RAND( void );

void InitRandomSeed( void )
{
	time_t ltime;
	time( &ltime );
	
	char buffer[11];
	_ltoa( (long)ltime, buffer, 10 );

	for( int i=0 ; i<(int)ltime%10 ; i++ )		CircleArray( buffer );
	
	srand( (unsigned int)atoi(buffer)  );
	
	for( int i=0 ; i<rand()%27 ; i++ )		srand( (unsigned int)rand() );


	Init_WELL_RAND( );

}




double* AllocArray1( int num )
{
	return (double*)malloc( sizeof(double) * num );
}



double** AllocArray2( int num1 , int num2 )
{
	double** A = (double**)malloc( sizeof(double*) * num1 );

	for( int i=0 ; i<num1 ; i++ )
		A[i] = (double*)malloc( sizeof(double) * num2 );

	return A;
}

void FreeArray2( double** A , int num1 )
{
	for( int i=0 ; i<num1 ; i++ )	free(A[i]);
	free(A);
}



double*** AllocArray3( int num1 , int num2 , int num3 )
{
	double*** A = (double***)malloc( sizeof(double**) * num1 );

	for( int i=0 ; i<num1 ; i++ )
	{
		A[i] = (double**)malloc( sizeof(double*) * num2 );

		for( int j=0 ; j<num2 ; j++ )
			A[i][j] = (double*)malloc( sizeof(double) * num3 );
	}

	return A;
}

void FreeArray3( double*** A , int num1 , int num2 )
{
	int h = 0;

	for( int i=0 ; i<num1 ; i++ )
	{
		for( int j=0 ; j<num2 ; j++ )	
			free( A[i][j] );

		free( A[i] );
	}

	free( A );
}




void HUE2RGB( float H, float& R, float& G, float& B )
{
	float h = H;

	if( h == 360.0 )	h = 0.0;

	h /= 60.0;			// H 값은 [0,360)사이 값
	int i = (int)h;		// <=H 인 가장 큰 정수값
	float f = h - i;	// F 는 H 의 실수 부분
	
	float q = 1.0f - f;
	

	switch(i)
	{
		case 0:		R = 1.0f;	G = f;		B = 0.0f;	break;
		case 1:		R = q;		G = 1.0f;	B = 0.0f;	break;
		case 2:		R = 0.0f;	G = 1.0f;	B = f;		break;
		case 3:		R = 0.0f;	G = q;		B = 1.0f;	break;
		case 4:		R = f;		G = 0.0f;	B = 1.0f;	break;
		case 5:		R = 1.0f;	G = 0.0f;	B = q;		break;
	}
}


void HSV_To_RGB(double h, double& r, double& g, double& b)
{
	double s=1.0, v=1.0;

	// S != 0 이므로 색상(H)이 있다.
	double f, p, q, t;
	int i;

	if(h == 360.0)	h = 0.0;

	h /= 60.0;                      // H 값은 [0,360)사이 값
	i = (int)h;                     // <=H 인 가장 큰 정수값
	f = h - i;                      // F 는 H 의 실수 부분
	p = v * (1.0 - s);
	q = v * (1.0 - (s * f));
	t = v * (1.0 - (s * (1.0 - f)));

	switch(i)
	{
		case 0: r = v; g = t; b = p; break;
		case 1: r = q; g = v; b = p; break;
		case 2: r = p; g = v; b = t; break;
		case 3: r = p; g = q; b = v; break;
		case 4: r = t; g = p; b = v; break;
		case 5: r = v; g = p; b = q; break;
	}

}
