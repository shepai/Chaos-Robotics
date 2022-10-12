/*************************************************************************
 *		Dynamic Simulator module using Russell Smith's ODE               *
 *		written by Yoonsik Shim											 *
 *		2002.12.30														 *
 *************************************************************************/




#include "..\neurouse\neurouse.h"



void SIMULATION::InitEval( void )
{
	LOG( +1 , "[BGN] SIMULATION::InitEval(void) --> CE"  );


	eval = new EVAL( this );

	if( eval )	eval->Init_Eval();


	LOG( -1 , "[END] SIMULATION::InitEval(void)" );
}


void SIMULATION::EndEval( void )
{
	if( eval )
	{
		delete eval;
		eval = NULL;

		LOG( 0 , "[BGN] SIMULATION::EndEval(void) [END]"  );
	}
}




void SIMULATION::InitRS( void )
{
	LOG( +1 , "[BGN] SIMULATION::InitRS(void) --> %s", ENUM_ROBOT_NAME[robotKind] );

	rs = new RIGIDSTATE(this);
	rs->Set_Body_RS();

	LOG( -1 , "[END] SIMULATION::InitRS(void)" );
}


void SIMULATION::EndRS( void )
{	
	if( rs )
	{
		LOG( 0 , "[BGN][END] SIMULATION::EndRS(void)" );

		delete rs;
		rs = NULL;
	}

}




void InitSimulation ( int T )	// SpNet has ODE engine as minimal 2-bodies
{	

	LOG( +1 , "[BGN] void InitSimulation(%d)" , T );



	SIMULATION* simul = g_sim[T];
	
	simul->world		= dWorldCreate();
	simul->space		= dHashSpaceCreate(0);
	simul->ground		= dCreatePlane ( simul->space , 0.0 , 1.0 , 0.0 , 0.0 );
	simul->contactgroup	= dJointGroupCreate (0);


	double grav_Y = -9.81;		
	dWorldSetGravity ( g_sim[T]->world , 0.0 , grav_Y , 0.0 );

	
//	dWorldSetERP (world[T], 0.2);
//	dWorldSetCFM (world[T], 0.00001);

	
	simul->rs->Construct_Body_ODE( simul->world, simul->space );	// Dummy ODE bodies for SpNet included...

	// Connection to Neural Effector is done here!!
	simul->rs->AfterConstruct( );	// For SpNet, rk4 variables must be re-added.
	

	
	if( simul->eval )
		simul->eval->Add_Eval_RK4_Var_RS( );


	simul->rs->ResetVariable( );	// ODE, spnet, voxels all reset	
	
	simul->rs->SetTransform_ODE( );		// Do this once even if SpNet !!

	
	// Remember original DoF for damage leg
	simul->rs->NUM_ODE_BODY_ORIGIN = simul->rs->NUM_ODE_BODY;


	LOG( -1 , "[END] void InitSimulation(%d)" , T );

}


void EndSimulation( int T )
{

	LOG( +1 , "[BGN] void EndSimulation(%d)" , T );

	SIMULATION* simul = g_sim[T];


	// delete Eval RK4 variables
	if( simul->eval )	simul->eval->Del_Eval_RK4_Var_RS( );

	
	// Mainly for ODE Robots BUT spnet & vxnet all have to do this...
	simul->rs->Destroy_ODE_Body();
	
	dJointGroupDestroy	(simul->contactgroup);	simul->contactgroup	= NULL;
	dGeomDestroy		(simul->ground);		simul->ground		= NULL;	
	dSpaceDestroy		(simul->space);			simul->space		= NULL;
	dWorldDestroy		(simul->world);			simul->world		= NULL;
	

	LOG( -1 , "[END] void EndSimulation(%d)" , T );

}





static void nearCallback (void *data, dGeomID o1, dGeomID o2)
{
	int T = *(int*)data;	// thread ID

	SIMULATION* simul = g_sim[T];


	int		surfMode	= 0;
	double	mu			= 0.0;
	double	mu2			= 0.0;
	double	slip1		= 0.0;
	double	slip2		= 0.0;
	double	soft_erp	= 0.0;
	double	soft_cfm	= 0.0;

	
	
	// Ground friction settings
			if( simul->rs->isCoulomb )	// Simulated Friction Cone
			{
				surfMode = dContactApprox1 | dContactSoftERP | dContactSoftCFM;
						
				mu	= simul->rs->mu_coeff;

				soft_erp = 0.2;
				soft_cfm = 0.000001;
			}
			else	// Simple slip-like model
			{
				surfMode = dContactSlip1 | dContactSlip2 | dContactSoftERP | dContactSoftCFM | dContactApprox1;
			
				mu		 = dInfinity;
			//	slip1 = slip2 = 0.005;
				slip1 = slip2 = 0.01;

				soft_erp = 0.2;
				soft_cfm = 0.001;
			}		
		
		




	dBodyID b1 = dGeomGetBody(o1);
	dBodyID b2 = dGeomGetBody(o2);
	if (b1 && b2 && dAreConnected (b1,b2))
		return;

	//const int N = 10;
	dContact contact[10];
	int n = dCollide ( o1 , o2 , 4 , &contact[0].geom , sizeof(dContact) );


	// Process Contact	
	if( n > 0 )
	{		
		for ( int i=0; i<n; i++) 
		{	
			contact[i].surface.mode = surfMode;
						
			contact[i].surface.mu		= mu;
			contact[i].surface.mu2		= mu2;
			contact[i].surface.slip1	= slip1;
			contact[i].surface.slip2	= slip2;
			contact[i].surface.soft_erp = soft_erp;
			contact[i].surface.soft_cfm = soft_cfm;
		
			dJointID c = dJointCreateContact (simul->world,simul->contactgroup,&contact[i]);
			dJointAttach (c, dGeomGetBody(contact[i].geom.g1), dGeomGetBody(contact[i].geom.g2));
		}
	}
	
}






