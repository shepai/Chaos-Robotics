

#include <stdio.h>
#include <malloc.h>
#include <math.h>

#include "..\Utils\otherfunc.h"
#include "oscillator.h"
#include "oscil_random_net.h"




void OSCILLATOR::FunctionV_RK4( void )
{
	if( isAlive )
	{
		Equation(y);	// Oscillator Equation

		for( int i=0 ; i<num_node ; i++ )
		{
			v[i] = ( v[i] + exin_con[i] + exin_sensor[i] ) * TCI;
		}

	}
	else
	{
		for( int i=0 ; i<num_node ; i++ )	v[i] = y[i] = 0.0;
	}

}





void OSCILLATOR::AuxCalc( void )
{
	// Acceleration
//	double b = 0.675;	double c = 1.75;
//	double accX = ( c*(1.0-x[0]*x[0])*v[0] - v[1] );
//	double accY = ( v[0] - b*v[1] ) / (c);


	// Numerically Calculated Rotation Centre
	double TC = net->learnT * 250.0;
	double h = net->StepSize;

	for( int i=0 ; i<2 ; i++ )
	{
		double cV = ( x[i] - CenterNumeric[i] ) / TC;
		
		CenterNumeric[i] += cV * h;
	}

	


	// Fixed for Hopf --> z = -1.0
	if( z < -0.1 )
	{
		Center[0] = Center[1] = 0.0;
	}
	else
	{
		// Variable Singular Point for BVP
		double nomi = pow( -83.99999999999999 + 81.0*z + 81.0*sqrt( 1.1273861078104317-2.074074074074074*z+z*z ) , 
							1.0/3.0 );
		Center[0] = -1.847964201499775 / nomi + 0.26456684199469993 * nomi;
		Center[1] = (Center[0]+0.7)/0.675;
	}
	
	// Phase
	Calculate_Phase_BVP( );

	// Instantaneous Period
	Calculate_Period_BVP( );

}


void OSCILLATOR::Frequency_Adaptation( double stepsize )
{
	double gTCI = 1.0 / (8.112837 * 5.0);

	double ev[2];
	double vMag = sqrt( v[0]*v[0]+v[1]*v[1] );

	if( vMag < 1E-7 )	ev[0] = ev[1] = 0.0;
	else
	{
		ev[0] = v[0] / vMag;		ev[1] = v[1] / vMag;
	}


	double	exinNV = ( exin_sensor[0] - exinNorm[0] ) * gTCI;		exinNorm[0] += stepsize*exinNV;
			exinNV = ( exin_sensor[1] - exinNorm[1] ) * gTCI;		exinNorm[1] += stepsize*exinNV;


	double dI = exin_sensor[0]*ev[0] + exin_sensor[1]*ev[1];

	double Tvel = -1.0 * dI * gTCI;

	TCI += stepsize*Tvel;
	

}


// Currently Wrapped Phase --> -pi < theta < pi
void OSCILLATOR::Calculate_Phase_BVP( void )
{
	prev_phase = phase;


	double PX = v[0];
	double PY = v[1];
	
	phase = atan2( PY , PX );

	if( phase < 0.0 )	phase += 6.283185307179586;		// change to 0<phase<2pi


}


void OSCILLATOR::Calculate_Period_BVP( void )
{	
	double VX = v[0];
		
	if( prev_VX >= 0.0  &&  VX < 0.0 )
	{
		period = net->simtime - prev_time;
		prev_time = net->simtime;
	}

	prev_VX = VX;

}



///////////////////////// Common Member Func ////////////////////////////



