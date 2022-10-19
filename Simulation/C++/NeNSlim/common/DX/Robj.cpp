


#include "Robj.h"
#include "..\..\common\dynamics\rigidode.h"
#include "../status.h"
#include "dxwnd.h"



static float texMargin = 0.07f;
static float texMargin2 = 0.0f;




//////////////////////// ROBJ //////////////////////////////////////////////

ROBJ::ROBJ()
{
	sim = NULL;
	D3DXMatrixIdentity( &matRot );
//	R = L = 
//	Lx = Ly = Lz = tx = ty = tz = 0.0;
}

ROBJ::~ROBJ()
{
	delete sim;
}

void ROBJ::ReadShape( char* name )
{
	sim = ReadSIM( name );
}

void ROBJ::ReadBox( char* name )
{
	sim = MakeBox( name );
}

void ROBJ::ReadCylinder( char* name )
{
	sim = MakeCylinder( name );
}



void AllocROBJ( DXHANDLE* dh )
{

	int NUM = dh->rs->NUM_ODE_BODY;
	
	

	ROBJ*	res = NULL;
	
	CAPNODE* node = g_sim[0]->rs->node;

		dh->NUM_CCYL = 0;
		for( int i=0 ; i<NUM ; i++ )
		{
			if( node[i].shape == 0 )
				dh->NUM_CCYL++;
		}
		
		// Geometry Setting
		dh->NUM_OBJ = NUM + 2*dh->NUM_CCYL + 2;

		res = new ROBJ[dh->NUM_OBJ];
		for( int i=0 ; i<dh->NUM_OBJ ; i++ )	res->dxh = dh;
		
		SetCurrentDirectory( g_Status->RunDir );

		// fixed floor --> leave this alone !!
		res[dh->NUM_OBJ-1].ReadShape( "../media/model/ground.sim" );
		res[dh->NUM_OBJ-2].ReadShape( "../media/model/skybox.sim" );
		

		char* texName[2];

	
		if( dh->rs->is_ODE_Robot )
		{
			texName[0] = "../media/model/sphere.sim";
			texName[1] = "../media/model/spheref.sim";
		}

		for( int i=NUM ; i<NUM+dh->NUM_CCYL ; i++ )					res[i].ReadShape( texName[0] );
		for( int i=NUM+dh->NUM_CCYL ; i<NUM+2*dh->NUM_CCYL ; i++ )	res[i].ReadShape( texName[1] );




		char* textureName[50];
		
		
		if( dh->rs->is_ODE_Robot )
		{
			textureName[0] = "swimmerB.bmp";								// swimmer body
			textureName[1] = "swimmerL1.bmp";								// swimmer limb number 1
			for( int i=2 ; i<NUM ; i++ )	textureName[i] = "swimmerL.bmp";	// swimmer limb others
		}

	//	textureName[0] = "greyscale.bmp";								// swimmer body
	//	textureName[1] = "greyscale.bmp";								// swimmer limb number 1
	//	for( i=2 ; i<NUM ; i++ )	textureName[i] = "greyscale.bmp";	// swimmer limb others
		


		int cylIdx = 0;
				
		for( int i=0 ; i<NUM ; i++ )
		{
			VEC3 renderpos = node[i].pos * dh->render_scale;

			if( node[i].shape == 0 )
			{
				// coloured
			//	if( i == 0 )	res[i].ReadShape( ".\\media\\cylinder_B.sim" );
			//	else			res[i].ReadShape( ".\\media\\cylinder_L.sim" );

				// grey
				res[i].ReadShape( "../media/model/cylinder_L.sim" );


				double renderR = node[i].R * dh->render_scale;
				double renderL = node[i].L * dh->render_scale;

				VEC3 push;
				push.Set( node[i].oz.x*(node[i].L*0.5), node[i].oz.y*(node[i].L*0.5), node[i].oz.z*(node[i].L*0.5));
				if( i == 0 )	push.Zero();
				VEC3 renderpush = push * dh->render_scale;

				res[i].SetScale( renderR, renderR, renderL );			
				res[i].tx = (float)(renderpos.x+renderpush.x);
				res[i].ty = (float)(renderpos.y+renderpush.y);
				res[i].tz = (float)(renderpos.z+renderpush.z);

				int uCylOffset = NUM;
				int dCylOffset = NUM+dh->NUM_CCYL;

				res[cylIdx+uCylOffset].SetScale( renderR, renderR, renderR );
				res[cylIdx+uCylOffset].SetTranslate( 0.0, 0.0, -0.5*renderL );
				res[cylIdx+uCylOffset].tx = (float)(renderpos.x+renderpush.x);
				res[cylIdx+uCylOffset].ty = (float)(renderpos.y+renderpush.y);
				res[cylIdx+uCylOffset].tz = (float)(renderpos.z+renderpush.z);

				res[cylIdx+dCylOffset].SetScale( renderR, renderR, renderR );
				res[cylIdx+dCylOffset].SetTranslate( 0.0, 0.0, 0.5*renderL );
				res[cylIdx+dCylOffset].tx = (float)(renderpos.x+renderpush.x);
				res[cylIdx+dCylOffset].ty = (float)(renderpos.y+renderpush.y);
				res[cylIdx+dCylOffset].tz = (float)(renderpos.z+renderpush.z);

				cylIdx++;

			}
			else if( node[i].shape == 1 )	// box
			{
				res[i].ReadBox( textureName[i] );

				double renderLx = node[i].Lx * dh->render_scale;
				double renderLy = node[i].Ly * dh->render_scale;
				double renderLz = node[i].L  * dh->render_scale;
				
				res[i].SetScale( renderLx, renderLy, renderLz );			
				res[i].tx = (float)(renderpos.x);
				res[i].ty = (float)(renderpos.y);
				res[i].tz = (float)(renderpos.z);
			}
			else if( node[i].shape == 2 )	// flat cylinder
			{
			//	res[i].ReadCylinder( "greyscale.bmp" );
				res[i].ReadShape( "../media/model/cylinder_L.sim" );

				double renderR = node[i].R * dh->render_scale;
				double renderL = node[i].L * dh->render_scale;

				res[i].SetScale( renderR, renderR, renderL );
				res[i].tx = (float)(renderpos.x);
				res[i].ty = (float)(renderpos.y);
				res[i].tz = (float)(renderpos.z);

			}
			else
			{

			}

		}

	
	dh->robj = res;

	
}







