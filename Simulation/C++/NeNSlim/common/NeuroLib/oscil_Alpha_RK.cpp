

#include <malloc.h>

#include "oscil_random_net.h"



// Alpha Motor Neurons //////////////////////////////////////////////////////////////////////////

static double TC_INV  = 0.0;
static double W_RATE  = 0.0;
static double B_RATE  = 0.0;
static double FB_GAIN = 0.0;

static double g_Bias_One = 1.0;


void OSCNET::Toggle_fb_Str_Alpha( void )
{
	double STR = 0.3;

	if( isDoyaAlphaLearn )	fb_Str_Alpha = STR;
	else
	{
		fb_Str_Alpha = 0.0;
		if( !is_Alpha_Reset )	fb_Str_Alpha = STR;
	}
}


// only half of them (real alpha's) are called
void ALPHANEURON::Progress_Alpha_Direct( void )
{
		
//	if( net->num_inter_neuron > 0 )		pAlpha->Progress_Interneuron_Direct( );


	double aStr = net->fb_Str_Alpha;

	double intrinsic_Val;
	if( pAlpha )	intrinsic_Val = pAlpha->x;
	else			intrinsic_Val = pOsc->x[0];

	
	intrinsic_Val = 0.0;	// 2019-09-04
	aStr = 1.0;

	
		double pFeedback = 0.0;
	for( int j=0 ; j<net->num_Ia_sensor ; j++ )
	{
		// should be fixed !! -- 2021-07-29
	//	pFeedback += weight[j] * ( *inpSen[j] );
	}
	
	// for display
	exin = pFeedback;

	double alphaRate = 1.0;//-net->zRate;

//	x = intrinsic_Val  +  aStr * (pFeedback- alphaRate*x);		// This will need alpha->x as a variable for LLC calculation !!
	x = intrinsic_Val  +  aStr * (pFeedback);

}


void ALPHANEURON::FunctionV_RK4( void )
{
	// THIS IS OLD - Apr 2017
	//	double alphaT = StepSize;
	//	double alphaT = learnT * 0.1;
	// stepsize 0.0025 X 10.0, if springNet: 0.01

	if( isAlive )
	{

	//	double alphaT = net->learnT * 0.008;//0.0016;	// == oscT/(5*5*5)
		double alphaT = net->StepSize;	// 2019-09-20

		// If alpha RK4 update
		
		double aStr = net->fb_Str_Alpha;

		double intrinsic_Val = pOsc->y[0];

		
		// aStr*(dE/dx)
	//	v = ( intrinsic_Val - y  +  aStr*(exin-y) ) / alphaT;
		v = ( intrinsic_Val - y  +  aStr*exin ) / alphaT;

	

	}
	else
	{
		v = y = 0.0;
	}

}


void OSCNET::LearnOSC_Doya_Alpha_RK4( void )
{
	W_RATE = B_RATE = 1.0 - transSigAlpha;

	for( int a=0 ; a<num_alpha_neuron ; a++ )	alpha[a]->doya.Learn( );
}



void ALPHANEURON::Init_Doya_Learn_Wrapper_Alpha( void )
{
	TC_INV = 1.0 / net->learnT;
	FB_GAIN = 1.0;

	doya.TCI    = &TC_INV;
	doya.wRate  = &W_RATE;
	doya.bRate  = &B_RATE;
	doya.fbGain = &FB_GAIN;


	if( net->is_Alpha_Integrate )	doya.x = &y;
	else							doya.x = &x;
	
	doya.exin	= &exin;

	int s = 0;


		for( int a=0 ; a<net->num_alpha_neuron ; a++ )
		{
			
				doya.Add_Weight_Ref( &yW[s], &weight_V[s], &net->IaSen[a]->IaL_y );		s++;
				doya.Add_Weight_Ref( &yW[s], &weight_V[s], &net->IaSen[a]->IaV_y );		s++;
				doya.Add_Weight_Ref( &yW[s], &weight_V[s], &net->IaSen[a]->IbF_y );		s++;

			
			// from none-homonymous OSC
			if( net->sim->nrp.IS_OSC_FED_TO_ALPHA  &&  (index != a) )
			{
				doya.Add_Weight_Ref( &yW[s], &weight_V[s], &net->osc[a]->y[0] );
				s++;
			}
			
		}


		// bias
		doya.Add_Weight_Ref( &yW[s], &weight_V[s], &g_Bias_One );		s++;
	

		int ghgh = 0;

	

}



//////// Inits and Zeros ///////////////////////////////////

ALPHANEURON::ALPHANEURON()
{

}

ALPHANEURON::ALPHANEURON( OSCNET* on , int idx , int num_afferent )
{
	index = idx;

	isAlive = true;

	net = on;


	net->sim->rk4_for_Init->Add_Var( &x, &y, &v );


	num_weight = num_afferent;

	weight   = (double*)malloc( sizeof(double) * num_weight );
	yW		 = (double*)malloc( sizeof(double) * num_weight );
	weight_V = (double*)malloc( sizeof(double) * num_weight );
	
	for( int i=0 ; i<num_weight ; i++ )	net->sim->rk4_for_Init->Add_Var( &weight[i], &yW[i], &weight_V[i] );


	Init_Doya_Learn_Wrapper_Alpha( );


}






