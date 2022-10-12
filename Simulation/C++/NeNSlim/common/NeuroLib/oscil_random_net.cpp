

#include <malloc.h>
#include <math.h>

#include "oscil_random_net.h"
#include "..\NeuroUse\neurouse.h"




OSCNET::OSCNET( SIMULATION* sss )
{
	sim = sss;

	


	isDoyaLearn = isDoyaAlphaLearn = isSensorCut = false;


	num_node = num_con = num_osc = 0;

	osc		= 0x00000000l;
	
	StepSize	= 0.0;
	tConst		= 0.0;


	for( int i=0 ; i<(int)NDOF ; i++ )	IDX[i][0] = IDX[i][1] = 0;
	

	learnT = 1.0;

	doyaStr[0] = doyaStr[1] = sqrt( (0.013*0.013+0.022*0.022) * 0.5 );	// RMS

	// 2017 Jan. for Doya Learning with Bias
	num_tt = 2;
	num_ff = 1;


	// alpha neurons
	num_alpha_neuron = num_Ia_sensor = num_proprioceptor = 0;

	is_Alpha_Reset = true;

	is_Alpha_Integrate = false;

	fb_Str_Alpha = 0.2;


	alpha = 0x00000000l;
	IaSen = 0x00000000l;

	SetController();

	simtime = 0.0;

}


void OSCNET::ManualNet( void )
{ 	

	ManualNet_CE_Common( );


	sim->rk4_for_Init->h = StepSize;

	for( int i=0 ; i<num_osc ; i++ )	osc[i]->AllocWeightDoya();



	sim->rk4_for_Init->Add_Var( &transSigDoya   , &transSigDoya_y   , &transSigDoya_v   );
	sim->rk4_for_Init->Add_Var( &transSigAlpha  , &transSigAlpha_y  , &transSigAlpha_v  );
	sim->rk4_for_Init->Add_Var( &transSigSensor , &transSigSensor_y , &transSigSensor_v );


	// 2021-07-28 -- Must reset TCI due to random oscT when EVAL_DOYA
	Init_Doya_Learn_Wrapper_OSC( );


	Zero( );
	RandY( );

	
}


void OSCNET::Zero( void )
{
	

	for( int n=0 ; n<num_osc ; n++ )	osc[n]->Zero();


	for( int i=0 ; i<(int)NSEN ; i++ )		senDisp[i] = 0.0;

	


	for( int i=0 ; i<50 ; i++ )				pdv[i] = phaseDiff[i] = prevPhaseDiff[i] = 0.0;


	for( int n=0 ; n<num_osc ; n++ )
	{
		for( int k=0 ; k<num_osc ; k++ )
		{
			weight[n][k] = weight2[n][k] = 0.0;
		}
	}

	zRate = 0.0;
	simtime = 0.0;

	transSigDoya = transSigSensor = transSigAlpha = 1.0;
	transSigDoya_y = transSigSensor_y = transSigAlpha_y = 0.0;
	transSigDoya_v = transSigSensor_v = transSigAlpha_v = 0.0;

	transNoRobot_y = transNoRobot_disp = 0.0;


	// alpha neurons
	for( int i=0 ; i<num_alpha_neuron ; i++ )	alpha[i]->Zero();

	for( int j=0 ; j<num_Ia_sensor ; j++ )		IaSen[j]->Zero();



	pDiff_Osc->Zero();
	
	if( pDiff_Alpha )	pDiff_Alpha->Zero();


	
}


OSCNET::~OSCNET()
{
	for( int n=0 ; n<num_osc ; n++ )	delete osc[n];
	free( osc );

	
	// alpha neurons
	if( num_alpha_neuron )
	{
		for( int i=0 ; i<num_alpha_neuron ; i++ )
			delete alpha[i];

		free( alpha );
	}

	// Ia Sensors
	if( num_Ia_sensor )
	{
		for( int j=0 ; j<num_Ia_sensor ; j++ )	delete IaSen[j];
		free( IaSen );
	}


	delete pDiff_Osc;
	if( pDiff_Alpha )	delete pDiff_Alpha;

}


void OSCNET::AddNode( int type , double tc )
{
	if( num_osc > 0 )	osc = (OSCILLATOR**)realloc( osc , sizeof(OSCILLATOR*) * (num_osc+1) );
	else				osc = (OSCILLATOR**)malloc( sizeof(OSCILLATOR*) );
	
	osc[num_osc] = new OSCILLATOR( type , num_osc , tc , this );

	for( int i=0 ; i<osc[num_osc]->num_node ; i++ )
	{
		IDX[num_node][0] = num_osc;
		IDX[num_node][1] = i;
		num_node++;
	}

	num_osc++;

}




void OSCNET::DeleteNode( int d )
{
	int numDof = osc[d]->num_node;
	
	delete osc[d];

	for( int n=d ; n<num_osc-1 ; n++ )
	{
		osc[n] = osc[n+1];
		osc[n]->index = n;
	}

	osc = (OSCILLATOR**)realloc( osc , sizeof(OSCILLATOR*) * (num_osc-1) );
	num_osc--;
	num_node -= numDof;
}


void OSCNET::DeleteNode( OSCILLATOR* del )
{
	int d = del->index;
	delete del;

	for( int n=d ; n<num_osc-1 ; n++ )
	{
		osc[n] = osc[n+1];
		osc[n]->index = n;
	}

	osc = (OSCILLATOR**)realloc( osc , sizeof(OSCILLATOR*) * (num_osc-1) );
	num_osc--;
}





