/* ***************************************************************************** */
/* Copyright:      Francois Panneton and Pierre L'Ecuyer, University of Montreal */
/*                 Makoto Matsumoto, Hiroshima University                        */
/* Notice:         This code can be used freely for personal, academic,          */
/*                 or non-commercial purposes. For commercial purposes,          */
/*                 please contact P. L'Ecuyer at: lecuyer@iro.UMontreal.ca       */
/* ***************************************************************************** */

#include <windows.h>
#include <math.h>

#define NUM_MAX_THREAD	128

#define W 32
#define R 32
#define M1 3
#define M2 24
#define M3 10

#define MAT0POS(t,v)	(v^(v>>t))
#define MAT0NEG(t,v)	(v^(v<<(-(t))))
#define Identity(v)		(v)

#define V0(n)		STATE[n][ state_i[n]                  ]
#define VM1(n)		STATE[n][(state_i[n]+M1) & 0x0000001fU]
#define VM2(n)		STATE[n][(state_i[n]+M2) & 0x0000001fU]
#define VM3(n)		STATE[n][(state_i[n]+M3) & 0x0000001fU]
#define VRm1(n)		STATE[n][(state_i[n]+31) & 0x0000001fU]

#define newV0(n)	STATE[n][(state_i[n]+31) & 0x0000001fU]
#define newV1(n)	STATE[n][ state_i[n]                  ]

#define FACT 2.32830643653869628906e-10


static unsigned int state_i[NUM_MAX_THREAD];
static unsigned int STATE[NUM_MAX_THREAD][R];

void Init_WELL_RAND( void )
{		
	for ( int i=0 ; i<NUM_MAX_THREAD ; i++ )
	{
		state_i[i] = 0;

		for ( int j=0 ; j<R ; j++ )
		{
			unsigned int curTime = timeGetTime();
			STATE[i][j] = curTime + ( ( curTime + (unsigned int)rand() ) % ((unsigned int)rand()+1) );
		}
	}
}


// WELL 1024a algorithm for Multi Thread --> use threadNum=0 for simgle thread
double RandWELL( int N )
{
	static unsigned int z0, z1, z2;

	z0    = VRm1(N);
	z1    = Identity( V0(N) )        ^ MAT0POS(   8 , VM1(N) );
	z2    = MAT0NEG ( -19 , VM2(N) ) ^ MAT0NEG( -14 , VM3(N) );

	newV1(N) = z1                 ^ z2; 
	newV0(N) = MAT0NEG (-11,z0)   ^ MAT0NEG(-7,z1)    ^ MAT0NEG(-13,z2) ;

	state_i[N] = (state_i[N] + 31) & 0x0000001fU;

	return (double)STATE[N][state_i[N]] * FACT;
}


int randbtn( int minValue, int maxValue )
{
	double wellrand = RandWELL(0);
	return (int)( wellrand * ( (double)(maxValue - minValue) + 0.99 ) ) + minValue;
}

int randbtn( int T , int minValue, int maxValue )
{
	double wellrand = RandWELL(T);
	return (int)( wellrand * ( (double)(maxValue - minValue) + 0.99 ) ) + minValue;
}


double randbtn( double min , double max )
{
	double res = RandWELL(0) * ( max - min ) + min;

	if( res > max )	res = max;
	if( res < min )	res = min;

	return res;
}


///// Gaussian Random ///////////////////////////////////////////////////////
double GaussianRandom( int num_thr , double mean, double stdev )
{
	double v1, v2, s, temp;

	do
	{
		v1 = 2.0 * RandWELL(num_thr) - 1.0;      // -1.0 ~ 1.0 까지의 값
		v2 = 2.0 * RandWELL(num_thr) - 1.0;      // -1.0 ~ 1.0 까지의 값		
		s = v1*v1 + v2*v2;
	}
	while ( s>=1 || s==0 );

	s = sqrt( (-2.0 * log(s)) / s );

	temp = v1 * s;
	temp = (stdev * temp) + mean;

	return temp;

}


double GaussianFunc2D_Std( double X , double Y , double sigma )
{
	return 0.5 * exp( -0.5 * (X*X + Y*Y) / (sigma*sigma) ) / (3.141592*sigma*sigma);
}

double GaussianFunc2D_One( double X , double Y , double sigma )
{
	return exp( -0.5 * (X*X + Y*Y) / (sigma*sigma) );
}