OSCILLATOR::OSCILLATOR( int typ , int idx , double tc , OSCNET* on )
{
	isAlive = true;

	baseZ = 0.73;

	Trans[0] = -0.21387241045; //-0.21245
	Trans[1] = 0.72018871485;

	num_sen = 0;

	type = typ;
	index = idx;
	net = on;

	TCI = 1.0/tc;


	switch( type )
	{
		case BVP:			num_node=2;	EquationFuncName = &OSCILLATOR::Equation_Asai_BVP;			break;	// 0
		case BVP2:			num_node=2;	EquationFuncName = &OSCILLATOR::Equation_Asai_BVP2;			break;	// 8
		
		case BVP_NO_ROBOT:	num_node=2;	EquationFuncName = &OSCILLATOR::Equation_Asai_BVP_NoRobot_2;	break;	// 9
		case HOPF:			num_node=2;	EquationFuncName = &OSCILLATOR::Equation_Hopf;				break;	// 10

		case BVP_COUPLED:	num_node=4;	EquationFuncName = &OSCILLATOR::Equation_Coupled_BVP;		break;	// 10

		case ROSSLER:		num_node=3;	EquationFuncName = &OSCILLATOR::Equation_Rossler;			break;	// 3
		case KAUFMAN:		num_node=3;	EquationFuncName = &OSCILLATOR::Equation_Kaufman;			break;	// 6

		case SPROTT_TANH:	num_node=3;	EquationFuncName = &OSCILLATOR::Equation_Sprott_Tanh;		break;	// 2
		case SPROTT_K:		num_node=3;	EquationFuncName = &OSCILLATOR::Equation_Sprott_K;			break;	// 4
		case SPROTT_Q:		num_node=3;	EquationFuncName = &OSCILLATOR::Equation_Sprott_Q;			break;	// 5
		
		

	}

	
	xInit = AllocArray1( num_node );
	
	x = AllocArray1( num_node );	y = AllocArray1( num_node );	v = AllocArray1( num_node );

	for( int i=0 ; i<num_node ; i++ )	net->sim->rk4_for_Init->Add_Var( &x[i], &y[i], &v[i] );



	xAvrg = AllocArray1( num_node );	yA = AllocArray1( num_node );	xAvrg_V = AllocArray1( num_node );

	for( int i=0 ; i<num_node ; i++ )	net->sim->rk4_for_Init->Add_Var( &xAvrg[i], &yA[i], &xAvrg_V[i] );



	exin_sensor		= AllocArray1( num_node );
	exin_con		= AllocArray1( num_node );
	exin_con_disp	= AllocArray1( num_node );
	exin_hand		= AllocArray1( num_node );

	// freq adapt
	exinNorm = AllocArray1( num_node );



}


// Here Incoming Weights are Fully Allocated
void OSCILLATOR::AllocWeightDoya( void )
{
	// Bias
	wBias	= (double*)malloc(sizeof(double) * num_node);
	yWb		= (double*)malloc(sizeof(double) * num_node);	
	wBias_V = (double*)malloc(sizeof(double) * num_node);

	for( int i=0 ; i<num_node ; i++ )	net->sim->rk4_for_Init->Add_Var( &wBias[i], &yWb[i], &wBias_V[i] );
	

	// Weights
	weight	 = (double***)malloc(sizeof(double**) * num_node);
	yW = (double***)malloc(sizeof(double**) * num_node);
	weight_V = (double***)malloc(sizeof(double**) * num_node);


	for( int tt=0 ; tt<num_node ; tt++ )
	{
		weight[tt]	 = (double**)malloc( sizeof(double*) * net->num_osc );
		yW[tt]		 = (double**)malloc( sizeof(double*) * net->num_osc );
		weight_V[tt] = (double**)malloc( sizeof(double*) * net->num_osc );
		
	
		for( int f=0 ; f<net->num_osc ; f++ )
		{
			int num_ff = net->num_ff;

			weight[tt][f]	= (double*)malloc( sizeof(double) * num_ff );
			yW[tt][f]		= (double*)malloc( sizeof(double) * num_ff );
			weight_V[tt][f] = (double*)malloc( sizeof(double) * num_ff );

			for( int ff=0 ; ff<num_ff ; ff++ )	net->sim->rk4_for_Init->Add_Var( &weight[tt][f][ff], &yW[tt][f][ff], &weight_V[tt][f][ff] );
		
		}
	}

	
	// Env Sen Weights
	if( num_sen > 0 )
	{
		weight_sen	 = (double**)malloc( sizeof(double*) * num_node );
		ySW			 = (double**)malloc( sizeof(double*) * num_node );
		weight_sen_V = (double**)malloc( sizeof(double*) * num_node );

		for ( int tt=0 ; tt<num_node ; tt++ )
		{		
			weight_sen[tt]	 = (double*)malloc( sizeof(double) * num_sen );
			ySW[tt]			 = (double*)malloc( sizeof(double) * num_sen );
			weight_sen_V[tt] = (double*)malloc( sizeof(double) * num_sen );

			for( int s=0 ; s<num_sen ; s++ )
				net->sim->rk4_for_Init->Add_Var( &weight_sen[tt][s], &ySW[tt][s], &weight_sen_V[tt][s] );

		}
	}


}




