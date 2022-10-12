
/*************************************************************************
 *		This file is for Doya learning & other adaptation ODEs           *
 *		Moved here in       											 *
 *		2021.02.03														 *
 *************************************************************************/


#include <malloc.h>
#include <math.h>

#include "oscil_random_net.h"


static double TC_INV  = 0.0;
static double W_RATE  = 0.0;
static double B_RATE  = 0.0;
static double FB_GAIN = 0.0;

static double g_Bias_One = 1.0;


void OSCNET::Init_Doya_Learn_Wrapper_OSC( void )
{

	TC_INV = 1.0 / (learnT * 1.0);


	
	for( int t=0 ; t<num_osc ; t++ )
	{
		

		for( int tt=0 ; tt<num_tt ; tt++ )
		{
			osc[t]->doya[tt].TCI    = &TC_INV;
			osc[t]->doya[tt].wRate  = &W_RATE;
			osc[t]->doya[tt].bRate  = &B_RATE;
			osc[t]->doya[tt].fbGain = &FB_GAIN;


			// target var & feedback container
			osc[t]->doya[tt].x		= &osc[t]->y[tt];
			osc[t]->doya[tt].exin	= &osc[t]->exin_con[tt];

			// presynaptic osc
			for( int f=0 ; f<num_osc ; f++ )
			{
				if( t != f )
				{					
					for( int ff=0 ; ff<num_ff ; ff++ )
					{
						osc[t]->doya[tt].Add_Weight_Ref( &osc[t]->yW[tt][f][ff],
														 &osc[t]->weight_V[tt][f][ff], 
														 &osc[f]->y[ff]					);
					}
				}
			}

			// bias weight
			osc[t]->doya[tt].Add_Weight_Ref( &osc[t]->yWb[tt],
											 &osc[t]->wBias_V[tt], 
											 &g_Bias_One			);


		}

	//	int hjhj = 0;

	}



}


// This is only for fully connected osc net (July 11th 2017) 
// Doya P Matrix Learning w/ bias
void OSCNET::LearnOSC_Doya_Node_RK4( void )
{

	
		W_RATE  = transSigDoya_y;
		B_RATE  = 1.0;
		FB_GAIN = 1.0 - transSigDoya_y;
	
	
	FB_GAIN *= doyaStr[0];	// this should be fixed: into one


	for( int t=0 ; t<num_osc ; t++ )
	{
		for( int tt=0 ; tt<num_tt ; tt++ )
		{
			// get average
			osc[t]->xAvrg_V[tt] = ( -osc[t]->yA[tt] + osc[t]->y[tt] ) * TC_INV;

			osc[t]->doya[tt].Learn( );
		}
	}

	
}




// For ODE Robot Learning Switches


void OSCNET::SetTransSigs( void )
{
	
	

		double epsil = sim->eval->zEpsilon;

		// Doya learning
		if( isDoyaLearn )
		{
			double targ_Doya = 1.0;		if( zRate < epsil )	targ_Doya = 0.0;
				
			transSigDoya_v = (targ_Doya - transSigDoya_y) / transT;
		}
		else
		{
			transSigDoya = transSigDoya_y = 1.0;
			transSigDoya_v = 0.0;
		}

		
		// alpha learning
		if( isDoyaAlphaLearn )
		{
			double targ_Alpha = 1.0;	if( zRate < epsil )	targ_Alpha = 0.0;

			transSigAlpha_v = (targ_Alpha - transSigAlpha_y) / transT;
		}
		else
		{
			transSigAlpha = transSigAlpha_y = 1.0;
			transSigAlpha_v = 0.0;
		}



		

		if( sim->rs->is_ODE_Robot )	///////////// ODE Robots ////////////////////////////////////////
		{

			if( isSensorCut )
			{
				double targ_Sensor = 1.0;			
				if( zRate < epsil )	targ_Sensor = 0.0;

				transSigSensor_v = (targ_Sensor - transSigSensor_y) / transT;

			}
			else
			{
				transSigSensor = transSigSensor_y = 1.0;
				transSigSensor_v = 0.0;
			}

		}
		

}



