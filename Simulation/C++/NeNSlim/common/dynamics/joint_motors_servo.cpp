
/********************************************/
/*											*/
/*           For Servo Actuation			*/
/*											*/
/********************************************/


#include "rigidode.h"





void RIGIDSTATE::Set_Effectors_Servo_Single( void )
{

	for( int i=0 ; i<NUM_ODE_BODY-1 ; i++ )
	{
		for( int d=0 ; d<motor[i].dof ; d++ )
		{
		//	double SIGNAL = sim->NeuroAlphaOut_x( -1 , motor[i].neuroidx1[d] );

			double SIGNAL = *motor[i].neuroAct1_x[d];


				double gain = 50.0;	//12.5;	// in Robogen: gain=0.5, step=0.04 ? --> gain/step = 12.5
			
				dReal Desired			= SIGNAL * motor[i].Hstop[d];
				dReal True				= motor[i].angle[d];
				dReal Error				= Desired - True;
				dReal desiredMotorSpeed	= gain*Error;

					
				double MAX_FORCE = 1000.0;
							
				dJointSetHingeParam( joint[i][d] , dParamFMax , MAX_FORCE );
				dJointSetHingeParam( joint[i][d] , dParamVel  , desiredMotorSpeed );


			// Save Prev Variables
			motor[i].activation1[d] = SIGNAL;
			motor[i].activation2[d] = -1976.0;
			
			motor[i].prevAngle[d]	= motor[i].angle[d];
			motor[i].prevVel[d]		= motor[i].vel[d];
		}

	}
	
}


void RIGIDSTATE::Set_Effectors_Servo_Double( void )	// simulation test for HW --> but without sampling rate
{
	
	for( int i=0 ; i<NUM_ODE_BODY-1 ; i++ )
	{
		for( int d=0 ; d<motor[i].dof ; d++ )
		{
		//	double x1 = sim->NeuroAlphaOut_x( -1 , motor[i].neuroidx1[d] );	// no Alpha when CTRNN
		//	double x2 = sim->NeuroAlphaOut_x( -1 , motor[i].neuroidx2[d] );

			double x1 = *motor[i].neuroAct1_x[d];
			double x2 = *motor[i].neuroAct2_x[d];
		
			double SIGNAL = 0.5 * (x1-x2);


				double gain = 50.0;
			
				dReal Desired			= SIGNAL * motor[i].Hstop[d];
				dReal True				= motor[i].angle[d];
				dReal Error				= Desired - True;
				dReal desiredMotorSpeed	= gain*Error;

					
				double MAX_FORCE = 1000.0;
				dJointSetHingeParam( joint[i][d] , dParamFMax , MAX_FORCE );
				dJointSetHingeParam( joint[i][d] , dParamVel  , desiredMotorSpeed );


			// Save Prev Variables
			motor[i].activation1[d] = x1;	// k1
			motor[i].activation2[d] = x2;	// k2
		
			motor[i].prevAngle[d]	= motor[i].angle[d];
			motor[i].prevVel[d]		= motor[i].vel[d];				
		}

	}
	
}




