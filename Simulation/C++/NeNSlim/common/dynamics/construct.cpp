


#include "rigidode.h"




//int GetEffIdx ( int , int , int , int );

void RIGIDSTATE::AfterConstruct( void )
{
	(this->*AfterConstructFuncName)();


	if( is_ODE_Robot )
	{
		for( int i=0 ; i<NUM_ODE_BODY-1 ; i++ )
		{
			for( int d=0 ; d<motor[i].dof ; d++ )
			{
				motor[i].neuroAct1_x[d] = sim->Get_Neuro_Effector_Pointer( sim->effIdx1[i][d] , true );
				motor[i].neuroAct2_x[d] = sim->Get_Neuro_Effector_Pointer( sim->effIdx2[i][d] , true );

				motor[i].neuroAct1_y[d] = sim->Get_Neuro_Effector_Pointer( sim->effIdx1[i][d] , false );
				motor[i].neuroAct2_y[d] = sim->Get_Neuro_Effector_Pointer( sim->effIdx2[i][d] , false );
			}
		}
	}

	

	

}


void RIGIDSTATE::Construct_Body_ODE( dWorldID world, dSpaceID space )
{

	dMass		m;
	dMatrix3	R;
	
	body		= new dBodyID[NUM_ODE_BODY];
	bodygeom	= new dGeomID[NUM_ODE_BODY];


	// Rigid body spacial
	for( int i=0 ; i<NUM_ODE_BODY ; i++ )
	{
		// Create and Locate
		body[i] = dBodyCreate (world);
		dRFrom2Axes ( R, node[i].ox.x , node[i].ox.y, node[i].ox.z ,   
						 node[i].oy.x , node[i].oy.y, node[i].oy.z   );	
		dBodySetRotation ( body[i] ,  R );		
		dBodySetPosition ( body[i] , node[i].pos.x , node[i].pos.y , node[i].pos.z );

		// Set mass & geometry
		if( node[i].shape == 0 )
		{
 			dMassSetCappedCylinder ( &m, (dReal)(node[i].DENSITY * (double)MASS_SCALE), 3, node[i].R, node[i].L );
			bodygeom[i] = dCreateCCylinder ( space, node[i].R, node[i].L );
		}
		else if( node[i].shape == 1 )
		{
			dMassSetBox ( &m , (dReal)node[i].DENSITY * (double)MASS_SCALE , node[i].Lx , node[i].Ly , node[i].L );
			bodygeom[i] = dCreateBox ( space , node[i].Lx , node[i].Ly , node[i].L );
		}
		else if( node[i].shape == 2 )
		{
 			dMassSetCylinder ( &m, (dReal)(node[i].DENSITY * (double)MASS_SCALE), 3, node[i].R, node[i].L );
		//	bodygeom[i] = dCreateCylinder  ( space, node[i].R, node[i].L );
			bodygeom[i] = dCreateBox ( space , 2.0*node[i].R , 2.0*node[i].R , node[i].L );
		}
		else{}


	//	dMassAdjust(&m, m.mass );
		dBodySetMass (body[i],&m);
		
		node[i].mass = m.mass;
		

		// Geometry for collision		
		dGeomSetRotation ( bodygeom[i] , R );
		dGeomSetPosition ( bodygeom[i] , node[i].pos.x , node[i].pos.y , node[i].pos.z );
		dGeomSetBody (bodygeom[i],body[i]);

	}


	joint		= (dJointID**)malloc( sizeof(dJointID*) * (NUM_ODE_BODY-1) );
	ball		= (dBodyID**)malloc( sizeof(dBodyID*) * (NUM_ODE_BODY-1) );
		
	for( int j=0 ; j<NUM_ODE_BODY-1 ; j++ )
	{
		joint[j]	= NULL;
		ball[j]		= NULL;
	}
	
	for( int j=0 ; j<(NUM_ODE_BODY-1) ; j++ )	CombineJoint( j, tupidx[j][0], tupidx[j][1], world );

	is_ODE_Alive = true;		// the body is created and the simulation is active
}




void RIGIDSTATE::CombineJoint( int i, int idx1 , int idx2 , dWorldID world )
{

	dMass		m;
	dMatrix3	R;
	
	joint[i] = new dJointID[motor[i].dof];

		for( int d=0 ; d<motor[i].dof ; d++ )	joint[i][d] = dJointCreateHinge (world,0);

		if( motor[i].dof > 1 )
		{
			ball[i]		= new dBodyID[motor[i].dof-1];
			dRFrom2Axes ( R, node[i+1].ox.x , node[i+1].ox.y, node[i+1].ox.z,  node[i+1].oy.x , node[i+1].oy.y, node[i+1].oy.z   );	
		
			for( int d=0 ; d<motor[i].dof-1 ; d++ )
			{
				ball[i][d] = dBodyCreate (world);
							
				dBodySetRotation ( ball[i][d] ,  R );
				dBodySetPosition ( ball[i][d] , motor[i].anchor.x , motor[i].anchor.y , motor[i].anchor.z );
			
				dMassSetSphere ( &m, BALLDENSITY, BALLRAD );
				dBodySetMass (ball[i][d],&m);
			}
				
			dJointAttach (joint[i][0] , body[idx1] , ball[i][0]);
			if( motor[i].dof > 2 )	dJointAttach (joint[i][1] , ball[i][0] , ball[i][1]);
			dJointAttach (joint[i][motor[i].dof-1] , ball[i][motor[i].dof-2] , body[idx2] );
		}
		else
		{
			if( idx2 < 0 )	dJointAttach (joint[i][0] , body[idx1] , 0 );
			else			dJointAttach (joint[i][0] , body[idx1] , body[idx2]);
		}		

		
		for( int d=0 ; d<motor[i].dof ; d++ )
		{
			dJointSetHingeAnchor( joint[i][d] , motor[i].anchor.x , motor[i].anchor.y , motor[i].anchor.z );
			dJointSetHingeAxis( joint[i][d] , motor[i].orien[d].x, motor[i].orien[d].y, motor[i].orien[d].z );
				
	//		dJointSetHingeParam		( joint[i][d] , dParamLoStop, motor[i].Lstop[d] );
	//		dJointSetHingeParam		( joint[i][d] , dParamHiStop, motor[i].Hstop[d] );

			dJointSetHingeParam		( joint[i][d] , dParamLoStop, -1.5 );
			dJointSetHingeParam		( joint[i][d] , dParamHiStop,  1.5 );

		//	dJointSetHingeParam( joint[i][d] , dParamCFM , 0.001 );

			dJointSetHingeParam		( joint[i][d] , dParamFudgeFactor, 0.1 );

			dJointSetFeedback( joint[i][d], &motor[i].fback[d] );


			dJointSetHingeParam( joint[i][d] , dParamFMax , 0.0 );
			dJointSetHingeParam( joint[i][d] , dParamVel  , 0.0 );

		}



}


