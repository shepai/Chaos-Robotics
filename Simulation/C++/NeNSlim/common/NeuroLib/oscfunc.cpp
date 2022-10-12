
#include <stdio.h>
#include <malloc.h>
#include <math.h>

#include "oscillator.h"


static double a = 0.7;
static double b = 0.675;
static double c = 1.75;
static double d = 0.013;
static double e = 0.022;

static double cInv = 1.0 / c;

#define INV_THREE	0.333333333333333



// BVP for ODE Articulated Animats (No Fins)
void OSCILLATOR::Equation_Asai_BVP( double* X )
{	
//	v[0] = c * ( (X[0]+Trans[0]) - (X[0]+Trans[0])*(X[0]+Trans[0])*(X[0]+Trans[0])/3.0 - (X[1]+Trans[1]) + z ) - d*(X[0]+Trans[0]);
//	v[1] = ( (X[0]+Trans[0]) - b*(X[1]+Trans[1]) + a ) / c;//   + d*(X[1]+Trans[1]);

	double x0 = X[0]+Trans[0];
	double x1 = X[1]+Trans[1];

	
	v[0] = c * ( x0 - x0*x0*x0*INV_THREE - x1 + z ) - d*x0;
	v[1] = ( x0 - b*x1 + a ) * cInv;
}

// BVP for ODE fin swimmers
void OSCILLATOR::Equation_Asai_BVP2( double* X )
{	
//	v[0] = c * ( (X[0]+Trans[0]) - (X[0]+Trans[0])*(X[0]+Trans[0])*(X[0]+Trans[0])/3.0 - (X[1]+Trans[1]) + z );
//	v[1] = ( (X[0]+Trans[0]) - b*(X[1]+Trans[1]) + a ) / c + e*(X[0]+Trans[0]);

	double x0 = X[0]+Trans[0];
	double x1 = X[1]+Trans[1];

	v[0] = c * ( x0 - x0*x0*x0*INV_THREE - x1 + z );
	v[1] = ( x0 - b*x1 + a ) * cInv + e*x0;
}

// BVP for No Robot - without delta
void OSCILLATOR::Equation_Asai_BVP_NoRobot( double* X )
{
	double x0 = X[0]+Trans[0];
	double x1 = X[1]+Trans[1];

	v[0] = c * ( x0 - x0*x0*x0*INV_THREE - x1 + z );
	v[1] = ( x0 - b*x1 + a ) * cInv;
}

// with delta
void OSCILLATOR::Equation_Asai_BVP_NoRobot_2( double* X )
{
	double x0 = X[0]+Trans[0];
	double x1 = X[1]+Trans[1];

	v[0] = c * ( x0 - x0*x0*x0*INV_THREE - x1 + z )	- d*x0;
	v[1] = ( x0 - b*x1 + a ) * cInv;
}


void OSCILLATOR::Equation_Coupled_BVP( double* X )
{	
	double x1 = X[0]+Trans[0];
	double y1 = X[1]+Trans[1];

	double x2 = X[2]+Trans[0];
	double y2 = X[3]+Trans[1];


	v[0] = c * ( x1 - x1*x1*x1/3.0 - y1 + z1 )	+ d*(x2-x1);
	v[1] = ( x1 - b*y1 + a )/c					+ e*(x2   );

	v[2] = c * ( x2 - x2*x2*x2/3.0 - y2 + z2 )	+ d*(x1-x2);
	v[3] = ( x2 - b*y2 + a )/c					+ e*(x1   );
}


// Hopf for No Robot
void OSCILLATOR::Equation_Hopf( double* X )
{
	double x0 = X[0]+Trans[0];
	double x1 = X[1]+Trans[1];

	double rr = x0*x0 + x1*x1;

	v[0] = (hopf_mu-rr)*x0 - hopf_freq*x1;
	v[1] = (hopf_mu-rr)*x1 + hopf_freq*x0;
}





///////////////// Miscellanaous ///////////////////////////////////////////////////////////////

void OSCILLATOR::Equation_Rossler( double* X )
{	
	double _a = 0.15;	
	double _b = 0.2;//z1;
	double _c = 10.0;
	
	v[0] = -X[1]-X[2];
	v[1] = X[0] + _a*X[1];
	v[2] = _b + X[2]*(X[0]-_c);	
}




void OSCILLATOR::Equation_Sprott_Tanh( double* X )
{	
	double _a = -0.2;
	double _b = 1.9;
		
	v[0] = X[1];
	v[1] = X[2];
	v[2] = -a*X[2]-X[1]-X[0]+_b*tanh(X[0]);
}



void OSCILLATOR::Equation_Kaufman( double* X )
{		
	double _b = 0.3;
		
	v[0] = -_b*X[0]+X[1]-X[1]*X[1]*X[1];
	v[1] = -_b*X[1]+X[2]-X[2]*X[2]*X[2];
	v[2] = -_b*X[2]+X[0]-X[0]*X[0]*X[0];
}






void OSCILLATOR::Equation_Sprott_K( double* X )
{			
	v[0] = X[0]*X[1] - X[2];
	v[1] = X[0] - X[1];
	v[2] = X[0] + 0.3*X[2];
}

void OSCILLATOR::Equation_Sprott_Q( double* X )
{			
	double A = 0.6;

	v[0] = X[1];
	v[1] = X[2];
	v[2] = -A*X[2] - X[1] + fabs(X[0])-1.0;
}





/////////////////  UnUsed  ///////////////////////////////

/*







void OSCILLATOR::Equation_Leaky( double* X )
{				  
	v[0] = -X[0];
}

void OSCILLATOR::Equation_Leaky_const( double* X )
{
	if( X[0] > 0.0 )	v[0] = -0.6;
	else				v[0] = 0.0;
}






void OSCILLATOR::Equation( void )
{
//	exin[0] = x[2] = sin( 0.15 * simtime );
//	exin[0] = x[2] = 1.0;

	double U = param[0];
	double W = param[1];

	double R = x[0]*x[0] + x[1]*x[1];

	v[0] = (U-R)*x[0] + W*x[1] + exin[0];
	v[1] = (U-R)*x[1] - W*x[0] + exin[1];	
}
*/


//////////////////////  Homeostatic 1-node /////////////////////////////
/*
double Activation( double x )
{
	return 1.0 / (1.0+exp(-x));
}

double Facilitation( double x )
{
	return -2.0*x + 1.0;
}


double FacilFunc( double fire )
{
	double res;

	double Lbound = 0.25;
	double Ubound = 1.0-Lbound;

	if( fire < Lbound )			res = (-1.0/Lbound)*fire + 1.0;
	else if( fire > Ubound )	res = fire/(Ubound-1.0) + 1.0/(1.0-Ubound) - 1.0;
	else						res = 0.0;

	return res;
}



void OSCILLATOR::Equation( void )
{
	double w  = param[0];
	double Tb = param[1];
	
	double y = x[0];
	double b = x[1];

	v[0] = -y + w*Activation(y+b) + exin[0];
	v[1] = Facilitation(Activation(y+b)) / Tb;
	
//	param[0] += fabs(param[0]) * Facil(Activation(y+b)) / 10.0;
	
}

*/
/*


/*


void OSCILLATOR::Equation( void )
{	
	exin[0] = x[2] = 1.0*sin( 0.1 * simtime );

	double S  = param[0];
	double U  = param[1];
	double r0 = param[2];

	double a = x[0];
	double b = x[1];

	double R = sqrt( a*a + b*b );

	v[0] = ( U*r0*a/R - b - U*a ) / S   +  exin[0];
	v[1] = ( U*r0*b/R + a - U*b ) / S;
}

*/