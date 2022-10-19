/*************************************************************************
 *		Dynamic Simulator module using Russell Smith's ODE               *
 *		written by Yoonsik Shim											 *
 *		2002.12.30														 *
 *		This is the main simulation loop function														 *
 *************************************************************************/



#include "RK4.h"

#include "../NeuroUse/neurouse.h"
#include "../dynamics/rigidode.h"






void Loop_ODE_Robot_Before( int T );
void Loop_ODE_Robot_After( int T );

// Only for OSC --> Integrate other net-kinds !! -- 2021.05.15
bool Loop_All( int T )
{
	RK4*		rk4		= g_sim[T]->rk4;
	OSCNET*		oscnet	= g_sim[T]->oscil;
	
	RIGIDSTATE* rs		= g_sim[T]->rs;
	EVAL*		eval	= g_sim[T]->eval;

	
	bool is_ODE_Robot = false;
	
	if( rs )
	{
		if( rs->is_ODE_Robot )	is_ODE_Robot = true;		
	}	



	if( is_ODE_Robot )
	{
		rs->Calc_CG( );

		Loop_ODE_Robot_Before(T);
	}
	


	// CALCULATE ALL DERIVATIVES
	rk4->RK4_STEP(0);
		
		for( int p=1 ; p<5 ; p++ )
		{
			// calc derivatives for Eval
			if( eval )	eval->Evaluate_CE( );
			
			// calc derivatives for OSC
			if( oscnet )
			{				
				oscnet->SetTransSigs( );
				oscnet->dFunction_RK4( );
			}	

			rk4->RK4_STEP( p );
		}
	
	// PROGRESS TIMESTEP !!  X <-- X + dt*dX
	rk4->Progress( );

	if( is_ODE_Robot )	Loop_ODE_Robot_After(T);



	///////////////////////////////////////////////////////////////////////////////////////////////////

	if( oscnet )
	{
		// right after calculating the final V(t)'s --> now we've got: X(t) & V(t)
		oscnet->pDiff_Osc->Calc_Phase_Diffs( );
	
		// Realtime calculation of approx. phase differences
		if( oscnet->pDiff_Alpha ) oscnet->pDiff_Alpha->Calc_Phase_Diffs( );


		// just for displaying osc's incoming weighted input - exin_con_disp[] using X(t)'s
		// IaSen[]:: rawSen & others... composite x-variables
		oscnet->Aux_Fill_Exin_Con_Disp( );

	
		// Calc osc singular point (unstable fp of limitcycle), phase, etc. with X(t+1)'s
		for( int n=0 ; n<oscnet->num_osc ; n++ )	oscnet->osc[n]->AuxCalc( );


		oscnet->simtime += rk4->h;
	}



	// Aux ODE robot simul
	if( rs )
	{		
		rs->robotStep += rk4->h;
		
		
		if( is_ODE_Robot )	rs->SetTransform_ODE();		// blowup checked !!
		
		if( rs->isBlow )	return true;

				
		// CoM & CoM vel for both spnets & ODE_robots
		if( rs->is_use_CG )	rs->Calc_CG( );
		

		// DX Cam for 3D rendering
		if( rs->is_use_CG )	rs->Calc_avrCG_for_DX_Cam( );	

	}


	return false;


}








