


// OSC-Alpha RK4 Routine for CE Robots, No_Robots, and SpNets !!!

#include "oscil_random_net.h"




void OSCNET::dFunction_RK4( void )
{

	// SAM included !!
	// 0.013 & 0.022 weight included !!
	for( int j=0 ; j<num_Ia_sensor ; j++ )	IaSen[j]->FunctionV_RK4( );


	
	
	// Osc's and Doya Learning
	LearnOSC_Doya_Node_RK4( );	// If NoRobot, this uses mu_Y from above.

	for( int n=0 ; n<num_osc ; n++ )	osc[n]->FunctionV_RK4( );

	


	// Alpha neurons & Afferent Weights
	LearnOSC_Doya_Alpha_RK4( );
		
	// If RK4 alpha update
	if( is_Alpha_Integrate )
	for( int i=0 ; i<num_alpha_neuron ; i++ )	alpha[i]->FunctionV_RK4( );


}







void OSCNET::Aux_Fill_Exin_Con_Disp( void )
{
	for( int j=0 ; j<num_Ia_sensor	  ; j++ )	IaSen[j]->Set_Disp_Var();


	for( int t=0 ; t<num_osc ; t++ )
	{
		for( int tt=0 ; tt<osc[t]->num_node ; tt++ )
		{
			osc[t]->vFeedback[tt] = 0.0;
			for( int f=0 ; f<num_osc ; f++ )
			{
				if( t != f )
				{
					for( int ff=0 ; ff<1 ; ff++ )
					{
						osc[t]->vFeedback[tt] += osc[t]->weight[tt][f][ff] * (osc[f]->x[ff]);
					}
				}
			}
			
			osc[t]->exin_con_disp[tt] = doyaStr[tt] * (1.0-transSigDoya) * osc[t]->vFeedback[tt];
		}
	}

}