// Do Damage -- Cutting off a leg should also cut off nerves
void ALPHANEURON::Damage( int damIdx )
{

	if( index == damIdx )
	{
		for( int j=0 ; j<num_weight ; j++ )
		{
			doya.inp[j] = 0x00000000l;
		}

	}
	else
	{	
	
		for( int i=0 ; i<num_weight ; i++ )
		{
			if( doya.inp[i] == &net->osc[damIdx]->y[0] )	doya.inp[i] = 0x00000000l;

			if( doya.inp[i] == &net->IaSen[damIdx]->IaL_y )	doya.inp[i] = 0x00000000l;
			if( doya.inp[i] == &net->IaSen[damIdx]->IaV_y )	doya.inp[i] = 0x00000000l;
			if( doya.inp[i] == &net->IaSen[damIdx]->IbF_y )	doya.inp[i] = 0x00000000l;
		}

	}

}


ALPHANEURON::~ALPHANEURON()
{
	free( weight );	free( weight_V );

	free( yW );
}

void ALPHANEURON::Zero( void )
{
	x = v = y = exin = 0.0;

	for( int j=0 ; j<num_weight ; j++ )
	{
		weight_V[j] = yW[j] = 0.0;
	}

	if( net->is_Alpha_Reset )
	{
		for( int j=0 ; j<num_weight ; j++ )	weight[j] = 0.0;
	}

	W_RATE = B_RATE = 0.0;
}





///////// Ia Sensor SAM module !! 2019-09-20 /////////////////////////////////////////////////////////////


#include <math.h>



static double NEURO_AVRG = 0.0;
static double NEURO_POW_LOG  = 0.7737;

void IASENSOR::FunctionV_RK4( void )	// located in OSCNET::dFunction_RK4()
{
	
	double sensor;


	
		double l_softPlus = log( 1.0 + exp(y_sensorScale) );
		double l_sensorVal = (rawSensor_y-y_rawAvrg) * l_softPlus	+ y_rawAvrg + y_sensorOffset;

		sensor = net->transSigSensor_y * l_sensorVal;

	
		double TCI = 1.0 / net->learnT;// * 0.2;

		if( isAlive )	// If not damaged...
		{
			double lRate = 1.0;

			sigAvrgV  = ( -y_sigAvrg  + l_sensorVal ) * TCI;
			rawAvrgV  = ( -y_rawAvrg  + rawSensor_y ) * TCI;
		
			sigPowV  = ( -y_sigPow   + log( (l_sensorVal-y_sigAvrg)*(l_sensorVal-y_sigAvrg) + 1.0 ) ) * TCI;
		
			sensorOffsetV  = lRate * ( NEURO_AVRG	 - y_sigAvrg ) * TCI;
			sensorScaleV   = lRate * ( NEURO_POW_LOG - y_sigPow  ) * TCI;
		}
		else // if this limb is broken
		{
			sigAvrgV = rawAvrgV = sigPowV = sensorOffsetV = sensorScaleV = 0.0;
			
			y_sigAvrg = y_rawAvrg = y_sigPow = y_sensorOffset = y_sensorScale = 0.0;

			net->osc[index]->exin_sensor[0] = 0.0;
			net->osc[index]->exin_sensor[1] = 0.0;
		}

	

	// Send to OSC
	net->osc[index]->exin_sensor[0] = sensor * 0.013;
	net->osc[index]->exin_sensor[1] = sensor * 0.022;

}



void IASENSOR::Set_Disp_Var( void )
{
	
	rawSensor_Disp = 1.0*IaL_y + 0.5*IaV_y - 1.0*IbF_y;

	
		double softPlus = log( 1.0 + exp(sensorScale) );
		sensorVal_Disp = (rawSensor_y-rawAvrg) * softPlus	+ rawAvrg + sensorOffset;
	
	net->senDisp[index] = sensorVal_Disp;
}







IASENSOR::IASENSOR( OSCNET* on )
{
	net = on;
	isAlive = true;

	sensorScaleInit = sensorOffsetInit = 0.0;

	
		net->sim->rk4_for_Init->Add_Var( &sigAvrg , &y_sigAvrg , &sigAvrgV );
		net->sim->rk4_for_Init->Add_Var( &sigPow , &y_sigPow , &sigPowV );
		net->sim->rk4_for_Init->Add_Var( &rawAvrg , &y_rawAvrg , &rawAvrgV );
		net->sim->rk4_for_Init->Add_Var( &sensorOffset , &y_sensorOffset , &sensorOffsetV );
		net->sim->rk4_for_Init->Add_Var( &sensorScale , &y_sensorScale , &sensorScaleV );
	
	

}

IASENSOR::~IASENSOR()
{
	
}

void IASENSOR::Zero( void )
{
	IaL_y = IaV_y = IbF_y = 0.0;	// being set by robotic system

	rawSensor_y = 0.0;
	rawSensor_Disp = sensorVal_Disp = 0.0;

	

	
		sensorScale = sensorScaleInit;
		sensorOffset = sensorOffsetInit;

		sigAvrg = sigPow = rawAvrg = 0.0;
	

}


