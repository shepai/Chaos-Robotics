/*************************************************************************
 *		Dynamic Simulator module using Russell Smith's ODE               *
 *		written by Yoonsik Shim											 *
 *		Joint State Measurement														 *
 *************************************************************************/

#include "rigidode.h"




double RIGIDSTATE::GetTotalPower( void )
{
	double POWER = 0.0;

	
	for( int t=0 ; t<NUM_ODE_BODY-1 ; t++ )
	{
	
		double avel = motor[t].vel[0];
			
		double pp = motor[t].torque[0] * avel;
		
		if(pp>0.0)	POWER += pp;
	
	}

	return POWER;

}




// Motor Torque for Quadruped & Hexapod
void RIGIDSTATE::MeasureJointForceQuad( void )
{	
	int HN = (NUM_ODE_BODY-1)/2;


	for( int n=0 ; n<HN ; n++ )
	{
		for( int d=0 ; d<motor[n].dof ; d++ )
		{
			VEC3 torq1;
			torq1.Set( motor[n].fback[d].t1[0] , motor[n].fback[d].t1[1] , motor[n].fback[d].t1[2] );

		//	VEC3 torq2;
		//	torq2.Set( motor[n].fback[d].t2[0] , motor[n].fback[d].t2[1] , motor[n].fback[n].t2[2] );

			VEC3 force1;
			force1.Set( motor[n].fback[d].f1[0] , motor[n].fback[d].f1[1] , motor[n].fback[d].f1[2] );

		//	VEC3 force2;
		//	force2.Set( motor[n].fback[d].f2[0] , motor[n].fback[d].f2[1] , motor[n].fback[d].f2[2] );


			
			VEC3 pos1;
		//	VEC3 pos2;
			const double* p1 = dBodyGetPosition( dJointGetBody(joint[n][d],0) );
		//	const double* p2 = dBodyGetPosition( dJointGetBody(joint[n][d],1) );
			pos1.Set( p1[0] , p1[1] , p1[2] );
		//	pos2.Set( p2[0] , p2[1] , p2[2] );

			VEC3 anchor;
			dVector3 an; 	dJointGetHingeAnchor ( joint[n][d], an );
			anchor.Set( an[0] , an[1] , an[2] );


			VEC3 ftorq1 = torq1 - (force1^(pos1-anchor));
		//	VEC3 ftorq2 = torq2 - (force2^(pos2-anchor));
					

			dVector3 a;
			dJointGetHingeAxis ( joint[n][d] , a );
			VEC3 axis;	axis.Set( a[0] , a[1] , a[2] );
			axis.Normalize();

			double TORQ = ftorq1 * axis;
			motor[n].torque[d] = TORQ;

		}
	}



	for( int n=HN ; n<NUM_ODE_BODY-1 ; n++ )
	{
		for( int d=0 ; d<motor[n].dof ; d++ )
		{
			VEC3 torq1;
			torq1.Set( motor[n-HN].fback[d].t1[0] , motor[n-HN].fback[d].t1[1] , motor[n-HN].fback[d].t1[2] );

			VEC3 torq2;
			torq2.Set( motor[n-HN].fback[d].t2[0] , motor[n-HN].fback[d].t2[1] , motor[n-HN].fback[d].t2[2] );

			VEC3 force1;
			force1.Set( motor[n-HN].fback[d].f1[0] , motor[n-HN].fback[d].f1[1] , motor[n-HN].fback[d].f1[2] );

			VEC3 force2;
			force2.Set( motor[n-HN].fback[d].f2[0] , motor[n-HN].fback[d].f2[1] , motor[n-HN].fback[d].f2[2] );


			
			VEC3 pos1,pos2,anchor;
			const double* p1 = dBodyGetPosition( dJointGetBody(joint[n-HN][d],0) );
			const double* p2 = dBodyGetPosition( dJointGetBody(joint[n-HN][d],1) );
			pos1.Set( p1[0] , p1[1] , p1[2] );
			pos2.Set( p2[0] , p2[1] , p2[2] );

			dVector3 an; 	dJointGetHingeAnchor ( joint[n-HN][d], an );
			anchor.Set( an[0] , an[1] , an[2] );


			VEC3 ftorq1 = torq1 - (force1^(pos1-anchor));
		//	VEC3 ftorq2 = torq2 - (force2^(pos2-anchor));
					

			dVector3 a;
			dJointGetHingeAxis ( joint[n][d] , a );
			VEC3 axis;	axis.Set( a[0] , a[1] , a[2] );
			axis.Normalize();

			double TORQ = ftorq1 * axis;
			motor[n].torque[d] = TORQ;

		}
	}




}


void RIGIDSTATE::MeasureJointForce( void )
{	

	for( int n=0 ; n<NUM_ODE_BODY-1 ; n++ )
	{

		for( int d=0 ; d<motor[n].dof ; d++ )
		{
			VEC3 torq1;
			torq1.Set( motor[n].fback[d].t1[0] , motor[n].fback[d].t1[1] , motor[n].fback[d].t1[2] );

			VEC3 torq2;
			torq2.Set( motor[n].fback[d].t2[0] , motor[n].fback[d].t2[1] , motor[n].fback[d].t2[2] );

			VEC3 force1;
			force1.Set( motor[n].fback[d].f1[0] , motor[n].fback[d].f1[1] , motor[n].fback[d].f1[2] );

			VEC3 force2;
			force2.Set( motor[n].fback[d].f2[0] , motor[n].fback[d].f2[1] , motor[n].fback[d].f2[2] );


			
			VEC3 pos1,pos2,anchor;
			const double* p1 = dBodyGetPosition( dJointGetBody(joint[n][d],0) );
			const double* p2 = dBodyGetPosition( dJointGetBody(joint[n][d],1) );
			pos1.Set( p1[0] , p1[1] , p1[2] );

			if( dJointGetBody(joint[n][d],1) )	pos2.Set( p2[0] , p2[1] , p2[2] );
			else								pos2.Zero();

			dVector3 an; 	dJointGetHingeAnchor ( joint[n][d], an );
			anchor.Set( an[0] , an[1] , an[2] );


			VEC3 ftorq1 = torq1 - (force1^(pos1-anchor));	// torq by motor = total torq - constraint torq
		//	VEC3 ftorq2 = torq2 - (force2^(pos2-anchor));	// opposite direction - use if this is necessary
					

			dVector3 a;
			dJointGetHingeAxis ( joint[n][d] , a );
			VEC3 axis;	axis.Set( a[0] , a[1] , a[2] );
			axis.Normalize();


			double TORQ = ftorq1 * axis;
			motor[n].torque[d] = TORQ;
			
		//	TORQ = ftorq2 * axis;
			motor[n].force[d] = force1.y;

		}
	}


	// contact detect
	for( int n=(NUM_ODE_BODY+1)/2 ; n<NUM_ODE_BODY ; n++ )
	{
		VEC3 pos = node[n].tpos + ( node[n].toz*(0.5*node[n].L ) );

		if( pos.y < node[n].R )
		{
			motor[n-1].contact = true;
			motor[n-1-(NUM_ODE_BODY-1)/2].contact = true;
		}
		else
		{
			motor[n-1].contact = false;
			motor[n-1-(NUM_ODE_BODY-1)/2].contact = false;
		}
		
	}

}