void DXHANDLE::SetRigidMatrix( int n , int r )
{
	D3DXMatrixIdentity( &robj[r].matRot );

	robj[r].matRot._11 = (float)rs->node[n].tox.x;
	robj[r].matRot._12 = (float)rs->node[n].tox.y;
	robj[r].matRot._13 = (float)rs->node[n].tox.z;
	robj[r].matRot._14 = 0.0f;
	robj[r].matRot._21 = (float)rs->node[n].toy.x;
	robj[r].matRot._22 = (float)rs->node[n].toy.y;
	robj[r].matRot._23 = (float)rs->node[n].toy.z;
	robj[r].matRot._24 = 0.0f;
	robj[r].matRot._31 = (float)rs->node[n].toz.x;
	robj[r].matRot._32 = (float)rs->node[n].toz.y;
	robj[r].matRot._33 = (float)rs->node[n].toz.z;
	robj[r].matRot._34 = 0.0f;

	robj[r].tx = (float)rs->node[n].tpos.x*render_scale;
	robj[r].ty = (float)rs->node[n].tpos.y*render_scale;
	robj[r].tz = (float)rs->node[n].tpos.z*render_scale;
}

void DXHANDLE::SetObjMatrix ( void )
{  

	int NUM = rs->NUM_ODE_BODY;
	int NUM_OR = rs->NUM_ODE_BODY_ORIGIN;

	
	int cylIdx = 0;

	for( int i=0 ; i<NUM ; i++ )
	{
		if( rs->node[i].shape == 0 )
		{
			SetRigidMatrix( i, i );
			SetRigidMatrix( i, cylIdx+NUM_OR );
			SetRigidMatrix( i, cylIdx+NUM_OR+NUM_CCYL );

			cylIdx++;
		}
		else
		{
			SetRigidMatrix( i, i );
		}

	}

}



void ROBJ::SetScale( double x, double y, double z )
{
	int i;
	
	for( i=0 ; i<sim->objMesh[0].num_vertex ; i++ )
	{
		sim->objMesh[0].v[i*3    ] *= (float)x;
		sim->objMesh[0].v[i*3 + 1] *= (float)y;
		sim->objMesh[0].v[i*3 + 2] *= (float)z;
	}


}


void ROBJ::SetTranslate( double x, double y, double z )
{
	int i;
	
	for( i=0 ; i<sim->objMesh[0].num_vertex ; i++ )
	{
		sim->objMesh[0].v[i*3    ] += (float)x;
		sim->objMesh[0].v[i*3 + 1] += (float)y;
		sim->objMesh[0].v[i*3 + 2] += (float)z;
	}


}


////////// Get functions ////////////////////

VEC3 GetCenterCAM( DXHANDLE* dxh )
{
	return dxh->rs->avrCG * dxh->render_scale;
}

VEC3 GetCenterMass( void ){		return g_sim[0]->rs->CG;		}



