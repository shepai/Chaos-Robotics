

#include "rigidode.h"



CAPNODE::CAPNODE()
{
	rs = NULL;
	index = -1;

	pos.Zero();
	
	ox.Zero();
	oy.Zero();
	oz.Zero();

	R = 0.0;
	L = 0.0;


	tpos.Set(0.0,0.0,0.0);
	lv.Set(0.0,0.0,0.0);
	av.Set(0.0,0.0,0.0);
	
	tox.Set(0.0,0.0,0.0);
	toy.Set(0.0,0.0,0.0);
	toz.Set(0.0,0.0,0.0);

	isFoot = false;

}


CAPNODE::~CAPNODE()
{

}


void CAPNODE::Zero( void )
{
	tox	 = ox;
	toy  = oy;
	toz  = oz;
	tpos = pos;

	lv.Set(0.0,0.0,0.0);
	av.Set(0.0,0.0,0.0);
}



MOTOR::MOTOR()
{
	rs = NULL;

	dof = 1;
	anchor.Zero();
	
	for( int d=0 ; d<3 ; d++ )
	{
		orien[d].Zero();

		neuroAct1_x[d] = neuroAct1_y[d] = NULL;
		neuroAct2_x[d] = neuroAct2_y[d] = NULL;
	}
	
}


MOTOR::~MOTOR()
{

}


void MOTOR::Zero( void )
{

	for( int d=0 ; d<3 ; d++ )
	{
		angle[d] = vel[d] = velLF[d] = torque[d] = force[d] = 0.0;
		activation1[d] = activation2[d] = 0.0;
		prevAngle[d] = prevVel[d] = 0.0;
	}

	
	
	contact = false;

	ACCEL = 0.0;

	
	IaL1 = IaL2 = 0.0;
	IaV1 = IaV2 = 0.0;
	IbF1 = IbF2 = 0.0;



	// for Servo
	F = Fs = 0.0;
	modelF = realF = 0.0;

	
	// phaseNew
	has_Revolute = false;
	prev_Vel = 0.0;
	revTime = 0.001;
	prev_revTime = 0.0;
	phaseNew = 0.0;

}



void RIGIDSTATE::ResetVariable( void )		// Zero() function
{
	
	// RS Common
	CG.Zero();		CGv.Zero();
	rkCG.Zero();	rkCGv.Zero();

	robotStep = 0.0;

	avrCG = InitPos;	// Camera follow
	

	// ODE Robots
	isBlow = false;

	for( int i=0 ; i<NUM_ODE_BODY ; i++ )	node[i].Zero();		
	for( int i=0 ; i<NUM_ODE_BODY-1 ; i++ )	motor[i].Zero();
	
	for( int i=0 ; i<10 ; i++ )	footTip[i].Zero();

		
	// pDiff New
	for( int i=0 ; i<32 ; i++ )	rCoord[i] = 0.0;

	is_rCoord_Has_Revolute = false;
	is_rCoord_update = false;
	
	measureTime = glPastTimeAmount = 0.0;

	for( int i=0 ; i<30 ; i++ )
	{
		revIdx[i] = i;	// just fill with different osc indices
	}


}



RIGIDSTATE::RIGIDSTATE( SIMULATION* sss )
{	
	
	sim = sss;
	
	is_ODE_Alive = false;
	is_ODE_Robot = false;

	is_use_CG = false;

	robotStep	= 0.0;	
	gainstr		= 0.0;

	NUM_ODE_BODY	= 0;
	BALLRAD		= 0.0;
	
	

	SetRobotFuncName();


	oscTr = -1.0;

	

	isCoulomb = true;
	mu_coeff = 1.0;

	

	NUM_TIP = 0;

	
}


RIGIDSTATE::~RIGIDSTATE()
{	
	delete [] node;
	delete [] motor;
	delete [] tupidx;

	if( is_ODE_Alive )	Destroy_ODE_Body();
	
}


void RIGIDSTATE::Destroy_ODE_Body( void )
{
		
	// destroy bodies and geometries
	for( int i=0 ; i<NUM_ODE_BODY ; i++ )
	{
		dBodyDestroy( body[i] );
		dGeomDestroy( bodygeom[i] );
	}

	delete [] body;			body		= NULL;
	delete [] bodygeom;		bodygeom	= NULL;
	

	// destroy joints
	for( int i=0 ; i<NUM_ODE_BODY-1 ; i++ )
	{
		for( int d=0 ; d<motor[i].dof   ; d++ )		dJointDestroy( joint[i][d] );
		for( int d=0 ; d<motor[i].dof-1 ; d++ )		dBodyDestroy ( ball[i][d] );

		delete joint[i];
		delete ball[i];
	}

	free( joint );			joint	= NULL;
	free( ball  );			ball	= NULL;
	
	is_ODE_Alive = false;
}



void RIGIDSTATE::Set_Body_RS( void )
{ 
	(this->*SetBodyFuncName)();

	for( int i=0 ; i<NUM_ODE_BODY ; i++ )
	{
		node[i].rs = this;
		node[i].index = i;
	}

	for( int i=0 ; i<NUM_ODE_BODY-1 ; i++ )
	{
		motor[i].rs = this;
	}
}