OSCILLATOR::OSCILLATOR()
{
	net = NULL;
}





OSCILLATOR::~OSCILLATOR()
{
	free( v );
	free( x );	free( xInit );
	free( exin_sensor );
	free( exin_con );
	free( exin_con_disp );
	free( exin_hand );

	
	free( exinNorm );


	free(y);	//free(k1);	free(k2);	free(k3);
	free(yA);	//free(ka1);	free(ka2);	free(ka3);
	free(xAvrg);	free(xAvrg_V);

	FreeArray3( weight	 , num_node , net->num_osc );
	FreeArray3( weight_V , num_node , net->num_osc );

	FreeArray3( yW  , num_node , net->num_osc );
//	FreeArray3( kw1 , num_node , net->num_osc );
//	FreeArray3( kw2 , num_node , net->num_osc );
//	FreeArray3( kw3 , num_node , net->num_osc );

	if( num_sen > 0 )
	{
		FreeArray2( weight_sen   , num_node );
		FreeArray2( weight_sen_V , num_node );

		FreeArray2( ySW  , num_node );
	//	FreeArray2( ksw1 , num_node );
	//	FreeArray2( ksw2 , num_node );
	//	FreeArray2( ksw3 , num_node );

	//	free( ySen );
	}


	// Bias
	free( wBias );	free( wBias_V );
	free( yWb ); //free( kwb1 ); free( kwb2 ); free( kwb3 );
	
}




void OSCILLATOR::Zero( void )
{	
	z = 0.0;

	TCI = 1.0/net->tConst;

	for( int i=0 ; i<num_node ; i++ )
	{
		x[i] = 0.0;	// xInit ??
		v[i] = y[i] = 0.0;
		exin_sensor[i] = exinNorm[i] = 0.0;
		exin_con[i] = exin_con_disp[i] = 0.0;
		exin_hand[i] = 0.0;

	
		// Doya Learning
		vLearned[i] = vFeedback[i] = 0.0;
		 
		xAvrg[i] = xAvrg_V[i] = 0.0;		
	}


	for( int tt=0 ; tt<num_node ; tt++ )
	{
		wBias[tt] = 0.0;	wBias_V[tt] = yWb[tt] = 0.0;//kwb1[tt] = kwb2[tt] = kwb3[tt] = 0.0;

		for( int f=0 ; f<net->num_osc ; f++ )
		{
			for( int ff=0 ; ff<net->num_ff ; ff++ )
			{
				weight[tt][f][ff] = 0.0;
				weight_V[tt][f][ff] = yW[tt][f][ff] = 0.0;//kw1[tt][f][ff] = kw2[tt][f][ff] = kw3[tt][f][ff] = 0.0;
			}
		}

		for( int s=0 ; s<num_sen ; s++ )
		{
		//	ySen[s] = 0.0;

			weight_sen[tt][s] = 0.0;
			weight_sen_V[tt][s] = ySW[tt][s] = 0.0;//ksw1[tt][s] = ksw2[tt][s] = ksw3[tt][s] = 0.0;
		}

	}


	// Centre
	Center[0] = Center[1] = 0.0;
	CenterNumeric[0] = CenterNumeric[1] = 0.0;


	// phase
	kpi = phase = prev_phase = prev_Y = 0.0;

	// period
	prev_time = prev_VX = period = 0.0;


	
	// sensor learning
	senDesired = senLearned = senAvrg = 0.0;
	

	(*doya[0].wRate) = (*doya[0].bRate) = (*doya[0].fbGain) = 0.0;

}

