


#include "rigidode.h"
#include "../NeuroUse/neurouse.h"





void RIGIDSTATE::Set_CE_Params_Global( void )
{
	LOG( 0 , "[BGN][END] RIGIDSTATE::Set_CE_Params_Global(void)" );

	is_use_CG = true;

	STEPSIZE = sim->oscil->StepSize;

	oscTr = sim->oscil->oscT;
	
}


// Common for Walkers and Swimmers
// Muscle model is an angular version of -- Ekeberg (1993) A combined neuronal and mechanical model of fish swimming
void RIGIDSTATE::Set_Muscle_and_Inertia( double Fmax )
{
	LOG( +1 , "[BGN] RIGIDSTATE::Set_Muscle_and_Inertia(%.1f)", Fmax );

	// effector real torque
	for( int i=0 ; i<NUM_ODE_BODY-1 ; i++ )
	{
		for( int d=0 ; d<motor[i].dof ; d++ )
		{
			motor[i].alpha[d]	= Fmax;
			motor[i].beta[d]	= 0.1*Fmax / motor[i].Hstop[d];
			motor[i].gamma[d]	=  20.0;
			motor[i].delta[d]	=   2.0 * sqrt( motor[i].beta[d] * (motor[i].gamma[d]+0.0) );
		}
	}

	
	/**/	LOG( 0 , "SetEffectorFuncName = &RIGIDSTATE::Set_Effectors_Two_Torque" );
	SetEffectorFuncName = &RIGIDSTATE::Set_Effectors_Two_Torque;


	// Get moment of inertia w/r/t joint
	for( int i=1 ; i<NUM_ODE_BODY ; i++ )
	{
		dMass m;
		dBodyGetMass( body[i] , &m );
		node[i].inertia = ( m.I[0] + m.mass*node[i].L*node[i].L*0.25 ) * (double)MASS_SCALE;	// I[0], I[5], I[10]
	}


	LOG( -1 , "[END] RIGIDSTATE::Set_Muscle_and_Inertia(%.1f)", Fmax );

}



void RIGIDSTATE::SetSensor( void )
{
	if( oscTr > 0.0 )	// if OscNet
	{
		for( int i=0 ; i<NUM_ODE_BODY-1 ; i++ )	motor[i].Calculate_Reflex( );
	}

	(this->*SetSensorFuncName)();
}


void RIGIDSTATE::Progress_Reflex_ODE_Robot( void )
{
	if( oscTr > 0.0 )	// if OscNet
	{
		for( int i=0 ; i<NUM_ODE_BODY-1 ; i++ )	motor[i].Progress_ODE_Robot_Reflex( );
	}
}




void RIGIDSTATE::SetBody_CE_Walkers_Common( double Fmax , bool beCb )
{
	LOG( +1 , "[BGN] RIGIDSTATE::SetBody_CE_Walkers_Common(%.1f,%s)" , Fmax, beCb ? "isCoulomb=true" : "isCoulomb=false" );

	is_ODE_Robot = true;

	Set_Muscle_and_Inertia( Fmax );


	// Ground Friction for Walkers
	isCoulomb = beCb;



	// CE-Robots Common Paramameters ////////////////////////////

	Set_CE_Params_Global( );


	LOG( -1 , "[END] RIGIDSTATE::SetBody_CE_Walkers_Common(%.1f,%s)" , Fmax, beCb ? "true" : "false" );
	
}








void RIGIDSTATE::SetSensor_CE_Walker_Common( void )
{

	for( int i=0 ; i<NUM_ODE_BODY-1 ; i++ )
	{
		// Send values to OSC
		sim->SetSensor_Proprioceptor( 2*i   , motor[i].IaL1 , motor[i].IaV1, motor[i].IbF1  );
		sim->SetSensor_Proprioceptor( 2*i+1 , motor[i].IaL2 , motor[i].IaV2, motor[i].IbF2  );
	}
	
}