int randbtn( int , int );

void OSCNET::RandY( void )
{	

			for( int n=0 ; n<num_node ; n++ )
			{	
				if( n%2 < 1 )	initPI[n] = randbtn( -2000 , 2000 );
				else			initPI[n] = randbtn( -2000 , 2000 );
			}




	for( int n=0 ; n<num_node ; n++ )
	{
		osc[IDX[n][0]]->xInit[IDX[n][1]] = (double)initPI[n]/1000.0;
	}


	



	for( int n=0 ; n<num_node ; n++ )
	{
		osc[IDX[n][0]]->x[IDX[n][1]] = osc[IDX[n][0]]->xInit[IDX[n][1]];
	}



}



static double PI = 3.1415926535897932384626433832795;

void PDIFF::Calc_Phase_Diffs( void )
{

	is_pCoord_Has_Revolute = false;

	for( int i=0 ; i<numOsc ; i++ )
	{	// Damage !! fix !!

		has_Revolute[i] = false;	

		if( prev_Vel[i] >= 0.0  && *v[i] < 0.0 )
		{
			prev_revTime[i] = revTime[i];

			revTime[i] = *sim_time;

			for( int c=1 ; c<numOsc ; c++ )	revIdx[c-1] = revIdx[c];

			revIdx[numOsc-1] = i;


			has_Revolute[i] = true;

			is_pCoord_Has_Revolute = true;

		}

		prev_Vel[i] = *v[i];
	}

	

	is_pCoord_update = false;

	if( is_pCoord_Has_Revolute )
	{

		// check all indices are in the cue
		int chkIdx[99];
		for( int c=0 ; c<numOsc ; c++ )	chkIdx[c] = 0;

		for( int c=0 ; c<numOsc ; c++ )
		{
			chkIdx[revIdx[c]] = 1;
		}

		int sumChk = 0;
		for( int c=0 ; c<numOsc ; c++ )
		{
			sumChk += chkIdx[c];
		}

	
		if( sumChk == numOsc )
		{
			is_pCoord_update = true;

			for( int i=0 ; i<numOsc ; i++ )
			{
				revIdxSend[i] = revIdx[i];
			}

			measureTime = revTime[revIdx[0]];

		
			phaseNew[revIdx[0]] = 0.0;	// exact value??

			for( int c=1 ; c<numOsc ; c++ )
			{
				int d = revIdx[c];

				double itvl1 = revTime[d] - prev_revTime[d];
				double itvl2 = measureTime - prev_revTime[d];
			
				phaseNew[d] = itvl2*2.0*PI / itvl1;
			}


			for( int i=1 ; i<numOsc ; i++ )
			{			
				pCoord[i-1] = phaseNew[i] - phaseNew[0];

				if( pCoord[i-1] < 0.0 )	pCoord[i-1] += 2.0*PI;
			}


		}
	}


	glPastTimeAmount = *sim_time - measureTime;	// for GL draw

}



PDIFF::PDIFF(){}

PDIFF::PDIFF( OSCNET* net , int vKind )
{
	sourceKind = vKind;

	switch( vKind )
	{
		case 0:

			numOsc = net->num_osc;
			
			v = (double**)malloc( sizeof(double*) * numOsc );

			for( int i=0 ; i<numOsc ; i++ )
			{
				v[i] = &net->osc[i]->v[0];
			}
		break;


		case 1:

			numOsc = net->num_alpha_neuron;
			
			v = (double**)malloc( sizeof(double*) * numOsc );

			for( int i=0 ; i<numOsc ; i++ )
			{
				v[i] = &net->alpha[i]->v;
			}
		break;

	}


	sim_time = &net->simtime;


	has_Revolute = (bool*  )malloc( sizeof(bool  ) * numOsc );
	prev_Vel	 = (double*)malloc( sizeof(double) * numOsc );
	phaseNew	 = (double*)malloc( sizeof(double) * numOsc );
	revTime		 = (double*)malloc( sizeof(double) * numOsc );
	prev_revTime = (double*)malloc( sizeof(double) * numOsc );


	// Outside Vars
	pCoord		= (double*)malloc( sizeof(double) * (numOsc-1) );
	pCoord_Anal	= (double*)malloc( sizeof(double) * (numOsc-1) );

	revIdx		= (int*)malloc( sizeof(int) * numOsc );
	revIdxSend	= (int*)malloc( sizeof(int) * numOsc );

}

PDIFF::~PDIFF()
{
	free( v );

	free( has_Revolute );
	free( prev_Vel );
	free( phaseNew );
	free( revTime );
	free( prev_revTime );

	free( pCoord );		free( pCoord_Anal );
	free( revIdx );		free( revIdxSend );
}

void PDIFF::Zero( void )
{
	for( int i=0 ; i<numOsc ; i++ )
	{
		has_Revolute[i] = false;
		prev_Vel[i] = prev_revTime[i] = phaseNew[i] = 0.0;
		revTime[i] =  0.001;
	}

	// phase diff coord
	for( int i=0 ; i<numOsc-1 ; i++ )
	{
		pCoord[i] = pCoord_Anal[i] = 0.0;
	}

	is_pCoord_Has_Revolute = false;
	is_pCoord_update = false;
	

	measureTime = glPastTimeAmount = 0.0;

	for( int i=0 ; i<numOsc ; i++ )
	{
		revIdx[i] = revIdxSend[i] = i;	// just fill with different osc indices
	}
}










