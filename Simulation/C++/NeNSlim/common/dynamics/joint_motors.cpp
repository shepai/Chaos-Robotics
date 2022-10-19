/*************************************************************************
 *		Dynamic Simulator module using Russell Smith's ODE               *
 *		written by Yoonsik Shim											 *
 *		2002.12.30														 *
 *************************************************************************/




#include "rigidode.h"


void MOTOR::Calculate_Reflex( void )
{

	// spindles
	double speed = fabs( vel[0]/Hstop[0] );
		
	double rVel = sqrt( speed );
	if( vel[0] < 0.0 )	rVel = -rVel;
		

				
	double ang1 = 2.0 - angle[0]/Hstop[0];
	double ang2 = 2.0 + angle[0]/Hstop[0];


	// tendons -- tanh(a)
	double x1 = activation1[0] + 1.0;
	double x2 = activation2[0] + 1.0;

//	double tendon1 = x1 * ang1 - 2.0;
//	double tendon2 = x2 * ang2 - 2.0;
	
	double tendon1 = x1 * ang1;
	double tendon2 = x2 * ang2;



	double rateT = 1.0 / ( rs->oscTr*0.05 );//0.025;

	IaV1_Vel = ( -rVel - IaV1 ) * rateT;
	IaV2_Vel = (  rVel - IaV2 ) * rateT;

	IaL1_Vel = (  ang1 - IaL1 ) * rateT;
	IaL2_Vel = (  ang2 - IaL2 ) * rateT;

	IbF1_Vel  = (  tendon1 - IbF1 ) * rateT;
	IbF2_Vel  = (  tendon2 - IbF2 ) * rateT;

	
	
}


void MOTOR::Progress_ODE_Robot_Reflex( void )
{
	double h = rs->STEPSIZE;

	IaV1 += h*IaV1_Vel;
	IaV2 += h*IaV2_Vel;

	IaL1 += h*IaL1_Vel;
	IaL2 += h*IaL2_Vel;

	IbF1 += h*IbF1_Vel;
	IbF2 += h*IbF2_Vel;
}




void MOTOR::Calculate_TwoTorque_Alpha( void )
{
//	int thrId = rs->thread_Idx;	// for multi-threaded EA

	for( int d=0 ; d<dof ; d++ )
	{

		double x1 = *neuroAct1_x[d];
		double x2 = *neuroAct2_x[d];


		x1 = tanh( x1 );
		x2 = tanh( x2 );
	

		ACCEL = alpha[d]*(x1-x2) - beta[d]*(x1+x2+gamma[d])*angle[d] - delta[d]*vel[d];

		activation1[d] = x1;
		activation2[d] = x2;

	}

}


// This is used for CE
void RIGIDSTATE::Set_Effectors_Two_Torque( void )
{	
	for( int i=0 ; i<NUM_ODE_BODY-1 ; i++ )
	{
		motor[i].Calculate_TwoTorque_Alpha();


		double FORCE = motor[i].ACCEL*node[i+1].inertia;

		dJointAddHingeTorque(  joint[i][0] , FORCE );
	}

}