// Actuation model chosen at Initialisation
void RIGIDSTATE::SetEffector( void )
{
	(this->*SetEffectorFuncName)();

}


// Things before advance timestep
void Loop_ODE_Robot_Before( int T )
{
	RIGIDSTATE* rs = g_sim[T]->rs;
	
		
		// Give Inputs to NN
		rs->SetSensor( );	// (e.g.) calc IaL using joint_X(t-1) then put into --> oscil->IaSen[n]->IaL .. etc.
		
		// Effector or Muscle
		rs->SetEffector( );	// set force/torque to joint motors
			

		// Collision Check
		int thrIdx = T;
		dSpaceCollide (g_sim[T]->space , (void*)(&thrIdx) , &nearCallback );
		
	
}


// Things after time advancement
void Loop_ODE_Robot_After( int T )
{
	RIGIDSTATE* rs = g_sim[T]->rs;
	

	// Advance simulation	
	/********************/	dWorldStep( g_sim[T]->world, rs->STEPSIZE );	/**************************/


		// Progress IaL etc . . .
		rs->Progress_Reflex_ODE_Robot();

		// measurement
		rs->MeasureJointForce( );
		
	// remove all contact joints
	dJointGroupEmpty ( g_sim[T]->contactgroup );
	
	
}




// Gather Realtime Information & blowup etc.
void RIGIDSTATE::SetTransform_ODE ( void )
{
	// rigid bodies
	for ( int i=0; i<NUM_ODE_BODY; i++ )
	{
		const double* po = dBodyGetPosition(body[i]);
		const double* R  = dBodyGetRotation(body[i]);
		node[i].tox.Set( R[0] , R[4] , R[8]  );	node[i].toy.Set( R[1] , R[5] , R[9]  );	node[i].toz.Set( R[2] , R[6] , R[10] );		
		node[i].tpos.Set( po[0] , po[1] , po[2] );

		const double* linvel = dBodyGetLinearVel (body[i]);
		const double* angvel = dBodyGetAngularVel(body[i]);
		node[i].lv.Set( linvel[0], linvel[1], linvel[2] );
		node[i].av.Set( angvel[0], angvel[1], angvel[2] );

	}

	// Motors
	for( int i=0 ; i<NUM_ODE_BODY-1 ; i++ )
	{					
		for( int d=0 ; d<motor[i].dof ; d++ )
		{			
			motor[i].angle[d] = dJointGetHingeAngle(joint[i][d]);
			motor[i].vel[d]	  = dJointGetHingeAngleRate(joint[i][d]);
		}

		// smoothing sharp joint vel for rCoord
		double vlfV = ( motor[i].vel[0] - motor[i].velLF[0] ) * 2.0; // tau=0.5;
		motor[i].velLF[0] += vlfV*STEPSIZE;
	}


	


	// BlowUp
	isBlow = false;
	for( int i=0 ; i<NUM_ODE_BODY-1 ; i++ )
	{
		if( node[i].lv.Mag() > 10000.0 )
		{	isBlow = true;	break;	}

		for( int d=0 ; d<motor[i].dof ; d++ )
		{
			if( motor[i].vel[d] > 1000.0 ){	isBlow = true;	break;	}
		}
	}


	
	// update foot position -- which cna be used for foot contact sensing later
	
		int tipIdx = 0;

		VEC3 tipPos;	tipPos.Zero();
		VEC3 tipVel;	tipVel.Zero();

		for( int i=0 ; i<NUM_ODE_BODY ; i++ )
		{
			if( node[i].isFoot )
			{
				tipPos = node[i].tpos + ( node[i].toz * (0.5*node[i].L) );
				footHeight[i] = tipPos.y;

				tipVel = node[i].lv + ( node[i].av^( node[i].toz * (0.5*node[i].L) ) );		
				VEC3 orien = node[0].toz;		orien.y = 0.0;
				footVel[i] = tipVel * orien;


				// if ground contact
				footTip[tipIdx] = tipPos;

				double hgt = -100.0;
				if( footHeight[i] - node[i].R < 1E-7 )	hgt = 0.0;
				
				footTip[tipIdx].y = hgt;
				
				tipIdx++;

			}
		}


}

// Centre of Mass (Euler integration)
void RIGIDSTATE::Calc_CG( void )
{
	double tmas = 0.0;
	
	CG.Zero();	CGv.Zero();

	
	if( is_ODE_Robot )
	{
		for( int i=0 ; i<NUM_ODE_BODY ; i++ )
		{
			CG = CG + (node[i].tpos*node[i].mass);
			CGv = CGv + (node[i].lv*node[i].mass);
			tmas += node[i].mass;
		}
	}	

	CG =  CG/tmas;
	CGv =  CGv/tmas;


}


// for camera following for 3d rendering
void RIGIDSTATE::Calc_avrCG_for_DX_Cam( void )
{
	double camTau = 1.0;
	
	camTau = oscTr;

	// for DX camera
	VEC3 avrCG_Vel = ( CG - avrCG ) / camTau;
	avrCG = avrCG + avrCG_Vel*STEPSIZE;
}






