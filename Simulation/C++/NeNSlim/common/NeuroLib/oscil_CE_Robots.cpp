
// First written in 13/02/2008
// Modified in 28/05/2014

#include "oscil_random_net.h"



NRPARAM::NRPARAM()
{
	DoF = 0;

	IS_ALPHA_RK4 = false;

	STEP_SIZE = 0.001;

	OSC_KIND = BVP;
	TIME_CONST = 1.0;

	
}


void NRPARAM::Select_NRPARAM( int num_dof )
{
	DoF = num_dof;	// number of joints --> two oscillators per DoF (antagonistic control)


	IS_ALPHA_RK4 = false;

	IS_OSC_FED_TO_ALPHA = false;

	STEP_SIZE = 0.001;
	OSC_KIND = BVP;
	TIME_CONST = 1.0;

	int system_Kind = 1;
	switch( system_Kind )
	{
		case 0:		break;	// Fin Swimmers

		case 1: // Walkers
			IS_ALPHA_RK4 = true;				
			STEP_SIZE = 0.0025;		TIME_CONST = 0.4;
		break;

		case 2:		break;	// SpringNets
		case 3:		break;	// NoRobots
		case 4:		break;	// Voxels
			
	}


}





///////////////////////////////// END of Customs ////////////////////////////////////////////////////////////////



#include <malloc.h>


void OSCNET::ManualNet_CE_Common( void )
{
	NRPARAM np = sim->nrp;
	
	StepSize = np.STEP_SIZE;
	
	tConst = np.TIME_CONST; // currently 0.4, 0.8 ,1.0 exist

	is_Alpha_Integrate = np.IS_ALPHA_RK4;


	int l_dof_Mult;
	
	int l_num_input_per_alpha;
	
	

			for( int i=0 ; i<np.DoF ; i++ ){	sim->effIdx1[i][0] = i*2;	sim->effIdx2[i][0] = i*2+1;		}

			l_dof_Mult = 2;			// 2 bvp per dof
			num_proprioceptor = 3;	// IaL, IaV, IbF -- 2022-08-01


		l_num_input_per_alpha = num_proprioceptor;

		// adaptive weight from none-honomymous osc-x
		if( np.IS_OSC_FED_TO_ALPHA )	l_num_input_per_alpha++;


	


	int l_NUM_OSC = np.DoF * l_dof_Mult;
		
	for( int i=0 ; i<l_NUM_OSC ; i++ )	AddNode( np.OSC_KIND , tConst );
	


	// alpha & IN neurons
	num_alpha_neuron = num_Ia_sensor = num_osc;


	if( num_Ia_sensor )
	{
		IaSen = (IASENSOR**)malloc( sizeof(IASENSOR*) * num_Ia_sensor );
		for( int j=0 ; j<num_Ia_sensor ; j++ )
		{
			IaSen[j] = new IASENSOR( this );
			IaSen[j]->index = j;
		}
	}


	if( num_alpha_neuron )
	{
		alpha = (ALPHANEURON**)malloc( sizeof(ALPHANEURON*) * num_alpha_neuron );

		
		int num_weight_per_alpha = l_num_input_per_alpha * num_alpha_neuron + 1;		// plus bias

		if( np.IS_OSC_FED_TO_ALPHA )	num_weight_per_alpha--;		// none from homonymous osc


		for( int i=0 ; i<num_alpha_neuron ; i++ )
		{
			alpha[i] = new ALPHANEURON( this , i, num_weight_per_alpha );

			alpha[i]->pOsc = osc[i];
			alpha[i]->pAlpha = 0x00000000l;
		}
	}

	
	// connectivity between osc's -- if 1, it means the connections only from excitatory neurons
	num_ff = 1;


	// Common CE Params
	baseZ = 0.73;

	if		( tConst < 0.2 )	oscT = 0.8112837;
	else if	( tConst < 0.5 )	oscT = 3.2451;		// T = 0.4
	else if	( tConst < 0.9 )	oscT = 6.49;		// T = 0.8
	else						oscT = 8.112837;	// T = 1.0

	learnT = oscT * 1.0;
	transT = oscT * 0.2;


	// pDiff object
	pDiff_Osc	= new PDIFF( this , 0 );
	pDiff_Alpha = new PDIFF( this , 1 );
}

